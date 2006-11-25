//  Boost big_radix_whole_test.cpp test file  --------------------------------//

//  Copyright 2006 Daryle Walker.  Distributed under the Boost Software License,
//  Version 1.0.  (See the accompanying file LICENSE_1_0.txt or a copy at
//  <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/math/> for the library's home page.

//  Revision History
//   25 Nov 2006  Initial version (Daryle Walker)

#define BOOST_TEST_MAIN  "big-radix-whole test"

#include <boost/math/big_radix_whole.hpp>  // for boost::math::big_radix_whole

#include <boost/test/unit_test.hpp>                  // for BOOST_CHECK_EQUAL...
#include <boost/test/floating_point_comparison.hpp>  // for BOOST_CHECK_CLOSE...
#include <boost/test/test_case_template.hpp>

#include <boost/cstdint.hpp>                     // for boost::uintmax_t
#include <boost/iterator/counting_iterator.hpp>  // for boost::counting_iterator
#include <boost/lambda/bind.hpp>                 // for boost::lamda::bind
#include <boost/lambda/lambda.hpp>               // for boost::lamda::_1, etc.
#include <boost/mpl/list.hpp>                    // for boost::mpl::list
#include <boost/lexical_cast.hpp>                // for boost::lexical_cast
#include <boost/pool/pool_alloc.hpp>             // for ...fast_pool_allocator
#include <boost/tuple/tuple.hpp>                 // for boost::tuple

#include <algorithm>  // for std::transform
#include <cstddef>    // for std::size_t, ptrdiff_t
#include <iomanip>    // for std:setw, resetiosflags, setfill
#include <ios>        // for std::ios_base, left, right, internal
#include <limits>     // for std::numeric_limits
#include <memory>     // for std::allocator
#include <new>        // for std::nothrow, std::bad_alloc
#include <sstream>    // for std::ostringstream, std::istringstream
#include <string>     // for std::string
#include <vector>     // for std::vector


#pragma mark Intro stuff

// Not every checked type is printable
BOOST_TEST_DONT_PRINT_LOG_VALUE( std::allocator<int> );
BOOST_TEST_DONT_PRINT_LOG_VALUE( fast_pool_allocator<int> );

// Types & templates
using boost::math::big_radix_whole;
using boost::fast_pool_allocator;

typedef big_radix_whole<10>  big_decimal;
typedef big_radix_whole< 8>  big_octal;
typedef big_radix_whole<16>  big_hexadecimal;

typedef fast_pool_allocator<int>  fast_allocator;

namespace
{

// A primitive allocation policy that can screw-up on demand
class rigged
{
    static  bool  rigged_;

public:
    // Required type-aliases
    typedef std::size_t           size_type;
    typedef std::ptrdiff_t  difference_type;

    // Rigging
    static  void    prime()   { rigged_ = true; }
    static  void  unprime()  { rigged_ = false; }
    static  bool    primed()  { return rigged_; }

    // Required functions
    static  char *  malloc( size_type const bytes )
    { return rigged_ ? 0 : new (std::nothrow) char[bytes]; }

    static  void  free( char * const block )
    { delete [] block; }
};

bool  rigged::rigged_ = false;

}  // anonymous namespace

typedef fast_pool_allocator<int, rigged>  rigged_allocator;

namespace
{

// Find the best square-root approximation for INT_MAX
class sqrt_int_max_finder
{
    template < int DigitAmount, bool DigitAmountEven = true >
    struct finder
    {
        static  int const  sqrt_max = 1 << DigitAmount / 2;
    };

    template < int DigitAmount >
    struct finder<DigitAmount, false>
    {
        // I think/hope that built-in integers MUST be base-2.
        // 70/99 is an approximation for 1/sqrt(2)
        static  int const  sqrt_max = (1 << (DigitAmount + 1) / 2) * 70 / 99;
    };

    typedef std::numeric_limits<int>  limits_type;

public:
    static  int const  sqrt_max = finder<limits_type::digits,
     (limits_type::digits % 2 == 0)>::sqrt_max;

};

template < int DigitAmount, bool DigitAmountEven >
int const  sqrt_int_max_finder::finder<DigitAmount, DigitAmountEven>::sqrt_max;

template < int DigitAmount >
int const  sqrt_int_max_finder::finder<DigitAmount, false>::sqrt_max;

int const  sqrt_int_max_finder::sqrt_max;

}  // anonymous namespace

typedef big_radix_whole<sqrt_int_max_finder::sqrt_max>  big_ultimate;


// Helper functions
namespace
{

// Get the beginning iterator for a built-in array
template < typename T, std::size_t N >
inline  T *  array_begin( T (&a)[N] )  { return &a[0]; }

template < typename T, std::size_t N >
inline  T const *  array_begin( T const (&a)[N] )  { return &a[0]; }

// Get the ending iterator for a built-in array
template < typename T, std::size_t N >
inline  T *  array_end( T (&a)[N] )  { return &a[N]; }

template < typename T, std::size_t N >
inline  T const *  array_end( T const (&a)[N] )  { return &a[N]; }

// Create parity maps
std::vector<int>  parity_list( std::size_t max_value )
{
    std::vector<int>  result;

    result.reserve( max_value + 1u );
    for ( std::size_t  i = 0 ; i <= max_value ; ++i )
    {
        result.push_back( i & 1u );
    }
    return result;
}

// Create a flipped copy of a vector
std::vector<bool>  flip_vector( std::vector<bool> const &v )
{  std::vector<bool>  result( v );  result.flip();  return result;  }

}  // anonymous namespace


#pragma mark -
#pragma mark Core Routine Suite

// The basic routines test suite
BOOST_AUTO_TEST_SUITE( core_routine_suite )

// Core constructor test
// (Since the internal implementation is private, we have
//  to hope that the core access routines really work.)
BOOST_AUTO_TEST_CASE( core_constructor_test )
{
    // Default construction
    {
        big_decimal const  object1;

        BOOST_CHECK_EQUAL( 0u, object1.digit_count() );

        BOOST_CHECK_EQUAL( 0, object1.digit_at(0u) );
        BOOST_CHECK_EQUAL( 0, object1.digit_at(1u) );
    }

    // Construction via conversion from built-in unsigned integer
    {
        big_decimal const  object2( 16807u );

        BOOST_CHECK_EQUAL( 5u, object2.digit_count() );

        BOOST_CHECK_EQUAL( 7, object2.digit_at(0u) );
        BOOST_CHECK_EQUAL( 0, object2.digit_at(1u) );
        BOOST_CHECK_EQUAL( 8, object2.digit_at(2u) );
        BOOST_CHECK_EQUAL( 6, object2.digit_at(3u) );
        BOOST_CHECK_EQUAL( 1, object2.digit_at(4u) );

        BOOST_CHECK_EQUAL( 0, object2.digit_at(5u) );
        BOOST_CHECK_EQUAL( 0, object2.digit_at(6u) );

        BOOST_CHECK_EQUAL( 7, object2.quick_digit_at(0u) );
        BOOST_CHECK_EQUAL( 0, object2.quick_digit_at(1u) );
        BOOST_CHECK_EQUAL( 8, object2.quick_digit_at(2u) );
        BOOST_CHECK_EQUAL( 6, object2.quick_digit_at(3u) );
        BOOST_CHECK_EQUAL( 1, object2.quick_digit_at(4u) );
    }

    // Conversion from built-in unsigned integer with single digit result
    {
        big_hexadecimal const  object3( 11u );

        BOOST_CHECK_EQUAL( 1u, object3.digit_count() );

        BOOST_CHECK_EQUAL( 11, object3.digit_at(0u) );

        BOOST_CHECK_EQUAL( 0, object3.digit_at(1u) );
        BOOST_CHECK_EQUAL( 0, object3.digit_at(2u) );

        BOOST_CHECK_EQUAL( 11, object3.quick_digit_at(0u) );
    }

    // Conversion from built-in unsigned integer with multiple digits result
    {
        big_octal const  object4( 11u );

        BOOST_CHECK_EQUAL( 2u, object4.digit_count() );

        BOOST_CHECK_EQUAL( 3, object4.digit_at(0u) );
        BOOST_CHECK_EQUAL( 1, object4.digit_at(1u) );

        BOOST_CHECK_EQUAL( 0, object4.digit_at(2u) );
        BOOST_CHECK_EQUAL( 0, object4.digit_at(3u) );

        BOOST_CHECK_EQUAL( 3, object4.quick_digit_at(0u) );
        BOOST_CHECK_EQUAL( 1, object4.quick_digit_at(1u) );
    }
}

// Allocator checking test
// (It's not too impressive for now since all the allocator types
//  I've found make all objects of the same type equivalent.)
BOOST_AUTO_TEST_CASE( allocator_checking_test )
{
    unsigned const  minimum_bits_per_int = 16u;

    // Standard allocator
    {
        big_decimal const  object1( 123u );

        BOOST_CHECK_EQUAL( object1.get_allocator(), std::allocator<int>() );
        BOOST_CHECK( object1.digit_limit() >= minimum_bits_per_int );
    }

    // Custom allocator
    {
        big_radix_whole<5, fast_allocator>  object2( 12u );

        BOOST_CHECK_EQUAL( object2.get_allocator(), fast_allocator() );
        BOOST_CHECK( object2.digit_limit() >= minimum_bits_per_int );
    }
}

// Printing test
// (It's not actually a core routine for the type, but it is for all the other
//  tests, since I want to use BOOST_CHECK_EQUAL fully, which requires printable
//  types.)
BOOST_AUTO_TEST_CASE( printing_test )
{
    using boost::lexical_cast;
    using std::string;

    BOOST_CHECK_EQUAL( lexical_cast<string>(big_decimal( 0u )), "0" );
    BOOST_CHECK_EQUAL( lexical_cast<string>(big_octal( 0u )), "0" );
    BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( 0u )), "0" );

    BOOST_CHECK_EQUAL( lexical_cast<string>(big_decimal( 5u )), "5" );
    BOOST_CHECK_EQUAL( lexical_cast<string>(big_octal( 5u )), "5" );
    BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( 5u )), "5" );

    BOOST_CHECK_EQUAL( lexical_cast<string>(big_decimal( 9u )), "9" );
    BOOST_CHECK_EQUAL( lexical_cast<string>(big_octal( 9u )), "11" );
    BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( 9u )), "9" );

    BOOST_CHECK_EQUAL( lexical_cast<string>(big_decimal( 11u )), "11" );
    BOOST_CHECK_EQUAL( lexical_cast<string>(big_octal( 11u )), "13" );
    BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( 11u )), "11" );

    BOOST_CHECK_EQUAL( lexical_cast<string>(big_decimal( 17u )), "17" );
    BOOST_CHECK_EQUAL( lexical_cast<string>(big_octal( 17u )), "21" );
    BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( 17u )), "1-1" );

    BOOST_CHECK_EQUAL( lexical_cast<string>(big_decimal( 513u )), "513" );
    BOOST_CHECK_EQUAL( lexical_cast<string>(big_octal( 513u )), "1001" );
    BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( 513u )), "2-0-1" );

    BOOST_CHECK_EQUAL( lexical_cast<string>(big_decimal( 16807u )), "16807" );
    BOOST_CHECK_EQUAL( lexical_cast<string>(big_octal( 16807u )), "40647" );
    BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( 16807u )),
     "4-1-10-7" );
}

// Equality operator test
// (It's not actually a core routine for the type, but it is for all the other
//  tests, since I want to use BOOST_CHECK_EQUAL to check the numeric operators,
//  which requires a type that supports equivalence checks.)
BOOST_AUTO_TEST_CASE( equality_operator_test )
{
    BOOST_CHECK( big_decimal(11u) == big_decimal(11u) );
    BOOST_CHECK( !(big_decimal( 11u ) != big_decimal( 11u )) );

    BOOST_CHECK( !(big_decimal( 11u ) == big_decimal( 9u )) );
    BOOST_CHECK( big_decimal(11u) != big_decimal(9u) );

    BOOST_CHECK( big_octal(11u) == big_octal(11u) );
    BOOST_CHECK( !(big_octal( 11u ) != big_octal( 11u )) );

    BOOST_CHECK( !(big_octal( 11u ) == big_octal( 9u )) );
    BOOST_CHECK( big_octal(11u) != big_octal(9u) );

    BOOST_CHECK( big_hexadecimal(11u) == big_hexadecimal(11u) );
    BOOST_CHECK( !(big_hexadecimal( 11u ) != big_hexadecimal( 11u )) );

    BOOST_CHECK( !(big_hexadecimal( 11u ) == big_hexadecimal( 9u )) );
    BOOST_CHECK( big_hexadecimal(11u) != big_hexadecimal(9u) );
}

// Automatically-defined copy operations test
// (Confirmation of the implicit copy constructor and assignment operator.)
BOOST_AUTO_TEST_CASE( basic_copy_test )
{
    big_octal const  a, b( 6u ), c( 13u );
    big_octal        aa( a ), bb( b ), cc( c );

    BOOST_CHECK_EQUAL( aa, a );
    BOOST_CHECK_EQUAL( bb, b );
    BOOST_CHECK_EQUAL( cc, c );

    aa = b;
    BOOST_CHECK( aa != a );
    BOOST_CHECK_EQUAL( aa, b );

    bb = c;
    BOOST_CHECK( bb != b );
    BOOST_CHECK_EQUAL( bb, c );

    cc = a;
    BOOST_CHECK( cc != c );
    BOOST_CHECK_EQUAL( cc, a );
}

BOOST_AUTO_TEST_SUITE_END()


#pragma mark -
#pragma mark Digit & Assignment Routine Suite

// The not-so-basic routines test suite
BOOST_AUTO_TEST_SUITE( digit_and_assignment_routine_suite )

// Digit-wise constructor test
BOOST_AUTO_TEST_CASE( digit_constructor_test )
{
    using boost::lexical_cast;
    using std::string;

    // Single digit, in-range
    {
        int const  digit = 12;

        BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( &digit, 1
         + &digit )), "12" );
    }

    // Single digit, over the range
    {
        int const  digit = 16;

        BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( &digit, 1
         + &digit )), "1-0" );
    }

    // Two digits, highest one over the range
    {
        int const  digits[] = { 3, 17 };

        BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( digits,
         array_end(digits) )), "1-1-3" );
    }

    // Two digits, lowest one over the range
    {
        int const  digits[] = { 19, 4 };

        BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( digits,
         array_end(digits) )), "5-3" );
    }

    // Two digits, both over the range
    {
        int const  digits[] = { 18, 20 };

        BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( digits,
         array_end(digits) )), "1-5-2" );
    }

    // Two digits, lowest over the range, highest in range; carries should bump
    // highest to over the range
    {
        int const  digits[] = { 49, 14 };

        BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( digits,
         array_end(digits) )), "1-1-1" );
    }

    // Three digits, only lowest over the range; carries should cascade to
    // create fourth digit
    {
        int const  digits[] = { 49, 14, 15 };

        BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( digits,
         array_end(digits) )), "1-0-1-1" );
    }

    // Single digit, under the range
    {
        int const  digit = -3;

        BOOST_CHECK_THROW( big_hexadecimal(&digit, 1 + &digit),
         boost::math::big_radix_whole_negative_result_error );
    }

    // Two digits, lowest under the range
    {
        int const  digits[] = { -3, 4 };

        BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( digits,
         array_end(digits) )), "3-13" );
    }

    // Two digits, highest under the range
    {
        int const  digits[] = { 10, -12 };

        BOOST_CHECK_THROW( big_hexadecimal(digits, array_end( digits )),
         boost::math::big_radix_whole_negative_result_error );
    }

    // Two digits, lowest over the range, highest under the range; carries just
    // enough to cancel
    {
        int const  digits[] = { 88, -5 };

        BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( digits,
         array_end(digits) )), "8" );
    }

    // Two digits, lowest over the range, highest under the range; carries more
    // than enough to cancel
    {
        int const  digits[] = { 51, -2 };

        BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( digits,
         array_end(digits) )), "1-3" );
    }

    // Three digits, lowest under range; carries enough to bring middle digit
    // under, but highest digit will take up the slack
    {
        int const  digits[] = { -48, 1, 5 };

        BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( digits,
         array_end(digits) )), "4-14-0" );
    }

    // Leading zeros
    {
        int const  digits[] = { 6, 0, 0 };

        BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( digits,
         array_end(digits) )), "6" );
    }

    // All zeros
    {
        int const  digits[] = { 0, 0, 0 };

        BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( digits,
         array_end(digits) )), "0" );
    }

    // Empty list
    {
        int const  digit = -64;  // actual value unimportant

        BOOST_CHECK_EQUAL( lexical_cast<string>(big_hexadecimal( &digit,
         &digit )), "0" );
    }
}

