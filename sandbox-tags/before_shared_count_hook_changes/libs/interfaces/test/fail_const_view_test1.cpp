// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.

// Disclaimer: Not a Boost library.

#include <boost/interfaces.hpp>
#include <boost/interfaces/const_view.hpp>
#include "./detail/hierarchies.hpp"

int main()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;

    // Test initializing ifirst.
    cfirst<1>           first1;
    const_view<ifirst>  first = first1;
    first.one(0, 0.0);
}
