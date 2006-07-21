//
// Boost.Process
// Miscellaneous utilities for the tests.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/test/unit_test.hpp>

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
