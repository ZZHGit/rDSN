/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Microsoft Corporation
 * 
 * -=- Robust Distributed System Nucleus (rDSN) -=- 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * Description:
 *     helpers for easier task programing atop of C api
 *
 * Revision history:
 *     Sep., 2015, @imzhenyu (Zhenyu Guo), first version
 *     xxxx-xx-xx, author, fix bug about xxx
 */

# pragma once

# include <dsn/service_api_c.h>
# include <dsn/ports.h>
# include <dsn/cpp/auto_codes.h>
# include <dsn/cpp/utils.h>
# include <dsn/cpp/rpc_stream.h>
# include <dsn/cpp/serialization.h>
# include <dsn/cpp/zlocks.h>
# include <dsn/cpp/autoref_ptr.h>
# include <dsn/internal/synchronize.h>
# include <dsn/internal/link.h>
# include <set>
# include <map>
# include <thread>

namespace dsn 
{
    typedef std::function<void()> task_handler;
    typedef std::function<void(error_code, size_t)> aio_handler;
    typedef std::function<void(error_code, dsn_message_t, dsn_message_t)> rpc_reply_handler;
    typedef std::function<void(dsn_message_t)> rpc_request_handler;
    class safe_task_handle;
    typedef ::dsn::ref_ptr<safe_task_handle> task_ptr;
    
    //
    // basic cpp task wrapper
    // which manages the task handle
    // and the interaction with task context manager, clientlet
    //        
    class safe_task_handle : public ::dsn::ref_counter
    {
    public:
        safe_task_handle()
        {
            _task = 0;
            _rpc_response = 0;
        }

        virtual ~safe_task_handle()
        {
            dsn_task_release_ref(_task);

            if (0 != _rpc_response)
                dsn_msg_release_ref(_rpc_response);
        }

        void set_task_info(dsn_task_t t)
        {
            _task = t;
            dsn_task_add_ref(t);
        }

        dsn_task_t native_handle() { return _task; }
                        
        virtual bool cancel(bool wait_until_finished, bool* finished = nullptr)
        {
            return dsn_task_cancel2(_task, wait_until_finished, finished);
        }

        bool wait()
        {
            return dsn_task_wait(_task);
        }

        bool wait(int timeout_millieseconds)
        {
            return dsn_task_wait_timeout(_task, timeout_millieseconds);
        }

        ::dsn::error_code error()
        {
            return dsn_task_error(_task);
        }
            
        size_t io_size()
        {
            return dsn_file_get_io_size(_task);
        }

        void enqueue(int delay_milliseconds = 0)
        {
            dsn_task_call(_task, delay_milliseconds);
        }
            
        void enqueue_aio(error_code err, size_t size)
        {
            dsn_file_task_enqueue(_task, err.get(), size);
        }

        dsn_message_t response()
        {
            if (_rpc_response == 0)
                _rpc_response = dsn_rpc_get_response(_task);
            return _rpc_response;
        }

        void enqueue_rpc_response(error_code err, dsn_message_t resp)
        {
            dsn_rpc_enqueue_response(_task, err.get(), resp);
        }

    private:
        dsn_task_t           _task;
        dsn_message_t        _rpc_response;
    };

    template<typename THandler>
    class safe_task : public safe_task_handle
    {
    public:
        safe_task(THandler& h, bool is_timer) : _handler(h), _is_timer(is_timer)
        {
        }

        safe_task(THandler& h) : _handler(h), _is_timer(false)
        {
        }

        virtual bool cancel(bool wait_until_finished, bool* finished = nullptr) override
        {
            bool r = safe_task_handle::cancel(wait_until_finished, finished);
            if (r)
            {
                _handler = nullptr;
            }
            return r;
        }

        static void on_cancel(void* task)
        {
            safe_task* t = (safe_task*)task;
            t->release_ref(); // added upon callback exec registration
        }

        static void exec(void* task)
        {
            safe_task* t = (safe_task*)task;
            t->_handler();
            if (!t->_is_timer)
            {
                t->_handler = nullptr;
                t->release_ref(); // added upon callback exec registration
            }
        }
        
        static void exec_rpc_response(dsn_error_t err, dsn_message_t req, dsn_message_t resp, void* task)
        {
            safe_task* t = (safe_task*)task;
            if (t->_handler)
            {
                t->_handler(err, req, resp);
                t->_handler = nullptr;
            }
            t->release_ref(); // added upon callback exec_rpc_response registration
        }

        static void exec_aio(dsn_error_t err, size_t sz, void* task)
        {
            safe_task* t = (safe_task*)task;
            if (t->_handler)
            {
                t->_handler(err, sz);
                t->_handler = nullptr;
            }
            t->release_ref(); // added upon callback exec_aio registration
        }
            
    private:
        bool                 _is_timer;
        THandler             _handler;
    };

    //
    // two staged computation task
    // this is used when a task handle is returned when a call is made,
    // while the task, is however, enqueued later after other operations when
    // certain parameters to the task is known (e.g., error code after logging)
    // in thise case, we can use two staged computation task as this is.
    //
    //    task_ptr task = tasking::create_late_task(...);
    //    ...
    //    return task;
    //
    //    ... after logging ....
    //    task->bind_and_enqueue([&](cb)=>{std::bind(cb, error)}, delay);
    //
    template<typename THandler>
    class safe_late_task : public safe_task_handle
    {
    public:
        safe_late_task(THandler& h)
            : _handler(h), _bound_handler(nullptr)
        {
        }

        operator task_ptr() const
        {
            return task_ptr(this);
        }

        virtual bool cancel(bool wait_until_finished, bool* finished = nullptr) override
        {
            bool r = safe_task_handle::cancel(wait_until_finished, finished);
            if (r)
            {
                _bound_handler = nullptr;
            }
            return r;
        }

        void bind_and_enqueue(
            std::function<std::function<void()> (THandler&)> binder,
            int delay_milliseconds = 0
            )
        {
            _bound_handler = binder(_handler);
            _handler = nullptr;
            dsn_task_call(native_handle(), delay_milliseconds);
        }

        static void on_cancel(void* task)
        {
            auto t = (safe_late_task<THandler>*)task;
            t->release_ref(); // added upon callback exec registration
        }

        static void exec(void* task)
        {
            auto t = (safe_late_task<THandler>*)task;
            t->_bound_handler();
            t->_bound_handler = nullptr;
            t->release_ref(); // added upon callback exec registration
        }

    private:
        std::function<void()> _bound_handler;
        THandler              _handler;
    };

    // ------- inlined implementation ----------
}
