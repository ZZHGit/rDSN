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

# include <iostream>
# include "gtest/gtest.h"
# include "test_utils.h"

# include "hpc_aio_provider_for_test.h"

extern void dsn_core_init();
extern void task_engine_module_init();
extern void command_manager_module_init();

int g_test_count = 0;
int g_test_ret = 0;

GTEST_API_ int main(int argc, char **argv) 
{
    testing::InitGoogleTest(&argc, argv);

    dsn::tools::register_component_provider<dsn::test::hpc_aio_provider_for_test>("dsn::test::hpc_aio_provider_for_test");

    // register all tools
    dsn_core_init();
    task_engine_module_init();
    command_manager_module_init();

    // register all possible services
    dsn::register_app<test_client>("test");
    
    // specify what services and tools will run in config file, then run
    dsn_run(argc, argv, false);

    // run in-rDSN tests
    while (g_test_count == 0)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (g_test_ret != 0)
    {
#ifndef ENABLE_GCOV
        dsn_exit(g_test_ret);
#endif
        return g_test_ret;
    }

    /*
    // run out-rDSN tests in Main thread
    dsn_mimic_app("client", 1);
    std::cout << "=========================================================== " << std::endl;
    std::cout << "================== run in Main thread ===================== " << std::endl;
    std::cout << "=========================================================== " << std::endl;
    exec_tests();
    if (g_test_ret != 0)
    {
        return g_test_ret;
    }
    */

    // run out-rDSN tests in other threads
    std::cout << "=========================================================== " << std::endl;
    std::cout << "================== run in non-rDSN threads ================ " << std::endl;
    std::cout << "=========================================================== " << std::endl;
    std::thread t([](){
        dsn_mimic_app("client", 1);
        exec_tests();
    });
    t.join();
    if (g_test_ret != 0)
    {
#ifndef ENABLE_GCOV
        dsn_exit(g_test_ret);
#endif
        return g_test_ret;
    }
    
    // exit without any destruction
#ifndef ENABLE_GCOV
    dsn_exit(0);
#endif
    return 0;    
}
