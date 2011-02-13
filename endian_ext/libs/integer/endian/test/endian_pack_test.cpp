//  endian_pack_test.cpp  ---------------------------------------------------------//

//  (C) Copyright VicenteJ Botet Escriba 2010-2011

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/integer/endian

//----------------------------------------------------------------------------//

//  This test probes for correct endianess, size, and value.

//  See endian_operations_test for tests of operator correctness and interaction
//  between operand types.

//----------------------------------------------------------------------------//

#include <boost/integer/endian/endian_pack.hpp>
#include <boost/cstdint.hpp>
#include <boost/progress.hpp>

#include <iostream>
#include <limits>
#include <climits>
#include <cstdlib>    // for atoi(), exit()
#include <cstring>    // for memcmp()

using namespace std;             // Not the best programming practice, but I
using namespace boost;           //   want to verify this combination of using
using namespace boost::integer;  //   namespaces works. See endian_operations_test
//                               //   for tests that don't do "using namespace".

#define VERIFY(predicate) verify( predicate, __LINE__ )
#define VERIFY_SIZE(actual, expected) verify_size( actual, expected, __LINE__ )
#define VERIFY_VALUE_AND_OPS(endian_t,expected_t,expected) verify_value_and_ops<endian_t, expected_t>( expected, __LINE__ )
#define VERIFY_BIG_REPRESENTATION(t) verify_representation<t>( true, __LINE__ )
#define VERIFY_LITTLE_REPRESENTATION(t) verify_representation<t>( false, __LINE__ )
#define VERIFY_NATIVE_REPRESENTATION(t) verify_native_representation<t>( __LINE__ )

namespace
{
  int err_count;

  void verify( bool x, int line )
  {
    if ( x ) return;
    ++err_count;
    cout << "Error: verify failed on line " << line << endl;
  }

  void verify_size( size_t actual, size_t expected, int line )
  {
    if ( actual == expected ) return;
    ++err_count;
    cout << "Error: verify size failed on line " << line << endl;
    cout << " A structure with an expected sizeof() " << expected
         << " had an actual sizeof() " << actual
         << "\n This will cause common uses of <boost/endian_pack.hpp> to fail\n";
  } 

  template <class Endian, class Base>
  void verify_value_and_ops( const Base & expected, int line )
  {
    Endian v( expected );
    verify( v == expected, line );

    Endian v2;
    v2.operator=( expected );
    verify( v2 == expected, line );

    //~ ++v; // verify integer_cover_operators being applied to this type -
         // will fail to compile if no endian_pack<> specialization is present
  }

  const char * big_rep    = "\x12\x34\x56\x78\x9A\xBC\xDE\xF0";
  const char * little_rep = "\xF0\xDE\xBC\x9A\x78\x56\x34\x12";

  template <class Endian>
  void verify_representation( bool is_big, int line )
  {
    int silence = 0;
    Endian x ( static_cast<typename Endian::value_type>
      (0x123456789abcdef0LL + silence) ); // will truncate

    if ( is_big )
      verify( memcmp( &x,
        reinterpret_cast<const char*>(big_rep)+8-sizeof(Endian),
          sizeof(Endian) ) == 0, line );
    else
      verify( memcmp( &x, little_rep, sizeof(Endian) ) == 0, line );
  }

  template <class Endian>
  inline void verify_native_representation( int line )
  {
#   ifdef BOOST_BIG_ENDIAN
      verify_representation<Endian>( true, line );
#   else
      verify_representation<Endian>( false, line );
#   endif
  }

  //  detect_endianness  -----------------------------------------------------//

