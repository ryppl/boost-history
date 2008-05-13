// Copyright 2008 Chintan Rao H.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


//[ dsearch_example

#include <boost/dsearch.hpp>
#include <boost/assert.hpp>

void dsearch_example()
{
    boost::dsearch::dsearch<int> object;
    int x = 0;

    BOOST_ASSERT(object.get_nothing_count()==0);
    object.nothing(x);
    BOOST_ASSERT(object.get_nothing_count()==1);

    BOOST_ASSERT(object.get_call_count()==1);
    object.nothing(x,4);
    BOOST_ASSERT(object.get_call_count()==2);
    BOOST_ASSERT(object.get_nothing_count()==5);

} // end void dsearch_example

int main()
{
    dsearch_example();
}
//]
