// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// This test should fail to compile for both static_move_ptr and 
// dynamic_move_ptr.

#include <boost/interfaces/unique_obj.hpp>
#include "../detail/hierarchies.hpp"

using namespace boost::interfaces;
using namespace boost::interfaces::test;

unique_obj<ifirst> source()
{
    return unique_obj<ifirst>(new cfirst<1>);
}

void sink(const unique_obj<ifirst> obj)
{
    unique_obj<ifirst> obj2 = obj;
}

int main()
{
    sink(source());
}
