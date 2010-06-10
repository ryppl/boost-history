//  endian_view_test.cpp  ---------------------------------------------------------//

//  Copyright Beman Dawes 1999-2008

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/endian_pack

//----------------------------------------------------------------------------//

//  This test probes for correct endianess, size, and value.

//  See endian_operations_test for tests of operator correctness and interaction
//  between operand types.

//----------------------------------------------------------------------------//

#include <boost/detail/lightweight_test.hpp>  // for main

#include <boost/integer/endian_view.hpp>
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


namespace
{
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
    
    
}
} // unnamed namespace

int main( int argc, char * argv[] )
{
    check_access();
    return boost::report_errors();
} // main
