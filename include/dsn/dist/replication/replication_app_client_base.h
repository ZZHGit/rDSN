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

//
// replication_app_client_base is the base class for clients for 
// all app to be replicated using this library
// 
# include <dsn/service_api_cpp.h>
# include <dsn/dist/replication/replication.types.h>
# include <dsn/dist/replication/replication_other_types.h>
# include <dsn/dist/replication/replication.codes.h>

# ifdef __TITLE__
# undef __TITLE__
# endif
# define __TITLE__ "replication.app.client.base.h"

namespace dsn { namespace replication {

    //error code
    DEFINE_ERR_CODE(ERR_REPLICATION_FAILURE)
    DEFINE_ERR_CODE(ERR_APP_EXIST)
    DEFINE_ERR_CODE(ERR_APP_NOT_EXIST)
    DEFINE_ERR_CODE(ERR_BUSY_CREATING)
    DEFINE_ERR_CODE(ERR_BUSY_DROPPING)
    
#pragma pack(push, 4)
    class replication_app_client_base : public virtual clientlet
    {
    public:
        static void load_meta_servers(/*out*/ std::vector< ::dsn::rpc_address>& servers, const char* section = "replication.meta_servers");

    public:
        replication_app_client_base(        
            const std::vector< ::dsn::rpc_address>& meta_servers, 
            const char* replicated_app_name, 
            int task_bucket_count = 13
            );
        ~replication_app_client_base();

        template<typename T, typename TRequest, typename TResponse>
        ::dsn::task_ptr write(
            uint64_t key_hash,
            dsn_task_code_t code,
            std::shared_ptr<TRequest>& req,

            // callback
            T* owner,
            void (T::*callback)(error_code, std::shared_ptr<TRequest>&, std::shared_ptr<TResponse>&),

            // other specific parameters   
            int timeout_milliseconds = 0,
            int reply_hash = 0
            )
        {
            
            dsn_message_t msg = dsn_msg_create_request(RPC_REPLICATION_CLIENT_WRITE, timeout_milliseconds, 0);
            
            task_ptr task = ::dsn::rpc::internal_use_only::create_rpc_call(
                msg,
                req,
                owner,
                callback,
                reply_hash
                );

            auto rc = create_write_context(key_hash, code, msg, task, reply_hash);
            ::marshall(msg, *req);
            call(rc);
            return std::move(task);
        }
        
        template<typename TRequest, typename TResponse>
        ::dsn::task_ptr write(
            uint64_t key_hash,
            dsn_task_code_t code,
            std::shared_ptr<TRequest>& req,

            // callback
            clientlet* owner,
            std::function<void(error_code, std::shared_ptr<TRequest>&, std::shared_ptr<TResponse>&)> callback,

            // other specific parameters   
            int timeout_milliseconds = 0,
            int reply_hash = 0
            )
        {
            
            dsn_message_t msg = dsn_msg_create_request(RPC_REPLICATION_CLIENT_WRITE, timeout_milliseconds, 0);
            
            task_ptr task = ::dsn::rpc::internal_use_only::create_rpc_call(
                msg,
                req,
                callback,
                reply_hash,
                owner
                );

            auto rc = create_write_context(key_hash, code, msg, task, reply_hash);
            ::marshall(msg, *req);
            call(rc);
            return std::move(task);
        }

        template<typename T, typename TRequest, typename TResponse>
        ::dsn::task_ptr write(
            uint64_t key_hash,
            dsn_task_code_t code,
            const TRequest& req,

            // callback
            T* owner,
            void (T::*callback)(error_code, const TResponse&, void*),
            void* context,

            // other specific parameters   
            int timeout_milliseconds = 0,
            int reply_hash = 0
            )
        {
            
            dsn_message_t msg = dsn_msg_create_request(RPC_REPLICATION_CLIENT_WRITE, timeout_milliseconds, 0);
            
            task_ptr task = ::dsn::rpc::internal_use_only::create_rpc_call(
                msg,
                owner,
                callback,
                context,
                reply_hash
                );
            auto rc = create_write_context(key_hash, code, msg, task, reply_hash);
            ::marshall(msg, req);
            call(rc);
            return std::move(task);
        }

