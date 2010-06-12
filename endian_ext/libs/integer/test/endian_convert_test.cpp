//  endian_type_test.cpp  ---------------------------------------------------------//

//  (C) Copyright VicenteJ Botet Escriba 2010

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/endian_pack

//----------------------------------------------------------------------------//

//  This test probes for correct endianess, size, and value.

//  See endian_operations_test for tests of operator correctness and interaction
//  between operand types.

//----------------------------------------------------------------------------//

#include <boost/detail/lightweight_test.hpp>  // for main

#include <iostream>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/integer/endian_type.hpp>
#include <boost/integer/endian_conversion.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/list.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/front.hpp>

#include <boost/static_assert.hpp>

//~ using namespace std;             // Not the best programming practice, but I
using namespace boost;           //   want to verify this combination of using
//~ using namespace boost::integer;  //   namespaces works. See endian_operations_test
//                               //   for tests that don't do "using namespace".

namespace X {

struct big_c {
    uint32_t a;
    uint16_t b;
};


struct little_c {
    int32_t a;
    int16_t b;
};

struct mixed_c {
    big_c a;
    little_c b;
};

}

struct network {};
    
namespace boost { 
namespace endianness {
    
    template <> 
    struct domain_map <network, X::big_c> {
        typedef mpl::vector<big,big> type;
    };
    template <> 
    struct domain_map <network, X::little_c> {
        typedef mpl::vector<little,little> type;
    };       
    
}
}

BOOST_FUSION_ADAPT_STRUCT(
    X::big_c,
    (uint32_t, a)
    (uint16_t, b)
)

BOOST_FUSION_ADAPT_STRUCT(
    X::little_c,
    (int32_t, a)
    (int16_t, b)
)


BOOST_FUSION_ADAPT_STRUCT(
    X::mixed_c,
    (X::big_c, a)
    (X::little_c, b)
)

BOOST_MPL_ASSERT_MSG((is_same<endianness::domain_map<network, X::big_c>::type,
                             mpl::vector<endianness::big,endianness::big> >::value), 
            SS, (network,endianness::domain_map<network, X::big_c>::type));

BOOST_MPL_ASSERT_MSG((is_same<endianness::domain_map<network, X::little_c>::type,
                             mpl::vector<endianness::little,endianness::little> >::value), 
            RR, (network,endianness::domain_map<network, X::little_c>::type));

typedef mpl::list<> floats;
typedef mpl::push_front<floats,int>::type types;

BOOST_MPL_ASSERT(( is_same< mpl::front<types>::type, int > ));


BOOST_MPL_ASSERT_MSG((
            is_same<
                mpl::front<
                    endianness::domain_map<network, X::mixed_c>::type
                >::type,
                endianness::domain_map<network, X::big_c>::type 
            >::value), 
            SS, 
            (   network,
                mpl::front<
                    endianness::domain_map<network, X::mixed_c>::type
                >::type, 
                endianness::domain_map<network,X::big_c>::type
            ));


BOOST_MPL_ASSERT_MSG((
            is_same<
                mpl::front<
                    endianness::domain_map<network, X::big_c>::type
                >::type,
                endianness::big 
            >::value), 
            SS, 
            (   network,
                mpl::front<
                    endianness::domain_map<network, X::big_c>::type
                >::type, 
                endianness::domain_map<network,X::big_c>::type
            ));

typedef endianness::domain_map<network, X::mixed_c>::type endian_mixed_c;

typedef mpl::begin<endian_mixed_c>::type begin_endian_mixed_c;
typedef mpl::deref<begin_endian_mixed_c>::type deref_begin_endian_mixed_c;

BOOST_MPL_ASSERT_MSG((
            is_same<
                deref_begin_endian_mixed_c,
                endianness::domain_map<network, X::big_c>::type 
            >::value), 
            SS, 
            (   deref_begin_endian_mixed_c, 
                endianness::domain_map<network, X::big_c>::type
            ));


namespace
{
    X::mixed_c m;
    
void check_endian_send()
{

    
    //~ std::cout << std::hex << m.a.a << std::endl;
    //~ std::cout << std::hex << m.a.b << std::endl;
    //~ std::cout << std::hex << m.b.a << std::endl;
    //~ std::cout << std::hex << m.b.b << std::endl;
    
    //~ integer::convert_from<network>(m.a);
    //~ integer::convert_from<network>(m.b);
    integer::convert_from<network>(m);
    //~ std::cout << std::hex << m.a.a << std::endl;
    //~ std::cout << std::hex << m.a.b << std::endl;
    //~ std::cout << std::hex << m.b.a << std::endl;
    //~ std::cout << std::hex << m.b.b << std::endl;

    integer::convert_to<network>(m);
    //~ std::cout << std::hex << m.a.a << std::endl;
    //~ std::cout << std::hex << m.a.b << std::endl;
    //~ std::cout << std::hex << m.b.a << std::endl;
    //~ std::cout << std::hex << m.b.b << std::endl;
    
    //~ return m.a.a+m.a.b+ m.b.a+m.b.b;
    //~ return 0;

    
}
} // unnamed namespace

int main( int argc, char * argv[] )
{
    m.b.a=0x01020304;
    m.b.b=0x0A0B;
    m.a.a=0x04030201;
    m.a.b=0x0B0A;    

    check_endian_send();
    return boost::report_errors();
    //~ return 1;
} // main