// Number-in-string constructor test
BOOST_AUTO_TEST_CASE( string_constructor_test )
{
    using boost::math::big_radix_whole_negative_result_error;
    using boost::math::big_radix_whole_bad_format_error;

    BOOST_CHECK_EQUAL( big_hexadecimal(""), big_hexadecimal(0u) );
    BOOST_CHECK_EQUAL( big_hexadecimal("0"), big_hexadecimal(0u) );
    BOOST_CHECK_EQUAL( big_hexadecimal("+0"), big_hexadecimal(0u) );
    BOOST_CHECK_EQUAL( big_hexadecimal("-00"), big_hexadecimal(0u) );
    BOOST_CHECK_EQUAL( big_hexadecimal("0-0-0"), big_hexadecimal(0u) );

    BOOST_CHECK_EQUAL( big_hexadecimal("1aDf"), big_hexadecimal(0x1ADFu) );
    BOOST_CHECK_EQUAL( big_hexadecimal("+1AdF"), big_hexadecimal(0x1ADFu) );
    BOOST_CHECK_EQUAL( big_hexadecimal("1-10-13-15"),
     big_hexadecimal(0x1ADFu) );
    BOOST_CHECK_EQUAL( big_hexadecimal("+1-10-13-15"),
     big_hexadecimal(0x1ADFu) );

    BOOST_CHECK_THROW( big_hexadecimal("-1aDf"),
     big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( big_hexadecimal("-1-10-13-15"),
     big_radix_whole_negative_result_error );

    BOOST_CHECK_EQUAL( big_decimal("+Z-00-0-1d"), big_decimal(35023u) );
    BOOST_CHECK_EQUAL( big_decimal("+z-0-00-D1"), big_decimal(35131u) );

    BOOST_CHECK_THROW( big_decimal("15+3"), big_radix_whole_bad_format_error );

    BOOST_CHECK_THROW( big_decimal("7*e"), big_radix_whole_bad_format_error );
    BOOST_CHECK_THROW( big_decimal("k4/"), big_radix_whole_bad_format_error );

    BOOST_CHECK_THROW( big_decimal("9--3"), big_radix_whole_bad_format_error );
    BOOST_CHECK_THROW( big_decimal("A-8-"), big_radix_whole_bad_format_error );

    // Need to force a number too large for a single register
    BOOST_REQUIRE( std::numeric_limits<unsigned>::digits <= 64 );
    BOOST_CHECK_EQUAL( boost::lexical_cast<std::string>(big_decimal(
     "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210" )),
     "3876543209876543209876543209876543210" );
    BOOST_CHECK_THROW( big_decimal("+ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210-8"),
     big_radix_whole_bad_format_error );
}

// Cross-version copy construction tests
BOOST_AUTO_TEST_CASE( cross_constructor_test )
{
    big_decimal const  zero( 0u ), one( 1u ), nine( 9u ), twelve( 12u ),
     fortynine( 49u ), sixtyfivethousandfivehundredthirtyfive( 65535u );

    // Zero-valued test
    BOOST_CHECK_EQUAL( big_octal(zero), big_octal(0u) );
    BOOST_CHECK_EQUAL( big_hexadecimal(zero), big_hexadecimal(0u) );

    // Single-digit test
    BOOST_CHECK_EQUAL( big_octal(one), big_octal(01u) );
    BOOST_CHECK_EQUAL( big_hexadecimal(one), big_hexadecimal(0x1u) );

    // Increase in digit count when going to smaller radix
    BOOST_CHECK_EQUAL( big_octal(nine), big_octal(011u) );
    BOOST_CHECK_EQUAL( big_hexadecimal(nine), big_hexadecimal(0x9u) );

    // Decrease in digit count when going to larger radix
    BOOST_CHECK_EQUAL( big_octal(twelve), big_octal(014u) );
    BOOST_CHECK_EQUAL( big_hexadecimal(twelve), big_hexadecimal(0xCu) );

    // Multiple-digit test, all same length
    BOOST_CHECK_EQUAL( big_octal(fortynine), big_octal(061u) );
    BOOST_CHECK_EQUAL( big_hexadecimal(fortynine), big_hexadecimal(0x31u) );

    // Multiply-digit test, all different lengths
    BOOST_CHECK_EQUAL( big_octal(sixtyfivethousandfivehundredthirtyfive),
     big_octal(0177777u) );
    BOOST_CHECK_EQUAL( big_hexadecimal(sixtyfivethousandfivehundredthirtyfive),
     big_hexadecimal(0xFFFFu) );

    // Same radix, different allocator
    typedef big_radix_whole<10, fast_allocator>  new_decimal;

    BOOST_CHECK_EQUAL( new_decimal(zero), new_decimal(0u) );
    BOOST_CHECK_EQUAL( new_decimal(one), new_decimal(1u) );
    BOOST_CHECK_EQUAL( new_decimal(nine), new_decimal(9u) );
    BOOST_CHECK_EQUAL( new_decimal(twelve), new_decimal(12u) );
    BOOST_CHECK_EQUAL( new_decimal(fortynine), new_decimal(49u) );
    BOOST_CHECK_EQUAL( new_decimal(sixtyfivethousandfivehundredthirtyfive),
     new_decimal(65535u) );
}

// Swap operations test
BOOST_AUTO_TEST_CASE( swapping_test )
{
    using boost::math::swap;

    // Make sure the two objects allocate different memory amounts
    big_decimal const  old_a( 4u ), old_b( 0u );
    big_decimal        a( old_a ), b( old_b );

    BOOST_CHECK_EQUAL( old_a, a );
    BOOST_CHECK_EQUAL( old_b, b );
    BOOST_REQUIRE( old_a.digit_count() != old_b.digit_count() );

    a.swap( b );
    BOOST_CHECK_EQUAL( old_a, b );
    BOOST_CHECK_EQUAL( old_b, a );

    b.swap( a );
    BOOST_CHECK_EQUAL( old_a, a );
    BOOST_CHECK_EQUAL( old_b, b );

    swap( b, a );
    BOOST_CHECK_EQUAL( old_a, b );
    BOOST_CHECK_EQUAL( old_b, a );

    swap( a, b );
    BOOST_CHECK_EQUAL( old_a, a );
    BOOST_CHECK_EQUAL( old_b, b );
}

// Object-reset test
BOOST_AUTO_TEST_CASE( reset_test )
{
    // Check non-change from zero to zero
    big_decimal const  z;
    big_decimal        a( 0u ), b;

    BOOST_CHECK_EQUAL( a, b );
    BOOST_CHECK_EQUAL( a, z );
    BOOST_CHECK_EQUAL( b, z );

    a.reset();
    BOOST_CHECK_EQUAL( a, z );

    b.reset();
    BOOST_CHECK_EQUAL( b, z );
    BOOST_CHECK_EQUAL( a, b );

    // Change non-zero values to zero
    big_decimal  c( 1u ), d( 213u );

    BOOST_CHECK( c != d );
    BOOST_CHECK( c != z );
    BOOST_CHECK( d != z );

    c.reset();
    BOOST_CHECK_EQUAL( c, z );

    d.reset();
    BOOST_CHECK_EQUAL( d, z );
    BOOST_CHECK_EQUAL( c, d );
}

// Object-assign and -configure test
BOOST_AUTO_TEST_CASE( assign_configure_test )
{
    big_hexadecimal const  zero( 0u );
    big_hexadecimal        a;

    BOOST_CHECK_EQUAL( a, zero );

    // Re-assign from value
    a.assign( 4u );
    BOOST_CHECK_EQUAL( a, big_hexadecimal(0x4u) );

    a.assign( 0u );
    BOOST_CHECK_EQUAL( a, zero );

    a.assign( 36u );
    BOOST_CHECK_EQUAL( a, big_hexadecimal(0x24u) );

    // Re-assign from digit list
    int const  dl[] = { 1, 2 };

    a.configure( array_begin(dl), array_end(dl) );
    BOOST_CHECK_EQUAL( a, big_hexadecimal(0x21u) );

    a.configure( &dl[1], &dl[1] );
    BOOST_CHECK_EQUAL( a, big_hexadecimal() );

    // Re-assign from digit string
    a.configure( "155" );
    BOOST_CHECK_EQUAL( a, big_hexadecimal(0x155u) );

    a.configure( "" );
    BOOST_CHECK_EQUAL( a, big_hexadecimal() );

    a.configure( "XYZ-89" );  // (3300 + 340 + 35) * 16 + 89 * 1
    BOOST_CHECK_EQUAL( a, big_hexadecimal("14-6-0-9") );
    BOOST_CHECK_EQUAL( a, big_hexadecimal(0xE609u) );

    // Re-assign from object
    big_hexadecimal const  eight( 0x8u ), twentyone( 0x15u );

    a.assign( eight );
    BOOST_CHECK_EQUAL( a, eight );

    a.assign( zero );
    BOOST_CHECK_EQUAL( a, zero );

    a.assign( twentyone );
    BOOST_CHECK_EQUAL( a, twentyone );

    // Re-assign from other object
    big_hexadecimal const  onehundredsixtyone( 0xA1u );

    a.assign( big_octal(0241u) );
    BOOST_CHECK_EQUAL( a, onehundredsixtyone );

    a.assign( big_radix_whole<16, fast_allocator>(0xA1u) );
    BOOST_CHECK_EQUAL( a, onehundredsixtyone );

    a.assign( big_radix_whole<200, fast_allocator>(161u) );
    BOOST_CHECK_EQUAL( a, onehundredsixtyone );

    // Re-assign to single digit
    a.assign_single( 0x8 );
    BOOST_CHECK_EQUAL( a, eight );  // multi-digit to single

    a.assign_single( 0xA );
    BOOST_CHECK_EQUAL( a, big_hexadecimal(0xAu) );  // single-digit to same

    a.assign_single( 0 );
    BOOST_CHECK_EQUAL( a, zero );  // single-digit to empty

    a.assign_single( 0 );
    BOOST_CHECK_EQUAL( a, zero );  // empty to same

    a.assign( twentyone );
    BOOST_CHECK_EQUAL( a, twentyone );
    a.assign_single( 0 );
    BOOST_CHECK_EQUAL( a, zero );  // multi-digit to empty

    a.assign_single( 0x8 );
    BOOST_CHECK_EQUAL( a, eight );  // empty to single-digit
}

BOOST_AUTO_TEST_SUITE_END()


#pragma mark -
#pragma mark Unary Operator & Routine Suite

// The unary operators and the corresponding self-serve member function
BOOST_AUTO_TEST_SUITE( unary_operator_and_routine_suite )

// Boolean conversion test
BOOST_AUTO_TEST_CASE( boolean_conversion_test )
{
    // Boolean conversion operator
    big_decimal const  a, b( 1u ), c( 2u ), d( 14u );

    BOOST_CHECK( !static_cast<bool>(a) );
    BOOST_CHECK(  static_cast<bool>(b) );
    BOOST_CHECK(  static_cast<bool>(c) );
    BOOST_CHECK(  static_cast<bool>(d) );

    // Self-application version of that operator
    big_decimal  aa( a ), bb( b ), cc( c ), dd( d );

    aa.verify_self();
    BOOST_CHECK_EQUAL( aa, a );

    bb.verify_self();
    BOOST_CHECK_EQUAL( bb, b );

    cc.verify_self();
    BOOST_CHECK_EQUAL( cc, b );

    dd.verify_self();
    BOOST_CHECK_EQUAL( dd, b );
}

// Boolean inversion test
BOOST_AUTO_TEST_CASE( boolean_inversion_test )
{
    // Logical-not operator
    big_decimal const  a, b( 1u ), c( 2u ), d( 14u );

    BOOST_CHECK_EQUAL( !a, true );
    BOOST_CHECK_EQUAL( !b, false );
    BOOST_CHECK_EQUAL( !c, false );
    BOOST_CHECK_EQUAL( !d, false );

    // Self-application version of that operator
    big_decimal  aa( a ), bb( b ), cc( c ), dd( d );

    aa.invert_self();
    BOOST_CHECK_EQUAL( aa, b );

    bb.invert_self();
    BOOST_CHECK_EQUAL( bb, a );

    cc.invert_self();
    BOOST_CHECK_EQUAL( cc, a );

    dd.invert_self();
    BOOST_CHECK_EQUAL( dd, a );
}

// Unary plus and minus test
BOOST_AUTO_TEST_CASE( identity_and_negation_test )
{
    using boost::math::big_radix_whole_negative_result_error;

    // Identity and negation operators
    big_decimal const  a, b( 1u ), c( 2u ), d( 14u );

    BOOST_CHECK_EQUAL( +a, a );
    BOOST_CHECK_EQUAL( +b, b );
    BOOST_CHECK_EQUAL( +c, c );
    BOOST_CHECK_EQUAL( +d, d );

    BOOST_CHECK_EQUAL( -a, a );
    BOOST_CHECK_THROW( -b, big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( -c, big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( -d, big_radix_whole_negative_result_error );

    // Self-application versions of those operators
    big_decimal  aa( a ), bb( b ), cc( c ), dd( d );

    aa.identify_self();
    BOOST_CHECK_EQUAL( aa, a );
    bb.identify_self();
    BOOST_CHECK_EQUAL( bb, b );
    cc.identify_self();
    BOOST_CHECK_EQUAL( cc, c );
    dd.identify_self();
    BOOST_CHECK_EQUAL( dd, d );

    aa.negate_self();
    BOOST_CHECK_EQUAL( aa, a );
    BOOST_CHECK_THROW(bb.negate_self(), big_radix_whole_negative_result_error);
    BOOST_CHECK_THROW(cc.negate_self(), big_radix_whole_negative_result_error);
    BOOST_CHECK_THROW(dd.negate_self(), big_radix_whole_negative_result_error);
}

// Increment and decrement test
BOOST_AUTO_TEST_CASE( unit_step_test )
{
    using boost::math::big_radix_whole_negative_result_error;

    // Test the border case at zero, and some regular values
    big_decimal const  zero( 0u ), one( 1u ), two( 2u );
    big_decimal        a;

    BOOST_CHECK_EQUAL( a++, zero );
    BOOST_CHECK_EQUAL( a, one );
    BOOST_CHECK_EQUAL( ++a, two );
    BOOST_CHECK_EQUAL( a, two );

    BOOST_CHECK_EQUAL( a--, two );
    BOOST_CHECK_EQUAL( a, one );
    BOOST_CHECK_EQUAL( --a, zero );
    BOOST_CHECK_EQUAL( a, zero );

    BOOST_CHECK_THROW( --a, big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( a--, big_radix_whole_negative_result_error );

    // Test "bunch-of-zeros"/"bunch-of-nines" transition
    big_decimal  b( 4199u );

    BOOST_CHECK_EQUAL( ++b, big_decimal(4200u) );
    BOOST_CHECK_EQUAL( --b, big_decimal(4199u) );

    // Test transition again, but at change-of-length boundary
    big_decimal  c( 10000u );

    BOOST_CHECK_EQUAL( --c, big_decimal(9999u) );
    BOOST_CHECK_EQUAL( ++c, big_decimal(10000u) );
}

BOOST_AUTO_TEST_SUITE_END()


#pragma mark -
#pragma mark Basic Binary Operator & Routine Suite

// The binary operators and the corresponding self-serve member functions
// (non-arithmetic)
BOOST_AUTO_TEST_SUITE( binary_basic_operator_and_routine_suite )

// Comparisons test
BOOST_AUTO_TEST_CASE( comparison_test )
{
    // Compare values of different digit-lengths
    big_decimal const  zero( 0u ), one( 1u ), twenty( 20u );

    BOOST_CHECK( zero.compare(zero) == 0 );
    BOOST_CHECK( zero.compare(one) < 0 );
    BOOST_CHECK( zero.compare(twenty) < 0 );
    BOOST_CHECK( zero >= zero );
    BOOST_CHECK( zero <= zero );
    BOOST_CHECK( zero <  one );
    BOOST_CHECK( zero <= one );
    BOOST_CHECK( zero <  twenty );
    BOOST_CHECK( zero <= twenty );

    BOOST_CHECK( one.compare(zero) > 0 );
    BOOST_CHECK( one.compare(one) == 0 );
    BOOST_CHECK( one.compare(twenty) < 0 );
    BOOST_CHECK( one >= zero );
    BOOST_CHECK( one >  zero );
    BOOST_CHECK( one >= one );
    BOOST_CHECK( one <= one );
    BOOST_CHECK( one <  twenty );
    BOOST_CHECK( one <= twenty );

    BOOST_CHECK( twenty.compare(zero) > 0 );
    BOOST_CHECK( twenty.compare(one) > 0 );
    BOOST_CHECK( twenty.compare(twenty) == 0 );
    BOOST_CHECK( twenty >= zero );
    BOOST_CHECK( twenty >  zero );
    BOOST_CHECK( twenty >= one );
    BOOST_CHECK( twenty >  one );
    BOOST_CHECK( twenty >= twenty );
    BOOST_CHECK( twenty <= twenty );

    // Compare values of same digit-length, but varying closeness.
    big_decimal const  aa[] = { 25481u,
     25480u, 25482u, 25491u, 25471u, 25581u,
     25381u, 26481u, 24481u, 35481u, 15481u };

    BOOST_CHECK( aa[0].compare(aa[ 0]) == 0 );
     BOOST_CHECK( aa[0] <= aa[ 0] );
     BOOST_CHECK( aa[0] >= aa[ 0] );
    BOOST_CHECK( aa[0].compare(aa[ 1]) >  0 );
     BOOST_CHECK( aa[0] >  aa[ 1] );
     BOOST_CHECK( aa[0] >= aa[ 1] );
    BOOST_CHECK( aa[0].compare(aa[ 2]) <  0 );
     BOOST_CHECK( aa[0] <= aa[ 2] );
     BOOST_CHECK( aa[0] <  aa[ 2] );
    BOOST_CHECK( aa[0].compare(aa[ 3]) <  0 );
     BOOST_CHECK( aa[0] <= aa[ 3] );
     BOOST_CHECK( aa[0] <  aa[ 3] );
    BOOST_CHECK( aa[0].compare(aa[ 4]) >  0 );
     BOOST_CHECK( aa[0] >  aa[ 4] );
     BOOST_CHECK( aa[0] >= aa[ 4] );
    BOOST_CHECK( aa[0].compare(aa[ 5]) <  0 );
     BOOST_CHECK( aa[0] <= aa[ 5] );
     BOOST_CHECK( aa[0] <  aa[ 5] );
    BOOST_CHECK( aa[0].compare(aa[ 6]) >  0 );
     BOOST_CHECK( aa[0] >  aa[ 6] );
     BOOST_CHECK( aa[0] >= aa[ 6] );
    BOOST_CHECK( aa[0].compare(aa[ 7]) <  0 );
     BOOST_CHECK( aa[0] <= aa[ 7] );
     BOOST_CHECK( aa[0] <  aa[ 7] );
    BOOST_CHECK( aa[0].compare(aa[ 8]) >  0 );
     BOOST_CHECK( aa[0] >  aa[ 8] );
     BOOST_CHECK( aa[0] >= aa[ 8] );
    BOOST_CHECK( aa[0].compare(aa[ 9]) <  0 );
     BOOST_CHECK( aa[0] <= aa[ 9] );
     BOOST_CHECK( aa[0] <  aa[ 9] );
    BOOST_CHECK( aa[0].compare(aa[10]) >  0 );
     BOOST_CHECK( aa[0] >  aa[10] );
     BOOST_CHECK( aa[0] >= aa[10] );

    // Inverted tests
    BOOST_CHECK( !(aa[0] >  aa[ 0]) );
    BOOST_CHECK( !(aa[0] <  aa[ 0]) );
    BOOST_CHECK( !(aa[0] >  aa[ 9]) );
    BOOST_CHECK( !(aa[0] >= aa[ 9]) );
    BOOST_CHECK( !(aa[0] <  aa[10]) );
    BOOST_CHECK( !(aa[0] <= aa[10]) );
}

// Shifting test
BOOST_AUTO_TEST_CASE( shifting_test )
{
    // Shifting zero values doesn't do anything
    big_decimal const  zero;
    big_decimal        zz( zero );

    BOOST_CHECK_EQUAL( zero >> 0u, zero );
    BOOST_CHECK_EQUAL( zero >> 1u, zero );
    BOOST_CHECK_EQUAL( zero >> 6u, zero );

    BOOST_CHECK_EQUAL( zero << 0u, zero );
    BOOST_CHECK_EQUAL( zero << 1u, zero );
    BOOST_CHECK_EQUAL( zero << 6u, zero );

    BOOST_CHECK_EQUAL( zz >>= 0u, zero );
    BOOST_CHECK_EQUAL( zz >>= 1u, zero );
    BOOST_CHECK_EQUAL( zz >>= 6u, zero );

    BOOST_CHECK_EQUAL( zz <<= 0u, zero );
    BOOST_CHECK_EQUAL( zz <<= 1u, zero );
    BOOST_CHECK_EQUAL( zz <<= 6u, zero );

    BOOST_CHECK_EQUAL( zz.mod_radix_shift_down(), 0 );
    BOOST_CHECK_EQUAL( zz, zero );

    zz.shift_up_add_single( 0 );
    BOOST_CHECK_EQUAL( zz, zero );

    // Shift up a nonzero digit from zero
    zz.shift_up_add_single( 4 );
    BOOST_CHECK_EQUAL( zz, big_decimal(4u) );

    BOOST_CHECK_EQUAL( zz.mod_radix_shift_down(), 4 );
    BOOST_CHECK_EQUAL( zz, zero );

    // Shift around non-zero values
    big_decimal const  one = 1u, ten = 10u, hundred = 100u, thousand = 1000u;

    BOOST_CHECK_EQUAL( one << 3u, thousand );
    BOOST_CHECK_EQUAL( one >> 3u, zero );
    BOOST_CHECK_EQUAL( hundred >> 3u, zero );
    BOOST_CHECK_EQUAL( hundred << 1u, thousand );
    BOOST_CHECK_EQUAL( hundred >> 1u, ten );
    BOOST_CHECK_EQUAL( hundred >> 2u, one );

    // Shift zero digits from non-zero
    big_decimal  a( one );

    a.shift_up_add_single( 0 );
    BOOST_CHECK_EQUAL( a, ten );

    a.shift_up_add_single( 0 );
    BOOST_CHECK_EQUAL( a, hundred );

    BOOST_CHECK_EQUAL( a.mod_radix_shift_down(), 0 );
    BOOST_CHECK_EQUAL( a, ten );

    // Shift down then up is lossy
    big_decimal const  two = 2u, twentythree = 23u, twohundredthirty = 230u;

    BOOST_CHECK_EQUAL( twentythree << 1u, twohundredthirty );
    BOOST_CHECK_EQUAL( twohundredthirty >> 1u, twentythree );
    BOOST_CHECK_EQUAL( (twohundredthirty >> 1u) << 1u, twohundredthirty );
    BOOST_CHECK_EQUAL( (twentythree << 1u) >> 1u, twentythree );
    BOOST_CHECK_EQUAL( twentythree >> 1u, two );
    BOOST_CHECK( (two << 1u) != twentythree );
    BOOST_CHECK( (twentythree >> 1u) << 1u != twentythree );

    // Shift custom digits to prevent loss
    big_decimal  b( two );

    b.shift_up_add_single( 3 );
    BOOST_CHECK_EQUAL( b, twentythree );

    b.shift_up_add_single( 0 );
    BOOST_CHECK_EQUAL( b, twohundredthirty );

    BOOST_CHECK_EQUAL( b.mod_radix_shift_down(), 0 );
    BOOST_CHECK_EQUAL( b, twentythree );

    BOOST_CHECK_EQUAL( b.mod_radix_shift_down(), 3 );
    BOOST_CHECK_EQUAL( b, two );
}

BOOST_AUTO_TEST_SUITE_END()


#pragma mark -
#pragma mark Single-Precision Binary Operator & Routine Suite

// The binary operators and the corresponding self-serve member function
// (artihmetic, single-precision)
BOOST_AUTO_TEST_SUITE( binary_single_operator_and_routine_suite )

// Add with single digits test
BOOST_AUTO_TEST_CASE( single_add_test )
{
    big_decimal const  zero, one(1u), two(2u), thirty(30u), fortyfive(45u);
    big_decimal        a;

    // Adding zero to anything, no matter the place, doesn't change anything
    a = zero;
    a.add_shifted_single( 0, 0u );
    BOOST_CHECK_EQUAL( a, zero );

    a = one;
    a.add_shifted_single( 0, 0u );
    BOOST_CHECK_EQUAL( a, one );
    a.add_shifted_single( 0, 1u );
    BOOST_CHECK_EQUAL( a, one );
    a.add_shifted_single( 0, 234u );
    BOOST_CHECK_EQUAL( a, one );

    a = fortyfive;
    a.add_shifted_single( 0, 0u );
    BOOST_CHECK_EQUAL( a, fortyfive );
    a.add_shifted_single( 0, 1u );
    BOOST_CHECK_EQUAL( a, fortyfive );
    a.add_shifted_single( 0, 2u );
    BOOST_CHECK_EQUAL( a, fortyfive );
    a.add_shifted_single( 0, 5678u );
    BOOST_CHECK_EQUAL( a, fortyfive );

    // Add within the given digit range, without carries
    a.add_shifted_single( 2, 0u );
    BOOST_CHECK_EQUAL( a, big_decimal(47u) );
    a.add_shifted_single( 4, 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(87u) );

    // Add enough to carry, but stay within the given digit range
    a.add_shifted_single( 5, 0u );
    BOOST_CHECK_EQUAL( a, big_decimal(92u) );

    // Add enough to highest-placed digit to carry to new place
    a.add_shifted_single( 1, 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(102u) );

    // Add enough to carry over a run of nines, but stay within range
    a.assign( 239945ul );
    a.add_shifted_single( 7, 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(240015ul) );

    // Add enough to carry over a nine-run to a new place
    a.assign( 99987654ul );
    a.add_shifted_single( 5, 4u );
    BOOST_CHECK_EQUAL( a, big_decimal(100037654ul) );

    // Add non-zero digit outside the range
    a = thirty;
    a.add_shifted_single( 3, 2u );
    BOOST_CHECK_EQUAL( a, big_decimal(330u) );
    a.add_shifted_single( 4, 4u );
    BOOST_CHECK_EQUAL( a, big_decimal(40330ul) );
    a.add_shifted_single( 5, 7u );
    BOOST_CHECK_EQUAL( a, big_decimal(50040330ul) );

    // Try out the one's-place shortcut version
    a = zero;
    a.add_single( 0 );
    BOOST_CHECK_EQUAL( a, zero );
    a.add_single( 2 );
    BOOST_CHECK_EQUAL( a, two );
    a.add_single( 9 );
    BOOST_CHECK_EQUAL( a, big_decimal(11u) );
    a.add_single( 9 );
    BOOST_CHECK_EQUAL( a, big_decimal(20u) );

    a.assign( 2899u );
    a.add_single( 3 );
    BOOST_CHECK_EQUAL( a, big_decimal(2902u) );

    a.assign( 999u );
    a.add_single( 4 );
    BOOST_CHECK_EQUAL( a, big_decimal(1003u) );
}

// Subtract with single digits test
BOOST_AUTO_TEST_CASE( single_subtract_test )
{
    using boost::math::big_radix_whole_negative_result_error;

    big_decimal const  zero, one(1u), two(2u), thirty(30u), fortyfive(45u);
    big_decimal        a;

    // Subtracting zero from anything, no matter the place, makes no change
    a = zero;
    a.subtract_shifted_single( 0, 0u );
    BOOST_CHECK_EQUAL( a, zero );

    a = one;
    a.subtract_shifted_single( 0, 0u );
    BOOST_CHECK_EQUAL( a, one );
    a.subtract_shifted_single( 0, 1u );
    BOOST_CHECK_EQUAL( a, one );
    a.subtract_shifted_single( 0, 234u );
    BOOST_CHECK_EQUAL( a, one );

    a = fortyfive;
    a.subtract_shifted_single( 0, 0u );
    BOOST_CHECK_EQUAL( a, fortyfive );
    a.subtract_shifted_single( 0, 1u );
    BOOST_CHECK_EQUAL( a, fortyfive );
    a.subtract_shifted_single( 0, 2u );
    BOOST_CHECK_EQUAL( a, fortyfive );
    a.subtract_shifted_single( 0, 5678u );
    BOOST_CHECK_EQUAL( a, fortyfive );

    // Subtract within the given digit range, without borrows
    a.subtract_shifted_single( 3, 0u );
    BOOST_CHECK_EQUAL( a, big_decimal(42u) );
    a.subtract_shifted_single( 2, 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(22u) );

    // Subtract enough to borrow, but stay within the given digit range
    a.subtract_shifted_single( 5, 0u );
    BOOST_CHECK_EQUAL( a, big_decimal(17u) );

    // Subtract enough from highest-placed digit to remove that digit
    a.subtract_shifted_single( 1, 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(7u) );

    // Subtract enough to borrow over a zero-run and remove top digit
    a.assign( 100634ul );
    a.subtract_shifted_single( 7, 2u );
    BOOST_CHECK_EQUAL( a, big_decimal(99934ul) );

    // Subtract enough to borrow over a run of zeros, but stay within range
    a.assign( 300028ul );
    a.subtract_shifted_single( 4, 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(299988ul) );

    // Subtract a larger non-zero digit just in the range
    BOOST_CHECK_THROW( a.subtract_shifted_single(6, 5u),
     big_radix_whole_negative_result_error );

    // Subtract a non-zero digit outside the range
    BOOST_CHECK_THROW( a.subtract_shifted_single(3, 6u),
     big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( a.subtract_shifted_single(4, 7u),
     big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( a.subtract_shifted_single(5, 9u),
     big_radix_whole_negative_result_error );

    // Try out the one's-place shortcut version
    a.assign( 3u );
    a.subtract_single( 1 );
    BOOST_CHECK_EQUAL( a, two );
    BOOST_CHECK_THROW( a.subtract_single(4),
     big_radix_whole_negative_result_error );
    a.subtract_single( 0 );
    BOOST_CHECK_EQUAL( a, two );
    a.subtract_single( 2 );
    BOOST_CHECK_EQUAL( a, zero );
    a.subtract_single( 0 );
    BOOST_CHECK_EQUAL( a, zero );
    BOOST_CHECK_THROW( a.subtract_single(1),
     big_radix_whole_negative_result_error );

    a = thirty;
    a.subtract_single( 3 );
    BOOST_CHECK_EQUAL( a, big_decimal(27u) );
    a.subtract_single( 9 );
    BOOST_CHECK_EQUAL( a, big_decimal(18u) );
    a.subtract_single( 8 );
    BOOST_CHECK_EQUAL( a, big_decimal(10u) );
    a.subtract_single( 2 );
    BOOST_CHECK_EQUAL( a, big_decimal(8u) );
}

// Subtract-absolutely with single digits test
BOOST_AUTO_TEST_CASE( single_subtract_absolutely_test )
{
    big_decimal const  zero, one( 1u ), two( 2u ), fortyfive( 45u );
    big_decimal        a;

    // Subtracting zero from anything, no matter the place, makes no change
    a = zero;
    BOOST_CHECK( !a.subtract_single_absolutely(0) );
    BOOST_CHECK_EQUAL( a, zero );
    BOOST_CHECK( !a.subtract_shifted_single_absolutely(0, 1u) );
    BOOST_CHECK_EQUAL( a, zero );

    a = one;
    BOOST_CHECK( !a.subtract_single_absolutely(0) );
    BOOST_CHECK_EQUAL( a, one );
    BOOST_CHECK( !a.subtract_shifted_single_absolutely(0, 0u) );
    BOOST_CHECK_EQUAL( a, one );
    BOOST_CHECK( !a.subtract_shifted_single_absolutely(0, 1u) );
    BOOST_CHECK_EQUAL( a, one );
    BOOST_CHECK( !a.subtract_shifted_single_absolutely(0, 234u) );
    BOOST_CHECK_EQUAL( a, one );

    a = fortyfive;
    BOOST_CHECK( !a.subtract_single_absolutely(0) );
    BOOST_CHECK_EQUAL( a, fortyfive );
    BOOST_CHECK( !a.subtract_shifted_single_absolutely(0, 0u) );
    BOOST_CHECK_EQUAL( a, fortyfive );
    BOOST_CHECK( !a.subtract_shifted_single_absolutely(0, 1u) );
    BOOST_CHECK_EQUAL( a, fortyfive );
    BOOST_CHECK( !a.subtract_shifted_single_absolutely(0, 2u) );
    BOOST_CHECK_EQUAL( a, fortyfive );
    BOOST_CHECK( !a.subtract_shifted_single_absolutely(0, 5678u) );
    BOOST_CHECK_EQUAL( a, fortyfive );

    // Subtract within the given digit range, without borrows
    BOOST_CHECK( !a.subtract_shifted_single_absolutely(3, 0u) );
    BOOST_CHECK_EQUAL( a, big_decimal(42u) );
    BOOST_CHECK( !a.subtract_shifted_single_absolutely(2, 1u) );
    BOOST_CHECK_EQUAL( a, big_decimal(22u) );

    // Subtract enough to borrow, but stay within the given digit range
    BOOST_CHECK( !a.subtract_shifted_single_absolutely(5, 0u) );
    BOOST_CHECK_EQUAL( a, big_decimal(17u) );

    // Subtract enough from highest-placed digit to remove that digit
    BOOST_CHECK( !a.subtract_shifted_single_absolutely(1, 1u) );
    BOOST_CHECK_EQUAL( a, big_decimal(7u) );

    // Subtract enough to borrow over a zero-run and remove top digit
    a.assign( 100634ul );
    BOOST_CHECK( !a.subtract_shifted_single_absolutely(7, 2u) );
    BOOST_CHECK_EQUAL( a, big_decimal(99934ul) );

    // Subtract enough to borrow over a run of zeros, but stay within range
    a.assign( 300028ul );
    BOOST_CHECK( !a.subtract_shifted_single_absolutely(4, 1u) );
    BOOST_CHECK_EQUAL( a, big_decimal(299988ul) );

    // Subtract a larger non-zero digit just in the range
    BOOST_CHECK( a.subtract_shifted_single_absolutely(6, 5u) );
    BOOST_CHECK_EQUAL( a, big_decimal(300012ul) );

    // Subtract a non-zero digit outside the range
    BOOST_CHECK( a.subtract_shifted_single_absolutely(3, 6u) );  // just outside
    BOOST_CHECK_EQUAL( a, big_decimal(2699988ul) );

    BOOST_CHECK( a.subtract_shifted_single_absolutely(5, 9u) );  // w/ padding
    BOOST_CHECK_EQUAL( a, big_decimal("4997300012") );

    // Subtract within single digits
    a = two;
    BOOST_CHECK( !a.subtract_single_absolutely(1) );
    BOOST_CHECK_EQUAL( a, one );

    // Subtract to zero
    BOOST_CHECK( !a.subtract_single_absolutely(1) );
    BOOST_CHECK_EQUAL( a, zero );

    // Subtract from zero
    BOOST_CHECK( a.subtract_single_absolutely(2) );
    BOOST_CHECK_EQUAL( a, two );

    // Subtract past zero
    BOOST_CHECK( a.subtract_single_absolutely(3) );
    BOOST_CHECK_EQUAL( a, one );

    // Double-to-single digit transition
    a.assign( 12u );
    BOOST_CHECK( !a.subtract_single_absolutely(1) );
    BOOST_CHECK_EQUAL( a, big_decimal(11u) );

    BOOST_CHECK( !a.subtract_single_absolutely(3) );
    BOOST_CHECK_EQUAL( a, big_decimal(8u) );

    // Subtract from zero to multiple-digit value
    a.reset();
    BOOST_CHECK( a.subtract_shifted_single_absolutely(3, 1u) );
    BOOST_CHECK_EQUAL( a, big_decimal(30u) );

    // Subtract to zero with multiple-digit value
    BOOST_CHECK( !a.subtract_shifted_single_absolutely(3, 1u) );
    BOOST_CHECK_EQUAL( a, zero );

    // Subtract past zero with reducing digit count
    a.assign( 187u );
    BOOST_CHECK( a.subtract_shifted_single_absolutely(2, 2u) );
    BOOST_CHECK_EQUAL( a, big_decimal(13u) );

    a.assign( 7965u );
    BOOST_CHECK( a.subtract_shifted_single_absolutely(8, 3u) );
    BOOST_CHECK_EQUAL( a, big_decimal(35u) );

    a.assign( 29994u );
    BOOST_CHECK( a.subtract_shifted_single_absolutely(3, 4u) );
    BOOST_CHECK_EQUAL( a, big_decimal(6u) );
}

// Multiply with single digits, with fused add, test
BOOST_AUTO_TEST_CASE( single_multiply_then_add_test )
{
    big_decimal const  zero, one(1u), two(2u), thirty(30u), fortyfive(45u);
    big_decimal        a;

    // Multiplying zero by anything, then adding, gives that last addend
    a = zero;
    a.multiply_single_add_single( 0, 0 );
    BOOST_CHECK_EQUAL( a, zero );

    a.multiply_single_add_single( 3, 0 );
    BOOST_CHECK_EQUAL( a, zero );

    a.multiply_single_add_single( 6, 1 );
    BOOST_CHECK_EQUAL( a, one );

    a.reset();
    a.multiply_single_add_single( 5, 2 );
    BOOST_CHECK_EQUAL( a, two );

    // Multiplying anything by zero, then adding, gives that last addend
    a = thirty;
    a.multiply_single_add_single( 0, 0 );
    BOOST_CHECK_EQUAL( a, zero );

    a = thirty;
    a.multiply_single_add_single( 0, 1 );
    BOOST_CHECK_EQUAL( a, one );

    // Multiply without carries
    a = thirty;
    a.multiply_single_add_single( 2, 4 );
    BOOST_CHECK_EQUAL( a, big_decimal(64u) );

    a.multiply_single_add_single( 1, 5 );
    BOOST_CHECK_EQUAL( a, big_decimal(69u) );

    // Carry only through the last addend
    a.multiply_single_add_single( 1, 2 );
    BOOST_CHECK_EQUAL( a, big_decimal(71u) );

    // Carry only through multiplication, creating new highest place
    a.multiply_single_add_single( 2, 3 );
    BOOST_CHECK_EQUAL( a, big_decimal(145u) );

    // Carry from both steps
    a.assign( 263u );
    a.multiply_single_add_single( 3, 2 );
    BOOST_CHECK_EQUAL( a, big_decimal(791u) );

    // Worst case
    a.assign( 999u );
    a.multiply_single_add_single( 9, 9 );
    BOOST_CHECK_EQUAL( a, big_decimal(9000u) );

    // Try shortcut version without the addition
    a.multiply_single( 3 );
    BOOST_CHECK_EQUAL( a, big_decimal(27000u) );

    a.assign( 3 );
    a.multiply_single( 2 );
    BOOST_CHECK_EQUAL( a, big_decimal(6u) );
}

// Multiply with single digits, with fused subtract, test
BOOST_AUTO_TEST_CASE( single_multiply_then_subtract_test )
{
    using boost::math::big_radix_whole_negative_result_error;

    big_decimal const  zero, one( 1u ), two( 2u ), thirty( 30u ),
                       fortyfive( 45u ), sixhundredseventyeight( 678u );
    big_decimal        a;

    // Multiplying anything by zero, then subtracting zero, gives zero;
    // subtracting a non-zero instead gives an error
    a = zero;
    BOOST_CHECK_THROW( a.multiply_single_subtract_single(0, 1),
     big_radix_whole_negative_result_error );
    a.multiply_single_subtract_single( 0, 0 );
    BOOST_CHECK_EQUAL( a, zero );

    a = two;
    BOOST_CHECK_THROW( a.multiply_single_subtract_single(0, 2),
     big_radix_whole_negative_result_error );
    a.multiply_single_subtract_single( 0, 0 );
    BOOST_CHECK_EQUAL( a, zero );

    a = thirty;
    BOOST_CHECK_THROW( a.multiply_single_subtract_single(0, 4),
     big_radix_whole_negative_result_error );
    a.multiply_single_subtract_single( 0, 0 );
    BOOST_CHECK_EQUAL( a, zero );

    a = sixhundredseventyeight;
    BOOST_CHECK_THROW( a.multiply_single_subtract_single(0, 8),
     big_radix_whole_negative_result_error );
    a.multiply_single_subtract_single( 0, 0 );
    BOOST_CHECK_EQUAL( a, zero );

    // Multiplying by a number too low to overcome the subtrahend gives an error
    a = zero;
    BOOST_CHECK_THROW( a.multiply_single_subtract_single(5, 2),
     big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( a.multiply_single_subtract_single(9, 1),
     big_radix_whole_negative_result_error );

    a = one;
    BOOST_CHECK_THROW( a.multiply_single_subtract_single(2, 5),
     big_radix_whole_negative_result_error );

    a = two;
    BOOST_CHECK_THROW( a.multiply_single_subtract_single(4, 9),
     big_radix_whole_negative_result_error );

    // Normal use with single-digit minuend-multiplicand
    a = one;
    a.multiply_single_subtract_single( 5, 3 );
    BOOST_CHECK_EQUAL( a, two );

    a.multiply_single_subtract_single( 7, 5 );
    BOOST_CHECK_EQUAL( a, big_decimal(9u) );

    a.multiply_single_subtract_single( 2, 8 );
    BOOST_CHECK_EQUAL( a, big_decimal(10u) );

    // Uses with multiple-digit minuend-multiplicands
    a.multiply_single_subtract_single( 1, 2 );
    BOOST_CHECK_EQUAL( a, big_decimal(8u) );  // decrease length

    a = fortyfive;
    a.multiply_single_subtract_single( 2, 9 );
    BOOST_CHECK_EQUAL( a, big_decimal(81u) );  // no length change; w/ borrow

    a.multiply_single_subtract_single( 3, 1 );
    BOOST_CHECK_EQUAL( a, big_decimal(242u) );  // increase length

    a.assign( 25u );
    a.multiply_single_subtract_single( 4, 2 );
    BOOST_CHECK_EQUAL( a, big_decimal(98u) );  // increase then decrease length

    a.multiply_single_subtract_single( 1, 3 );
    BOOST_CHECK_EQUAL( a, big_decimal(95u) );  // no length change nor borrow
}

// Multiply with single digits, with fused add, in reverse order, test
BOOST_AUTO_TEST_CASE( add_single_product_test )
{
    big_decimal const  zero, one(1u), two(2u), thirty(30u), fortyfive(45u);
    big_decimal        a;

    // Adding a zero single-product doesn't change anything
    a = zero;
    a.add_single_product( 0, 0 );
    BOOST_CHECK_EQUAL( a, zero );

    a = one;
    a.add_single_product( 4, 0 );
    BOOST_CHECK_EQUAL( a, one );

    a = thirty;
    a.add_single_product( 0, 3 );
    BOOST_CHECK_EQUAL( a, thirty );

    a.add_shifted_single_product( 0, 2, 1u );
    BOOST_CHECK_EQUAL( a, thirty );
    a.add_shifted_single_product( 5, 0, 2u );
    BOOST_CHECK_EQUAL( a, thirty );
    a.add_shifted_single_product( 0, 0, 3u );
    BOOST_CHECK_EQUAL( a, thirty );

    // Check results with 0, 1, or more digits in the bignum and 1 or 2 digits
    // in the short product.  With and without a zero in the one's place.  With
    // and without extra carries.
    a = zero;
    a.add_single_product( 2, 1 );
    BOOST_CHECK_EQUAL( a, two );

    a = zero;
    a.add_single_product( 6, 5 );
    BOOST_CHECK_EQUAL( a, thirty );

    a = zero;
    a.add_single_product( 5, 9 );
    BOOST_CHECK_EQUAL( a, fortyfive );

    a = two;
    a.add_single_product( 2, 1 );
    BOOST_CHECK_EQUAL( a, big_decimal(4u) );

    a = two;
    a.add_single_product( 3, 3 );
    BOOST_CHECK_EQUAL( a, big_decimal(11u) );

    a = two;
    a.add_single_product( 6, 5 );
    BOOST_CHECK_EQUAL( a, big_decimal(32u) );

    a = two;
    a.add_single_product( 7, 7 );
    BOOST_CHECK_EQUAL( a, big_decimal(51u) );

    a = fortyfive;
    a.add_single_product( 2, 1 );
    BOOST_CHECK_EQUAL( a, big_decimal(47u) );

    a = fortyfive;
    a.add_single_product( 3, 2 );
    BOOST_CHECK_EQUAL( a, big_decimal(51u) );

    a = fortyfive;
    a.add_single_product( 4, 5 );
    BOOST_CHECK_EQUAL( a, big_decimal(65u) );

    a = fortyfive;
    a.add_single_product( 7, 8 );
    BOOST_CHECK_EQUAL( a, big_decimal(101u) );

    a.assign( 66u );
    a.add_single_product( 7, 8 );
    BOOST_CHECK_EQUAL( a, big_decimal(122u) );

    // Worst case
    a.assign( 9999u );
    a.add_single_product( 9, 9 );
    BOOST_CHECK_EQUAL( a, big_decimal(10080u) );

    // Out-of-bounds adding
    a.assign( 120u );
    a.add_shifted_single_product( 8, 9, 3u );
    BOOST_CHECK_EQUAL( a, big_decimal(72120ul) );
    a.add_shifted_single_product( 3, 2, 7u );
    BOOST_CHECK_EQUAL( a, big_decimal(60072120ul) );
}

// Multiply with single digits, with fused subtract, in reverse order, test
BOOST_AUTO_TEST_CASE( subtract_single_product_test )
{
    using boost::math::big_radix_whole_negative_result_error;

    big_decimal const  zero, one( 1u ), two( 2u ), thirty( 30u ),
                       fortyfive( 45u ), sixhundredseventyeight( 678u );
    big_decimal        a;

    // Subtracting a zero single-product doesn't change anything
    a = zero;
    a.subtract_single_product( 0, 0 );
    BOOST_CHECK_EQUAL( a, zero );

    a = one;
    a.subtract_single_product( 4, 0 );
    BOOST_CHECK_EQUAL( a, one );

    a = thirty;
    a.subtract_single_product( 0, 3 );
    BOOST_CHECK_EQUAL( a, thirty );

    a = sixhundredseventyeight;
    a.subtract_single_product( 0, 3 );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );

    a.subtract_shifted_single_product( 0, 6, 1u );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );
    a.subtract_shifted_single_product( 7, 0, 2u );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );
    a.subtract_shifted_single_product( 0, 0, 3u );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );
    a.subtract_shifted_single_product( 8, 0, 11u );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );

    // Check results with 0, 1, 2, or more digits in the bignum and 1 or 2
    // digits in the short product.  With and without a zero in the one's place.
    // With and without extra borrows.
    a = zero;
    BOOST_CHECK_THROW( a.subtract_single_product(2, 1),
     big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( a.subtract_single_product(6, 5),
     big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( a.subtract_single_product(5, 9),
     big_radix_whole_negative_result_error );

    a = two;
    a.subtract_single_product( 1, 1 );
    BOOST_CHECK_EQUAL( a, one );
    BOOST_CHECK_THROW( a.subtract_single_product(3, 2),
     big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( a.subtract_single_product(4, 5),
     big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( a.subtract_single_product(9, 9),
     big_radix_whole_negative_result_error );
    a.subtract_single_product( 1, 1 );
    BOOST_CHECK_EQUAL( a, zero );

    a = fortyfive;
    BOOST_CHECK_THROW( a.subtract_single_product(8, 7),
     big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( a.subtract_single_product(8, 6),
     big_radix_whole_negative_result_error );
    a.subtract_single_product( 1, 1 );
    BOOST_CHECK_EQUAL( a, big_decimal(44u) );
    a.subtract_single_product( 2, 7 );
    BOOST_CHECK_EQUAL( a, thirty );
    a.subtract_single_product( 2, 5 );
    BOOST_CHECK_EQUAL( a, big_decimal(20u) );
    a.subtract_single_product( 4, 3 );
    BOOST_CHECK_EQUAL( a, big_decimal(8u) );

    a = sixhundredseventyeight;
    a.subtract_single_product( 2, 3 );
    BOOST_CHECK_EQUAL( a, big_decimal(672u) );
    a.subtract_single_product( 3, 3 );
    BOOST_CHECK_EQUAL( a, big_decimal(663u) );
    a.subtract_single_product( 4, 5 );
    BOOST_CHECK_EQUAL( a, big_decimal(643u) );
    a.subtract_single_product( 7, 9 );
    BOOST_CHECK_EQUAL( a, big_decimal(580u) );
    a.subtract_single_product( 9, 9 );
    BOOST_CHECK_EQUAL( a, big_decimal(499u) );

    a.subtract_shifted_single_product( 7, 7, 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(9u) );
    BOOST_CHECK_THROW( a.subtract_shifted_single_product(3, 2, 5u),
     big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( a.subtract_shifted_single_product(4, 3, 2u),
     big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( a.subtract_shifted_single_product(2, 4, 1u),
     big_radix_whole_negative_result_error );
}

// Multiply with single digits, fused subtract-absolutely, reverse order, test
BOOST_AUTO_TEST_CASE( subtract_single_product_absolutely_test )
{
    big_decimal const  zero, eight( 8u ), thirtyfive( 35u ),
                       onehundredsixtyseven( 167u );
    big_decimal        a;

    // Subtracting a zero-valued product doesn't change anything
    a = zero;
    BOOST_CHECK( !a.subtract_single_product_absolutely(0, 0) );
    BOOST_CHECK_EQUAL( a, zero );
    BOOST_CHECK( !a.subtract_single_product_absolutely(3, 0) );
    BOOST_CHECK_EQUAL( a, zero );
    BOOST_CHECK( !a.subtract_single_product_absolutely(0, 8) );
    BOOST_CHECK_EQUAL( a, zero );

    a = thirtyfive;
    BOOST_CHECK( !a.subtract_single_product_absolutely(0, 0) );
    BOOST_CHECK_EQUAL( a, thirtyfive );
    BOOST_CHECK( !a.subtract_single_product_absolutely(2, 0) );
    BOOST_CHECK_EQUAL( a, thirtyfive );
    BOOST_CHECK( !a.subtract_single_product_absolutely(0, 7) );
    BOOST_CHECK_EQUAL( a, thirtyfive );

    // Subtracting with product digit(s) outside allocation
    a = zero;
    BOOST_CHECK( a.subtract_single_product_absolutely(4, 6) );  // 2 non-0 dgt's
    BOOST_CHECK_EQUAL( a, big_decimal(24u) );
    a.reset();
    BOOST_CHECK( a.subtract_single_product_absolutely(3, 3) );  // single digit
    BOOST_CHECK_EQUAL( a, big_decimal(9u) );
    a.reset();
    BOOST_CHECK( a.subtract_single_product_absolutely(2, 5) );  // only hi non-0
    BOOST_CHECK_EQUAL( a, big_decimal(10u) );
    a.reset();
    BOOST_CHECK( a.subtract_shifted_single_product_absolutely(7, 8, 2u) );
    BOOST_CHECK_EQUAL( a, big_decimal(5600u) );

    // Subtracting with product digit(s) straddling allocation
    a = eight;
    BOOST_CHECK( a.subtract_single_product_absolutely(4, 6) );  // 2 non-0 dgt's
    BOOST_CHECK_EQUAL( a, big_decimal(16u) );
    a = eight;
    BOOST_CHECK( a.subtract_single_product_absolutely(3, 3) );  // 1 digit, -
    BOOST_CHECK_EQUAL( a, big_decimal(1u) );
    a = eight;
    BOOST_CHECK( !a.subtract_single_product_absolutely(2, 3) ); // 1 digit, +
    BOOST_CHECK_EQUAL( a, big_decimal(2u) );
    a = eight;
    BOOST_CHECK( a.subtract_single_product_absolutely(5, 8) );  // only hi non-0
    BOOST_CHECK_EQUAL( a, big_decimal(32u) );

    // Subtracting with product digit(s) within allocation
    a = thirtyfive;
    BOOST_CHECK( a.subtract_single_product_absolutely(7, 8) );  // 2 non-0, -
    BOOST_CHECK_EQUAL( a, big_decimal(21u) );
    a = thirtyfive;
    BOOST_CHECK( !a.subtract_single_product_absolutely(4, 6) ); // 2 non-0, +
    BOOST_CHECK_EQUAL( a, big_decimal(11u) );
    a = thirtyfive;
    BOOST_CHECK( !a.subtract_single_product_absolutely(3, 3) ); // single digit
    BOOST_CHECK_EQUAL( a, big_decimal(26u) );
    a = thirtyfive;
    BOOST_CHECK( a.subtract_single_product_absolutely(5, 8) );  // hi non-0, -
    BOOST_CHECK_EQUAL( a, big_decimal(5u) );
    a = thirtyfive;
    BOOST_CHECK( !a.subtract_single_product_absolutely(5, 4) );  // hi non-0, +
    BOOST_CHECK_EQUAL( a, big_decimal(15u) );

    // Subtract with product digit(s) totally within allocation
    a = onehundredsixtyseven;
    BOOST_CHECK( !a.subtract_single_product_absolutely(2, 4) ); // 1d, borrow
    BOOST_CHECK_EQUAL( a, big_decimal(159u) );
    BOOST_CHECK( !a.subtract_single_product_absolutely(5, 1) ); // 1d, no borrow
    BOOST_CHECK_EQUAL( a, big_decimal(154u) );
    BOOST_CHECK( !a.subtract_single_product_absolutely(3, 6) ); // 2d, borrow
    BOOST_CHECK_EQUAL( a, big_decimal(136u) );
    BOOST_CHECK( !a.subtract_single_product_absolutely(2, 7) ); // 2d, no borrow
    BOOST_CHECK_EQUAL( a, big_decimal(122u) );
    BOOST_CHECK( !a.subtract_single_product_absolutely(9, 8) ); // 2d, borrow @1
    BOOST_CHECK_EQUAL( a, big_decimal(50u) );
}

// Divide by single digit, with single-digit remainder, test
BOOST_AUTO_TEST_CASE( single_div_and_mod_test )
{
    using boost::math::big_radix_whole_divide_by_zero_error;

    big_decimal const  zero, one( 1u ), two( 2u ), thirty( 30u ),
                       fortyfive( 45u ), sixhundredseventyeight( 678u );
    big_decimal        a;

    // Dividing by zero is an error
    a = zero;
    BOOST_CHECK_THROW( a.div_and_mod_single(0),
     big_radix_whole_divide_by_zero_error );

    a = two;
    BOOST_CHECK_THROW( a.div_and_mod_single(0),
     big_radix_whole_divide_by_zero_error );

    a = thirty;
    BOOST_CHECK_THROW( a.div_and_mod_single(0),
     big_radix_whole_divide_by_zero_error );

    a = sixhundredseventyeight;
    BOOST_CHECK_THROW( a.div_and_mod_single(0),
     big_radix_whole_divide_by_zero_error );

    // Division of zero always gives zero
    a = zero;
    BOOST_CHECK_EQUAL( a.div_and_mod_single(1), 0 );
    BOOST_CHECK_EQUAL( a, zero );

    BOOST_CHECK_EQUAL( a.div_and_mod_single(2), 0 );
    BOOST_CHECK_EQUAL( a, zero );

    BOOST_CHECK_EQUAL( a.div_and_mod_single(3), 0 );
    BOOST_CHECK_EQUAL( a, zero );

    BOOST_CHECK_EQUAL( a.div_and_mod_single(5), 0 );
    BOOST_CHECK_EQUAL( a, zero );

    BOOST_CHECK_EQUAL( a.div_and_mod_single(7), 0 );
    BOOST_CHECK_EQUAL( a, zero );

    // No remainder, no length change
    a = two;
    BOOST_CHECK_EQUAL( a.div_and_mod_single(2), 0 );
    BOOST_CHECK_EQUAL( a, one );

    a.assign( 9u );
    BOOST_CHECK_EQUAL( a.div_and_mod_single(3), 0 );
    BOOST_CHECK_EQUAL( a, big_decimal(3u) );

    a.assign( 90u );
    BOOST_CHECK_EQUAL( a.div_and_mod_single(2), 0 );
    BOOST_CHECK_EQUAL( a, fortyfive );

    a = sixhundredseventyeight;
    BOOST_CHECK_EQUAL( a.div_and_mod_single(6), 0 );
    BOOST_CHECK_EQUAL( a, big_decimal(113u) );

    // No remainder, change length
    a.assign( 14u );
    BOOST_CHECK_EQUAL( a.div_and_mod_single(7), 0 );
    BOOST_CHECK_EQUAL( a, two );

    a.assign( 102u );
    BOOST_CHECK_EQUAL( a.div_and_mod_single(3), 0 );
    BOOST_CHECK_EQUAL( a, big_decimal(34u) );

    // Has remainder, change length
    a = two;
    BOOST_CHECK_EQUAL( a.div_and_mod_single(5), 2 );
    BOOST_CHECK_EQUAL( a, zero );

    a = thirty;
    BOOST_CHECK_EQUAL( a.div_and_mod_single(7), 2 );
    BOOST_CHECK_EQUAL( a, big_decimal(4u) );

    a.assign( 100u );
    BOOST_CHECK_EQUAL( a.div_and_mod_single(9), 1 );
    BOOST_CHECK_EQUAL( a, big_decimal(11u) );

    // Has remainder, no length change
    a = fortyfive;
    BOOST_CHECK_EQUAL( a.div_and_mod_single(2), 1 );
    BOOST_CHECK_EQUAL( a, big_decimal(22u) );

    a = sixhundredseventyeight;
    BOOST_CHECK_EQUAL( a.div_and_mod_single(5), 3 );
    BOOST_CHECK_EQUAL( a, big_decimal(135u) );

    // Reverse check
    a.multiply_single_add_single( 5, 3 );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );
}

// Modulo by single digit test
BOOST_AUTO_TEST_CASE( single_mod_test )
{
    using boost::math::big_radix_whole_divide_by_zero_error;

    big_decimal const  zero, two(2u), thirty(30u), sixhundredseventyeight(678u);
    big_decimal        a;

    // Dividing by zero is an error
    a = zero;
    BOOST_CHECK_THROW( a.modulo_single(0),
     big_radix_whole_divide_by_zero_error );

    a = two;
    BOOST_CHECK_THROW( a.modulo_single(0),
     big_radix_whole_divide_by_zero_error );

    a = thirty;
    BOOST_CHECK_THROW( a.modulo_single(0),
     big_radix_whole_divide_by_zero_error );

    a = sixhundredseventyeight;
    BOOST_CHECK_THROW( a.modulo_single(0),
     big_radix_whole_divide_by_zero_error );

    // Division of zero always gives a zero remainder
    a = zero;
    BOOST_CHECK_EQUAL( a.modulo_single(1), 0 );
    BOOST_CHECK_EQUAL( a.modulo_single(2), 0 );
    BOOST_CHECK_EQUAL( a.modulo_single(3), 0 );
    BOOST_CHECK_EQUAL( a.modulo_single(5), 0 );
    BOOST_CHECK_EQUAL( a.modulo_single(7), 0 );

    // Non-zero divisor, various dividend digit-lengths
    a = two;
    BOOST_CHECK_EQUAL( a.modulo_single(1), 0 );
    BOOST_CHECK_EQUAL( a.modulo_single(2), 0 );
    BOOST_CHECK_EQUAL( a.modulo_single(5), 2 );

    a.assign_single( 8 );
    BOOST_CHECK_EQUAL( a.modulo_single(2), 0 );
    BOOST_CHECK_EQUAL( a.modulo_single(3), 2 );
    BOOST_CHECK_EQUAL( a.modulo_single(5), 3 );
    BOOST_CHECK_EQUAL( a.modulo_single(9), 8 );

    ++a;
    BOOST_CHECK_EQUAL( a.modulo_single(2), 1 );
    BOOST_CHECK_EQUAL( a.modulo_single(3), 0 );
    BOOST_CHECK_EQUAL( a.modulo_single(5), 4 );

    a.assign( 14u );
    BOOST_CHECK_EQUAL( a.modulo_single(7), 0 );
    a.add_single( 2 );
    BOOST_CHECK_EQUAL( a.modulo_single(7), 2 );
    a.add_single( 7 );
    BOOST_CHECK_EQUAL( a.modulo_single(7), 2 );
    a.add_single( 9 );
    BOOST_CHECK_EQUAL( a.modulo_single(7), 4 );

    a = sixhundredseventyeight;
    BOOST_CHECK_EQUAL( a.modulo_single(6), 0 );
    BOOST_CHECK_EQUAL( a.modulo_single(5), 3 );
}

// Modulo by 2, a.k.a. odd vs. even, test
// (There are different implementations for odd radices, even radices, and a
// base-2 exclusive.  Need to test zero, single-digit, and multi-digit values.)
typedef ::boost::mpl::list<big_radix_whole<2>, big_radix_whole<3>, big_decimal>
  parity_test_types;
BOOST_AUTO_TEST_CASE_TEMPLATE( parity_test, T, parity_test_types )
{
    using namespace boost::lambda;

    using std::vector;
    using std::transform;

    // The highest value to check (lowest is zero)
    std::size_t const  highest( 27u ), length( highest + 1u );

    // The expected results
    vector<int> const   parities = parity_list( highest );
    vector<bool> const  odds( parities.begin(), parities.end() );
    vector<bool> const  evens = flip_vector( odds );

    // Set up the source values (as big_radix_whole objects)
    boost::counting_iterator<T> const  cb( 0u ), ce( length );

    // Parity test
    {
        // Compute the results
        vector<int>  parity_results( length );

        transform( cb, ce, parity_results.begin(), bind(&T::parity, _1) );

        // Check the results
        BOOST_CHECK_EQUAL_COLLECTIONS( parity_results.begin(),
         parity_results.end(), parities.begin(), parities.end() );
    }

    // Odd-check test
    {
        // Compute the results
        vector<bool>  odd_results( length );

        transform( cb, ce, odd_results.begin(), bind(&T::is_odd, _1) );

        // Check the results
        BOOST_CHECK_EQUAL_COLLECTIONS( odd_results.begin(), odd_results.end(),
         odds.begin(), odds.end() );
    }

    // Even-check test
    {
        // Compute the results
        vector<bool>  even_results( length );

        transform( cb, ce, even_results.begin(), bind(&T::is_even, _1) );

        // Check the results
        BOOST_CHECK_EQUAL_COLLECTIONS( even_results.begin(), even_results.end(),
         evens.begin(), evens.end() );
    }
}

BOOST_AUTO_TEST_SUITE_END()


#pragma mark -
#pragma mark Mixed-Precision Binary Operator & Routine Suite

// The binary operators and the corresponding self-serve member function
// (artihmetic, various precisions mixed together)
BOOST_AUTO_TEST_SUITE( binary_mixed_operator_and_routine_suite )

// Multiply with mixed sizes, fused add, reverse order, test
BOOST_AUTO_TEST_CASE( add_mixed_product_test )
{
    big_decimal const  zero, one( 1u ), two( 2u ), thirty( 30u ),
                       fortyfive( 45u ), sixhundredseventyeight( 678u );
    big_decimal        a;

    // Adding a zero product to something doesn't change that something
    a = one;
    a.add_mixed_product( fortyfive, 0, 3u );
    BOOST_CHECK_EQUAL( a, one );
    a.add_mixed_product( zero, 0, 4u );
    BOOST_CHECK_EQUAL( a, one );
    a.add_mixed_product( zero, 7 );
    BOOST_CHECK_EQUAL( a, one );

    // Adding a product to zero gets that product
    a = zero;
    a.add_mixed_product( fortyfive, 5, 2u );
    BOOST_CHECK_EQUAL( a, big_decimal(22500u) );  // use all reserved space

    a.reset();
    a.add_mixed_product( two, 4 );
    BOOST_CHECK_EQUAL( a, big_decimal(8u) );  // 1-digit, no carry (1 spare sp)

    a.reset();
    a.add_mixed_product( fortyfive, 2, 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(900u) );  // 2-digit, 1 carry (1 spare sp)

    // Add product to non-zero
    a = one;
    a.add_mixed_product( fortyfive, 5, 2u );
    BOOST_CHECK_EQUAL( a, big_decimal(22501u) );  // no overlap
    a.add_mixed_product( two, 4 );
    BOOST_CHECK_EQUAL( a, big_decimal(22509u) );  // overlap w/ no carry
    a.add_mixed_product( fortyfive, 2, 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(23409u) );  // overlap w/ carry in +
    a.add_mixed_product( big_decimal(249u), 4, 2u );
    BOOST_CHECK_EQUAL( a, big_decimal(123009ul) );  // carries in + and *

    a = thirty;
    a.add_mixed_product( sixhundredseventyeight, 9, 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(61050ul) );  // carries in *
}

// Multiply with mixed sizes, fused subtract, reverse order, test
BOOST_AUTO_TEST_CASE( subtract_mixed_product_test )
{
    using boost::math::big_radix_whole_negative_result_error;

    big_decimal const  zero, one( 1u ), two( 2u ), thirty( 30u ),
                       fortyfive( 45u ), sixhundredseventyeight( 678u );
    big_decimal        a;

    // Subtracting a zero product from something doesn't change that something
    a = one;
    a.subtract_mixed_product( two, 0 );
    BOOST_CHECK_EQUAL( a, one );
    a.subtract_mixed_product( zero, 9 );
    BOOST_CHECK_EQUAL( a, one );
    a.subtract_mixed_product( zero, 0, 7u );
    BOOST_CHECK_EQUAL( a, one );

    // Subtracting something non-zero from zero is an error
    a.reset();
    BOOST_CHECK_THROW( a.subtract_mixed_product(fortyfive, 4),
     big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( a.subtract_mixed_product(thirty, 5),
     big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( a.subtract_mixed_product(two, 9, 8u),
     big_radix_whole_negative_result_error );

    // Conventional use
    a = fortyfive;
    a.subtract_mixed_product( thirty, 1 );
    BOOST_CHECK_EQUAL( a, big_decimal(15u) );  // no carry* nor borrow-
    a.subtract_mixed_product( two, 5 );
    BOOST_CHECK_EQUAL( a, big_decimal(5u) );  // carry*, no borrow-, reduce len

    a.assign( 60u );
    a.subtract_mixed_product( two, 3, 1u );
    BOOST_CHECK_EQUAL( a, zero );  // no carry* nor borrow-, reduce to 0

    a = sixhundredseventyeight;
    a.subtract_mixed_product( thirty, 2, 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(78u) );  // no carry* no borrow- reduce le
    a = sixhundredseventyeight;
    a.subtract_mixed_product( fortyfive, 2 );
    BOOST_CHECK_EQUAL( a, big_decimal(588u) );  // carry* and borrow-

    a = fortyfive;
    a.subtract_mixed_product( two, 3 );
    BOOST_CHECK_EQUAL( a, big_decimal(39u) );  // no carry, have borrow-

    // Oversized subtrahends (check for correct restore)
    a = fortyfive;
    BOOST_CHECK_THROW( a.subtract_mixed_product(two, 3, 1u),
     big_radix_whole_negative_result_error );  // oversized peak digit
    BOOST_CHECK_EQUAL( a, fortyfive );
    BOOST_CHECK_THROW( a.subtract_mixed_product(big_decimal( 6u ), 8),
     big_radix_whole_negative_result_error );  // oversized one's digit
    BOOST_CHECK_EQUAL( a, fortyfive );
    BOOST_CHECK_THROW( a.subtract_mixed_product(one, 8, 2u),
     big_radix_whole_negative_result_error );  // oversized index
    BOOST_CHECK_EQUAL( a, fortyfive );
    BOOST_CHECK_THROW( a.subtract_mixed_product(sixhundredseventyeight, 9),
     big_radix_whole_negative_result_error );  // oversized full-factor
    BOOST_CHECK_EQUAL( a, fortyfive );
    BOOST_CHECK_THROW( a.subtract_mixed_product(thirty, 1, 1u),
     big_radix_whole_negative_result_error );  // oversized full-factor + index
    BOOST_CHECK_EQUAL( a, fortyfive );
}

// Multiply with mixed sizes, fused subtract-absolutely, reverse order, test
BOOST_AUTO_TEST_CASE( subtract_mixed_product_absolutely_test )
{
    big_decimal const  zero, one( 1u ), two( 2u ), thirty( 30u ),
                       fortyfive( 45u ), sixhundredseventyeight( 678u );
    big_decimal        a;

    // Subtracting zero from something doesn't change that something
    a = one;
    BOOST_CHECK( !a.subtract_mixed_product_absolutely(two, 0) );
    BOOST_CHECK_EQUAL( a, one );
    BOOST_CHECK( !a.subtract_mixed_product_absolutely(zero, 2) );
    BOOST_CHECK_EQUAL( a, one );
    BOOST_CHECK( !a.subtract_mixed_product_absolutely(zero, 0, 3u) );
    BOOST_CHECK_EQUAL( a, one );

    // Subtracting from zero by a non-zero
    a.reset();
    BOOST_CHECK( a.subtract_mixed_product_absolutely(two, 3, 1u) );
    BOOST_CHECK_EQUAL( a, big_decimal(60u) );  // no carry*, excess sp
    a.reset();
    BOOST_CHECK( a.subtract_mixed_product_absolutely(fortyfive, 3) );
    BOOST_CHECK_EQUAL( a, big_decimal(135u) );  // carry*, used sp

    // Subtracting equal or lesser values
    a = sixhundredseventyeight;
    BOOST_CHECK( !a.subtract_mixed_product_absolutely(fortyfive, 3) );
    BOOST_CHECK_EQUAL( a, big_decimal(543u) );  // carry*, no borrow-
    BOOST_CHECK( !a.subtract_mixed_product_absolutely(thirty, 2) );
    BOOST_CHECK_EQUAL( a, big_decimal(483u) );  // no carry*, borrow-
    BOOST_CHECK( !a.subtract_mixed_product_absolutely(two, 4, 1u) );
    BOOST_CHECK_EQUAL( a, big_decimal(403u) );  // no carry*, no borrow-
    BOOST_CHECK( !a.subtract_mixed_product_absolutely(big_decimal( 38u ), 8) );
    BOOST_CHECK_EQUAL( a, big_decimal(99u) );  // carry*, borrow-, reduce length
    BOOST_CHECK( !a.subtract_mixed_product_absolutely(big_decimal( 11u ), 9) );
    BOOST_CHECK_EQUAL( a, zero );  // no carry*, no borrow-, reduce to 0

    // Subtracting greater values
    a = sixhundredseventyeight;
    BOOST_CHECK( a.subtract_mixed_product_absolutely(fortyfive, 2, 1u) );
    BOOST_CHECK_EQUAL( a, big_decimal(222u) );  // borrow at peak
    BOOST_CHECK( a.subtract_mixed_product_absolutely(thirty, 3, 3u) );
    BOOST_CHECK_EQUAL( a, big_decimal(89778ul) );  // no overlap
    BOOST_CHECK( a.subtract_mixed_product_absolutely(two, 8, 4u) );
    BOOST_CHECK_EQUAL( a, big_decimal(70222ul) );  // overlap
    BOOST_CHECK( a.subtract_mixed_product_absolutely(big_decimal(12u), 6, 3u) );
    BOOST_CHECK_EQUAL( a, big_decimal(1778u) );  // cascading borrow, reduce len
}

BOOST_AUTO_TEST_SUITE_END()


#pragma mark -
#pragma mark Full-Precision Binary Operator & Routine Suite

// The binary operators and the corresponding self-serve member function
// (artihmetic, full-precision)
BOOST_AUTO_TEST_SUITE( binary_full_operator_and_routine_suite )

// Add with multiple digits test
BOOST_AUTO_TEST_CASE( full_add_test )
{
    big_decimal const  zero, one( 1u ), two( 2u ), thirty( 30u ),
                       fortyfive( 45u ), sixhundredseventyeight( 678u );
    big_decimal        a;

    // Adding zero to something doesn't change that something
    a = zero;
    a.add_shifted_full( zero, 5u );
    BOOST_CHECK_EQUAL( a, zero );
    a += zero;
    BOOST_CHECK_EQUAL( a, zero );
    BOOST_CHECK_EQUAL( a + zero, zero );

    a = one;
    a.add_shifted_full( zero, 2u );
    BOOST_CHECK_EQUAL( a, one );
    a += zero;
    BOOST_CHECK_EQUAL( a, one );
    BOOST_CHECK_EQUAL( a + zero, one );

    a = sixhundredseventyeight;
    a.add_shifted_full( zero, 1u );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );
    a.add_shifted_full( zero, 2u );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );
    a.add_shifted_full( zero, 3u );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );
    a.add_shifted_full( zero, 4u );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );
    a += zero;
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );
    BOOST_CHECK_EQUAL( zero + a, sixhundredseventyeight );

    // Adding values shifted past the augend's digits
    a = zero;
    a.add_shifted_full( two, 3u );
    BOOST_CHECK_EQUAL( a, big_decimal(2000u) );  // spaced
    a.reset();
    a += thirty;
    BOOST_CHECK_EQUAL( a, thirty );  // immediately after

    a.add_shifted_full( fortyfive, 4u );
    BOOST_CHECK_EQUAL( a, big_decimal(450030ul) );  // spaced
    a.add_shifted_full( one, 6u );
    BOOST_CHECK_EQUAL( a, big_decimal(1450030ul) );  // immediately after

    // Adding values partially shifted past the augend's digits
    a = sixhundredseventyeight;
    a.add_shifted_full( sixhundredseventyeight, 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(7458u) );  // carries
    a.assign( 6047u );
    a.add_shifted_full( big_decimal(302u), 2u );
    BOOST_CHECK_EQUAL( a, big_decimal(36247ul) );  // no carries

    // Adding values shifted to peak where the augend peaks
    a = sixhundredseventyeight;
    a.add_shifted_full( big_decimal(21u), 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(888u) );  // no carries
    a.add_shifted_full( big_decimal(13u), 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(1018u) );  // propagated internal carry
    a.add_shifted_full( big_decimal(91u), 2u );
    BOOST_CHECK_EQUAL( a, big_decimal(10118u) );  // peak-place carry

    // Adding values shifted, but totally within, the augend's digits
    a.assign( 53294ul );
    a.add_shifted_full( big_decimal(23u), 2u );
    BOOST_CHECK_EQUAL( a, big_decimal(55594ul) );  // no carries
    a.add_shifted_full( big_decimal(11u), 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(55704ul) );  // internal carry
    a.assign( 99954ul );
    a.add_shifted_full( big_decimal(6u), 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(100014ul) );  // propagated internal carry
    a.assign( 997431ul );
    a.add_shifted_full( big_decimal(28u), 2u );
    BOOST_CHECK_EQUAL( a, big_decimal(1000231ul) );  // propagated double carry

    // Adding unshifted values
    a = fortyfive;
    a.add_shifted_full( thirty, 0u );
    BOOST_CHECK_EQUAL( a, big_decimal(75u) );  // no carries
    a += big_decimal( 7u );
    BOOST_CHECK_EQUAL( a, big_decimal(82u) );  // internal carry
    BOOST_CHECK_EQUAL( a + big_decimal(18u), one << 2u );  // propagating carry
}

// Subtract with multiple digits test
BOOST_AUTO_TEST_CASE( full_subtract_test )
{
    using boost::math::big_radix_whole_negative_result_error;

    big_decimal const  zero, one( 1u ), two( 2u ), thirty( 30u ),
                       fortyfive( 45u ), sixhundredseventyeight( 678u );
    big_decimal        a;

    // Subtracting zero from something doesn't change that something
    a = zero;
    a.subtract_shifted_full( zero, 5u );
    BOOST_CHECK_EQUAL( a, zero );
    a -= zero;
    BOOST_CHECK_EQUAL( a, zero );
    BOOST_CHECK_EQUAL( a - zero, zero );

    a = one;
    a.subtract_shifted_full( zero, 2u );
    BOOST_CHECK_EQUAL( a, one );
    a -= zero;
    BOOST_CHECK_EQUAL( a, one );
    BOOST_CHECK_EQUAL( a - zero, one );

    a = sixhundredseventyeight;
    a.subtract_shifted_full( zero, 1u );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );
    a.subtract_shifted_full( zero, 2u );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );
    a.subtract_shifted_full( zero, 3u );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );
    a.subtract_shifted_full( zero, 4u );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );
    a -= zero;
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );
    BOOST_CHECK_EQUAL( a - zero, sixhundredseventyeight );

    // Subtracting from zero, by a non-zero, is an error
    a.reset();
    BOOST_CHECK_THROW( a.subtract_shifted_full(one, 45u),
     big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( a.subtract_shifted_full(one, 1u),
     big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( a.subtract_shifted_full(one, 0u),
     big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( a -= one, big_radix_whole_negative_result_error );
    BOOST_CHECK_THROW( zero - one, big_radix_whole_negative_result_error );

    // Other ways of subtracting a larger number is an error
    a = fortyfive;
    BOOST_CHECK_THROW( a -= sixhundredseventyeight,
     big_radix_whole_negative_result_error );  // longer
    BOOST_CHECK_THROW( a.subtract_shifted_full(two, 2u),
     big_radix_whole_negative_result_error );  // shifted past
    BOOST_CHECK_THROW( a.subtract_shifted_full(two, 5u),
     big_radix_whole_negative_result_error );  // way shifted past
    BOOST_CHECK_THROW( a.subtract_shifted_full(big_decimal( 5u ), 1u),
     big_radix_whole_negative_result_error );  // greater after shifting to peak
    a.assign( 67700ul );
    BOOST_CHECK_THROW( a.subtract_shifted_full(sixhundredseventyeight, 2u),
     big_radix_whole_negative_result_error );  // greater after shifting to peak

    // Subtracting to zero
    a.assign( 67800ul );
    a.subtract_shifted_full( sixhundredseventyeight, 2u );
    BOOST_CHECK_EQUAL( a, zero );

    a = fortyfive;
    a -= fortyfive;
    BOOST_CHECK_EQUAL( a, zero );
    BOOST_CHECK_EQUAL( thirty - thirty, zero );

    // Subtract without borrows
    a = fortyfive;
    a -= thirty;
    BOOST_CHECK_EQUAL( a, big_decimal(15u) );

    // Subtract with internal borrows
    a.assign( 7532u );
    a.subtract_shifted_full( fortyfive, 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(7082u) );

    // Subtract with cascading borrows
    a -= big_decimal( 91u );
    BOOST_CHECK_EQUAL( a, big_decimal(6991u) );  // keep length

    a.assign( 1001u );
    a.subtract_shifted_full( two, 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(981u) );  // reduce length
}

// Subtract-absolutely with multiple digits test
BOOST_AUTO_TEST_CASE( full_subtract_absolutely_test )
{
    big_decimal const  zero, one( 1u ), two( 2u ), thirty( 30u ),
                       fortyfive( 45u ), sixhundredseventyeight( 678u );
    big_decimal        a;

    // Subtracting zero from something doesn't change that something
    a = zero;
    BOOST_CHECK( !a.subtract_shifted_full_absolutely(zero, 5u) );
    BOOST_CHECK_EQUAL( a, zero );
    BOOST_CHECK( !a.subtract_full_absolutely(zero) );
    BOOST_CHECK_EQUAL( a, zero );

    a = one;
    BOOST_CHECK( !a.subtract_shifted_full_absolutely(zero, 2u) );
    BOOST_CHECK_EQUAL( a, one );
    BOOST_CHECK( !a.subtract_full_absolutely(zero) );
    BOOST_CHECK_EQUAL( a, one );

    a = sixhundredseventyeight;
    BOOST_CHECK( !a.subtract_shifted_full_absolutely(zero, 1u) );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );
    BOOST_CHECK( !a.subtract_shifted_full_absolutely(zero, 2u) );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );
    BOOST_CHECK( !a.subtract_shifted_full_absolutely(zero, 3u) );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );
    BOOST_CHECK( !a.subtract_shifted_full_absolutely(zero, 4u) );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );
    BOOST_CHECK( !a.subtract_full_absolutely(zero) );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );

    // Subtracting from zero, by a non-zero
    a.reset();
    BOOST_CHECK( a.subtract_shifted_full_absolutely(one, 5u) );
    BOOST_CHECK_EQUAL( a, big_decimal(100000ul) );
    a.reset();
    BOOST_CHECK( a.subtract_shifted_full_absolutely(thirty, 2u) );
    BOOST_CHECK_EQUAL( a, big_decimal(3000u) );
    a.reset();
    BOOST_CHECK( a.subtract_shifted_full_absolutely(fortyfive, 1u) );
    BOOST_CHECK_EQUAL( a, big_decimal(450u) );
    a.reset();
    BOOST_CHECK( a.subtract_full_absolutely(sixhundredseventyeight) );
    BOOST_CHECK_EQUAL( a, sixhundredseventyeight );

    // Subtracting equal or lesser values
    a.assign( 200u );
    BOOST_CHECK( !a.subtract_shifted_full_absolutely(two, 2u) );
    BOOST_CHECK_EQUAL( a, zero );
    a.assign( 300u );
    BOOST_CHECK( !a.subtract_shifted_full_absolutely(thirty, 1u) );
    BOOST_CHECK_EQUAL( a, zero );
    a = fortyfive;
    BOOST_CHECK( !a.subtract_full_absolutely(fortyfive) );
    BOOST_CHECK_EQUAL( a, zero );

    a = sixhundredseventyeight;
    BOOST_CHECK( !a.subtract_full_absolutely(fortyfive) );
    BOOST_CHECK_EQUAL( a, big_decimal(633u) );  // no borrow
    a = sixhundredseventyeight;
    BOOST_CHECK( !a.subtract_shifted_full_absolutely(fortyfive, 1u) );
    BOOST_CHECK_EQUAL( a, big_decimal(228u) );  // no borrow

    BOOST_CHECK( !a.subtract_full_absolutely(thirty) );
    BOOST_CHECK_EQUAL( a, big_decimal(198u) );  // borrow
    a.assign( 10034u );
    BOOST_CHECK( !a.subtract_shifted_full_absolutely(fortyfive, 1u) );
    BOOST_CHECK_EQUAL( a, big_decimal(9584u) );  // borrow w/ shorter length

    // Subtracting greater values
    a = sixhundredseventyeight;
    BOOST_CHECK( a.subtract_shifted_full_absolutely(fortyfive, 2u) );
    BOOST_CHECK_EQUAL( a, big_decimal(3822u) );  // slight overlap
    a = sixhundredseventyeight;
    BOOST_CHECK( a.subtract_shifted_full_absolutely(fortyfive, 3u) );
    BOOST_CHECK_EQUAL( a, big_decimal(44322ul) );  // adjacent
    a = sixhundredseventyeight;
    BOOST_CHECK( a.subtract_shifted_full_absolutely(fortyfive, 4u) );
    BOOST_CHECK_EQUAL( a, big_decimal(449322ul) );  // spaced
    a = sixhundredseventyeight;
    BOOST_CHECK( a.subtract_shifted_full_absolutely(fortyfive, 6u) );
    BOOST_CHECK_EQUAL( a, big_decimal(44999322ul) );  // really spaced

    a = sixhundredseventyeight;
    BOOST_CHECK( a.subtract_shifted_full_absolutely(big_decimal( 48u ), 2u) );
    BOOST_CHECK_EQUAL( a, big_decimal(4122u) );  // slight overlap with borrow

    a = sixhundredseventyeight;
    BOOST_CHECK( a.subtract_shifted_full_absolutely(big_decimal( 68u ), 1u) );
    BOOST_CHECK_EQUAL( a, two );  // greater before peak, reduce length
    a = sixhundredseventyeight;
    BOOST_CHECK( a.subtract_shifted_full_absolutely(big_decimal( 7u ), 2u) );
    BOOST_CHECK_EQUAL( a, big_decimal(22u) );  // just greater @peak, reduce len
    a = sixhundredseventyeight;
    BOOST_CHECK( a.subtract_shifted_full_absolutely(big_decimal( 8u ), 2u) );
    BOOST_CHECK_EQUAL( a, big_decimal(122u) );  // greater @ peak, same length
}