  void detect_endianness()
  {
    union View
    { 
      long long i;
      unsigned char c[8];
    };

    View v = { 0x0102030405060708LL };  // initialize v.i
    
    if ( memcmp( v.c, "\10\7\6\5\4\3\2\1", 8) == 0 )
    {
      cout << "This machine is little-endian.\n";
  #   ifdef BOOST_BIG_INTEGER_OPERATORS
        cout << "yet boost/detail/endian_pack.hpp defines BOOST_BIG_INTEGER_OPERATORS.\n"
          "You must fix boost/detail/endian_pack.hpp for boost/endian_pack.hpp to work correctly.\n"
          "Please report the fix to the Boost mailing list.\n";
        exit(1);
  #   endif
    } 
    else if ( memcmp( v.c, "\1\2\3\4\5\6\7\10", 8) == 0 )
    {
      cout << "This machine is big-endian.\n";
  #   ifdef BOOST_LITTLE_INTEGER_OPERATORS
        cout << "yet boost/detail/endian_pack.hpp defines BOOST__LITTLE_INTEGER_OPERATORS.\n"
          "You must fix boost/detail/endian_pack.hpp for boost/endian_pack.hpp to work correctly.\n"
          "Please report the fix to the Boost mailing list.\n";
        exit(1);
  #   endif
    }
    else
    { 
      cout << "This machine is neither strict big-endian nor strict little-endian\n"
        "You must modify boost/endian_pack.hpp for it to work correctly.\n";
      exit(1);
    }
    cout << "That should not matter and is presented for your information only.\n";
  } // detect_endianness

  //  check_size  ------------------------------------------------------------//

  void check_size()
  {
    VERIFY( numeric_limits<signed char>::digits == 7 );
    VERIFY( numeric_limits<unsigned char>::digits == 8 );

    VERIFY_SIZE( sizeof( big8_pt ), 1 );
    VERIFY_SIZE( sizeof( big16_pt ), 2 );
    VERIFY_SIZE( sizeof( big24_pt ), 3 );
    VERIFY_SIZE( sizeof( big32_pt ), 4 );
    VERIFY_SIZE( sizeof( big40_pt ), 5 );
    VERIFY_SIZE( sizeof( big48_pt ), 6 );
    VERIFY_SIZE( sizeof( big56_pt ), 7 );
    VERIFY_SIZE( sizeof( big64_pt ), 8 );

    VERIFY_SIZE( sizeof( ubig8_pt ), 1 );
    VERIFY_SIZE( sizeof( ubig16_pt ), 2 );
    VERIFY_SIZE( sizeof( ubig24_pt ), 3 );
    VERIFY_SIZE( sizeof( ubig32_pt ), 4 );
    VERIFY_SIZE( sizeof( ubig40_pt ), 5 );
    VERIFY_SIZE( sizeof( ubig48_pt ), 6 );
    VERIFY_SIZE( sizeof( ubig56_pt ), 7 );
    VERIFY_SIZE( sizeof( ubig64_pt ), 8 );

    VERIFY_SIZE( sizeof( little8_pt ), 1 );
    VERIFY_SIZE( sizeof( little16_pt ), 2 );
    VERIFY_SIZE( sizeof( little24_pt ), 3 );
    VERIFY_SIZE( sizeof( little32_pt ), 4 );
    VERIFY_SIZE( sizeof( little40_pt ), 5 );
    VERIFY_SIZE( sizeof( little48_pt ), 6 );
    VERIFY_SIZE( sizeof( little56_pt ), 7 );
    VERIFY_SIZE( sizeof( little64_pt ), 8 );

    VERIFY_SIZE( sizeof( ulittle8_pt ), 1 );
    VERIFY_SIZE( sizeof( ulittle16_pt ), 2 );
    VERIFY_SIZE( sizeof( ulittle24_pt ), 3 );
    VERIFY_SIZE( sizeof( ulittle32_pt ), 4 );
    VERIFY_SIZE( sizeof( ulittle40_pt ), 5 );
    VERIFY_SIZE( sizeof( ulittle48_pt ), 6 );
    VERIFY_SIZE( sizeof( ulittle56_pt ), 7 );
    VERIFY_SIZE( sizeof( ulittle64_pt ), 8 );

    VERIFY_SIZE( sizeof( native8_pt ), 1 );
    VERIFY_SIZE( sizeof( native16_pt ), 2 );
    VERIFY_SIZE( sizeof( native24_pt ), 3 );
    VERIFY_SIZE( sizeof( native32_pt ), 4 );
    VERIFY_SIZE( sizeof( native40_pt ), 5 );
    VERIFY_SIZE( sizeof( native48_pt ), 6 );
    VERIFY_SIZE( sizeof( native56_pt ), 7 );
    VERIFY_SIZE( sizeof( native64_pt ), 8 );

    VERIFY_SIZE( sizeof( unative8_pt ), 1 );
    VERIFY_SIZE( sizeof( unative16_pt ), 2 );
    VERIFY_SIZE( sizeof( unative24_pt ), 3 );
    VERIFY_SIZE( sizeof( unative32_pt ), 4 );
    VERIFY_SIZE( sizeof( unative40_pt ), 5 );
    VERIFY_SIZE( sizeof( unative48_pt ), 6 );
    VERIFY_SIZE( sizeof( unative56_pt ), 7 );
    VERIFY_SIZE( sizeof( unative64_pt ), 8 );

    VERIFY_SIZE( sizeof( aligned_big16_pt ), 2 );
    VERIFY_SIZE( sizeof( aligned_big32_pt ), 4 );
    VERIFY_SIZE( sizeof( aligned_big64_pt ), 8 );

    VERIFY_SIZE( sizeof( aligned_ubig16_pt ), 2 );
    VERIFY_SIZE( sizeof( aligned_ubig32_pt ), 4 );
    VERIFY_SIZE( sizeof( aligned_ubig64_pt ), 8 );

    VERIFY_SIZE( sizeof( aligned_little16_pt ), 2 );
    VERIFY_SIZE( sizeof( aligned_little32_pt ), 4 );
    VERIFY_SIZE( sizeof( aligned_little64_pt ), 8 );

    VERIFY_SIZE( sizeof( aligned_ulittle16_pt ), 2 );
    VERIFY_SIZE( sizeof( aligned_ulittle32_pt ), 4 );
    VERIFY_SIZE( sizeof( aligned_ulittle64_pt ), 8 );
  } // check_size

