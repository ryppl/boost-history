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

static
std::string
get_helpers_path(void)
{
    boost::filesystem::path hp = boost::filesystem::initial_path();

#if defined(__APPLE__)
    hp /= "./bin/darwin/debug/link-static/runtime-link-static/helpers";
#else
    hp /= "./bin/gcc/debug/link-static/runtime-link-static/helpers";
#endif

    BOOST_REQUIRE(boost::filesystem::exists(hp));

    return hp.string();
}