// Multiply with multiple digits, fused add, reverse order, test
BOOST_AUTO_TEST_CASE( add_full_product_test )
{
    big_decimal const  zero, one( 1u ), two( 2u ), thirty( 30u ),
                       fortyfive( 45u ), sixhundredseventyeight( 678u );
    big_decimal        a;

    // Adding a zero product to something doesn't change that something
    a = one;
    a.add_full_product( zero, two );
    BOOST_CHECK_EQUAL( a, one );
    a.add_full_product( sixhundredseventyeight, zero );
    BOOST_CHECK_EQUAL( a, one );
    a.add_shifted_full_product( zero, zero, 5u );
    BOOST_CHECK_EQUAL( a, one );

    // Adding a product to zero gets that product
    a = zero;
    a.add_shifted_full_product( fortyfive, thirty, 2u );
    BOOST_CHECK_EQUAL( a, big_decimal(135000ul) );  // 1 carry rd, 1 spare sp
    a.reset();
    a.add_full_product( big_decimal(58u), big_decimal(27u) );
    BOOST_CHECK_EQUAL( a, big_decimal(1566u) );  // 2 carry rds, 1 spare sp

    // Full product add
    a.assign( 8500u );
    a.add_full_product( big_decimal(58u), big_decimal(27u) );
    BOOST_CHECK_EQUAL( a, big_decimal(10066u) );  // 2 carry rds, used spare sp
    a.add_shifted_full_product( big_decimal(21u), big_decimal(32u), 6u );
    BOOST_CHECK_EQUAL( a, big_decimal(672010066ul) );  // no carries, 1 spare sp

    // Worst case
    a.assign( 9999u );
    a.add_full_product( big_decimal(99u), big_decimal(99u) );
    BOOST_CHECK_EQUAL( a, big_decimal(19800u) );

    // Operator tests (as long as op* and op*= use "add_full_product")
    a.reset();
    BOOST_CHECK_EQUAL( a *= one, zero );
    BOOST_CHECK_EQUAL( a *= zero, zero );
    BOOST_CHECK_EQUAL( two * a, zero );

    a = one;
    BOOST_CHECK_EQUAL( a *= two, two );
    BOOST_CHECK_EQUAL( a *= thirty, big_decimal(60u) );
    BOOST_CHECK_EQUAL( fortyfive * a, big_decimal(2700u) );
    BOOST_CHECK_EQUAL( (fortyfive * a) * sixhundredseventyeight,
     big_decimal(1830600ul) );
}