  //  check_alignment  -------------------------------------------------------//

  void check_alignment()
  {
    // structs with offsets % 2 == 1 for type of size > 1 to ensure no alignment
    // bytes added for any size > 1

    struct big_struct
    {
      big8_pt    v0;
      big16_pt    v1;
      big24_pt    v3;
      char      v6;
      big32_pt    v7;
      big40_pt    v11;
      char      v16;
      big48_pt    v17;
      big56_pt    v23;
      char      v30;
      big64_pt    v31;
    };

    struct ubig_struct
    {
      ubig8_pt    v0;
      ubig16_pt    v1;
      ubig24_pt    v3;
      char       v6;
      ubig32_pt    v7;
      ubig40_pt    v11;
      char       v16;
      ubig48_pt    v17;
      ubig56_pt    v23;
      char       v30;
      ubig64_pt    v31;
    };

    struct little_struct
    {
      little8_pt    v0;
      little16_pt    v1;
      little24_pt    v3;
      char         v6;
      little32_pt    v7;
      little40_pt    v11;
      char         v16;
      little48_pt    v17;
      little56_pt    v23;
      char         v30;
      little64_pt    v31;
    };

    struct ulittle_struct
    {
      ulittle8_pt    v0;
      ulittle16_pt    v1;
      ulittle24_pt    v3;
      char          v6;
      ulittle32_pt    v7;
      ulittle40_pt    v11;
      char          v16;
      ulittle48_pt    v17;
      ulittle56_pt    v23;
      char          v30;
      ulittle64_pt    v31;
    };

    struct native_struct
    {
      native8_pt    v0;
      native16_pt    v1;
      native24_pt    v3;
      char         v6;
      native32_pt    v7;
      native40_pt    v11;
      char         v16;
      native48_pt    v17;
      native56_pt    v23;
      char         v30;
      native64_pt    v31;
    };

    struct unative_struct
    {
      unative8_pt    v0;
      unative16_pt    v1;
      unative24_pt    v3;
      char          v6;
      unative32_pt    v7;
      unative40_pt    v11;
      char          v16;
      unative48_pt    v17;
      unative56_pt    v23;
      char          v30;
      unative64_pt    v31;
    };

    int saved_err_count = err_count;

    VERIFY_SIZE( sizeof(big_struct), 39 );
    VERIFY_SIZE( sizeof(ubig_struct), 39 );
    VERIFY_SIZE( sizeof(little_struct), 39 );
    VERIFY_SIZE( sizeof(ulittle_struct), 39 );
    VERIFY_SIZE( sizeof(native_struct), 39 );
    VERIFY_SIZE( sizeof(unative_struct), 39 );

    if ( saved_err_count == err_count )
    { 
      cout <<
        "Size and alignment for structures of endian_pack types are as expected.\n";
    }
  } // check_alignment

