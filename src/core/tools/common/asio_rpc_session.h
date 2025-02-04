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

# include <dsn/internal/rpc_message.h>
# include <dsn/internal/priority_queue.h>
# include <dsn/internal/message_parser.h>
# include <boost/asio.hpp>
# include "asio_net_provider.h"

namespace dsn {
    namespace tools {

        class asio_rpc_session : public rpc_session
        {
        public:
            asio_rpc_session(
                asio_network_provider& net,
                ::dsn::rpc_address remote_addr,
                std::shared_ptr<boost::asio::ip::tcp::socket>& socket,
                std::shared_ptr<message_parser>& parser,
                bool is_client
                );
            virtual ~asio_rpc_session();
            virtual void send(uint64_t signature) override { return write(signature); }

        public:
            virtual void connect() override;            
            
        private:
            void do_read(size_t sz = 256);
            void write(uint64_t signature);
            void on_failure();
            void set_options();  
            void on_message_read(message_ex* msg)
            {
                if (is_client())
                    on_recv_reply(msg->header->id, msg, 0);
                else
                    on_recv_request(msg, 0);
            }

        private:
            std::shared_ptr<boost::asio::ip::tcp::socket> _socket;            
        };
    }
}
