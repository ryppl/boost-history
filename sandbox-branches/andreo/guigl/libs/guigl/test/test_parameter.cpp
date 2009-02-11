/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/guigl/parameters.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace boost::guigl;

struct size_and_label
{
    typedef argument_pack<tag::label, tag::size>::type argument_pack_type;

    size_and_label(const argument_pack_type &args)
    {
        label = args[_label];
        size = args[_size];
    }
    std::string label;
    size_type size;
};

BOOST_AUTO_TEST_CASE( test_parameter)
{
    size_and_label one(( _label = "name 1", _size = size_type(1, 2) ));
    size_and_label two(( _label = "name 2" ));
    
    BOOST_CHECK_EQUAL(one.size.x, 1);
    BOOST_CHECK_EQUAL(tag::size::default_value().x, 200);
    BOOST_CHECK_EQUAL(two.size.x, 200);
    
}