// Multiply with multiple digits, fused subtract, reverse order, test
BOOST_AUTO_TEST_CASE( subtract_full_product_test )
{
    using boost::math::big_radix_whole_negative_result_error;

    big_decimal const  zero, one( 1u ), two( 2u ), thirty( 30u ),
                       fortyfive( 45u ), sixhundredseventyeight( 678u );
    big_decimal        a;

    // Subtracting a zero product from something doesn't change that something
    a = one;
    a.subtract_full_product( zero, two );
    BOOST_CHECK_EQUAL( a, one );
    a.subtract_full_product( sixhundredseventyeight, zero );
    BOOST_CHECK_EQUAL( a, one );
    a.subtract_shifted_full_product( zero, zero, 5u );
    BOOST_CHECK_EQUAL( a, one );

    a.reset();
    a.subtract_full_product( zero, zero );
    BOOST_CHECK_EQUAL( a, zero );

    // Subtracting a non-zero product from zero gets an error
    BOOST_CHECK_THROW( a.subtract_full_product(one, two),
     big_radix_whole_negative_result_error );  // 1st oversized
    BOOST_CHECK_EQUAL( a, zero );

    // Other ways of subtracting a larger number is an error
    a = two;
    BOOST_CHECK_THROW( a.subtract_full_product(thirty, one),
     big_radix_whole_negative_result_error );  // 1st oversized
    BOOST_CHECK_EQUAL( a, two );
    BOOST_CHECK_THROW( a.subtract_full_product(one, thirty),
     big_radix_whole_negative_result_error );  // 2nd oversized
    BOOST_CHECK_EQUAL( a, two );
    BOOST_CHECK_THROW( a.subtract_shifted_full_product(one, one, 1u),
     big_radix_whole_negative_result_error );  // index oversized
    BOOST_CHECK_EQUAL( a, two );

    a = thirty;
    BOOST_CHECK_THROW( a.subtract_shifted_full_product(thirty, one, 1u),
     big_radix_whole_negative_result_error );  // 1st + index outside
    BOOST_CHECK_EQUAL( a, thirty );
    BOOST_CHECK_THROW( a.subtract_shifted_full_product(one, thirty, 1u),
     big_radix_whole_negative_result_error );  // 2nd + index outside
    BOOST_CHECK_EQUAL( a, thirty );
    a = sixhundredseventyeight;
    BOOST_CHECK_THROW( a.subtract_shifted_full_product(thirty, fortyfive, 1u),
     big_radix_whole_negative_result_error );  // 1st + 2nd + index outside
    a = thirty;
    BOOST_CHECK_THROW( a.subtract_full_product(one, fortyfive),
     big_radix_whole_negative_result_error );  // product too large
    BOOST_CHECK_EQUAL( a, thirty );

    // Equal values
    a.subtract_full_product( big_decimal(5u), big_decimal(6u) );  // 2-1*1=0
    BOOST_CHECK_EQUAL( a, zero );
    a = thirty;
    a.subtract_shifted_full_product( one, big_decimal(3u), 1u );  // 2-1*1^1=0
    BOOST_CHECK_EQUAL( a, zero );
    a = thirty;
    a.subtract_full_product( thirty, one );  // 2-2*1=0
    BOOST_CHECK_EQUAL( a, zero );
    a = thirty;
    a.subtract_full_product( two, big_decimal(15u) );  //2-1*2=0
    BOOST_CHECK_EQUAL( a, zero );

    // Lesser values
    a = two;
    a.subtract_full_product( one, one );
    BOOST_CHECK_EQUAL( a, one );  // 1-1*1=1, no borrow
    a = fortyfive;
    a.subtract_shifted_full_product( two, two, 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(5u) );  // 2-1*1^1=1, no borrow
    a = thirty;
    a.subtract_full_product( big_decimal(5u), big_decimal(3u) );
    BOOST_CHECK_EQUAL( a, big_decimal(15u) );  // 2-1*1=2, 1 borrow
    a = sixhundredseventyeight;
    a.subtract_shifted_full_product( thirty, two, 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(78u) );  // 3-2*1^1=2, no borrow
    a.assign( 677u );
    a.subtract_full_product( big_decimal(13u), big_decimal(6u) );
    BOOST_CHECK_EQUAL( a, big_decimal(599u) );  // 3-2*1=3, cascade borrow
    a.subtract_full_product( big_decimal(3u), big_decimal(167u) );
    BOOST_CHECK_EQUAL( a, big_decimal(98u) );  // 3-1*3=2, no borrow
    a.assign( 1024u );
    a.subtract_shifted_full_product( two, fortyfive, 1u );
    BOOST_CHECK_EQUAL( a, big_decimal(124u) );  // 4-1*2^1=3, borrow, reduce len
}

