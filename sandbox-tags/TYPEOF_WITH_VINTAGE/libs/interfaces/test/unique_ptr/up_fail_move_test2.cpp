// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

#include <boost/interfaces/unique_ptr.hpp>
#include "../detail/hierarchies.hpp"

using namespace boost::interfaces;
using namespace boost::interfaces::test;

unique_ptr<ifirst> source()
{
    return unique_ptr<ifirst>(new cfirst<1>);
}

void sink(unique_ptr<ifirst> ptr)
{
    unique_ptr<ifirst> ptr2 = ptr;
}

int main()
{
    sink(source());
}