        template<typename TRequest, typename TResponse>
        ::dsn::task_ptr write(
            uint64_t key_hash,
            dsn_task_code_t code,
            const TRequest& req,

            // callback
            clientlet* owner,
            std::function<void(error_code, const TResponse&, void*)> callback,
            void* context,

            // other specific parameters   
            int timeout_milliseconds = 0,
            int reply_hash = 0
            )
        {
            
            dsn_message_t msg = dsn_msg_create_request(RPC_REPLICATION_CLIENT_WRITE, timeout_milliseconds, 0);
            
            task_ptr task = ::dsn::rpc::internal_use_only::create_rpc_call(
                msg,
                callback,
                context,
                reply_hash,
                owner
                );

            auto rc = create_write_context(key_hash, code, msg, task, reply_hash);
            ::marshall(msg, req);
            call(rc);
            return std::move(task);
        }

        template<typename T, typename TRequest, typename TResponse>
        ::dsn::task_ptr read(
            uint64_t key_hash,
            dsn_task_code_t code,
            std::shared_ptr<TRequest>& req,

            // callback
            T* owner,
            void (T::*callback)(error_code, std::shared_ptr<TRequest>&, std::shared_ptr<TResponse>&),

            // other specific parameters   
            int timeout_milliseconds = 0,            
            int reply_hash = 0,
            read_semantic_t read_semantic = ReadOutdated,
            decree snapshot_decree = invalid_decree // only used when ReadSnapshot
            )
        {
            
            dsn_message_t msg = dsn_msg_create_request(RPC_REPLICATION_CLIENT_READ, timeout_milliseconds, 0);
            
            task_ptr task = ::dsn::rpc::internal_use_only::create_rpc_call(
                msg,
                req,
                owner,
                callback,
                reply_hash
                );

            auto rc = create_read_context(key_hash, code, msg, task, read_semantic, snapshot_decree, reply_hash);
            ::marshall(msg, *req);
            call(rc);
            return std::move(task);
        }

        template<typename TRequest, typename TResponse>
        ::dsn::task_ptr read(
            uint64_t key_hash,
            dsn_task_code_t code,
            std::shared_ptr<TRequest>& req,

            // callback
            clientlet* owner,
            std::function<void(error_code, std::shared_ptr<TRequest>&, std::shared_ptr<TResponse>&)> callback,

            // other specific parameters   
            int timeout_milliseconds = 0,
            int reply_hash = 0,
            read_semantic_t read_semantic = ReadOutdated,
            decree snapshot_decree = invalid_decree // only used when ReadSnapshot
            )
        {
            
            dsn_message_t msg = dsn_msg_create_request(RPC_REPLICATION_CLIENT_READ, timeout_milliseconds, 0);
            
            task_ptr task = ::dsn::rpc::internal_use_only::create_rpc_call(
                msg,
                req,
                callback,
                reply_hash,
                owner
                );

            auto rc = create_read_context(key_hash, code, msg, task, read_semantic, snapshot_decree, reply_hash);
            ::marshall(msg, *req);
            call(rc);
            return std::move(task);
        }

        template<typename T, typename TRequest, typename TResponse>
        ::dsn::task_ptr read(
            uint64_t key_hash,
            dsn_task_code_t code,
            const TRequest& req,

            // callback
            T* owner,
            void (T::*callback)(error_code, const TResponse&, void*),
            void* context,

            // other specific parameters   
            int timeout_milliseconds = 0,
            int reply_hash = 0,
            read_semantic_t read_semantic = ReadOutdated,
            decree snapshot_decree = invalid_decree // only used when ReadSnapshot
            )
        {
            
            dsn_message_t msg = dsn_msg_create_request(RPC_REPLICATION_CLIENT_READ, timeout_milliseconds, 0);
            
            task_ptr task = ::dsn::rpc::internal_use_only::create_rpc_call(
                msg,
                owner,
                callback,
                context,
                reply_hash
                );

            auto rc = create_read_context(key_hash, code, msg, task, read_semantic, snapshot_decree, reply_hash);
            ::marshall(msg, req);
            call(rc);
            return std::move(task);
        }