  //  check_representation_and_range_and_ops  --------------------------------//

  void check_representation_and_range_and_ops()
  {

    VERIFY_BIG_REPRESENTATION( big8_pt );
    VERIFY_VALUE_AND_OPS( big8_pt, int_least8_t,  0x7f );
    VERIFY_VALUE_AND_OPS( big8_pt, int_least8_t, -0x80 );

    VERIFY_BIG_REPRESENTATION( big16_pt );
    VERIFY_VALUE_AND_OPS( big16_pt, int_least16_t,  0x7fff );
    VERIFY_VALUE_AND_OPS( big16_pt, int_least16_t, -0x8000 );

    VERIFY_BIG_REPRESENTATION( big24_pt );
    VERIFY_VALUE_AND_OPS( big24_pt, int_least32_t,  0x7fffff );
    VERIFY_VALUE_AND_OPS( big24_pt, int_least32_t, -0x800000 );

    VERIFY_BIG_REPRESENTATION( big32_pt );
    VERIFY_VALUE_AND_OPS( big32_pt, int_least32_t,  0x7fffffff );
    VERIFY_VALUE_AND_OPS( big32_pt, int_least32_t, -0x7fffffff-1 );

    VERIFY_BIG_REPRESENTATION( big40_pt );
    VERIFY_VALUE_AND_OPS( big40_pt, int_least64_t,  0x7fffffffffLL );
    VERIFY_VALUE_AND_OPS( big40_pt, int_least64_t, -0x8000000000LL );

    VERIFY_BIG_REPRESENTATION( big48_pt );
    VERIFY_VALUE_AND_OPS( big48_pt, int_least64_t,  0x7fffffffffffLL );
    VERIFY_VALUE_AND_OPS( big48_pt, int_least64_t, -0x800000000000LL );

    VERIFY_BIG_REPRESENTATION( big56_pt );
    VERIFY_VALUE_AND_OPS( big56_pt, int_least64_t,  0x7fffffffffffffLL );
    VERIFY_VALUE_AND_OPS( big56_pt, int_least64_t, -0x80000000000000LL );

    VERIFY_BIG_REPRESENTATION( big64_pt );
    VERIFY_VALUE_AND_OPS( big64_pt, int_least64_t,  0x7fffffffffffffffLL );
    VERIFY_VALUE_AND_OPS( big64_pt, int_least64_t, -0x7fffffffffffffffLL-1 );

    VERIFY_BIG_REPRESENTATION( ubig8_pt );
    VERIFY_VALUE_AND_OPS( ubig8_pt, uint_least8_t,  0xff );

    VERIFY_BIG_REPRESENTATION( ubig16_pt );
    VERIFY_VALUE_AND_OPS( ubig16_pt, uint_least16_t, 0xffff );

    VERIFY_BIG_REPRESENTATION( ubig24_pt );
    VERIFY_VALUE_AND_OPS( ubig24_pt, uint_least32_t, 0xffffff );

    VERIFY_BIG_REPRESENTATION( ubig32_pt );
    VERIFY_VALUE_AND_OPS( ubig32_pt, uint_least32_t, 0xffffffff );

    VERIFY_BIG_REPRESENTATION( ubig40_pt );
    VERIFY_VALUE_AND_OPS( ubig40_pt, uint_least64_t, 0xffffffffffLL );

    VERIFY_BIG_REPRESENTATION( ubig48_pt );
    VERIFY_VALUE_AND_OPS( ubig48_pt, uint_least64_t, 0xffffffffffffLL );

    VERIFY_BIG_REPRESENTATION( ubig56_pt );
    VERIFY_VALUE_AND_OPS( ubig56_pt, uint_least64_t, 0xffffffffffffffLL );

    VERIFY_BIG_REPRESENTATION( ubig64_pt );
    VERIFY_VALUE_AND_OPS( ubig64_pt, uint_least64_t, 0xffffffffffffffffLL );

    VERIFY_LITTLE_REPRESENTATION( little8_pt );
    VERIFY_VALUE_AND_OPS( little8_pt, int_least8_t,   0x7f );
    VERIFY_VALUE_AND_OPS( little8_pt, int_least8_t,  -0x80 );

    VERIFY_LITTLE_REPRESENTATION( little16_pt );
    VERIFY_VALUE_AND_OPS( little16_pt, int_least16_t,  0x7fff );
    VERIFY_VALUE_AND_OPS( little16_pt, int_least16_t, -0x8000 );

    VERIFY_LITTLE_REPRESENTATION( little24_pt );
    VERIFY_VALUE_AND_OPS( little24_pt, int_least32_t,  0x7fffff );
    VERIFY_VALUE_AND_OPS( little24_pt, int_least32_t, -0x800000 );

    VERIFY_LITTLE_REPRESENTATION( little32_pt );
    VERIFY_VALUE_AND_OPS( little32_pt, int_least32_t,  0x7fffffff );
    VERIFY_VALUE_AND_OPS( little32_pt, int_least32_t, -0x7fffffff-1 );

    VERIFY_LITTLE_REPRESENTATION( little40_pt );
    VERIFY_VALUE_AND_OPS( little40_pt, int_least64_t,  0x7fffffffffLL );
    VERIFY_VALUE_AND_OPS( little40_pt, int_least64_t, -0x8000000000LL );

    VERIFY_LITTLE_REPRESENTATION( little48_pt );
    VERIFY_VALUE_AND_OPS( little48_pt, int_least64_t,  0x7fffffffffffLL );
    VERIFY_VALUE_AND_OPS( little48_pt, int_least64_t, -0x800000000000LL );

    VERIFY_LITTLE_REPRESENTATION( little56_pt );
    VERIFY_VALUE_AND_OPS( little56_pt, int_least64_t,  0x7fffffffffffffLL );
    VERIFY_VALUE_AND_OPS( little56_pt, int_least64_t, -0x80000000000000LL );

    VERIFY_LITTLE_REPRESENTATION( little64_pt );
    VERIFY_VALUE_AND_OPS( little64_pt, int_least64_t,  0x7fffffffffffffffLL );
    VERIFY_VALUE_AND_OPS( little64_pt, int_least64_t, -0x7fffffffffffffffLL-1 );

    VERIFY_LITTLE_REPRESENTATION( ulittle8_pt );
    VERIFY_VALUE_AND_OPS( ulittle8_pt, uint_least8_t, 0xff );

    VERIFY_LITTLE_REPRESENTATION( ulittle16_pt );
    VERIFY_VALUE_AND_OPS( ulittle16_pt, uint_least16_t, 0xffff );

    VERIFY_LITTLE_REPRESENTATION( ulittle24_pt );
    VERIFY_VALUE_AND_OPS( ulittle24_pt, uint_least32_t, 0xffffff );

    VERIFY_LITTLE_REPRESENTATION( ulittle32_pt );
    VERIFY_VALUE_AND_OPS( ulittle32_pt, uint_least32_t, 0xffffffff );

    VERIFY_LITTLE_REPRESENTATION( ulittle40_pt );
    VERIFY_VALUE_AND_OPS( ulittle40_pt, uint_least64_t, 0xffffffffffLL );

    VERIFY_LITTLE_REPRESENTATION( ulittle48_pt );
    VERIFY_VALUE_AND_OPS( ulittle48_pt, uint_least64_t, 0xffffffffffffLL );

    VERIFY_LITTLE_REPRESENTATION( ulittle56_pt );
    VERIFY_VALUE_AND_OPS( ulittle56_pt, uint_least64_t, 0xffffffffffffffLL );

    VERIFY_LITTLE_REPRESENTATION( ulittle64_pt );
    VERIFY_VALUE_AND_OPS( ulittle64_pt, uint_least64_t, 0xffffffffffffffffLL );

    VERIFY_NATIVE_REPRESENTATION( native8_pt );
    VERIFY_VALUE_AND_OPS( native8_pt, int_least8_t,   0x7f );
    VERIFY_VALUE_AND_OPS( native8_pt, int_least8_t,  -0x80 );

    VERIFY_NATIVE_REPRESENTATION( native16_pt );
    VERIFY_VALUE_AND_OPS( native16_pt, int_least16_t,  0x7fff );
    VERIFY_VALUE_AND_OPS( native16_pt, int_least16_t, -0x8000 );

    VERIFY_NATIVE_REPRESENTATION( native24_pt );
    VERIFY_VALUE_AND_OPS( native24_pt, int_least32_t,  0x7fffff );
    VERIFY_VALUE_AND_OPS( native24_pt, int_least32_t, -0x800000 );

    VERIFY_NATIVE_REPRESENTATION( native32_pt );
    VERIFY_VALUE_AND_OPS( native32_pt, int_least32_t,  0x7fffffff );
    VERIFY_VALUE_AND_OPS( native32_pt, int_least32_t, -0x7fffffff-1 );

    VERIFY_NATIVE_REPRESENTATION( native40_pt );
    VERIFY_VALUE_AND_OPS( native40_pt, int_least64_t,  0x7fffffffffLL );
    VERIFY_VALUE_AND_OPS( native40_pt, int_least64_t, -0x8000000000LL );

    VERIFY_NATIVE_REPRESENTATION( native48_pt );
    VERIFY_VALUE_AND_OPS( native48_pt, int_least64_t,  0x7fffffffffffLL );
    VERIFY_VALUE_AND_OPS( native48_pt, int_least64_t, -0x800000000000LL );

    VERIFY_NATIVE_REPRESENTATION( native56_pt );
    VERIFY_VALUE_AND_OPS( native56_pt, int_least64_t,  0x7fffffffffffffLL );
    VERIFY_VALUE_AND_OPS( native56_pt, int_least64_t, -0x80000000000000LL );

    VERIFY_NATIVE_REPRESENTATION( native64_pt );
    VERIFY_VALUE_AND_OPS( native64_pt, int_least64_t,  0x7fffffffffffffffLL );
    VERIFY_VALUE_AND_OPS( native64_pt, int_least64_t, -0x7fffffffffffffffLL-1 );

    VERIFY_NATIVE_REPRESENTATION( unative8_pt );
    VERIFY_VALUE_AND_OPS( unative8_pt, uint_least8_t, 0xff );

    VERIFY_NATIVE_REPRESENTATION( unative16_pt );
    VERIFY_VALUE_AND_OPS( unative16_pt, uint_least16_t, 0xffff );

    VERIFY_NATIVE_REPRESENTATION( unative24_pt );
    VERIFY_VALUE_AND_OPS( unative24_pt, uint_least32_t, 0xffffff );

    VERIFY_NATIVE_REPRESENTATION( unative32_pt );
    VERIFY_VALUE_AND_OPS( unative32_pt, uint_least32_t, 0xffffffff );

    VERIFY_NATIVE_REPRESENTATION( unative40_pt );
    VERIFY_VALUE_AND_OPS( unative40_pt, uint_least64_t, 0xffffffffffLL );

    VERIFY_NATIVE_REPRESENTATION( unative48_pt );
    VERIFY_VALUE_AND_OPS( unative48_pt, uint_least64_t, 0xffffffffffffLL );

    VERIFY_NATIVE_REPRESENTATION( unative56_pt );
    VERIFY_VALUE_AND_OPS( unative56_pt, uint_least64_t, 0xffffffffffffffLL );

    VERIFY_NATIVE_REPRESENTATION( unative64_pt );
    VERIFY_VALUE_AND_OPS( unative64_pt, uint_least64_t, 0xffffffffffffffffLL );

    VERIFY_BIG_REPRESENTATION( aligned_big16_pt );
    VERIFY_VALUE_AND_OPS( aligned_big16_pt, int_least16_t,  0x7fff );
    VERIFY_VALUE_AND_OPS( aligned_big16_pt, int_least16_t, -0x8000 );

    VERIFY_BIG_REPRESENTATION( aligned_big32_pt );
    VERIFY_VALUE_AND_OPS( aligned_big32_pt, int_least32_t,  0x7fffffff );
    VERIFY_VALUE_AND_OPS( aligned_big32_pt, int_least32_t, -0x7fffffff-1 );

    VERIFY_BIG_REPRESENTATION( aligned_big64_pt );
    VERIFY_VALUE_AND_OPS( aligned_big64_pt, int_least64_t,  0x7fffffffffffffffLL );
    VERIFY_VALUE_AND_OPS( aligned_big64_pt, int_least64_t, -0x7fffffffffffffffLL-1 );

    VERIFY_BIG_REPRESENTATION( aligned_ubig16_pt );
    VERIFY_VALUE_AND_OPS( aligned_ubig16_pt, uint_least16_t, 0xffff );

    VERIFY_BIG_REPRESENTATION( aligned_ubig32_pt );
    VERIFY_VALUE_AND_OPS( aligned_ubig32_pt, uint_least32_t, 0xffffffff );

    VERIFY_BIG_REPRESENTATION( aligned_ubig64_pt );
    VERIFY_VALUE_AND_OPS( aligned_ubig64_pt, uint_least64_t, 0xffffffffffffffffLL );

    VERIFY_LITTLE_REPRESENTATION( aligned_little16_pt );
    VERIFY_VALUE_AND_OPS( aligned_little16_pt, int_least16_t,  0x7fff );
    VERIFY_VALUE_AND_OPS( aligned_little16_pt, int_least16_t, -0x8000 );

    VERIFY_LITTLE_REPRESENTATION( aligned_little32_pt );
    VERIFY_VALUE_AND_OPS( aligned_little32_pt, int_least32_t,  0x7fffffff );
    VERIFY_VALUE_AND_OPS( aligned_little32_pt, int_least32_t, -0x7fffffff-1 );

    VERIFY_LITTLE_REPRESENTATION( aligned_little64_pt );
    VERIFY_VALUE_AND_OPS( aligned_little64_pt, int_least64_t,  0x7fffffffffffffffLL );
    VERIFY_VALUE_AND_OPS( aligned_little64_pt, int_least64_t, -0x7fffffffffffffffLL-1 );

    VERIFY_LITTLE_REPRESENTATION( aligned_ulittle16_pt );
    VERIFY_VALUE_AND_OPS( aligned_ulittle16_pt, uint_least16_t, 0xffff );

    VERIFY_LITTLE_REPRESENTATION( aligned_ulittle32_pt );
    VERIFY_VALUE_AND_OPS( aligned_ulittle32_pt, uint_least32_t, 0xffffffff );

    VERIFY_LITTLE_REPRESENTATION( aligned_ulittle64_pt );
    VERIFY_VALUE_AND_OPS( aligned_ulittle64_pt, uint_least64_t, 0xffffffffffffffffLL );

  } // check_representation_and_range

