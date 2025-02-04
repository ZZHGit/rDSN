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
 *     ddl client interface
 *
 * Revision history:
 *     2015-12-30, xiaotz, first version
 */

#include <cctype>
#include <dsn/dist/replication.h>

namespace dsn{ namespace replication{

class client_ddl : public clientlet
{
public:
    client_ddl(const std::vector<dsn::rpc_address>& meta_servers);

    dsn::error_code create_app(const std::string& app_name, const std::string& app_type, int partition_count, int replica_count);

    dsn::error_code drop_app(const std::string& app_name);

    dsn::error_code list_apps(const dsn::replication::app_status status, const std::string& file_name);

    dsn::error_code list_app(const std::string& app_name, bool detailed, const std::string& file_name);

private:
    bool static valid_app_char(int c);

    void end_meta_request(task_ptr callback, int retry_times, error_code err, dsn_message_t request, dsn_message_t resp);

    template<typename TRequest, typename TResponse>
    dsn::task_ptr request_meta(
            dsn_task_code_t code,
            std::shared_ptr<TRequest>& req,

            // callback
            clientlet* owner,
            std::function<void(error_code, std::shared_ptr<TRequest>&, std::shared_ptr<TResponse>&)> callback,

            // other specific parameters
            int timeout_milliseconds= 0,
            int reply_hash = 0
            )
    {
        dsn_message_t msg = dsn_msg_create_request(code, timeout_milliseconds, 0);
        ::marshall(msg, *req);

        task_ptr task = ::dsn::rpc::internal_use_only::create_rpc_call(
                 msg,
                 req,
                 callback,
                 reply_hash,
                 owner
                 );
        rpc::call(
            _meta_servers,
            msg,
            this,
            std::bind(&client_ddl::end_meta_request,
            this,
            task,
            0,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3
            ),
            0
         );
        return std::move(task);
    }

private:
    dsn::rpc_address _meta_servers;
    std::vector<dsn::rpc_address> _meta_server_vector;
};

}} //namespace