        template<typename TRequest, typename TResponse>
        ::dsn::task_ptr read(
            uint64_t key_hash,
            dsn_task_code_t code,
            const TRequest& req,

            // callback
            clientlet* owner,
            std::function<void(error_code, const TResponse&, void*)> callback,
            void* context,

            // other specific parameters   
            int timeout_milliseconds = 0,
            int reply_hash = 0,
            read_semantic_t read_semantic = ReadOutdated,
            decree snapshot_decree = invalid_decree // only used when ReadSnapshot
            )
        {
            
            dsn_message_t msg = dsn_msg_create_request(RPC_REPLICATION_CLIENT_READ, timeout_milliseconds, 0);
            
            task_ptr task = ::dsn::rpc::internal_use_only::create_rpc_call(
                msg,
                callback,
                context,
                reply_hash,
                owner
                );

            auto rc = create_read_context(key_hash, code, msg, task, read_semantic, snapshot_decree, reply_hash);
            ::marshall(msg, req);
            call(rc);
            return std::move(task);
        }

        ::dsn::rpc_address get_meta_servers() const { return _meta_servers; }

        int get_partition_count() const { return _app_partition_count; }

    public:

        struct request_context : public ref_counter
        {
            int                   partition_index;
            uint64_t              key_hash;
            ::dsn::task_ptr callback_task;
            read_request_header   read_header;
            write_request_header  write_header;
            bool                  is_read;
            char*                 header_pos; // write header after body is written
            int                   timeout_ms; // init timeout
            uint64_t              timeout_ts_us; // timeout at this timing point
            dsn_message_t         request;

            ::dsn::service::zlock lock; // [
            dsn::task_ptr         timeout_timer; // when partition config is unknown at the first place
            bool                  completed;
            // ]
        };
        typedef ::dsn::ref_ptr<request_context> request_context_ptr;

    protected:
        virtual int get_partition_index(int partition_count, uint64_t key_hash);
        
    private:
        struct partition_context
        {
            dsn::task_ptr     query_config_task;
            std::list<request_context_ptr> requests;
        };

        typedef std::unordered_map<int, partition_context*> pending_replica_requests;

        mutable dsn::service::zlock     _requests_lock;
        pending_replica_requests  _pending_replica_requests;

    private:
        request_context* create_write_context(
            uint64_t key_hash,
            dsn_task_code_t code,
            dsn_message_t request,
            ::dsn::task_ptr& callback,
            int reply_hash = 0
            );

        request_context* create_read_context(
            uint64_t key_hash,
            dsn_task_code_t code,
            dsn_message_t request,
            ::dsn::task_ptr& callback,
            dsn::replication::read_semantic_t read_semantic = ReadOutdated,
            decree snapshot_decree = invalid_decree, // only used when ReadSnapshot
            int reply_hash = 0
            );

    private:
        std::string                             _app_name;
        dsn::rpc_address                        _meta_servers;

        mutable dsn::service::zrwlock_nr       _config_lock;
        std::unordered_map<int, partition_configuration> _config_cache;
        int                                     _app_id;
        int                                     _app_partition_count;

    private:
        // local routines
        dsn::rpc_address get_address(bool is_write, read_semantic_t semantic, const partition_configuration& config);
        error_code get_address(int pidx, bool is_write, /*out*/ dsn::rpc_address& addr, dsn::replication::read_semantic_t semantic);
        void clear_all_pending_tasks();

        // with replica
        void call(request_context_ptr request, bool from_meta_ack = false);
        void call_with_address(dsn::rpc_address address, request_context_ptr request);
        void replica_rw_reply(error_code err, dsn_message_t request, dsn_message_t response, request_context_ptr& rc);
        void end_request(request_context_ptr& request, error_code err, dsn_message_t resp);
        void on_replica_request_timeout(request_context_ptr& rc);

        // with meta server
        dsn::task_ptr query_partition_config(request_context_ptr request);
        void query_partition_configuration_reply(error_code err, dsn_message_t request, dsn_message_t response, request_context_ptr context);
    };
#pragma pack(pop)
}} // namespace
