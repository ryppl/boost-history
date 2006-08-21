//
// Boost.Process
// Miscellaneous utilities for the tests.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// This code may be used under either of the following two licences:
//
//      Permission is hereby granted, free of charge, to any person
//      obtaining a copy of this software and associated documentation
//      files (the "Software"), to deal in the Software without
//      restriction, including without limitation the rights to use,
//      copy, modify, merge, publish, distribute, sublicense, and/or
//      sell copies of the Software, and to permit persons to whom the
//      Software is furnished to do so, subject to the following
//      conditions:
//
//      The above copyright notice and this permission notice shall be
//      included in all copies or substantial portions of the Software.
//
//      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//      OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//      NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//      HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//      WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//      FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//      OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
// Or:
//
//      Distributed under the Boost Software License, Version 1.0.
//      (See accompanying file LICENSE_1_0.txt or copy at
//      http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/process/config.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/test/unit_test.hpp>

// ------------------------------------------------------------------------

enum bp_api_type {
    posix_api,
    win32_api,
}
#if defined(BOOST_PROCESS_POSIX_API)
bp_api_type = posix_api;
#elif defined(BOOST_PROCESS_WIN32_API)
bp_api_type = win32_api;
#else
#   error "Unsupported platform."
#endif

// ------------------------------------------------------------------------

inline
std::string
get_helpers_path(void)
{
    boost::filesystem::path hp = boost::filesystem::initial_path();

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    hp /= "./bin/msvc/debug/link-static/runtime-link-static/helpers.exe";
#elif defined(__APPLE__)
    hp /= "./bin/darwin/debug/link-static/runtime-link-static/helpers";
#else
    hp /= "./bin/gcc/debug/link-static/runtime-link-static/helpers";
#endif

    BOOST_REQUIRE(boost::filesystem::exists(hp));

    return hp.string();
}

// ------------------------------------------------------------------------

inline
std::istream&
portable_getline(std::istream& is, std::string& str)
{
    std::getline(is, str);
    std::string::size_type pos = str.rfind('\r');
    if (pos != std::string::npos)
        str.erase(pos);
    return is;
}