// Multiply with multiple digits, fused subtract-absolutely, reverse order, test
BOOST_AUTO_TEST_CASE( subtract_full_product_absolutely_test )
{
    big_decimal const  zero, one( 1u ), two( 2u ), thirty( 30u ),
                       fortyfive( 45u ), sixhundredseventyeight( 678u );
    big_decimal        a;

    // Subtracting a zero product from something doesn't change that something
    a = one;
    BOOST_CHECK( !a.subtract_full_product_absolutely(zero, two) );
    BOOST_CHECK_EQUAL( a, one );
    BOOST_CHECK( !a.subtract_full_product_absolutely(sixhundredseventyeight,
     zero) );
    BOOST_CHECK_EQUAL( a, one );
    BOOST_CHECK( !a.subtract_shifted_full_product_absolutely(zero, zero, 5u) );
    BOOST_CHECK_EQUAL( a, one );

    a.reset();
    BOOST_CHECK( !a.subtract_full_product_absolutely(zero, zero) );
    BOOST_CHECK_EQUAL( a, zero );

    // Subtracting a non-zero product from zero
    BOOST_CHECK( a.subtract_full_product_absolutely(one, two) );
    BOOST_CHECK_EQUAL( a, two );
    a.reset();
    BOOST_CHECK( a.subtract_full_product_absolutely(thirty, one) );
    BOOST_CHECK_EQUAL( a, thirty );
    a.reset();
    BOOST_CHECK( a.subtract_shifted_full_product_absolutely(fortyfive,
     sixhundredseventyeight, 1u) );
    BOOST_CHECK_EQUAL( a, big_decimal(305100ul) );

    // Subtracting lesser values
    a = sixhundredseventyeight;
    BOOST_CHECK( !a.subtract_shifted_full_product_absolutely(one, fortyfive,
     1u) );
    BOOST_CHECK_EQUAL( a, big_decimal(228u) );  // no carry* nor borrow-
    BOOST_CHECK( !a.subtract_full_product_absolutely(two, thirty) );
    BOOST_CHECK_EQUAL( a, big_decimal(168u) );  // no carry*, 1 borrow-
    BOOST_CHECK( !a.subtract_full_product_absolutely(big_decimal( 3u ),
     big_decimal( 23u )) );
    BOOST_CHECK_EQUAL( a, big_decimal(99u) ); // no carry*, chain borrow-, short

    a.assign( 200u );
    BOOST_CHECK( !a.subtract_full_product_absolutely(fortyfive,
     big_decimal( 3u )) );
    BOOST_CHECK_EQUAL( a, big_decimal(65u) );  // carry*, chain borrow-, shorter

    // Subtracting equal values
    BOOST_CHECK( !a.subtract_full_product_absolutely(big_decimal( 13u ),
     big_decimal( 5u )) );
    BOOST_CHECK_EQUAL( a, zero );  // carry*
    a = sixhundredseventyeight;
    BOOST_CHECK( !a.subtract_full_product_absolutely(big_decimal( 113u ),
     big_decimal( 6u )) );
    BOOST_CHECK_EQUAL( a, zero );  // carry*
    a.assign( 4u );
    BOOST_CHECK( !a.subtract_full_product_absolutely(two, two) );
    BOOST_CHECK_EQUAL( a, zero );  // no carry*

    // Subtracting greater values (from non-zeros)
    a = sixhundredseventyeight;
    BOOST_CHECK(a.subtract_shifted_full_product_absolutely(two, fortyfive, 1u));
    BOOST_CHECK_EQUAL( a, big_decimal(222u) );  // borrow at peak
    BOOST_CHECK(a.subtract_shifted_full_product_absolutely(thirty, thirty, 2u));
    BOOST_CHECK_EQUAL( a, big_decimal(89778ul) );  // no overlap
    BOOST_CHECK( a.subtract_shifted_full_product_absolutely(two,
     big_decimal( 8u ), 4u) );
    BOOST_CHECK_EQUAL( a, big_decimal(70222ul) );  // overlap
    BOOST_CHECK( a.subtract_shifted_full_product_absolutely(big_decimal( 12u ),
     big_decimal( 6u ), 3u) );
    BOOST_CHECK_EQUAL( a, big_decimal(1778u) );  // cascading borrow, reduce len
}

