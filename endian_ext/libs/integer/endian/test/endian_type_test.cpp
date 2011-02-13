//  endian_type_test.cpp  ---------------------------------------------------------//

//  (C) Copyright VicenteJ Botet Escriba 2010-2011

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/integer/endian

//----------------------------------------------------------------------------//

//  This test probes for correct endianess, size, and value.

//  See endian_operations_test for tests of operator correctness and interaction
//  between operand types.

//----------------------------------------------------------------------------//

#include <boost/detail/lightweight_test.hpp>  // for main

#include <iostream>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/integer/endian/endian_type.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

using namespace std;             // Not the best programming practice, but I
using namespace boost;           //   want to verify this combination of using
using namespace boost::integer;  //   namespaces works. See endian_operations_test
//                               //   for tests that don't do "using namespace".

namespace X {

struct big_c {
    ubig32_t a;
    ubig16_t b;
};


struct little_c {
    ulittle32_t a;
    ulittle16_t b;
};


struct mixed_c {
    big_c a;
    little_c b;
};


}

BOOST_FUSION_ADAPT_STRUCT(
    X::big_c,
    (ubig32_t, a)
    (ubig16_t, a)
)

BOOST_FUSION_ADAPT_STRUCT(
    X::little_c,
    (ulittle32_t, a)
    (ulittle16_t, a)
)


BOOST_FUSION_ADAPT_STRUCT(
    X::mixed_c,
    (X::big_c, a)
    (X::little_c, a)
)


namespace
{
void check_endian_type()
{
 
    // fundamental types
    //~ BOOST_TEST((endian_type<int>::value == endianness::native));   
    BOOST_TEST((is_native<int>::value));   

    // endian types
    //~ BOOST_TEST((endian_type<endian_pack<endianness::little,short> >::value == endianness::little));   
    BOOST_TEST((is_little<endian_pack<little_endian,short> >::value));   
    //~ BOOST_TEST((endian_type<endian<endianness::big,int> >::value== endianness::big));   
    BOOST_TEST((is_big<integer::endian<big_endian,int> >::value));   

    // array of endian types
    //~ BOOST_TEST((endian_type<endian_pack<endianness::little,short>[2] >::value == endianness::little));   
    BOOST_TEST((is_little<endian_pack<little_endian,short>[2] >::value));   
    //~ BOOST_TEST((endian_type<endian<endianness::big,int>[3] >::value== endianness::big));   
    BOOST_TEST((is_big<integer::endian<big_endian,int>[3] >::value));   

    
    //~ typedef remove_reference<remove_cv<fusion::result_of::front<X::big_c>::type>::type>::type front_type;
    //~ typedef remove_reference<
                //~ remove_cv<
                    //~ fusion::result_of::deref<
                        //~ fusion::result_of::begin<X::big_c>::type
                    //~ >::type
                //~ >::type
            //~ >::type deref_type;
        //~ // typedef typename fusion::result_of::deref<It>::type deref_it_type;    

    
    //~ BOOST_MPL_ASSERT_MSG((is_same<front_type, ubig32_t>::value), FAIL, (front_type, ubig32_t) );
    //~ BOOST_MPL_ASSERT_MSG((is_same<deref_type, ubig32_t>::value), FAIL, (deref_type, ubig32_t) );
    
    //~ typedef fusion::result_of::begin<X::big_c>::type It;
    //~ typedef fusion::result_of::end<X::big_c>::type End;
    //~ typedef fusion::result_of::deref<It>::type deref_it_type;    
    //~ typedef remove_reference<
            //~ remove_cv<deref_it_type>::type 
    //~ >::type it_type;
    //~ static const BOOST_SCOPED_ENUM(endianness) it_value = endian_type< it_type >::value;
    //~ typedef mpl::if_c<it_value==endianness::big,
        //~ mpl::int_<integer_detail::endian_type_loop<endianness::big,fusion::result_of::next<It>::type, End>::value>,
        //~ mpl::int_<endianness::mixed>
    //~ >::type res_type;    
    //~ BOOST_MPL_ASSERT_MSG((res_type::value==endianness::big), FAIL, (res_type, it_type, deref_it_type, It) );
    
    
    //~ static const BOOST_SCOPED_ENUM(endianness) front_value= 
        //~ endian_type< front_type >::value;
    
    //~ std::cout << "front=" << endian_type<X::big_c>::value << std::endl;
    //~ std::cout << "ubig32_t=" << endian_type<ubig32_t>::value << std::endl;
    //~ // struct of bigs of endian types
    //~ std::cout << "X::big_c=" << endian_type<X::big_c>::value << std::endl;
    //~ BOOST_TEST((endian_type<X::big_c>::value == endianness::big));   
    BOOST_TEST((is_big<X::big_c>::value));   
    //~ BOOST_TEST((endian_type<X::little_c>::value == endianness::little));   
    BOOST_TEST((is_little<X::little_c>::value));   
    //~ BOOST_TEST((endian_type<X::mixed_c>::value == endianness::mixed));   
    BOOST_TEST((is_mixed<X::mixed_c>::value));   
    
}
} // unnamed namespace

int main(  )
{
    check_endian_type();
    return boost::report_errors();
} // main
