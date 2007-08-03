// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/support.hpp>

using namespace boost;

struct dummy_producer_tag;
struct dummy_consumer_tag;

struct test
{
    typedef dummy_producer_tag producer_category;
    typedef dummy_consumer_tag consumer_category;
};

int main()
{
    test t1, t2;
    
    //boost::dataflow::connect(t1, t2);
}
