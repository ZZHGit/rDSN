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
# include "mutation_log.h"
# include <gtest/gtest.h>
# include <chrono>
# include <condition_variable>

using namespace ::dsn;
using namespace ::dsn::replication;

TEST(replication, mutation_log_learn)
{
    std::chrono::steady_clock clock;
    global_partition_id gpid = { 1, 1 };
    std::string str = "hello, world!";
    std::string logp = "./test-log";

    // prepare mutations
    std::vector<mutation_ptr> mutations;
    auto time_tic = clock.now();
    for (int i = 0; i < 1000; i++)
    {
        mutation_ptr mu(new mutation());
        mu->data.header.ballot = 1;
        mu->data.header.decree = i + 2;
        mu->data.header.gpid = gpid;
        mu->data.header.last_committed_decree = i;
        mu->data.header.log_offset = 0;

        binary_writer writer;
        //we want a update mutation to be ~4kByte
        for (int j = 0; j < 300; j++)
        {
            writer.write(str);
        }
        mu->data.updates.push_back(writer.get_buffer());

        mutation::client_info ci;
        ci.code = 100;
        ci.req = nullptr;
        mu->client_requests.push_back(ci);

        mutations.push_back(mu);
    }
    auto time_toc = clock.now();
    std::cout << "prepare mutations time(us): " << std::chrono::duration_cast<std::chrono::microseconds>(time_toc - time_tic).count() << std::endl;

    decree learn_points[] = {584,585,586, 594,595,596,604,605,606 };

    for (auto& lp : learn_points)
    {
        // prepare dir
        utils::filesystem::remove_path(logp);
        utils::filesystem::create_directory(logp);

        // writing logs
        time_tic = clock.now();
        mutation_log_ptr mlog = new mutation_log(logp, 4, 32, true, gpid);
        mlog->open(nullptr);
        for (auto& mu : mutations)
        {
            mlog->append(mu, LPC_AIO_IMMEDIATE_CALLBACK, nullptr, nullptr, 0);
        }
        mlog->flush();
        time_toc = clock.now();
        std::cout << "learn_point[" << lp << "]: write time(us): " << std::chrono::duration_cast<std::chrono::microseconds>(time_toc - time_tic).count() << std::endl;

        // gc
        time_tic = clock.now();
        decree durable_decree = lp;
        mlog->garbage_collection(gpid, durable_decree, 0);
        mlog->close();
        time_toc = clock.now();
        std::cout << "learn_point[" << lp << "]: gc time(us): " << std::chrono::duration_cast<std::chrono::microseconds>(time_toc - time_tic).count() << std::endl;

        // reading logs
        time_tic = clock.now();
        mlog = new mutation_log(logp, 1, 1, true, gpid);
        mlog->open([](mutation_ptr& mu)->bool{ return true; });
        time_toc = clock.now();
        std::cout << "learn_point[" << lp << "]: read time(us): " << std::chrono::duration_cast<std::chrono::microseconds>(time_toc - time_tic).count() << std::endl;

        // learning
        time_tic = clock.now();
        learn_state state;
        mlog->get_learn_state(gpid, durable_decree + 1, state);
        mlog->close();
        mlog = nullptr;
        time_toc = clock.now();
        std::cout << "learn_point[" << lp << "]: learn time(us): " << std::chrono::duration_cast<std::chrono::microseconds>(time_toc - time_tic).count() << std::endl;

        // replaying
        time_tic = clock.now();
        int64_t offset = 0;
        std::set<decree> learned_decress;
        mutation_log::replay(state.files,
            [&mutations, &learned_decress, &clock](mutation_ptr& mu)->bool
            {
                //wait for 5 usec mimicing mutation replay time
                auto tic = clock.now();
                while (std::chrono::duration_cast<std::chrono::microseconds>(clock.now() - tic).count() < 5)
                {
                    ;
                }
                learned_decress.insert(mu->data.header.decree);

                mutation_ptr wmu = mutations[mu->data.header.decree - 2];
                EXPECT_TRUE(memcmp((const void*)&wmu->data.header,
                    (const void*)&mu->data.header,
                    sizeof(mu->data.header)) == 0
                    );
                EXPECT_TRUE(wmu->data.updates.size() == mu->data.updates.size());
                EXPECT_TRUE(wmu->data.updates[0].length() == mu->data.updates[0].length());
                EXPECT_TRUE(memcmp((const void*)wmu->data.updates[0].data(),
                    (const void*)mu->data.updates[0].data(),
                    mu->data.updates[0].length()) == 0
                    );
                EXPECT_TRUE(wmu->client_requests.size() == mu->client_requests.size());
                EXPECT_TRUE(wmu->client_requests[0].code == mu->client_requests[0].code);

                return true;
            },
            offset
            ).end_tracking();
        time_toc = clock.now();
        std::cout << "learn_point[" << lp << "]: replay time(us): " << std::chrono::duration_cast<std::chrono::microseconds>(time_toc - time_tic).count() << std::endl;

        // checking
        time_tic = clock.now();
        for (decree s = durable_decree + 1; s < 1000; s++)
        {
            auto it = learned_decress.find(s);
            ASSERT_TRUE(it != learned_decress.end());
        }
        time_toc = clock.now();
        std::cout << "learn_point[" << lp << "]: check time(us): " << std::chrono::duration_cast<std::chrono::microseconds>(time_toc - time_tic).count() << std::endl;

        // clear all
        utils::filesystem::remove_path(logp);
    }
}
