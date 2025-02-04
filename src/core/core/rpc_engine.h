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
 *     rpc service
 *
 * Revision history:
 *     Mar., 2015, @imzhenyu (Zhenyu Guo), first version
 *     xxxx-xx-xx, author, fix bug about xxx
 */

# pragma once

# include <dsn/internal/task.h>
# include <dsn/internal/network.h>
# include <dsn/internal/synchronize.h>
# include <dsn/internal/global_config.h>

namespace dsn {

class service_node;
class rpc_engine;

//
// client matcher for matching RPC request and RPC response, and handling timeout
// (1) the whole network may share a single client matcher,
// (2) or we usually prefere each <src, dst> pair use a client matcher to have better inquery performance
// (3) or we have certain cases we want RPC responses from node which is not the initial target node
//     the RPC request message is sent to. In this case, a shared rpc_engine level matcher is used.
//
// WE NOW USE option (3) so as to enable more features and the performance should not be degraded (due to 
// less std::shared_ptr<rpc_client_matcher> operations in rpc_timeout_task
//
#define MATCHER_BUCKET_NR 13
class rpc_client_matcher : public ref_counter
{
public:
    rpc_client_matcher(rpc_engine* engine)
        :_engine(engine)
    {

    }

    ~rpc_client_matcher();

    //
    // when a two-way RPC call is made, register the requst id and the callback
    // which also registers a timer for timeout tracking
    //
    void on_call(message_ex* request, rpc_response_task* call);

    //
    // when a RPC response is received, call this function to trigger calback
    //  key - message.header.id
    //  reply - rpc response message
    //  delay_ms - sometimes we want to delay the delivery of the message for certain purposes
    //
    bool on_recv_reply(uint64_t key, message_ex* reply, int delay_ms);

private:
    friend class rpc_timeout_task;
    void on_rpc_timeout(uint64_t key);

private:
    rpc_engine*               _engine;
    struct match_entry
    {
        rpc_response_task*    resp_task;
        task*                 timeout_task;
    };
    typedef std::unordered_map<uint64_t, match_entry> rpc_requests;
    rpc_requests                  _requests[MATCHER_BUCKET_NR];
    ::dsn::utils::ex_lock_nr_spin _requests_lock[MATCHER_BUCKET_NR];
};

class rpc_server_dispatcher
{
public:
    bool  register_rpc_handler(rpc_handler_ptr& handler);
    rpc_handler_ptr unregister_rpc_handler(dsn_task_code_t rpc_code);
    rpc_request_task* on_request(message_ex* msg, service_node* node);
    int handler_count() const 
    {
        utils::auto_read_lock l(_handlers_lock); 
        return static_cast<int>(_handlers.size()); 
    }

private:
    typedef std::unordered_map<std::string, rpc_handler_ptr> rpc_handlers;
    rpc_handlers                  _handlers;
    mutable utils::rw_lock_nr     _handlers_lock;
};

class rpc_engine
{
public:
    rpc_engine(configuration_ptr config, service_node* node);

    //
    // management routines
    //
    ::dsn::error_code start(
        const service_app_spec& spec, 
        io_modifer& ctx
        );

    //
    // rpc registrations
    //
    bool  register_rpc_handler(rpc_handler_ptr& handler, uint64_t vnid);
    rpc_handler_ptr unregister_rpc_handler(dsn_task_code_t rpc_code, uint64_t vnid);

    //
    // rpc routines
    //
    void call(message_ex* request, rpc_response_task* call);    
    void on_recv_request(message_ex* msg, int delay_ms);
    void reply(message_ex* response, error_code err = ERR_OK);

    //
    // information inquery
    //
    service_node* node() const { return _node; }
    ::dsn::rpc_address primary_address() const { return _local_primary_address; }
    rpc_client_matcher* matcher() { return &_rpc_matcher; }

    // call with ip address only
    void call_ip(rpc_address addr, message_ex* request, rpc_response_task* call, bool reset_request_id = false);

private:
    network* create_network(
        const network_server_config& netcs, 
        bool client_only,
        io_modifer& ctx
        );

private:
    configuration_ptr                                _config;    
    service_node                                     *_node;
    std::vector<std::vector<network*>>               _client_nets; // <format, <CHANNEL, network*>>
    std::unordered_map<int, std::vector<network*>>   _server_nets; // <port, <CHANNEL, network*>>
    ::dsn::rpc_address                               _local_primary_address;
    rpc_client_matcher                               _rpc_matcher;
    rpc_server_dispatcher                            _rpc_dispatcher;    

    utils::rw_lock_nr                                    _vnodes_lock;
    std::unordered_map<uint64_t, rpc_server_dispatcher*> _vnodes;
    
    volatile bool                 _is_running;
    static bool                   _message_crc_required;
};

// ------------------------ inline implementations --------------------

} // end namespace

