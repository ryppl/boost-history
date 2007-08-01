// Copyright 2007 Daryle Walker.
// Distributed under the Boost Software License, Version 1.0. (See the
// accompanying file LICENSE_1_0.txt or a copy at
// <http://www.boost.org/LICENSE_1_0.txt>.)


//[ hat_example

#include <boost/hat.hpp>
#include <boost/assert.hpp>

void hat_example()
{
    boost::hat::hat<int> object;
    int x = 0;

    BOOST_ASSERT(object.get_nothing_count()==0);
    object.nothing(x);
    BOOST_ASSERT(object.get_nothing_count()==1);

    BOOST_ASSERT(object.get_call_count()==1);
    object.nothing(x,4);
    BOOST_ASSERT(object.get_call_count()==2);
    BOOST_ASSERT(object.get_nothing_count()==5);

} // end void hat_example

int main()
{
    hat_example();
}
//]
