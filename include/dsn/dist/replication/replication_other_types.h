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

/**
 * Autogenerated by Thrift Compiler (@PACKAGE_VERSION@)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef replication_OTHER_TYPES_H
#define replication_OTHER_TYPES_H

# include <dsn/cpp/autoref_ptr.h>
# include <dsn/dist/replication/replication.types.h>
# include <sstream>
# include <dsn/cpp/json_helper.h>
# include <dsn/internal/enum_helper.h>

namespace dsn {
    namespace replication {

        typedef int32_t app_id;
        typedef int64_t ballot;
        typedef int64_t decree;

        #define invalid_ballot ((::dsn::replication::ballot)-1LL)
        #define invalid_decree ((::dsn::replication::decree)-1LL)
        #define invalid_offset (-1LL)
        #define invalid_signature 0

        class replica;
        typedef dsn::ref_ptr<replica> replica_ptr;
                    
        class replica_stub;
        typedef dsn::ref_ptr<replica_stub> replica_stub_ptr;

        class mutation;
        typedef dsn::ref_ptr<mutation> mutation_ptr;

        class mutation_log;
        typedef dsn::ref_ptr<mutation_log> mutation_log_ptr;


        ENUM_BEGIN(partition_status, PS_INVALID)
            ENUM_REG(PS_INACTIVE)
            ENUM_REG(PS_ERROR)
            ENUM_REG(PS_PRIMARY)
            ENUM_REG(PS_SECONDARY)
            ENUM_REG(PS_POTENTIAL_SECONDARY)
        ENUM_END(partition_status)

        ENUM_BEGIN(learn_type, LT_NONE)
            ENUM_REG(LT_CACHE)
            ENUM_REG(LT_APP)
            ENUM_REG(LT_LOG)
        ENUM_END(learn_type)

        ENUM_BEGIN(learner_status, Learning_INVALID)
            ENUM_REG(LearningWithoutPrepare)
            ENUM_REG(LearningWithPrepareTransient)
            ENUM_REG(LearningWithPrepare)
            ENUM_REG(LearningSucceeded)
            ENUM_REG(LearningFailed)
        ENUM_END(learner_status)

        ENUM_BEGIN(config_type, CT_NONE)
            ENUM_REG(CT_ASSIGN_PRIMARY)
            ENUM_REG(CT_UPGRADE_TO_PRIMARY)
            ENUM_REG(CT_ADD_SECONDARY)
            ENUM_REG(CT_DOWNGRADE_TO_SECONDARY)
            ENUM_REG(CT_DOWNGRADE_TO_INACTIVE)
            ENUM_REG(CT_REMOVE)
            ENUM_REG(CT_UPGRADE_TO_SECONDARY)
        ENUM_END(config_type)

        ENUM_BEGIN(app_status, app_status::invalid)
            ENUM_REG(app_status::available)
            ENUM_REG(app_status::creating)
            ENUM_REG(app_status::creating_failed)
            ENUM_REG(app_status::dropping)
            ENUM_REG(app_status::dropping_failed)
            ENUM_REG(app_status::dropped)
            ENUM_REG(app_status::all)
        ENUM_END(app_status)

        inline void json_encode(std::stringstream& out, const partition_configuration& config)
        {
            JSON_DICT_ENTRIES(out, config, app_type, gpid, ballot, max_replica_count, primary, secondaries, last_drops, last_committed_decree);
        }
        inline void json_encode(std::stringstream& out, const partition_status& status)
        {
            json_encode(out, enum_to_string(status));
        }
        inline void json_encode(std::stringstream& out, const replica_configuration& config)
        {
            JSON_DICT_ENTRIES(out, config, gpid, ballot, primary, status, learner_signature);
        }
        
        inline void json_encode(std::stringstream& out, const global_partition_id& gpid)
        {
            JSON_DICT_ENTRIES(out, gpid, app_id, pidx);
        }

        inline void json_encode(std::stringstream& out, const app_status& status)
        {
            out << "\"" << enum_to_string(status) << "\"";
        }

    }

    inline void json_encode(std::stringstream& out, const dsn::rpc_address& address)
    {
        out << "\"" << address.to_string() << "\"";
    }
} // namespace

#endif
