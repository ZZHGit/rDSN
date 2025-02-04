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

# include <dsn/service_api_c.h>
# include <dsn/internal/logging_provider.h>
# include <dsn/tool_api.h>
# include "service_engine.h"
# include <dsn/cpp/auto_codes.h>

DSN_API dsn_log_level_t dsn_log_start_level = dsn_log_level_t::LOG_LEVEL_INFORMATION;

static void log_on_sys_exit(::dsn::sys_exit_type)
{
    ::dsn::logging_provider* logger = ::dsn::service_engine::fast_instance().logging();
    if (logger != nullptr)
    {
        logger->flush();
    }
}

void dsn_log_init()
{
    dsn_log_start_level = enum_from_string(
        dsn_config_get_value_string("core", "logging_start_level", enum_to_string(dsn_log_start_level),
            "logs with level below this will not be logged"),
        dsn_log_level_t::LOG_LEVEL_INVALID
        );

    dassert(dsn_log_start_level != dsn_log_level_t::LOG_LEVEL_INVALID, "invalid [core] logging_start_level specified");

    // register log flush on exit
    ::dsn::tools::sys_exit.put_back(log_on_sys_exit, "log.flush");
}

DSN_API dsn_log_level_t dsn_log_get_start_level()
{
    return dsn_log_start_level;
}

DSN_API void dsn_logv(const char *file, const char *function, const int line, dsn_log_level_t log_level, const char* title, const char* fmt, va_list args)
{
    ::dsn::logging_provider* logger = ::dsn::service_engine::instance().logging();
    if (logger != nullptr)
    {
        logger->dsn_logv(file, function, line, log_level, title, fmt, args);
    }
    else
    {
        printf("%s:%d:%s():", title, line, function);
        vprintf(fmt, args);
        printf("\n");
    }
}

DSN_API void dsn_logf(const char *file, const char *function, const int line, dsn_log_level_t log_level, const char* title, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    dsn_logv(file, function, line, log_level, title, fmt, ap);
    va_end(ap);
}

DSN_API void dsn_log(const char *file, const char *function, const int line, dsn_log_level_t log_level, const char* title)
{
    dsn_logf(file, function, line, log_level, title, "");
}
