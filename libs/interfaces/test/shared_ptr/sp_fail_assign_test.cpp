// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// This test should compile and execute successfully for both static_move_ptr
// and dynamic_move_ptr.

#include <boost/interfaces/shared_ptr.hpp>
#include "../detail/hierarchies.hpp"

int main()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;
    shared_ptr<ifirst> ptr(new cfirst<1>);
    cfirst<1> first;
    *ptr = first;
}