// Divide by multiple digits, with multiple-digit remainder, test
BOOST_AUTO_TEST_CASE( full_div_and_mod_test )
{
    using boost::math::big_radix_whole_divide_by_zero_error;

    big_decimal const  zero, one( 1u ), two( 2u ), thirty( 30u ),
                       fortyfive( 45u ), sixhundredseventyeight( 678u );

    // Dividing by zero is an error
    BOOST_CHECK_THROW( zero.divide_by(zero),
     big_radix_whole_divide_by_zero_error );

    BOOST_CHECK_THROW( one.divide_by(zero),
     big_radix_whole_divide_by_zero_error );
    BOOST_CHECK_THROW( two.divide_by(zero),
     big_radix_whole_divide_by_zero_error );
    BOOST_CHECK_THROW( thirty.divide_by(zero),
     big_radix_whole_divide_by_zero_error );
    BOOST_CHECK_THROW( fortyfive.divide_by(zero),
     big_radix_whole_divide_by_zero_error );
    BOOST_CHECK_THROW( sixhundredseventyeight.divide_by(zero),
     big_radix_whole_divide_by_zero_error );

    // Dividing by a single-digit number is relatively quick
    big_decimal const  three( 3u ), four( 4u ), five( 5u ), six( 6u ),
                       seven( 7u ), fifteen( 15u );
    big_decimal        q, r;

    boost::tuple<big_decimal &, big_decimal &>  qr( q, r );

    qr = zero.divide_by( one );
    BOOST_CHECK_EQUAL( q, zero ); BOOST_CHECK_EQUAL( r, zero );
    qr = zero.divide_by( two );
    BOOST_CHECK_EQUAL( q, zero ); BOOST_CHECK_EQUAL( r, zero );

    qr = one.divide_by( one );
    BOOST_CHECK_EQUAL( q, one ); BOOST_CHECK_EQUAL( r, zero );
    qr = one.divide_by( two );
    BOOST_CHECK_EQUAL( q, zero ); BOOST_CHECK_EQUAL( r, one );

    qr = two.divide_by( one );
    BOOST_CHECK_EQUAL( q, two ); BOOST_CHECK_EQUAL( r, zero );
    qr = two.divide_by( two );
    BOOST_CHECK_EQUAL( q, one ); BOOST_CHECK_EQUAL( r, zero );
    qr = two.divide_by( three );
    BOOST_CHECK_EQUAL( q, zero ); BOOST_CHECK_EQUAL( r, two );

    qr = six.divide_by( one );
    BOOST_CHECK_EQUAL( q, six ); BOOST_CHECK_EQUAL( r, zero );
    qr = six.divide_by( two );
    BOOST_CHECK_EQUAL( q, three ); BOOST_CHECK_EQUAL( r, zero );
    qr = six.divide_by( three );
    BOOST_CHECK_EQUAL( q, two ); BOOST_CHECK_EQUAL( r, zero );
    qr = six.divide_by( four );
    BOOST_CHECK_EQUAL( q, one ); BOOST_CHECK_EQUAL( r, two );
    qr = six.divide_by( five );
    BOOST_CHECK_EQUAL( q, one ); BOOST_CHECK_EQUAL( r, one );
    qr = six.divide_by( six );
    BOOST_CHECK_EQUAL( q, one ); BOOST_CHECK_EQUAL( r, zero );
    qr = six.divide_by( seven );
    BOOST_CHECK_EQUAL( q, zero ); BOOST_CHECK_EQUAL( r, six );

    qr = thirty.divide_by( one );
    BOOST_CHECK_EQUAL( q, thirty ); BOOST_CHECK_EQUAL( r, zero );
    qr = thirty.divide_by( two );
    BOOST_CHECK_EQUAL( q, fifteen ); BOOST_CHECK_EQUAL( r, zero );
    qr = thirty.divide_by( three );
    BOOST_CHECK_EQUAL( q, big_decimal(10u) ); BOOST_CHECK_EQUAL( r, zero );
    qr = thirty.divide_by( four );
    BOOST_CHECK_EQUAL( q, seven ); BOOST_CHECK_EQUAL( r, two );
    qr = thirty.divide_by( five );
    BOOST_CHECK_EQUAL( q, six ); BOOST_CHECK_EQUAL( r, zero );
    qr = thirty.divide_by( six );
    BOOST_CHECK_EQUAL( q, five ); BOOST_CHECK_EQUAL( r, zero );
    qr = thirty.divide_by( seven );
    BOOST_CHECK_EQUAL( q, four ); BOOST_CHECK_EQUAL( r, two );

    // Multiple-digit divisors
    big_decimal const  eighteen( 18u );

    qr = zero.divide_by( thirty );
    BOOST_CHECK_EQUAL( q, zero ); BOOST_CHECK_EQUAL( r, zero );
    qr = one.divide_by( fortyfive );
    BOOST_CHECK_EQUAL( q, zero ); BOOST_CHECK_EQUAL( r, one );

    qr = thirty.divide_by( thirty );
    BOOST_CHECK_EQUAL( q, one ); BOOST_CHECK_EQUAL( r, zero );
    qr = thirty.divide_by( fortyfive );
    BOOST_CHECK_EQUAL( q, zero ); BOOST_CHECK_EQUAL( r, thirty );
    qr = fortyfive.divide_by( thirty );
    BOOST_CHECK_EQUAL( q, one ); BOOST_CHECK_EQUAL( r, fifteen );
    qr = fortyfive.divide_by( fortyfive );
    BOOST_CHECK_EQUAL( q, one ); BOOST_CHECK_EQUAL( r, zero );

    qr = thirty.divide_by( sixhundredseventyeight );
    BOOST_CHECK_EQUAL( q, zero ); BOOST_CHECK_EQUAL( r, thirty );
    qr = fortyfive.divide_by( sixhundredseventyeight );
    BOOST_CHECK_EQUAL( q, zero ); BOOST_CHECK_EQUAL( r, fortyfive );
    qr = sixhundredseventyeight.divide_by( thirty );
    BOOST_CHECK_EQUAL( q, big_decimal(22u) ); BOOST_CHECK_EQUAL( r, eighteen );
    qr = sixhundredseventyeight.divide_by( fortyfive );
    BOOST_CHECK_EQUAL( q, fifteen ); BOOST_CHECK_EQUAL( r, three );
    qr = sixhundredseventyeight.divide_by( sixhundredseventyeight );
    BOOST_CHECK_EQUAL( q, one ); BOOST_CHECK_EQUAL( r, zero );

    // Multiple-digit everything
    big_decimal  a( 30528u );

    qr = a.divide_by( sixhundredseventyeight );
    BOOST_CHECK_EQUAL( q, fortyfive ); BOOST_CHECK_EQUAL( r, eighteen );
    qr = a.divide_by( thirty );
    BOOST_CHECK_EQUAL( q, big_decimal(1017u) ); BOOST_CHECK_EQUAL(r, eighteen);

    // Force trial divisor to be too large
    big_decimal const  fiftyeight( 58u );

    a.assign( 409u );
    qr = a.divide_by( fiftyeight );
    BOOST_CHECK_EQUAL( q, seven ); BOOST_CHECK_EQUAL( r, three );
      // add back to non-zero remainder
    BOOST_CHECK_EQUAL( a, (q *= fiftyeight) += r );  // reverse check

    a.subtract_single( 3 );
    qr = a.divide_by( fiftyeight );
    BOOST_CHECK_EQUAL( q, seven ); BOOST_CHECK_EQUAL( r, zero );
      // add back to zero remainder

    // Knuth's special test: digit expansion of (Radix^X - 1)(Radix^Y - 1) has
    // Min(X, Y) - 1 digits of Radix-1 followed by a digit of Radix-2 followed
    // by |X - Y| digits of Radix-1 followed by Min(X, Y) - 1 digits of 0
    // followed by a digit of 1 [for positive integer X and Y, of course].
    a.assign( 998999003ul );
    qr = a.divide_by( big_decimal(999u) );
    BOOST_CHECK_EQUAL( q, big_decimal(999999ul) ); BOOST_CHECK_EQUAL( r, two );

    a.configure( "99899999001" );
    qr = a.divide_by( (one << 8u) - one );
    BOOST_CHECK_EQUAL( q, (one << 3) - one ); BOOST_CHECK_EQUAL( r, zero );

    // Operator tests (as long as op/, op/=, op%, and op%= use "divide_by")
    a.reset();
    BOOST_CHECK_THROW( a /= zero, big_radix_whole_divide_by_zero_error );
    BOOST_CHECK_THROW( a %= zero, big_radix_whole_divide_by_zero_error );
    BOOST_CHECK_THROW( a / zero, big_radix_whole_divide_by_zero_error );
    BOOST_CHECK_THROW( a % zero, big_radix_whole_divide_by_zero_error );

    ++a;
    BOOST_CHECK_THROW( a /= zero, big_radix_whole_divide_by_zero_error );
    BOOST_CHECK_THROW( a %= zero, big_radix_whole_divide_by_zero_error );
    BOOST_CHECK_THROW( a / zero, big_radix_whole_divide_by_zero_error );
    BOOST_CHECK_THROW( a % zero, big_radix_whole_divide_by_zero_error );

    BOOST_CHECK_EQUAL( a /= one, one );
    BOOST_CHECK_EQUAL( a %= one, zero );
    ++a;
    BOOST_CHECK_EQUAL( a / one, one );
    BOOST_CHECK_EQUAL( a % one, zero );
    BOOST_CHECK_EQUAL( a / two, zero );
    BOOST_CHECK_EQUAL( a % two, one );

    ++a;
    BOOST_CHECK_EQUAL( a / one, two );
    BOOST_CHECK_EQUAL( a % one, zero );
    BOOST_CHECK_EQUAL( a / two, one );
    BOOST_CHECK_EQUAL( a % two, zero );
    BOOST_CHECK_EQUAL( a / three, zero );
    BOOST_CHECK_EQUAL( a % three, two );

    a = fortyfive;
    BOOST_CHECK_EQUAL( a / thirty, one );
    BOOST_CHECK_EQUAL( a % thirty, fifteen );
    BOOST_CHECK_EQUAL( a / fortyfive, one );
    BOOST_CHECK_EQUAL( a % fortyfive, zero );
    BOOST_CHECK_EQUAL( a / sixhundredseventyeight, zero );
    BOOST_CHECK_EQUAL( a % sixhundredseventyeight, fortyfive );
}

