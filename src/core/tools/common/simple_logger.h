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

# pragma once

# include <dsn/tool_api.h>
# include <thread>
# include <cstdio>

namespace dsn {
    namespace tools {


        class screen_logger : public logging_provider
        {
        public:
            screen_logger(const char* log_dir);
            virtual ~screen_logger(void);

            virtual void dsn_logv(const char *file,
                const char *function,
                const int line,
                dsn_log_level_t log_level,
                const char* title,
                const char *fmt,
                va_list args
                );

            virtual void flush();

        private:
            ::dsn::utils::ex_lock_nr _lock;
            bool _short_header;
        };


        class simple_logger : public logging_provider
        {
        public:
            simple_logger(const char* log_dir);
            virtual ~simple_logger(void);

            virtual void dsn_logv(const char *file,
                const char *function,
                const int line,
                dsn_log_level_t log_level,
                const char* title,
                const char *fmt,
                va_list args
                );

            virtual void flush();

        private:
            void create_log_file();

        private:
            std::string _log_dir;
            ::dsn::utils::ex_lock_nr _lock;
            FILE* _log;
            int _start_index;
            int _index;
            int _lines;
            bool _short_header;
            bool _fast_flush;
            dsn_log_level_t _stderr_start_level;
        };

    }
}
