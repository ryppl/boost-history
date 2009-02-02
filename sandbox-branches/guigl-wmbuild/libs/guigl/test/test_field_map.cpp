/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/field_map/field_map.hpp>

#include <boost/typeof/typeof.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace boost;

using namespace boost::field_map;

namespace field {

    struct label
    {
        typedef std::string type;
    };
    
    struct size
    {
        typedef int type;
        
        static type default_value()
        {
            return 3;
        }
    };
    
    struct volume
    {
        typedef double type;
        
        static type default_value()
        {
            return 3.0;
        }
    };

}


BOOST_FIELD_MAP_FIELD_FUNCTION(field::label, label)
BOOST_FIELD_MAP_FIELD_FUNCTION(field::size, size)

BOOST_FIELD_MAP_FIELD_ASSIGNABLE(field::label, label_)
BOOST_FIELD_MAP_FIELD_ASSIGNABLE(field::size, size_)
BOOST_FIELD_MAP_FIELD_ASSIGNABLE(field::volume, volume_)


inline map<>::type empty()
{
    return map<>::type();
}

BOOST_AUTO_TEST_CASE( test_map ) 
{
    BOOST_AUTO(hello, label("hello"));
    BOOST_CHECK_EQUAL(hello.at<field::label>(), "hello");
    
    BOOST_AUTO(hello_1, hello&size(1));
    BOOST_CHECK_EQUAL(hello_1.at<field::label>(), "hello");
    BOOST_CHECK_EQUAL(hello_1.at<field::size>(), 1);
    
    BOOST_AUTO(one, size(1));
    BOOST_CHECK_EQUAL(one.at<field::size>(), 1);
    
    BOOST_AUTO(one_hello, one&label("hello"));
    BOOST_CHECK_EQUAL(one_hello.at<field::size>(), 1);
    BOOST_CHECK_EQUAL(one_hello.at<field::label>(), "hello");
    
//  Following does not compile
//  guigl::parameter::label("hello").label("goodbye");
//  guigl::parameter::size(1).size(1);
}


const std::string &function_taking_label(const map<field::label>::type &args)
{
    return args.at<field::label>();
}

const int &function_taking_size(const map<field::size>::type &args)
{
    return args.at<field::size>();
}

const int &function_taking_both(const map<field::label, field::size>::type &args)
{
    return args.at<field::size>();
}


BOOST_AUTO_TEST_CASE( test_call ) 
{    
    BOOST_CHECK_EQUAL(function_taking_label(label_="hello"), "hello");
    BOOST_CHECK_EQUAL(function_taking_size((size(1))), 1);
    
    // parameters can be passed in any order
    BOOST_CHECK_EQUAL(function_taking_both((label_="hello", size_=1)), 1);
    BOOST_CHECK_EQUAL(function_taking_both((size_=1,label_="hello")), 1);
    // size has a default value
    BOOST_CHECK_EQUAL(function_taking_both((label("hello"))), field::size::default_value());
    
    // extraneous parameters will be ignored
    BOOST_CHECK_EQUAL(function_taking_label((label("hello"),size(1))), "hello");
    
    // empty parameter map can be used for no parameters (all default values)
    BOOST_CHECK_EQUAL(function_taking_size(empty()), field::size::default_value());
}

class named_constructable
{
public:
    // a macro could extend the number of parameters if desired
    template<typename FM0, typename FM1, typename FM2>
    named_constructable(const FM0 &a0, const FM1 &a1, const FM2 &a2)
    {
        // this could be done lazily...
        map<field::label, field::size, field::volume>::type args = a0 & a1 & a2;
        
        label = args.at<field::label>();
        size = args.at<field::size>();
        volume = args.at<field::volume>();
    }
    std::string label;
    int size;
    double volume;
};


BOOST_AUTO_TEST_CASE( test_multi_param_construction )
{
    named_constructable constructed(label_ = "hello", size_ = 1, volume_ = 1.0);
    BOOST_CHECK_EQUAL(constructed.label, "hello");
    BOOST_CHECK_EQUAL(constructed.size, 1);    
    BOOST_CHECK_EQUAL(constructed.volume, 1.0);    

}