BOOST_AUTO_TEST_SUITE_END()


#pragma mark -
#pragma mark Regular Function & Routine Suite

// Conventional functions and any self-serve member functions
BOOST_AUTO_TEST_SUITE( regular_function_and_routine_suite )

// Magnitude and direction test
BOOST_AUTO_TEST_CASE( abs_sgn_test )
{
    using boost::math::abs;
    using boost::math::sgn;

    big_decimal const  zero = 0u, one = 1u, two = 2u, ten = 10u, thirteen = 13u,
                       fourhundredfiftysix = 456u;

    // Check absolute value
    BOOST_CHECK_EQUAL( abs(zero), zero );
    BOOST_CHECK_EQUAL( abs(one), one );
    BOOST_CHECK_EQUAL( abs(two), two );
    BOOST_CHECK_EQUAL( abs(ten), ten );
    BOOST_CHECK_EQUAL( abs(thirteen), thirteen );
    BOOST_CHECK_EQUAL( abs(fourhundredfiftysix), fourhundredfiftysix );

    // Check (arithmetic) sign
    BOOST_CHECK_EQUAL( 0, sgn(zero) );
    BOOST_CHECK_EQUAL( 1, sgn(one) );
    BOOST_CHECK_EQUAL( 1, sgn(two) );
    BOOST_CHECK_EQUAL( 1, sgn(ten) );
    BOOST_CHECK_EQUAL( 1, sgn(thirteen) );
    BOOST_CHECK_EQUAL( 1, sgn(fourhundredfiftysix) );

    // Check the member-function versions
    big_decimal  object = zero;

    object.absolve_self();
    BOOST_CHECK_EQUAL( object, zero );
    object.sign_self();
    BOOST_CHECK_EQUAL( object, zero );

    object = one;
    object.absolve_self();
    BOOST_CHECK_EQUAL( object, one );
    object.sign_self();
    BOOST_CHECK_EQUAL( object, one );

    object = two;
    object.absolve_self();
    BOOST_CHECK_EQUAL( object, two );
    object.sign_self();
    BOOST_CHECK_EQUAL( object, one );

    object = ten;
    object.absolve_self();
    BOOST_CHECK_EQUAL( object, ten );
    object.sign_self();
    BOOST_CHECK_EQUAL( object, one );

    object = thirteen;
    object.absolve_self();
    BOOST_CHECK_EQUAL( object, thirteen );
    object.sign_self();
    BOOST_CHECK_EQUAL( object, one );

    object = fourhundredfiftysix;
    object.absolve_self();
    BOOST_CHECK_EQUAL( object, fourhundredfiftysix );
    object.sign_self();
    BOOST_CHECK_EQUAL( object, one );
}

