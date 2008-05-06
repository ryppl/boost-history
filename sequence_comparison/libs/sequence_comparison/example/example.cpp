// Copyright 2008 Chris Goller, Jeff Flinn, Brook Milligan and Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


//[ sequence_comparison_example

#include <boost/sequence_comparison.hpp>
#include <boost/assert.hpp>

void sequence_comparison_example()
{
    boost::sequence_comparison::sequence_comparison<int> object;
    int x = 0;

    BOOST_ASSERT(object.get_nothing_count()==0);
    object.nothing(x);
    BOOST_ASSERT(object.get_nothing_count()==1);

    BOOST_ASSERT(object.get_call_count()==1);
    object.nothing(x,4);
    BOOST_ASSERT(object.get_call_count()==2);
    BOOST_ASSERT(object.get_nothing_count()==5);

} // end void sequence_comparison_example

int main()
{
    sequence_comparison_example();
}
//]
