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
 *     2015-12-25, @shengofsun(Weijie Sun) add types for create/drop app
 *     xxxx-xx-xx, author, first version
 *     xxxx-xx-xx, author, fix bug about xxx
 */
# pragma once

//
// uncomment the following line if you want to use 
// data encoding/decoding from the original tool instead of rDSN
// in this case, you need to use these tools to generate
// type files with --gen=cpp etc. options
//
// !!! WARNING: not feasible for replicated service yet!!! 
//
// # define DSN_NOT_USE_DEFAULT_SERIALIZATION

# include <dsn/service_api_cpp.h>


# ifdef DSN_NOT_USE_DEFAULT_SERIALIZATION

# include <dsn/thrift_helper.h>
# include "replication_types.h" 

namespace dsn { namespace replication { 
    // ---------- global_partition_id -------------
    inline void marshall(::dsn::binary_writer& writer, const global_partition_id& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<global_partition_id>(&proto, val, &global_partition_id::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ global_partition_id& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<global_partition_id>(&proto, val, &global_partition_id::read);
    };

    // ---------- mutation_header -------------
    inline void marshall(::dsn::binary_writer& writer, const mutation_header& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<mutation_header>(&proto, val, &mutation_header::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ mutation_header& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<mutation_header>(&proto, val, &mutation_header::read);
    };

    // ---------- mutation_data -------------
    inline void marshall(::dsn::binary_writer& writer, const mutation_data& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<mutation_data>(&proto, val, &mutation_data::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ mutation_data& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<mutation_data>(&proto, val, &mutation_data::read);
    };

    // ---------- partition_configuration -------------
    inline void marshall(::dsn::binary_writer& writer, const partition_configuration& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<partition_configuration>(&proto, val, &partition_configuration::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ partition_configuration& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<partition_configuration>(&proto, val, &partition_configuration::read);
    };

    // ---------- replica_configuration -------------
    inline void marshall(::dsn::binary_writer& writer, const replica_configuration& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<replica_configuration>(&proto, val, &replica_configuration::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ replica_configuration& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<replica_configuration>(&proto, val, &replica_configuration::read);
    };

    // ---------- prepare_msg -------------
    inline void marshall(::dsn::binary_writer& writer, const prepare_msg& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<prepare_msg>(&proto, val, &prepare_msg::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ prepare_msg& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<prepare_msg>(&proto, val, &prepare_msg::read);
    };

    // ---------- read_request_header -------------
    inline void marshall(::dsn::binary_writer& writer, const read_request_header& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<read_request_header>(&proto, val, &read_request_header::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ read_request_header& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<read_request_header>(&proto, val, &read_request_header::read);
    };

    // ---------- write_request_header -------------
    inline void marshall(::dsn::binary_writer& writer, const write_request_header& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<write_request_header>(&proto, val, &write_request_header::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ write_request_header& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<write_request_header>(&proto, val, &write_request_header::read);
    };

    // ---------- rw_response_header -------------
    inline void marshall(::dsn::binary_writer& writer, const rw_response_header& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<rw_response_header>(&proto, val, &rw_response_header::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ rw_response_header& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<rw_response_header>(&proto, val, &rw_response_header::read);
    };

    // ---------- prepare_ack -------------
    inline void marshall(::dsn::binary_writer& writer, const prepare_ack& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<prepare_ack>(&proto, val, &prepare_ack::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ prepare_ack& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<prepare_ack>(&proto, val, &prepare_ack::read);
    };

    // ---------- learn_state -------------
    inline void marshall(::dsn::binary_writer& writer, const learn_state& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<learn_state>(&proto, val, &learn_state::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ learn_state& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<learn_state>(&proto, val, &learn_state::read);
    };

    // ---------- learn_request -------------
    inline void marshall(::dsn::binary_writer& writer, const learn_request& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<learn_request>(&proto, val, &learn_request::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ learn_request& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<learn_request>(&proto, val, &learn_request::read);
    };

    // ---------- learn_response -------------
    inline void marshall(::dsn::binary_writer& writer, const learn_response& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<learn_response>(&proto, val, &learn_response::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ learn_response& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<learn_response>(&proto, val, &learn_response::read);
    };

    // ---------- group_check_request -------------
    inline void marshall(::dsn::binary_writer& writer, const group_check_request& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<group_check_request>(&proto, val, &group_check_request::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ group_check_request& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<group_check_request>(&proto, val, &group_check_request::read);
    };

    // ---------- group_check_response -------------
    inline void marshall(::dsn::binary_writer& writer, const group_check_response& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<group_check_response>(&proto, val, &group_check_response::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ group_check_response& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<group_check_response>(&proto, val, &group_check_response::read);
    };

    // ---------- app_info -------------
    inline void marshall(::dsn::binary_writer& writer, const app_info& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<app_info>(&proto, val, &app_info::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ app_info& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<app_info>(&proto, val, &app_info::read);
    };

    // ---------- meta_request_header -------------
    inline void marshall(::dsn::binary_writer& writer, const meta_request_header& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<meta_request_header>(&proto, val, &meta_request_header::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ meta_request_header& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<meta_request_header>(&proto, val, &meta_request_header::read);
    };

    // ---------- meta_response_header -------------
    inline void marshall(::dsn::binary_writer& writer, const meta_response_header& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<meta_response_header>(&proto, val, &meta_response_header::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ meta_response_header& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<meta_response_header>(&proto, val, &meta_response_header::read);
    };

    // ---------- configuration_update_request -------------
    inline void marshall(::dsn::binary_writer& writer, const configuration_update_request& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<configuration_update_request>(&proto, val, &configuration_update_request::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_update_request& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<configuration_update_request>(&proto, val, &configuration_update_request::read);
    };

    // ---------- configuration_update_response -------------
    inline void marshall(::dsn::binary_writer& writer, const configuration_update_response& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<configuration_update_response>(&proto, val, &configuration_update_response::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_update_response& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<configuration_update_response>(&proto, val, &configuration_update_response::read);
    };

    // ---------- configuration_proposal_request -------------
    inline void marshall(::dsn::binary_writer& writer, const configuration_proposal_request& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<configuration_proposal_request>(&proto, val, &configuration_proposal_request::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_proposal_request& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<configuration_proposal_request>(&proto, val, &configuration_proposal_request::read);
    };

    // ---------- configuration_query_by_node_request -------------
    inline void marshall(::dsn::binary_writer& writer, const configuration_query_by_node_request& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<configuration_query_by_node_request>(&proto, val, &configuration_query_by_node_request::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_query_by_node_request& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<configuration_query_by_node_request>(&proto, val, &configuration_query_by_node_request::read);
    };

    // ---------- create_app_options -------------
    inline void marshall(::dsn::binary_writer& writer, const create_app_options& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<create_app_options>(&proto, val, &create_app_options::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ create_app_options& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<create_app_options>(&proto, val, &create_app_options::read);
    };

    // ---------- configuration_create_app_request -------------
    inline void marshall(::dsn::binary_writer& writer, const configuration_create_app_request& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<configuration_create_app_request>(&proto, val, &configuration_create_app_request::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_create_app_request& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<configuration_create_app_request>(&proto, val, &configuration_create_app_request::read);
    };

    // ---------- drop_app_options -------------
    inline void marshall(::dsn::binary_writer& writer, const drop_app_options& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<drop_app_options>(&proto, val, &drop_app_options::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ drop_app_options& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<drop_app_options>(&proto, val, &drop_app_options::read);
    };

    // ---------- configuration_drop_app_request -------------
    inline void marshall(::dsn::binary_writer& writer, const configuration_drop_app_request& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<configuration_drop_app_request>(&proto, val, &configuration_drop_app_request::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_drop_app_request& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<configuration_drop_app_request>(&proto, val, &configuration_drop_app_request::read);
    };

    // ---------- configuration_list_apps_request -------------
    inline void marshall(::dsn::binary_writer& writer, const configuration_list_apps_request& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<configuration_list_apps_request>(&proto, val, &configuration_list_apps_request::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_list_apps_request& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<configuration_list_apps_request>(&proto, val, &configuration_list_apps_request::read);
    };

    // ---------- configuration_create_app_response -------------
    inline void marshall(::dsn::binary_writer& writer, const configuration_create_app_response& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<configuration_create_app_response>(&proto, val, &configuration_create_app_response::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_create_app_response& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<configuration_create_app_response>(&proto, val, &configuration_create_app_response::read);
    };

    // ---------- configuration_drop_app_response -------------
    inline void marshall(::dsn::binary_writer& writer, const configuration_drop_app_response& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<configuration_drop_app_response>(&proto, val, &configuration_drop_app_response::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_drop_app_response& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<configuration_drop_app_response>(&proto, val, &configuration_drop_app_response::read);
    };

    // ---------- configuration_list_apps_response -------------
    inline void marshall(::dsn::binary_writer& writer, const configuration_list_apps_response& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<configuration_list_apps_response>(&proto, val, &configuration_list_apps_response::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_list_apps_response& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<configuration_list_apps_response>(&proto, val, &configuration_list_apps_response::read);
    };

    // ---------- configuration_query_by_node_response -------------
    inline void marshall(::dsn::binary_writer& writer, const configuration_query_by_node_response& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<configuration_query_by_node_response>(&proto, val, &configuration_query_by_node_response::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_query_by_node_response& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<configuration_query_by_node_response>(&proto, val, &configuration_query_by_node_response::read);
    };

    // ---------- configuration_query_by_index_request -------------
    inline void marshall(::dsn::binary_writer& writer, const configuration_query_by_index_request& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<configuration_query_by_index_request>(&proto, val, &configuration_query_by_index_request::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_query_by_index_request& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<configuration_query_by_index_request>(&proto, val, &configuration_query_by_index_request::read);
    };

    // ---------- configuration_query_by_index_response -------------
    inline void marshall(::dsn::binary_writer& writer, const configuration_query_by_index_response& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<configuration_query_by_index_response>(&proto, val, &configuration_query_by_index_response::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_query_by_index_response& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<configuration_query_by_index_response>(&proto, val, &configuration_query_by_index_response::read);
    };

    // ---------- query_replica_decree_request -------------
    inline void marshall(::dsn::binary_writer& writer, const query_replica_decree_request& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<query_replica_decree_request>(&proto, val, &query_replica_decree_request::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ query_replica_decree_request& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<query_replica_decree_request>(&proto, val, &query_replica_decree_request::read);
    };

    // ---------- query_replica_decree_response -------------
    inline void marshall(::dsn::binary_writer& writer, const query_replica_decree_response& val)
    {
        boost::shared_ptr< ::dsn::binary_writer_transport> transport(new ::dsn::binary_writer_transport(writer));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::marshall_rpc_args<query_replica_decree_response>(&proto, val, &query_replica_decree_response::write);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ query_replica_decree_response& val)
    {
        boost::shared_ptr< ::dsn::binary_reader_transport> transport(new ::dsn::binary_reader_transport(reader));
        ::apache::thrift::protocol::TBinaryProtocol proto(transport);
        ::dsn::unmarshall_rpc_args<query_replica_decree_response>(&proto, val, &query_replica_decree_response::read);
    };

} } 


# else // use rDSN's data encoding/decoding

namespace dsn { namespace replication { 
    // ---------- partition_status -------------
    enum partition_status
    {
        PS_INACTIVE = 0,
        PS_ERROR = 1,
        PS_PRIMARY = 2,
        PS_SECONDARY = 3,
        PS_POTENTIAL_SECONDARY = 4,
        PS_INVALID = 5,
    };

    DEFINE_POD_SERIALIZATION(partition_status);

    // ---------- read_semantic_t -------------
    enum read_semantic_t
    {
        ReadLastUpdate = 0,
        ReadOutdated = 1,
        ReadSnapshot = 2,
    };

    DEFINE_POD_SERIALIZATION(read_semantic_t);

    // ---------- learn_type -------------
    enum learn_type
    {
        LT_NONE = 0,
        LT_CACHE = 1,
        LT_APP = 2,
        LT_LOG = 3,
    };

    DEFINE_POD_SERIALIZATION(learn_type);

    // ---------- learner_status -------------
    enum learner_status
    {
        LearningWithoutPrepare = 0,
        LearningWithPrepareTransient = 1,
        LearningWithPrepare = 2, // stage 2, preare while checkpinting
        LearningSucceeded = 3,
        LearningFailed = 4,
        Learning_INVALID = 5,
    };

    DEFINE_POD_SERIALIZATION(learner_status);

    // ---------- config_type -------------
    enum config_type
    {
        CT_NONE = 0,
        CT_ASSIGN_PRIMARY = 1,
        CT_UPGRADE_TO_PRIMARY = 2,
        CT_ADD_SECONDARY = 3,
        CT_DOWNGRADE_TO_SECONDARY = 4,
        CT_DOWNGRADE_TO_INACTIVE = 5,
        CT_REMOVE = 6,
        CT_UPGRADE_TO_SECONDARY = 7,
    };

    DEFINE_POD_SERIALIZATION(config_type);

    // ---------- app_status -------------
    enum class app_status
    {
        available = 0,
        creating = 1,
        creating_failed = 2,
        dropping = 3,
        dropping_failed = 4,
        dropped = 5,
        all = 6,
        invalid = 7,
    };

    DEFINE_POD_SERIALIZATION(app_status);


    // ---------- global_partition_id -------------
    struct global_partition_id
    {
        int32_t app_id;
        int32_t pidx;
    };

    DEFINE_POD_SERIALIZATION(global_partition_id)

    // ---------- mutation_header -------------
    struct mutation_header
    {
        global_partition_id gpid;
        int64_t ballot;
        int64_t decree;
        int64_t log_offset;
        int64_t last_committed_decree;
    };

    DEFINE_POD_SERIALIZATION(mutation_header)

    // ---------- mutation_data -------------
    struct mutation_data
    {
        mutation_header header;
        std::vector< ::dsn::blob> updates;
    };

    inline void marshall(::dsn::binary_writer& writer, const mutation_data& val)
    {
        marshall(writer, val.header);
        marshall(writer, val.updates.size());
        for (const auto& bb : val.updates)
        {
            marshall(writer, bb.length());
        }
        for (const auto& bb : val.updates)
        {
            writer.write(bb.data(), bb.length());
        }
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ mutation_data& val)
    {
        unmarshall(reader, val.header);
        decltype(val.updates.size()) size;
        unmarshall(reader, size);
        val.updates.resize(size);
        std::vector<decltype(val.updates.front().length())>  lengths(size, 0);
        for (auto& length : lengths)
        {
            unmarshall(reader, length);
        }
        for (size_t i = 0; i < size; i ++)
        {
            std::shared_ptr<char> holder(new char[lengths[i]]);
            reader.read(holder.get(), lengths[i]);
            val.updates[i].assign(holder, 0, lengths[i]);
        }
    };

    // ---------- partition_configuration -------------
    struct partition_configuration
    {
        std::string app_type;
        global_partition_id gpid;
        int64_t ballot;
        int32_t max_replica_count;
        ::dsn::rpc_address primary;
        std::vector< ::dsn::rpc_address> secondaries;
        std::vector< ::dsn::rpc_address> last_drops;
        int64_t last_committed_decree;
    };

    inline void marshall(::dsn::binary_writer& writer, const partition_configuration& val)
    {
        marshall(writer, val.app_type);
        marshall(writer, val.gpid);
        marshall(writer, val.ballot);
        marshall(writer, val.max_replica_count);
        marshall(writer, val.primary);
        marshall(writer, val.secondaries);
        marshall(writer, val.last_drops);
        marshall(writer, val.last_committed_decree);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ partition_configuration& val)
    {
        unmarshall(reader, val.app_type);
        unmarshall(reader, val.gpid);
        unmarshall(reader, val.ballot);
        unmarshall(reader, val.max_replica_count);
        unmarshall(reader, val.primary);
        unmarshall(reader, val.secondaries);
        unmarshall(reader, val.last_drops);
        unmarshall(reader, val.last_committed_decree);
    };

    // ---------- replica_configuration -------------
    struct replica_configuration
    {
        global_partition_id gpid;
        int64_t ballot;
        ::dsn::rpc_address primary;
        partition_status status;
        uint64_t         learner_signature;
    };

    DEFINE_POD_SERIALIZATION(replica_configuration)

    // ---------- prepare_msg -------------
    struct prepare_msg
    {
        replica_configuration config;
        mutation_data mu;
    };

    inline void marshall(::dsn::binary_writer& writer, const prepare_msg& val)
    {
        marshall(writer, val.config);
        marshall(writer, val.mu);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ prepare_msg& val)
    {
        unmarshall(reader, val.config);
        unmarshall(reader, val.mu);
    };

    // ---------- read_request_header -------------
    struct read_request_header
    {
        global_partition_id gpid;
        std::string code;
        read_semantic_t semantic;
        int64_t version_decree;
    };

    inline void marshall(::dsn::binary_writer& writer, const read_request_header& val)
    {
        marshall(writer, val.gpid);
        marshall(writer, val.code);
        marshall(writer, val.semantic);
        marshall(writer, val.version_decree);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ read_request_header& val)
    {
        unmarshall(reader, val.gpid);
        unmarshall(reader, val.code);
        unmarshall(reader, val.semantic);
        unmarshall(reader, val.version_decree);
    };

    // ---------- write_request_header -------------
    struct write_request_header
    {
        global_partition_id gpid;
        std::string code;
    };

    inline void marshall(::dsn::binary_writer& writer, const write_request_header& val)
    {
        marshall(writer, val.gpid);
        marshall(writer, val.code);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ write_request_header& val)
    {
        unmarshall(reader, val.gpid);
        unmarshall(reader, val.code);
    };

    // ---------- rw_response_header -------------
    struct rw_response_header
    {
        ::dsn::error_code err;
    };

    inline void marshall(::dsn::binary_writer& writer, const rw_response_header& val)
    {
        marshall(writer, val.err);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ rw_response_header& val)
    {
        unmarshall(reader, val.err);
    };

    // ---------- prepare_ack -------------
    struct prepare_ack
    {
        global_partition_id gpid;
        ::dsn::error_code err;
        int64_t ballot;
        int64_t decree;
        int64_t last_committed_decree_in_app;
        int64_t last_committed_decree_in_prepare_list;
    };

    inline void marshall(::dsn::binary_writer& writer, const prepare_ack& val)
    {
        marshall(writer, val.gpid);
        marshall(writer, val.err);
        marshall(writer, val.ballot);
        marshall(writer, val.decree);
        marshall(writer, val.last_committed_decree_in_app);
        marshall(writer, val.last_committed_decree_in_prepare_list);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ prepare_ack& val)
    {
        unmarshall(reader, val.gpid);
        unmarshall(reader, val.err);
        unmarshall(reader, val.ballot);
        unmarshall(reader, val.decree);
        unmarshall(reader, val.last_committed_decree_in_app);
        unmarshall(reader, val.last_committed_decree_in_prepare_list);
    };

    // ---------- learn_state -------------
    struct learn_state
    {
        int64_t from_decree_excluded;
        int64_t to_decree_included;
        std::vector< ::dsn::blob>  meta;
        std::vector< std::string>  files;
    };

    inline void marshall(::dsn::binary_writer& writer, const learn_state& val)
    {
        marshall(writer, val.from_decree_excluded);
        marshall(writer, val.to_decree_included);
        marshall(writer, val.meta);
        marshall(writer, val.files);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ learn_state& val)
    {
        unmarshall(reader, val.from_decree_excluded);
        unmarshall(reader, val.to_decree_included);
        unmarshall(reader, val.meta);
        unmarshall(reader, val.files);
    };

    // ---------- learn_request -------------
    struct learn_request
    {
        global_partition_id gpid;
        ::dsn::rpc_address learner;
        int64_t signature;
        int64_t last_committed_decree_in_app;
        int64_t last_committed_decree_in_prepare_list;
        ::dsn::blob app_specific_learn_request;
    };

    inline void marshall(::dsn::binary_writer& writer, const learn_request& val)
    {
        marshall(writer, val.gpid);
        marshall(writer, val.learner);
        marshall(writer, val.signature);
        marshall(writer, val.last_committed_decree_in_app);
        marshall(writer, val.last_committed_decree_in_prepare_list);
        marshall(writer, val.app_specific_learn_request);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ learn_request& val)
    {
        unmarshall(reader, val.gpid);
        unmarshall(reader, val.learner);
        unmarshall(reader, val.signature);
        unmarshall(reader, val.last_committed_decree_in_app);
        unmarshall(reader, val.last_committed_decree_in_prepare_list);
        unmarshall(reader, val.app_specific_learn_request);
    };

    // ---------- learn_response -------------
    struct learn_response
    {
        ::dsn::error_code err;
        replica_configuration config;
        int64_t last_committed_decree;
        int64_t prepare_start_decree;
        learn_type type;
        learn_state state;
        rpc_address address;
        std::string base_local_dir;
    };

    inline void marshall(::dsn::binary_writer& writer, const learn_response& val)
    {
        marshall(writer, val.err);
        marshall(writer, val.config);
        marshall(writer, val.last_committed_decree);
        marshall(writer, val.prepare_start_decree);
        marshall(writer, val.type);
        marshall(writer, val.state);
        marshall(writer, val.address);
        marshall(writer, val.base_local_dir);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ learn_response& val)
    {
        unmarshall(reader, val.err);
        unmarshall(reader, val.config);
        unmarshall(reader, val.last_committed_decree);
        unmarshall(reader, val.prepare_start_decree);
        unmarshall(reader, val.type);
        unmarshall(reader, val.state);
        unmarshall(reader, val.address);
        unmarshall(reader, val.base_local_dir);
    };

    // ---------- group_check_request -------------
    struct group_check_request
    {
        std::string app_type;
        ::dsn::rpc_address node;
        replica_configuration config;
        int64_t last_committed_decree;
    };

    inline void marshall(::dsn::binary_writer& writer, const group_check_request& val)
    {
        marshall(writer, val.app_type);
        marshall(writer, val.node);
        marshall(writer, val.config);
        marshall(writer, val.last_committed_decree);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ group_check_request& val)
    {
        unmarshall(reader, val.app_type);
        unmarshall(reader, val.node);
        unmarshall(reader, val.config);
        unmarshall(reader, val.last_committed_decree);
    };

    // ---------- group_check_response -------------
    struct group_check_response
    {
        global_partition_id gpid;
        ::dsn::error_code err;
        int64_t last_committed_decree_in_app;
        int64_t last_committed_decree_in_prepare_list;
        learner_status learner_status_;
        int64_t learner_signature;
        ::dsn::rpc_address node;
    };

    inline void marshall(::dsn::binary_writer& writer, const group_check_response& val)
    {
        marshall(writer, val.gpid);
        marshall(writer, val.err);
        marshall(writer, val.last_committed_decree_in_app);
        marshall(writer, val.last_committed_decree_in_prepare_list);
        marshall(writer, val.learner_status_);
        marshall(writer, val.learner_signature);
        marshall(writer, val.node);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ group_check_response& val)
    {
        unmarshall(reader, val.gpid);
        unmarshall(reader, val.err);
        unmarshall(reader, val.last_committed_decree_in_app);
        unmarshall(reader, val.last_committed_decree_in_prepare_list);
        unmarshall(reader, val.learner_status_);
        unmarshall(reader, val.learner_signature);
        unmarshall(reader, val.node);
    };

    // ---------- app_info -------------
    struct app_info
    {
        app_status status;
        std::string app_type;
        std::string app_name;
        int32_t app_id;
        int32_t partition_count;
    };

    inline void marshall(::dsn::binary_writer& writer, const app_info& val)
    {
        marshall(writer, val.status);
        marshall(writer, val.app_type);
        marshall(writer, val.app_name);
        marshall(writer, val.app_id);
        marshall(writer, val.partition_count);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ app_info& val)
    {
        unmarshall(reader, val.status);
        unmarshall(reader, val.app_type);
        unmarshall(reader, val.app_name);
        unmarshall(reader, val.app_id);
        unmarshall(reader, val.partition_count);
    };

    // ---------- meta_request_header -------------
    struct meta_request_header
    {
        std::string rpc_tag;
    };

    inline void marshall(::dsn::binary_writer& writer, const meta_request_header& val)
    {
        marshall(writer, val.rpc_tag);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ meta_request_header& val)
    {
        unmarshall(reader, val.rpc_tag);
    };

    // ---------- meta_response_header -------------
    struct meta_response_header
    {
        ::dsn::error_code err;
        ::dsn::rpc_address primary_address;
    };

    inline void marshall(::dsn::binary_writer& writer, const meta_response_header& val)
    {
        marshall(writer, val.err);
        marshall(writer, val.primary_address);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ meta_response_header& val)
    {
        unmarshall(reader, val.err);
        unmarshall(reader, val.primary_address);
    };

    // ---------- configuration_update_request -------------
    struct configuration_update_request
    {
        partition_configuration config;
        config_type type;
        ::dsn::rpc_address node;
    };

    inline void marshall(::dsn::binary_writer& writer, const configuration_update_request& val)
    {
        marshall(writer, val.config);
        marshall(writer, val.type);
        marshall(writer, val.node);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_update_request& val)
    {
        unmarshall(reader, val.config);
        unmarshall(reader, val.type);
        unmarshall(reader, val.node);
    };

    // ---------- configuration_update_response -------------
    struct configuration_update_response
    {
        ::dsn::error_code err;
        partition_configuration config;
    };

    inline void marshall(::dsn::binary_writer& writer, const configuration_update_response& val)
    {
        marshall(writer, val.err);
        marshall(writer, val.config);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_update_response& val)
    {
        unmarshall(reader, val.err);
        unmarshall(reader, val.config);
    };

    // ---------- configuration_proposal_request -------------
    struct configuration_proposal_request
    {
        partition_configuration config;
        config_type type;
        ::dsn::rpc_address node;
        bool is_clean_data;
        bool is_upgrade;
    };

    inline void marshall(::dsn::binary_writer& writer, const configuration_proposal_request& val)
    {
        marshall(writer, val.config);
        marshall(writer, val.type);
        marshall(writer, val.node);
        marshall(writer, val.is_clean_data);
        marshall(writer, val.is_upgrade);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_proposal_request& val)
    {
        unmarshall(reader, val.config);
        unmarshall(reader, val.type);
        unmarshall(reader, val.node);
        unmarshall(reader, val.is_clean_data);
        unmarshall(reader, val.is_upgrade);
    };

    // ---------- create_app_options -------------
    struct create_app_options
    {
        int32_t partition_count;
        int32_t replica_count;
        bool success_if_exist;
        std::string app_type;
    };

    inline void marshall(::dsn::binary_writer& writer, const create_app_options& val)
    {
        marshall(writer, val.partition_count);
        marshall(writer, val.replica_count);
        marshall(writer, val.success_if_exist);
        marshall(writer, val.app_type);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ create_app_options& val)
    {
        unmarshall(reader, val.partition_count);
        unmarshall(reader, val.replica_count);
        unmarshall(reader, val.success_if_exist);
        unmarshall(reader, val.app_type);
    };

    // ---------- configuration_create_app_request -------------
    struct configuration_create_app_request
    {
        std::string app_name;
        create_app_options options;
    };

    inline void marshall(::dsn::binary_writer& writer, const configuration_create_app_request& val)
    {
        marshall(writer, val.app_name);
        marshall(writer, val.options);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_create_app_request& val)
    {
        unmarshall(reader, val.app_name);
        unmarshall(reader, val.options);
    };

    // ---------- drop_app_options -------------
    struct drop_app_options
    {
        bool success_if_not_exist;
    };

    inline void marshall(::dsn::binary_writer& writer, const drop_app_options& val)
    {
        marshall(writer, val.success_if_not_exist);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ drop_app_options& val)
    {
        unmarshall(reader, val.success_if_not_exist);
    };

    // ---------- configuration_drop_app_request -------------
    struct configuration_drop_app_request
    {
        std::string app_name;
        drop_app_options options;
    };

    inline void marshall(::dsn::binary_writer& writer, const configuration_drop_app_request& val)
    {
        marshall(writer, val.app_name);
        marshall(writer, val.options);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_drop_app_request& val)
    {
        unmarshall(reader, val.app_name);
        unmarshall(reader, val.options);
    };

    // ---------- configuration_list_apps_request -------------
    struct configuration_list_apps_request
    {
        app_status status;
    };

    inline void marshall(::dsn::binary_writer& writer, const configuration_list_apps_request& val)
    {
        marshall(writer, val.status);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_list_apps_request& val)
    {
        unmarshall(reader, val.status);
    };


    // ---------- configuration_create_app_response -------------
    struct configuration_create_app_response
    {
        ::dsn::error_code err;
        int32_t appid;
    };

    inline void marshall(::dsn::binary_writer& writer, const configuration_create_app_response& val)
    {
        marshall(writer, val.err);
        marshall(writer, val.appid);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_create_app_response& val)
    {
        unmarshall(reader, val.err);
        unmarshall(reader, val.appid);
    };

    // ---------- configuration_drop_app_response -------------
    struct configuration_drop_app_response
    {
        ::dsn::error_code err;
    };

    inline void marshall(::dsn::binary_writer& writer, const configuration_drop_app_response& val)
    {
        marshall(writer, val.err);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_drop_app_response& val)
    {
        unmarshall(reader, val.err);
    };

    // ---------- configuration_query_by_node_request -------------
    struct configuration_query_by_node_request
    {
        ::dsn::rpc_address node;
    };

    inline void marshall(::dsn::binary_writer& writer, const configuration_query_by_node_request& val)
    {
        marshall(writer, val.node);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_query_by_node_request& val)
    {
        unmarshall(reader, val.node);
    };

    // ---------- configuration_list_apps_response -------------
    struct configuration_list_apps_response
    {
        ::dsn::error_code err;
        std::vector< app_info> infos;
    };

    inline void marshall(::dsn::binary_writer& writer, const configuration_list_apps_response& val)
    {
        marshall(writer, val.err);
        marshall(writer, val.infos);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_list_apps_response& val)
    {
        unmarshall(reader, val.err);
        unmarshall(reader, val.infos);
    };

    // ---------- configuration_query_by_node_response -------------
    struct configuration_query_by_node_response
    {
        ::dsn::error_code err;
        std::vector< partition_configuration> partitions;
    };

    inline void marshall(::dsn::binary_writer& writer, const configuration_query_by_node_response& val)
    {
        marshall(writer, val.err);
        marshall(writer, val.partitions);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_query_by_node_response& val)
    {
        unmarshall(reader, val.err);
        unmarshall(reader, val.partitions);
    };

    // ---------- configuration_query_by_index_request -------------
    struct configuration_query_by_index_request
    {
        std::string app_name;
        std::vector< int32_t> partition_indices;
    };

    inline void marshall(::dsn::binary_writer& writer, const configuration_query_by_index_request& val)
    {
        marshall(writer, val.app_name);
        marshall(writer, val.partition_indices);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_query_by_index_request& val)
    {
        unmarshall(reader, val.app_name);
        unmarshall(reader, val.partition_indices);
    };

    // ---------- configuration_query_by_index_response -------------
    struct configuration_query_by_index_response
    {
        ::dsn::error_code err;
        int32_t app_id;
        int32_t partition_count;
        std::vector< partition_configuration> partitions;
    };

    inline void marshall(::dsn::binary_writer& writer, const configuration_query_by_index_response& val)
    {
        marshall(writer, val.err);
        marshall(writer, val.app_id);
        marshall(writer, val.partition_count);
        marshall(writer, val.partitions);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ configuration_query_by_index_response& val)
    {
        unmarshall(reader, val.err);
        unmarshall(reader, val.app_id);
        unmarshall(reader, val.partition_count);
        unmarshall(reader, val.partitions);
    };

    // ---------- query_replica_decree_request -------------
    struct query_replica_decree_request
    {
        global_partition_id gpid;
        ::dsn::rpc_address node;
    };

    inline void marshall(::dsn::binary_writer& writer, const query_replica_decree_request& val)
    {
        marshall(writer, val.gpid);
        marshall(writer, val.node);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ query_replica_decree_request& val)
    {
        unmarshall(reader, val.gpid);
        unmarshall(reader, val.node);
    };

    // ---------- query_replica_decree_response -------------
    struct query_replica_decree_response
    {
        ::dsn::error_code err;
        int64_t last_decree;
    };

    inline void marshall(::dsn::binary_writer& writer, const query_replica_decree_response& val)
    {
        marshall(writer, val.err);
        marshall(writer, val.last_decree);
    };

    inline void unmarshall(::dsn::binary_reader& reader, /*out*/ query_replica_decree_response& val)
    {
        unmarshall(reader, val.err);
        unmarshall(reader, val.last_decree);
    };

} } 

#endif 
