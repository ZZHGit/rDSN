/*
 * The MIT License (MIT)

 * Copyright (c) 2015 Microsoft Corporation, Robust Distributed System Nucleus(rDSN)

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once

# include <dsn/tool_api.h>

namespace dsn {
    namespace tools {

        class wrong_perf_counter : public perf_counter
        {
        public:
            wrong_perf_counter(const char *section, const char *name, perf_counter_type type)
                : perf_counter(section, name, type)
            {}
            ~wrong_perf_counter(void) {}

            virtual void   increment() { _val++; }
            virtual void   decrement() { _val--; }
            virtual void   add(uint64_t val) { _val += val; }
            virtual void   set(uint64_t val) { _val = val; } // sample
            virtual double get_value() { return static_cast<double>(_val.load()); }
            virtual double get_percentile(counter_percentile_type type) { return 0.0; }

        private:
            std::atomic<uint64_t> _val;
        };

    }
}