  long iterations = 10000000;
  
  template< class Endian >
  Endian timing_test( const char * s)
  {
    cout << s << " timing test, " << iterations << " iterations: ";
    progress_timer t;

    Endian v = 1;
    for ( long i = 0; i < iterations; ++i )
    {
      v += 1;
      v *= 3;
      ++v;
      v *= i;
      if ( i == 0 ) VERIFY_VALUE_AND_OPS( Endian, typename Endian::value_type, 21 );
    }
    return v;
  }

} // unnamed namespace

int main( int argc, char * argv[] )
{
  cout << "Usage: "
       << argv[0] << " [#],\n where # specifies iteration count\n"
          " default iteration count is 1000000" << endl;

  if ( argc > 1 )
    iterations = atol( argv[1] );
  if ( iterations < 1 ) iterations = 1;

  detect_endianness();
  check_size();
  check_alignment();
  check_representation_and_range_and_ops();

  //timing_test<big32_t> ( "big32_t" );
  //timing_test<aligned_big32_t>( "aligned_big32_t" );
  //timing_test<little32_t> ( "little32_t" );
  //timing_test<aligned_little32_t>( "aligned_little32_t" );

  cout << "\n" << err_count << " errors detected\nTest "
       << (err_count==0 ? "passed\n\n" : "failed\n\n");

  return err_count ? 1 : 0;
} // main
