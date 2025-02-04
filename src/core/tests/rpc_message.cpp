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
 *     Unit-test for rpc_message.
 *
 * Revision history:
 *     Nov., 2015, @qinzuoyan (Zuoyan Qin), first version
 *     xxxx-xx-xx, author, fix bug about xxx
 */

# include <dsn/internal/rpc_message.h>
# include <../core/transient_memory.h>
# include <gtest/gtest.h>

using namespace ::dsn;

#define CRC_INVALID 0xdead0c2c
DEFINE_TASK_CODE_RPC(RPC_CODE_FOR_TEST, TASK_PRIORITY_COMMON, ::dsn::THREAD_POOL_DEFAULT)

TEST(core, message_ex)
{
    { // create_request
        uint64_t next_id = message_ex::new_id() + 1;
        message_ex* m = message_ex::create_request(RPC_CODE_FOR_TEST, 100, 1);
        ASSERT_EQ(0, m->get_count());

        message_header& h = *m->header;
        ASSERT_EQ(CRC_INVALID, h.hdr_crc32);
        ASSERT_EQ(CRC_INVALID, h.body_crc32);
        ASSERT_EQ(0, h.body_length);
        ASSERT_EQ(0, h.version);
        ASSERT_EQ(next_id, h.id);
        ASSERT_EQ(0, h.rpc_id); ///////////////////
        ASSERT_STREQ(dsn_task_code_to_string(RPC_CODE_FOR_TEST), h.rpc_name);
        ASSERT_EQ(0, h.vnid);
        ASSERT_EQ(0, h.context.context);
        ASSERT_EQ(100, h.client.timeout_ms);
        ASSERT_EQ(1, h.client.hash);
        ASSERT_EQ(0, h.client.port);

        ASSERT_EQ(1u, m->buffers.size());
        ASSERT_EQ((int)RPC_CODE_FOR_TEST, m->local_rpc_code);

        ASSERT_TRUE(m->is_right_header());
        ASSERT_TRUE(m->is_right_body(true));
        ASSERT_TRUE(message_ex::is_right_header((char*)m->header));
        ASSERT_EQ(0, message_ex::get_body_length((char*)m->header));

        m->add_ref();
        ASSERT_EQ(1, m->get_count());
        m->release_ref();
    }

    { // create_response
        message_ex* request = message_ex::create_request(RPC_CODE_FOR_TEST, 0, 0);
        request->from_address = rpc_address("127.0.0.1", 8080);
        request->to_address = rpc_address("127.0.0.1", 9090);
        request->header->rpc_id = 123456;

        message_ex* response = request->create_response();

        message_header& h = *response->header;
        ASSERT_EQ(CRC_INVALID, h.hdr_crc32);
        ASSERT_EQ(CRC_INVALID, h.body_crc32);
        ASSERT_EQ(0, h.body_length);
        ASSERT_EQ(0, h.version);
        ASSERT_EQ(request->header->id, h.id);
        ASSERT_EQ(request->header->rpc_id, h.rpc_id); ///////////////////
        ASSERT_STREQ(dsn_task_code_to_string(RPC_CODE_FOR_TEST_ACK), h.rpc_name);
        ASSERT_EQ(0, h.vnid);
        ASSERT_EQ(0, h.context.context);
        ASSERT_EQ(0, h.server.error);

        ASSERT_EQ(1u, response->buffers.size());
        ASSERT_EQ((int)RPC_CODE_FOR_TEST_ACK, response->local_rpc_code);
        ASSERT_EQ(request->from_address, response->to_address);
        ASSERT_EQ(request->to_address, response->from_address);

        ASSERT_TRUE(response->is_right_header());
        ASSERT_TRUE(response->is_right_body(true));
        ASSERT_TRUE(message_ex::is_right_header((char*)response->header));
        ASSERT_EQ(0, message_ex::get_body_length((char*)response->header));

        response->add_ref();
        response->release_ref();

        request->add_ref();
        request->release_ref();
    }

    { // write
        message_ex* request = message_ex::create_request(RPC_CODE_FOR_TEST, 100, 1);
        const char* data = "adaoihfeuifgggggisdosghkbvjhzxvdafdiofgeof";
        size_t data_size = strlen(data);

        void* ptr;
        size_t sz;

        request->write_next(&ptr, &sz, data_size);
        memcpy(ptr, data, data_size);
        request->write_commit(data_size);
        ASSERT_EQ(1u, request->buffers.size());
        ASSERT_EQ(ptr, request->rw_ptr(0));
        ASSERT_EQ((void*)((char*)ptr + 10), request->rw_ptr(10));
        ASSERT_EQ(nullptr, request->rw_ptr(data_size));

        tls_trans_mem_alloc(1024); // reset tls buffer

        request->write_next(&ptr, &sz, data_size);
        memcpy(ptr, data, data_size);
        request->write_commit(data_size);
        ASSERT_EQ(2u, request->buffers.size());
        ASSERT_EQ(ptr, request->rw_ptr(data_size));
        ASSERT_EQ((void*)((char*)ptr + 10), request->rw_ptr(data_size + 10));
        ASSERT_EQ(nullptr, request->rw_ptr(data_size + data_size));

        ASSERT_TRUE(request->is_right_header());
        ASSERT_TRUE(request->is_right_body(true));

        request->seal(false);
        ASSERT_TRUE(request->is_right_header());
        ASSERT_TRUE(request->is_right_body(true));

        request->seal(true);
        ASSERT_TRUE(request->is_right_header());
        ASSERT_TRUE(request->is_right_body(true));

        request->add_ref();
        request->release_ref();
    }

    { // read
        message_ex* request = message_ex::create_request(RPC_CODE_FOR_TEST, 100, 1);
        const char* data = "adaoihfeuifgggggisdosghkbvjhzxvdafdiofgeof";
        size_t data_size = strlen(data);

        void* ptr;
        size_t sz;

        request->write_next(&ptr, &sz, data_size);
        memcpy(ptr, data, data_size);
        request->write_commit(data_size);

        request->seal(true);
        ASSERT_TRUE(request->is_right_body(true));

        ASSERT_EQ(1u, request->buffers.size());
        message_ex* receive = message_ex::create_receive_message(request->buffers[0]);
        ASSERT_EQ(1u, receive->buffers.size());
        ASSERT_TRUE(receive->is_right_header());
        ASSERT_TRUE(receive->is_right_body(false));
        ASSERT_TRUE(request->is_right_body(true));

        ASSERT_STREQ(dsn_task_code_to_string(RPC_CODE_FOR_TEST), receive->header->rpc_name);

        ASSERT_TRUE(receive->read_next(&ptr, &sz));
        ASSERT_EQ(data_size, sz);
        ASSERT_EQ(std::string(data), std::string((const char*)ptr, sz));
        receive->read_commit(sz);

        ASSERT_FALSE(receive->read_next(&ptr, &sz));

        receive->add_ref();
        receive->release_ref();

        request->add_ref();
        request->release_ref();
    }

    { // c interface
        dsn_message_t request = dsn_msg_create_request(RPC_CODE_FOR_TEST, 100, 1);
        dsn_msg_options_t opts;

        opts.context.context = 444;
        opts.vnid = 333;

        dsn_msg_set_options(request, &opts, DSN_MSGM_CONTEXT | DSN_MSGM_VNID);
        message_ex* m = (message_ex*)request;
        m->from_address = rpc_address("127.0.0.1", 8080);
        m->to_address = rpc_address("127.0.0.1", 9090);

        dsn_msg_get_options(request, &opts);
        ASSERT_EQ(100, opts.timeout_ms);
        ASSERT_EQ(1, opts.thread_hash);
        ASSERT_EQ(333u, opts.vnid);
        ASSERT_EQ(444u, opts.context.context);

        ASSERT_EQ(rpc_address("127.0.0.1", 8080), rpc_address(dsn_msg_from_address(request)));
        ASSERT_EQ(rpc_address("127.0.0.1", 9090), rpc_address(dsn_msg_to_address(request)));

        const char* data = "adaoihfeuifgggggisdosghkbvjhzxvdafdiofgeof";
        size_t data_size = strlen(data);

        void* ptr;
        size_t sz;

        dsn_msg_write_next(request, &ptr, &sz, data_size);
        memcpy(ptr, data, data_size);
        dsn_msg_write_commit(request, data_size);

        ASSERT_EQ(data_size, dsn_msg_body_size(request));

        ptr = dsn_msg_rw_ptr(request, 0);
        ASSERT_EQ(std::string(data), std::string((const char*)ptr, data_size));
        ptr = dsn_msg_rw_ptr(request, 10);
        ASSERT_EQ(std::string(data + 10), std::string((const char*)ptr, data_size - 10));
        ptr = dsn_msg_rw_ptr(request, data_size);
        ASSERT_EQ(nullptr, ptr);

        dsn_message_t response = dsn_msg_create_response(request);
        ASSERT_EQ(rpc_address("127.0.0.1", 9090), rpc_address(dsn_msg_from_address(response)));
        ASSERT_EQ(rpc_address("127.0.0.1", 8080), rpc_address(dsn_msg_to_address(response)));

        ASSERT_EQ(1u, m->buffers.size());
        dsn_message_t receive = (dsn_message_t)message_ex::create_receive_message(m->buffers[0]);

        ASSERT_EQ(data_size, dsn_msg_body_size(receive));

        dsn_msg_read_next(receive, &ptr, &sz);
        ASSERT_EQ(data_size, sz);
        ASSERT_EQ(std::string(data), std::string((const char*)ptr, sz));
        dsn_msg_read_commit(receive, sz);

        dsn_msg_add_ref(receive);
        dsn_msg_release_ref(receive);

        dsn_msg_add_ref(response);
        dsn_msg_release_ref(response);

        dsn_msg_add_ref(request);
        dsn_msg_release_ref(request);
    }
}

