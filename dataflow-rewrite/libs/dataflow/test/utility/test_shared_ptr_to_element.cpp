/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/utility/shared_ptr_to_element.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/mpl/int.hpp>


#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test_c ) 
{
    using namespace boost;
    using boost::dataflow::utility::shared_ptr_to_element_c;
    
    typedef fusion::vector2<int, float> vector_type;
    
    weak_ptr<vector_type> weak_vector_ptr;
    {
        shared_ptr<float> float_ptr;
        weak_ptr<int> weak_int_ptr;
        {
            shared_ptr<int> int_ptr;
            {
                shared_ptr<vector_type> vector_ptr(new vector_type(1, 2.5));
                weak_vector_ptr = vector_ptr;

                int_ptr = shared_ptr_to_element_c<0>(vector_ptr);
                weak_int_ptr = int_ptr;
                
                BOOST_CHECK_EQUAL(vector_ptr.use_count(), 2);
                BOOST_CHECK_EQUAL(*int_ptr, 1);

                float_ptr = shared_ptr_to_element_c<1>(vector_ptr);
                
                BOOST_CHECK_EQUAL(vector_ptr.use_count(), 3);
                BOOST_CHECK_EQUAL(*float_ptr, 2.5);
                
                shared_ptr<float> other_float_ptr = float_ptr;
                BOOST_CHECK_EQUAL(vector_ptr.use_count(), 4);
            }

            BOOST_CHECK_EQUAL(weak_vector_ptr.use_count(), 2);
        }
        
        BOOST_CHECK_EQUAL(weak_vector_ptr.use_count(), 1);
        BOOST_CHECK_EQUAL(weak_int_ptr.use_count(), 1);
    }

    BOOST_CHECK(weak_vector_ptr.expired());
}

BOOST_AUTO_TEST_CASE( test ) 
{
    using namespace boost;
    using boost::dataflow::utility::shared_ptr_to_element;
    
    typedef fusion::vector2<int, float> vector_type;
    shared_ptr<float> float_ptr;
    shared_ptr<int> int_ptr;
    shared_ptr<vector_type> vector_ptr(new vector_type(1, 2.5));

    int_ptr = shared_ptr_to_element<mpl::int_<0> >(vector_ptr);
    
    BOOST_CHECK_EQUAL(vector_ptr.use_count(), 2);
    BOOST_CHECK_EQUAL(*int_ptr, 1);

    float_ptr = shared_ptr_to_element<mpl::int_<1> >(vector_ptr);
    
    BOOST_CHECK_EQUAL(vector_ptr.use_count(), 3);
    BOOST_CHECK_EQUAL(*float_ptr, 2.5);
}


