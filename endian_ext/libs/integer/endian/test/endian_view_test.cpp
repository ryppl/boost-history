//  endian_view_test.cpp  ---------------------------------------------------------//

//  Copyright Beman Dawes 1999-2008
//  (C) Copyright VicenteJ Botet Escriba 2010-2011

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/integer/endian

//----------------------------------------------------------------------------//

//  This test probes for correct endianess, size, and value.

//  See endian_operations_test for tests of operator correctness and interaction
//  between operand types.

//----------------------------------------------------------------------------//
#include <iostream>

#include <boost/detail/lightweight_test.hpp>  // for main

#include <boost/integer/endian/endian_view.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
//~ #include <boost/cstdint.hpp>
//~ #include <boost/progress.hpp>

//~ #include <iostream>
//~ #include <limits>
//~ #include <climits>
//~ #include <cstdlib>    // for atoi(), exit()
//~ #include <cstring>    // for memcmp()

using namespace std;             // Not the best programming practice, but I
using namespace boost;           //   want to verify this combination of using
using namespace boost::integer;  //   namespaces works. See endian_operations_test
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
namespace integer {
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

namespace
{
        X::mixed_c m;

void check_access()
{
    // structs with offsets % 2 == 1 for type of size > 1 to ensure no alignment
    // bytes added for any size > 1

    struct a_struct
    {
      char      c1;
      short     s1;
      int       i1;
      char      c2;
      short     s2;
      int       i2;
    };    
    
    a_struct a;
    as_big(a.c1)=0x0A;
    as_endian<big_endian>(a.s1)=0x0102;
    as_big(a.i1)=0x01020304;
    
    
    as_little(a.c2)=0x0A;
    as_little(a.s2)=0x0102;
    as_little(a.i2)=0x01020304;

    std::cout << std::hex << int(a.c1) << std::endl;
    std::cout << std::hex << a.s1 << std::endl;
    std::cout << std::hex << a.i1 << std::endl;
    std::cout << std::hex << int(a.c2) << std::endl;
    std::cout << std::hex << a.s2 << std::endl;
    std::cout << std::hex << a.i2 << std::endl;

    a.c1=as_big(a.c1);
    a.s1=as_big(a.s1);
    a.i1=as_big(a.i1);
    
    a.c2=as_little(a.c2);
    a.s2=as_little(a.s2);
    a.i2=as_little(a.i2);

    BOOST_TEST(a.c1 == 0x0A);   
    BOOST_TEST(a.s1 == 0x0102);   
    BOOST_TEST(a.i1 == 0x01020304);   
    
    BOOST_TEST(a.c2 == 0x0A);   
    BOOST_TEST(a.s2 == 0x0102);   
    BOOST_TEST(a.i2 == 0x01020304);   

    as_big(a.c1)=as_little(a.c1);
    as_big(a.s1)=as_little(a.s1);
    as_big(a.i1)=as_little(a.i1);
    
    std::cout << std::hex << int(a.c1) << std::endl;
    std::cout << std::hex << a.s1 << std::endl;
    std::cout << std::hex << a.i1 << std::endl;

    std::cout << "**********" << std::endl;
    std::cout << std::hex << m.a.a << std::endl;
    std::cout << std::hex << m.a.b << std::endl;
    std::cout << std::hex << m.b.a << std::endl;
    std::cout << std::hex << m.b.b << std::endl;

    as_endian<network>(a.i1)=as_endian<network>(a.i1);
    std::cout << "**********" << std::endl;
    std::cout << std::hex << m.a.a << std::endl;
    std::cout << std::hex << m.a.b << std::endl;
    std::cout << std::hex << m.b.a << std::endl;
    std::cout << std::hex << m.b.b << std::endl;
    
    //~ as_big(m)=as_big(m);
    
    as(m)=as_endian<network>(m);
    std::cout << "**********" << std::endl;
    std::cout << std::hex << m.a.a << std::endl;
    std::cout << std::hex << m.a.b << std::endl;
    std::cout << std::hex << m.b.a << std::endl;
    std::cout << std::hex << m.b.b << std::endl;
    
    //~ as(m)=as_big(m);
}
} // unnamed namespace

int main( )
{
    m.b.a=0x01020304;
    m.b.b=0x0A0B;
    m.a.a=0x04030201;
    m.a.b=0x0B0A;    

    check_access();
    return boost::report_errors();
    //~ return 1;
} // main