// Counting digit values
BOOST_AUTO_TEST_CASE( specific_digit_count_test )
{
    using namespace boost::lambda;

    using std::transform;

    // Prepare items to compute, hold, and compare results
    typedef big_octal::size_type  size_type;

    boost::counting_iterator<big_octal::digit_type> const  cb( 0 ),
                                            ce( big_octal::radix );
    std::vector<size_type>             scratch( big_octal::radix );

    // The compact representation doesn't have any digits for zero
    {
        big_octal const  zero( 0u );
        size_type const  results_0[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

        transform( cb, ce, scratch.begin(),
         bind(&big_octal::specific_digit_count, zero, _1) );
        BOOST_CHECK_EQUAL_COLLECTIONS( scratch.begin(), scratch.end(),
         array_begin(results_0), array_end(results_0) );
    }

    // Other single-digit values contain only themselves
    {
        big_octal const  five( 05u );
        size_type const  results_5[] = { 0, 0, 0, 0, 0, 1, 0, 0 };

        transform( cb, ce, scratch.begin(),
         bind(&big_octal::specific_digit_count, five, _1) );
        BOOST_CHECK_EQUAL_COLLECTIONS( scratch.begin(), scratch.end(),
         array_begin(results_5), array_end(results_5) );
    }

    // Internal zeros do count (also tests multiple digits per number)
    {
        big_octal const  onehundredninetyseven( 0305u );
        size_type const  results_197[] = { 1, 0, 0, 1, 0, 1, 0, 0 };

        transform( cb, ce, scratch.begin(),
         bind(&big_octal::specific_digit_count, onehundredninetyseven, _1) );
        BOOST_CHECK_EQUAL_COLLECTIONS( scratch.begin(), scratch.end(),
         array_begin(results_197), array_end(results_197) );
    }

    {
        big_octal const  thirtytwo( 040 );
        size_type const  results_32[] = { 1, 0, 0, 0, 1, 0, 0, 0 };

        transform( cb, ce, scratch.begin(),
         bind(&big_octal::specific_digit_count, thirtytwo, _1) );
        BOOST_CHECK_EQUAL_COLLECTIONS( scratch.begin(), scratch.end(),
         array_begin(results_32), array_end(results_32) );
    }

    // Multiple occurrences of a digit (including nothing else)
    {
        big_octal const  fourhundredthirtyeight( 0666u );
        size_type const  results_438[] = { 0, 0, 0, 0, 0, 0, 3, 0 };

        transform( cb, ce, scratch.begin(),
         bind(&big_octal::specific_digit_count, fourhundredthirtyeight, _1) );
        BOOST_CHECK_EQUAL_COLLECTIONS( scratch.begin(), scratch.end(),
         array_begin(results_438), array_end(results_438) );
    }

    {
        big_octal const  seventyseventhousandeighthundredseventeen( 0227771ul );
        size_type const  results_77817[] = { 0, 1, 2, 0, 0, 0, 0, 3 };

        transform( cb, ce, scratch.begin(),
         bind(&big_octal::specific_digit_count,
         seventyseventhousandeighthundredseventeen, _1) );
        BOOST_CHECK_EQUAL_COLLECTIONS( scratch.begin(), scratch.end(),
         array_begin(results_77817), array_end(results_77817) );
    }

    {
        big_octal const  onethousandninehundredfifteen( 03573u );
        size_type const  results_1915[] = { 0, 0, 0, 2, 0, 1, 0, 1 };

        transform( cb, ce, scratch.begin(),
         bind(&big_octal::specific_digit_count, onethousandninehundredfifteen,
         _1) );
        BOOST_CHECK_EQUAL_COLLECTIONS( scratch.begin(), scratch.end(),
         array_begin(results_1915), array_end(results_1915) );
    }
}

// Digit sum and digital root
BOOST_AUTO_TEST_CASE( digit_sum_root_test )
{
    using boost::math::digit_sum;

    {
        // Zero values are their own digit sum and digital root
        big_decimal const  zero( 0u );

        BOOST_CHECK_EQUAL( zero.digital_root(), 0 );
        BOOST_CHECK_EQUAL( digit_sum(zero), zero );

        big_decimal  a = zero;

        a.digit_summate_self();
        BOOST_CHECK_EQUAL( a, zero );

        // Other single-digit values are also their own D.S. & D.R.
        big_decimal const  one( 1u ), six( 6u );

        BOOST_CHECK_EQUAL( one.digital_root(), 1 );
        BOOST_CHECK_EQUAL( digit_sum(one), one );

        BOOST_CHECK_EQUAL( six.digital_root(), 6 );
        BOOST_CHECK_EQUAL( digit_sum(six), six );

        a = one;
        a.digit_summate_self();
        BOOST_CHECK_EQUAL( a, one );
        a = six;
        a.digit_summate_self();
        BOOST_CHECK_EQUAL( a, six );

        // Try out multiple-digit values
        big_decimal const  eighteen( 18u ), sevenhundredthirtyeight( 738u );

        BOOST_CHECK_EQUAL( eighteen.digital_root(), 9 );
        BOOST_CHECK_EQUAL( digit_sum(eighteen), big_decimal(9u) );

        BOOST_CHECK_EQUAL( sevenhundredthirtyeight.digital_root(), 9 );
        BOOST_CHECK_EQUAL( digit_sum(sevenhundredthirtyeight), eighteen );

        // Demonstrate additive persistence with the smallest numbers for each
        // length (0 steps: 0; 1 step: 10; 2 steps: 19; 3 steps: 199; 4 steps:
        // 2 * 10**22 - 1).  The 5-step demonstration would need a 128-bit
        // memory space!  (It's 2 * 10**[2 * [10**22 - 1] / 9] - 1.)
        big_decimal const  ten( 10u ), nineteen( 19u ),
         onehundredninetynine( 199u ),
         twentysextillion_minus_1( "19999999999999999999999" );

        BOOST_CHECK_EQUAL( twentysextillion_minus_1.digital_root(), 1 );

        a = twentysextillion_minus_1;
        a.digit_summate_self();
        BOOST_CHECK_EQUAL( a, onehundredninetynine );
        a.digit_summate_self();
        BOOST_CHECK_EQUAL( a, nineteen );
        a.digit_summate_self();
        BOOST_CHECK_EQUAL( a, ten );
        a.digit_summate_self();
        BOOST_CHECK_EQUAL( a, one );
        a.digit_summate_self();
        BOOST_CHECK_EQUAL( a, one );
    }

    // Try to force so many digits that multiple adding runs are needed
    {
        big_ultimate const  one_u( 1u );
        big_ultimate        m( one_u );
        std::size_t const   block_size = std::numeric_limits<int>::max() /
                             big_ultimate::radix;

        m <<= 3u + block_size;
        BOOST_REQUIRE( m.digit_count() > block_size );
        m.digit_summate_self();
        BOOST_CHECK_EQUAL( m, one_u );
    }

    // Try other radices
    {
        big_octal const  zero( 0u );

        BOOST_CHECK_EQUAL( zero.digital_root(), 0 );
        BOOST_CHECK_EQUAL( digit_sum(zero), zero );

        big_octal const  one( 01u ), eight( 010u ), fifteen( 017u ),
                         onehundredtwentyseven( 0177u ),
                         two_up_55_minus_1( "1777777777777777777" );

        BOOST_CHECK_EQUAL( two_up_55_minus_1.digital_root(), 01 );

        big_octal  a = two_up_55_minus_1;

        a.digit_summate_self();
        BOOST_CHECK_EQUAL( a, onehundredtwentyseven );
        a.digit_summate_self();
        BOOST_CHECK_EQUAL( a, fifteen );
        a.digit_summate_self();
        BOOST_CHECK_EQUAL( a, eight );
        a.digit_summate_self();
        BOOST_CHECK_EQUAL( a, one );
        a.digit_summate_self();
        BOOST_CHECK_EQUAL( a, one );
    }

    {
        big_hexadecimal const  zero( 0u );

        BOOST_CHECK_EQUAL( zero.digital_root(), 0 );
        BOOST_CHECK_EQUAL( digit_sum(zero), zero );

        big_hexadecimal const  one( 0x1u ), sixteen( 0x10u ), thirtyone( 0x1Fu ),
                               fivehundredeleven( 0x1FFu ), two_up_137_minus_1(
                                "1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" );

        BOOST_CHECK_EQUAL( two_up_137_minus_1.digital_root(), 0x1 );

        big_hexadecimal  a = two_up_137_minus_1;

        a.digit_summate_self();
        BOOST_CHECK_EQUAL( a, fivehundredeleven );
        a.digit_summate_self();
        BOOST_CHECK_EQUAL( a, thirtyone );
        a.digit_summate_self();
        BOOST_CHECK_EQUAL( a, sixteen );
        a.digit_summate_self();
        BOOST_CHECK_EQUAL( a, one );
        a.digit_summate_self();
        BOOST_CHECK_EQUAL( a, one );
    }
}

// Digit-reversal
BOOST_AUTO_TEST_CASE( reverse_digits_test )
{
    // Single-digit values are their own reversal
    big_decimal const  zero( 0u ), three( 3u );
    big_decimal        a;

    a = zero;
    a.reverse_digits();
    BOOST_CHECK_EQUAL( a, zero );

    a = three;
    a.reverse_digits();
    BOOST_CHECK_EQUAL( a, three );

    // Reverse multiple-digit values
    big_decimal const  sixtysix( 66u ), eighthundredseventyeight( 878u );

    a.assign( 45u );
    a.reverse_digits();
    BOOST_CHECK_EQUAL( a, big_decimal(54u) );

    a = sixtysix;
    a.reverse_digits();
    BOOST_CHECK_EQUAL( a, sixtysix );

    a = eighthundredseventyeight;
    a.reverse_digits();
    BOOST_CHECK_EQUAL( a, eighthundredseventyeight );

    a.assign( 1059u );
    a.reverse_digits();
    BOOST_CHECK_EQUAL( a, big_decimal(9501u) );

    // Reversing with trailing zeros isn't recoverable
    big_decimal const  nine( 9u ), ninety( 90u );

    a = ninety;
    a.reverse_digits();
    BOOST_CHECK_EQUAL( a, nine );
    a.reverse_digits();
    BOOST_CHECK_EQUAL( a, nine );

    a.assign( 780u );
    a.reverse_digits();
    BOOST_CHECK_EQUAL( a, big_decimal(87u) );
    a.reverse_digits();
    BOOST_CHECK_EQUAL( a, big_decimal(78u) );

    a.assign( 302800ul );
    a.reverse_digits();
    BOOST_CHECK_EQUAL( a, big_decimal(8203u) );
    a.reverse_digits();
    BOOST_CHECK_EQUAL( a, big_decimal(3028u) );
}

BOOST_AUTO_TEST_SUITE_END()


#pragma mark -
#pragma mark Extra Suite

// Extra items
BOOST_AUTO_TEST_SUITE( extra_suite )

// Numeric-limits specialization
BOOST_AUTO_TEST_CASE( numeric_limits_test )
{
    typedef std::numeric_limits<big_decimal>  limits_type;

    BOOST_REQUIRE( limits_type::is_specialized );

    BOOST_CHECK_EQUAL( limits_type::min(), big_decimal(0u) );

    BOOST_CHECK( not limits_type::is_signed );
    BOOST_CHECK( limits_type::is_integer );
    BOOST_CHECK( limits_type::is_exact );
    BOOST_CHECK( limits_type::radix == 10 );
      // "BOOST_CHECK_EQUAL" would disqualify "radix" from being a compile-time
      // constant; i.e. it would need a full object definition in the header

    BOOST_CHECK( not limits_type::is_iec559 );
    BOOST_CHECK( not limits_type::is_bounded );
    BOOST_CHECK( not limits_type::is_modulo );

    BOOST_CHECK( not limits_type::traps );
}

// Conversion member functions
BOOST_AUTO_TEST_CASE( conversion_test )
{
    using std::numeric_limits;
    using boost::uintmax_t;
    using boost::math::big_radix_whole_conversion_error;

    typedef numeric_limits<uintmax_t>  uintmax_limits;
    typedef numeric_limits<double>      double_limits;

    BOOST_REQUIRE( uintmax_limits::is_specialized );

    // Check conversions to built-in integer
    BOOST_CHECK_EQUAL( 0u, big_decimal().to_uintmax() );
    BOOST_CHECK_EQUAL( 1u, big_decimal(1u).to_uintmax() );
    BOOST_CHECK_EQUAL( 23u, big_decimal(23u).to_uintmax() );

    BOOST_CHECK_THROW( (big_radix_whole<uintmax_limits::radix>(1u) <<
     uintmax_limits::digits + 1u).to_uintmax(),
     big_radix_whole_conversion_error );

    // Check conversions to built-in float
    BOOST_CHECK_EQUAL( 0.0, big_decimal().to_double() );
    BOOST_CHECK_CLOSE( 4.0, big_decimal(4u).to_double(), 1 );
    BOOST_CHECK_CLOSE( 56.0, big_decimal(56u).to_double(), 1 );

    BOOST_CHECK_THROW( (big_radix_whole<double_limits::radix>(1u) <<
     double_limits::max_exponent + 1u).to_double(),
     big_radix_whole_conversion_error );

    // Check conversions to string; single and multiple digits, hyphens and no
    // hyphens, single or mulitple digits per big digit, zero-value digits
    BOOST_CHECK_EQUAL( "0", big_decimal().to_string() );
    BOOST_CHECK_EQUAL( "7", big_decimal(7u).to_string() );
    BOOST_CHECK_EQUAL( "89", big_decimal(89u).to_string() );
    BOOST_CHECK_EQUAL( "5-9", big_hexadecimal(89u).to_string() );
    BOOST_CHECK_EQUAL( "131", big_octal(89u).to_string() );
    BOOST_CHECK_EQUAL( "1-14", big_hexadecimal(0x1Eu).to_string() );
    BOOST_CHECK_EQUAL( "503", big_octal(0503u).to_string() );

    // Check generic numeric conversion, bounded destination type
    BOOST_CHECK_EQUAL( big_decimal().to_uintmax(),
     big_decimal().to_number<uintmax_t>() );
    BOOST_CHECK_EQUAL( big_decimal(1u).to_uintmax(),
     big_decimal(1u).to_number<uintmax_t>() );
    BOOST_CHECK_EQUAL( big_decimal(23u).to_uintmax(),
     big_decimal(23u).to_number<uintmax_t>() );
    BOOST_CHECK_EQUAL( big_decimal().to_double(),
     big_decimal().to_number<double>() );
    BOOST_CHECK_EQUAL( big_decimal(4u).to_double(),
     big_decimal(4u).to_number<double>() );
    BOOST_CHECK_EQUAL( big_decimal(56u).to_double(),
     big_decimal(56u).to_number<double>() );

    BOOST_CHECK_THROW( (big_radix_whole<uintmax_limits::radix>(1u) <<
     uintmax_limits::digits + 1u).to_number<uintmax_t>(),
     big_radix_whole_conversion_error );
    BOOST_CHECK_THROW( (big_radix_whole<double_limits::radix>(1u) <<
     double_limits::max_exponent + 1u).to_number<double>(),
     big_radix_whole_conversion_error );

    // Check generic numeric conversion, unbounded destination type
    BOOST_CHECK_EQUAL( big_decimal().to_number<big_octal>(), big_octal() );
    BOOST_CHECK_EQUAL( big_decimal().to_number<big_hexadecimal>(),
     big_hexadecimal() );  // zero

    BOOST_CHECK_EQUAL( big_decimal(1u).to_number<big_octal>(), big_octal(01u) );
    BOOST_CHECK_EQUAL( big_decimal(1u).to_number<big_hexadecimal>(),
     big_hexadecimal(0x1u) );  // one

    BOOST_CHECK_EQUAL( big_decimal(9u).to_number<big_octal>(),big_octal(011u) );
    BOOST_CHECK_EQUAL( big_decimal(9u).to_number<big_hexadecimal>(),
     big_hexadecimal(0x9u) );  // one-digit except for lesser radix

    BOOST_CHECK_EQUAL(big_decimal(12u).to_number<big_octal>(), big_octal(014u));
    BOOST_CHECK_EQUAL( big_decimal(12u).to_number<big_hexadecimal>(),
     big_hexadecimal(0xCu) );  // two-digits except for greater radix

    BOOST_CHECK_EQUAL(big_decimal(31u).to_number<big_octal>(), big_octal(037u));
    BOOST_CHECK_EQUAL( big_decimal(31u).to_number<big_hexadecimal>(),
     big_hexadecimal(0x1Fu) );  // two digits for all radices

    BOOST_CHECK_EQUAL( big_decimal(5764801ul).to_number<big_octal>(),
     big_octal(025773301ul) );
    BOOST_CHECK_EQUAL( big_decimal(5764801ul).to_number<big_hexadecimal>(),
     big_hexadecimal(0x57F6C1ul) );  // lots of digits
}

// Stream-output test
// (A more complete version of the printing test done in the basic section.)
BOOST_AUTO_TEST_CASE( output_test )
{
    using std::setw;
    using std::ios_base;
    using std::left;
    using std::setfill;

    // The only enhancement supported is alignment, and that is only "left"
    // versus non-"left" (i.e. "internal" is treated the same as "right").
    // You need to also support width and fill to support alignment.
    std::ostringstream  oss;
    std::string const   blank( "" );

    BOOST_CHECK_EQUAL( oss.width(), 0 );
    oss << setw( 3 ) << big_octal( 0u );
    BOOST_CHECK_EQUAL( oss.width(), 0 );
    BOOST_CHECK_EQUAL( oss.str(), "  0" );

    BOOST_CHECK( (oss.flags() & ios_base::adjustfield) != ios_base::left );
    oss.str( blank );
    oss << setw( 3 ) << left << big_octal( 03u );
    BOOST_CHECK( (oss.flags() & ios_base::adjustfield) == ios_base::left );
    BOOST_CHECK_EQUAL( oss.str(), "3  " );
    oss << std::resetiosflags( ios_base::adjustfield );
    BOOST_CHECK( (oss.flags() & ios_base::adjustfield) != ios_base::left );

    // Fill doesn't matter if the number length exceeds the width
    oss.str( blank );
    oss << setw( 3 ) << big_octal( 01001u );
    BOOST_CHECK_EQUAL( oss.str(), "1001" );

    // Contrast when the width is made oversized again
    oss << ' ' << setfill( '*' ) << left << setw( 6 ) << big_octal( 01002u );
    BOOST_CHECK_EQUAL( oss.str(), "1001 1002**" );
    oss.fill( ' ' );
    oss.unsetf( ios_base::adjustfield );

    // Do similar with a radix over ten
    oss.str( blank );
    oss << setw( 2 ) << left << big_hexadecimal( 0x0u );
    BOOST_CHECK_EQUAL( oss.str(), "0 " );

    oss << setw( 5 ) << setfill( '#' ) << std::right << big_hexadecimal( 0x3u );
    BOOST_CHECK_EQUAL( oss.str(), "0 ####3" );
    oss << ' ' << setw( 1 ) << big_hexadecimal( 0xAu );
    BOOST_CHECK_EQUAL( oss.str(), "0 ####3 10" );

    // Now use hyphen-chained multi-digit values
    oss.str( blank );
    oss << setw( 5 ) << std::internal << big_hexadecimal( 0xB2u );
    BOOST_CHECK_EQUAL( oss.str(), "#11-2" );

    oss << ' ' << setw( 6 ) << left << big_hexadecimal( 0x8Cu );
    BOOST_CHECK_EQUAL( oss.str(), "#11-2 8-12##" );

    oss << setw( 4 ) << big_hexadecimal( 0xEF93u );
    BOOST_CHECK_EQUAL( oss.str(), "#11-2 8-12##14-15-9-3" );
}

// Stream-input test
BOOST_AUTO_TEST_CASE( input_test )
{
    using boost::lexical_cast;
    using std::ios_base;
    using std::bad_alloc;

    // Zero check
    big_octal const        zero_o;
    big_hexadecimal const  zero_h;

    BOOST_CHECK_EQUAL( lexical_cast<big_octal>("0"), zero_o );
    BOOST_CHECK_EQUAL( lexical_cast<big_octal>("000000"), zero_o );
    BOOST_CHECK_EQUAL( lexical_cast<big_octal>("+000000"), zero_o );
    BOOST_CHECK_EQUAL( lexical_cast<big_octal>("-000000"), zero_o );

    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("0"), zero_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("000000"), zero_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("+000000"), zero_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("-000000"), zero_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("0-00-000"), zero_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("-00-0-000"), zero_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("+00-000-0"), zero_h );

    // Single-digit non-zero check
    big_octal const        three_o( 03u );
    big_hexadecimal const  eight_h( 0x8u );

    BOOST_CHECK_EQUAL( lexical_cast<big_octal>("3"), three_o );
    BOOST_CHECK_EQUAL( lexical_cast<big_octal>("+3"), three_o );
    BOOST_CHECK_EQUAL( lexical_cast<big_octal>("003"), three_o );
    BOOST_CHECK_EQUAL( lexical_cast<big_octal>("+000003"), three_o );

    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("8"), eight_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("+8"), eight_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("0008"), eight_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("+008"), eight_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("0-8"), eight_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("+0-00-8"), eight_h );

    // Greater-than-10 single-digit check
    big_hexadecimal const  thirteen_h( 0xDu );

    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("13"), thirteen_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("+13"), thirteen_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("0013"), thirteen_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("+013"), thirteen_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("0-13"), thirteen_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("+0-00-13"), thirteen_h );

    // Multiple-digits check
    big_octal const        thirteen_o( 015u );
    big_hexadecimal const  sevenhundred_h( 0x2BCu );

    BOOST_CHECK_EQUAL( lexical_cast<big_octal>("15"), thirteen_o );
    BOOST_CHECK_EQUAL( lexical_cast<big_octal>("+15"), thirteen_o );
    BOOST_CHECK_EQUAL( lexical_cast<big_octal>("015"), thirteen_o );
    BOOST_CHECK_EQUAL( lexical_cast<big_octal>("+000015"), thirteen_o );

    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("2-11-12"),
     sevenhundred_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("+2-11-12"),
     sevenhundred_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("0002-011-12"),
     sevenhundred_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("+02-11-00012"),
     sevenhundred_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("0-2-0011-12"),
     sevenhundred_h );
    BOOST_CHECK_EQUAL( lexical_cast<big_hexadecimal>("+0-00-2-11-12"),
     sevenhundred_h );

    // Stopping and error detection, radix <= 10
    std::istringstream  iss( "" );
    big_octal           scratch_o;
    char                extra = '\0';

    iss >> scratch_o;
    BOOST_CHECK( !iss );  // EOF before getting any digits
    iss.clear();

    iss.str( "+" );
    iss >> scratch_o;
    BOOST_CHECK( !iss );  // sign symbol without any following digits
    iss.clear();
    BOOST_CHECK_EQUAL( iss.peek(), static_cast<int>('+') );

    iss.str( "AB" );
    iss >> scratch_o;
    BOOST_CHECK( !iss );  // non-digit reached before getting any digits
    iss.clear();
    BOOST_CHECK_EQUAL( iss.peek(), static_cast<int>('A') );

    iss.str( "2498" );
    iss >> scratch_o;
    BOOST_CHECK( iss );
    BOOST_CHECK_EQUAL( scratch_o, big_octal(024u) );  // stops at 1st non-octal
    BOOST_CHECK_EQUAL( iss.peek(), static_cast<int>('9') );

    iss.str( "00057Vw" );
    iss >> scratch_o;
    BOOST_CHECK( iss );
    BOOST_CHECK_EQUAL( scratch_o, big_octal(057u) );  // stops at 1st non-digit
    BOOST_CHECK_EQUAL( iss.peek(), static_cast<int>('V') );

    iss.str( "-35aB" );
    iss >> scratch_o;
    BOOST_CHECK( !iss );  // negative value
    iss.clear();
    BOOST_CHECK_EQUAL( iss.peek(), static_cast<int>('a') );

    // Stopping and error detection, radix > 10
    big_hexadecimal  scratch_h;

    iss.str( "" );
    iss >> scratch_h;
    BOOST_CHECK( !iss );  // EOF before getting any digits
    iss.clear();

    iss.str( "-" );
    iss >> scratch_h;
    BOOST_CHECK( !iss );  // sign symbol without any following digits
    iss.clear();
    BOOST_CHECK_EQUAL( iss.peek(), static_cast<int>('-') );

    iss.str( "Cd" );
    iss >> scratch_h;
    BOOST_CHECK( !iss );  // non-digit reached before getting any digits
    iss.clear();
    BOOST_CHECK_EQUAL( iss.peek(), static_cast<int>('C') );

    iss.str( "2-5-" );
    iss >> scratch_h;
    BOOST_CHECK( !iss );  // separator NOT followed by digit, but by EOF
    iss.clear();

    iss.str( "2-5x" );
    iss >> scratch_h;
    BOOST_CHECK( iss );
    BOOST_CHECK_EQUAL( scratch_h, big_hexadecimal(0x25u) );
    BOOST_CHECK_EQUAL( iss.peek(), static_cast<int>('x') );  // stop at 1st n.d.

    iss.str( "2-5-x" );
    iss >> scratch_h;
    BOOST_CHECK( !iss );  // separator NOT followed by digit, but by non-digit
    iss.clear();
    BOOST_CHECK_EQUAL( iss.peek(), static_cast<int>('x') );

    iss.str( "8-10-24-3" );
    iss >> scratch_h;
    BOOST_CHECK( !iss );  // read number (third) is too large to be a digit
    iss.clear();
    iss >> extra;
    BOOST_CHECK( iss );
    BOOST_CHECK_EQUAL( extra, '-' );
    BOOST_CHECK_EQUAL( iss.peek(), static_cast<int>('3') );

    iss.str( "-12-5 y" );
    iss >> scratch_h;
    BOOST_CHECK( !iss );  // negative value
    iss.clear();
    BOOST_CHECK_EQUAL( iss.peek(), static_cast<int>(' ') );

    // Check exception compatibility
    big_radix_whole<3, rigged_allocator>  object1;

    iss.str( "211" );
    rigged::prime();
    iss >> object1;
    BOOST_CHECK( iss.bad() );  // exception -> error-bit
    iss.clear();

    iss.seekg( 0, ios_base::beg );
    iss.exceptions( ios_base::badbit );
    BOOST_CHECK_THROW( iss >> object1, std::bad_alloc );  // keep original ecpt.
    iss.clear();
    iss.exceptions( ios_base::goodbit );
    rigged::unprime();

    // Same tests with radix > 10
    big_radix_whole<11, rigged_allocator>  object2;

    iss.str( "2-10" );
    rigged::prime();
    iss >> object2;
    BOOST_CHECK( iss.bad() );
    iss.clear();

    iss.seekg( 0, ios_base::beg );
    iss.exceptions( ios_base::badbit );
    BOOST_CHECK_THROW( iss >> object2, std::bad_alloc );
    iss.clear();
    rigged::unprime();

    // Exceptions from format errors
    iss.exceptions( ios_base::failbit );
    iss.str( "" );
    BOOST_CHECK_THROW( iss >> object1, ios_base::failure );
    iss.clear();
    BOOST_CHECK_THROW( iss >> object2, ios_base::failure );
    iss.clear();

    iss.str( "-" );
    BOOST_CHECK_THROW( iss >> object1, ios_base::failure );
    iss.clear();
    iss.str( "+" );
    BOOST_CHECK_THROW( iss >> object2, ios_base::failure );
    iss.clear();

    iss.str( "-21" );
    BOOST_CHECK_THROW( iss >> object1, ios_base::failure );
    iss.clear();
    iss.str( "-10-6" );
    BOOST_CHECK_THROW( iss >> object2, ios_base::failure );
    iss.clear();

    iss.str( "7-851" );
    BOOST_CHECK_THROW( iss >> object2, ios_base::failure );
    iss.clear();
}

BOOST_AUTO_TEST_SUITE_END()
