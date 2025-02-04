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
 *     What is this file about?
 *
 * Revision history:
 *     xxxx-xx-xx, author, first version
 *     xxxx-xx-xx, author, fix bug about xxx
 */

#pragma once

# include <dsn/tool_api.h>
# include <condition_variable>

namespace dsn 
{
    namespace tools
    {
        class hpc_task_queue : public task_queue
        {
        public:
            hpc_task_queue(task_worker_pool* pool, int index, task_queue* inner_provider);

            virtual void     enqueue(task* task) override;
            virtual task*    dequeue(/*inout*/int& batch_size) override;

        private:            
            utils::ex_lock_nr_spin        _lock;
            std::condition_variable_any   _cond;
            slist<task>                   _tasks;
        };

        class hpc_task_priority_queue : public task_queue
        {
        public:
            hpc_task_priority_queue(task_worker_pool* pool, int index, task_queue* inner_provider);

            virtual void     enqueue(task* task) override;
            virtual task*    dequeue(/*inout*/int& batch_size) override;

        private:
            utils::ex_lock_nr_spin        _lock[TASK_PRIORITY_COUNT];
            slist<task>                   _tasks[TASK_PRIORITY_COUNT];
            utils::semaphore              _sema;
        };
    }
}
