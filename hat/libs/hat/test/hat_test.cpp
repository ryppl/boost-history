//  Boost hat_test.cpp test file  --------------------------------------------//

// Copyright 2007 Daryle Walker.
// Distributed under the Boost Software License, Version 1.0. (See the
// accompanying file LICENSE_1_0.txt or a copy at
// <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/hat/> for the library's home page.


#define BOOST_TEST_MAIN  "hat test"

#include <boost/hat.hpp>

#include <boost/test/unit_test.hpp>                  // unit testing framework
#include <boost/test/floating_point_comparison.hpp>  // enable f.p. test macros
#include <boost/test/test_case_template.hpp>         // enable testing templates

#include <boost/cstdint.hpp>                     // sized-integer type-aliases
#include <boost/iterator/counting_iterator.hpp>  // numbers as iterators
#include <boost/pool/pool_alloc.hpp>             // alternate allocator
#include <boost/random.hpp>                      // RNG engines

#include <algorithm>   // for std::min, max
#include <cmath>       // for std::abs (double)
#include <complex>     // for std::complex & related
#include <cstddef>     // for std::size_t, NULL
#include <cstdlib>     // for std::srand, rand
#include <ctime>       // for std::time
#include <functional>  // for std::less
#include <iterator>    // for std::distance, insert_iterator
#include <map>         // for std::map
#include <memory>      // for std::allocator
#include <utility>     // for std::make_pair, pair


#pragma mark Intro stuff

// Put any using-ed types & templates, and typedefs here
using boost::fast_pool_allocator;
using boost::hat;
using boost::mt19937;


// Put custom types/templates and helper functions here
namespace
{

typedef hat<int>        int_hat;
typedef hat<double>  double_hat;

typedef fast_pool_allocator<int>  fast_allocator;

typedef std::complex<double>  complexd;

typedef boost::counting_iterator<int>  int_iterator;

// Several ways to compare complex numbers
template < typename T >
class complex_compare_xy
{
public:
    typedef std::complex<T>  first_argument_type, second_argument_type;
    typedef bool             result_type;

    result_type  operator ()( first_argument_type const &l,
     second_argument_type const &r ) const
    {
        return ( l.real() < r.real() ) || ( (l.real() == r.real()) && (l.imag()
         < r.imag()) );
    }
};

template < typename T >
class complex_compare_ma
{
public:
    typedef std::complex<T>  first_argument_type, second_argument_type;
    typedef bool             result_type;

    result_type  operator ()( first_argument_type const &l,
     second_argument_type const &r ) const
    {
        return ( abs(l) < abs(r) ) || ( (abs(l) == abs(r)) && (arg(l) <
         arg(r)) );
    }
};

enum compare_complex_by
{
    by_rectangular,
    by_polar
};

template < typename T >
class complex_compare_either
{
    compare_complex_by  method_;

public:
    typedef std::complex<T>  first_argument_type, second_argument_type;
    typedef bool             result_type;

    explicit  complex_compare_either( compare_complex_by method )
        : method_( method )  {}

    result_type  operator ()( first_argument_type const &l,
     second_argument_type const &r ) const
    {
        return ( this->method_ == by_rectangular )
         ? complex_compare_xy<T>()( l, r ) : complex_compare_ma<T>()( l, r );
    }

    bool  operator ==( complex_compare_either const &r ) const
        { return this->method_ == r.method_; }
    bool  operator !=( complex_compare_either const &r ) const
        { return !this->operator ==(r); }
};

typedef complex_compare_xy<double>      complexd_compare_xy;
typedef complex_compare_ma<double>      complexd_compare_ma;
typedef complex_compare_either<double>  complexd_compare_either;

typedef hat<complexd, mt19937, complexd_compare_xy>      complex_xy_hat;
typedef hat<complexd, mt19937, complexd_compare_ma>      complex_ma_hat;
typedef hat<complexd, mt19937, complexd_compare_either>  complex_either_hat;

typedef std::pair<int_hat::const_iterator, int_hat::const_iterator>
  int_hat_const_iterator_pair;

}  // unnamed namespace


// Mark any unprintable tested types here
BOOST_TEST_DONT_PRINT_LOG_VALUE( std::allocator<int> );
BOOST_TEST_DONT_PRINT_LOG_VALUE( int_hat );
BOOST_TEST_DONT_PRINT_LOG_VALUE( int_hat::const_iterator );
BOOST_TEST_DONT_PRINT_LOG_VALUE( int_hat::const_reverse_iterator );
BOOST_TEST_DONT_PRINT_LOG_VALUE( fast_allocator );
BOOST_TEST_DONT_PRINT_LOG_VALUE( complex_compare_either<double> );
BOOST_TEST_DONT_PRINT_LOG_VALUE( int_hat_const_iterator_pair );
    // Apple's GCC gave me an error when I didn't (make and)
    // use the "int_hat_const_iterator_pair" typedef above.


#pragma mark -
#pragma mark Core Routine Suite

// The basic routines test suite
BOOST_AUTO_TEST_SUITE( hat_core_routine_suite )

// Core constructor test, with some iterator try-outs
// (Since the internal implementation is private, we have to hope
//  that the core access routines [i.e. iterator access] really work.)
BOOST_AUTO_TEST_CASE( hat_core_constructor_test )
{
    // Default construction
    {
        // Constant-access
        int_hat const  object1;

        BOOST_CHECK( object1.begin() == object1.end() );  // try iter ==

        BOOST_CHECK_EQUAL( object1.begin(), object1.end() );
        BOOST_CHECK_EQUAL( object1.rbegin(), object1.rend() );

        // Mutable access
        int_hat  object2;

        BOOST_CHECK_EQUAL( object2.begin(), object2.end() );
        BOOST_CHECK_EQUAL( object2.rbegin(), object2.rend() );
    }

    std::size_t const   values_count = 10u;
    int const           values[] = { 2, 5, 8, 1, 4, 7, 0, 3, 6, 9 };
    int const   ordered_values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int const  reversed_values[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

    // Construction with iterator range
    {
        // Constant-access
        int_hat const  object3( values, values + values_count );

        BOOST_CHECK( object3.begin() != object3.end() );  // try iter !=
        BOOST_CHECK_EQUAL( *object3.begin(), 0 );         // try iter *
        BOOST_CHECK_EQUAL( *++object3.begin(), 1 );       // try iter ++pre
        BOOST_CHECK_EQUAL( *--object3.end(), 9 );         // try iter --pre

        BOOST_CHECK_EQUAL_COLLECTIONS( object3.begin(), object3.end(),
         ordered_values, ordered_values + values_count );
        BOOST_CHECK_EQUAL_COLLECTIONS( reversed_values, reversed_values +
         values_count, object3.rbegin(), object3.rend() );

        int_hat::const_iterator  i1( object3.begin() );  // try iter copy-ctr

        BOOST_CHECK_EQUAL( *i1++, 0 );  // try iter post++
        BOOST_CHECK_EQUAL( *i1, 1 );    // ...here
        ++i1;                           // to confirm next movement
        BOOST_CHECK_EQUAL( *i1--, 2 );  // try iter post--
        BOOST_CHECK_EQUAL( *i1, 1 );    // ...here

        // Mutable access
        int_hat  object4( values, values + values_count );

        BOOST_CHECK_EQUAL_COLLECTIONS( ordered_values, ordered_values +
         values_count, object4.begin(), object4.end() );
        BOOST_CHECK_EQUAL_COLLECTIONS( object4.rbegin(), object4.rend(),
         reversed_values, reversed_values + values_count );

        // Use a different element type
        complex_xy_hat const  object5( values, values + values_count );
        complexd const  complex_ordered_values[] = { 0.0, 1.0, 2.0, 3.0, 4.0,
         5.0, 6.0, 7.0, 8.0, 9.0 };

        BOOST_CHECK_EQUAL_COLLECTIONS( complex_ordered_values,
         complex_ordered_values + values_count, object5.begin(),
         object5.end() );
        BOOST_CHECK_EQUAL( (--object5.end())->imag(), 0.0 );  // try iter ->
        BOOST_CHECK_EQUAL( (--object5.end())->real(), 9.0 );  // ...twice

        // Should violate preconditions if uncommented (wrong iterator type)
        //complex_xy_hat const  objectX( std::insert_iterator<int_hat>(object4,
        //                       object4.begin()),
        //                       std::insert_iterator<int_hat>(object4,
        //                       object4.end()) );
        // Should violate preconditions if uncommented (no element conversion)
        //int_hat const         objectY( complex_ordered_values,
        //                       complex_ordered_values + values_count );
    }

    // Implicitly-defined copy-construction and assignment operator
    {
        // Copy constructor
        int_hat const  object6( values, values + values_count );
        int_hat const  object7( object6 );

        BOOST_CHECK_EQUAL_COLLECTIONS( reversed_values, reversed_values +
         values_count, object6.rbegin(), object6.rend() );
        BOOST_CHECK_EQUAL_COLLECTIONS( object7.begin(), object7.end(),
         ordered_values, ordered_values + values_count );

        // Assignment
        int_hat const  object8( values + values_count / 2u, values +
                        values_count );
        int_hat        object9( values, values + values_count / 2u );

        int const  ordered_lo_values[] = { 1, 2, 4, 5, 8 };
        int const  ordered_hi_values[] = { 0, 3, 6, 7, 9 };

        BOOST_CHECK_EQUAL_COLLECTIONS( object8.begin(), object8.end(),
         ordered_hi_values, ordered_hi_values + values_count / 2u );
        BOOST_CHECK_EQUAL_COLLECTIONS( ordered_lo_values, ordered_lo_values +
         values_count / 2u, object9.begin(), object9.end() );

        object9 = object8;
        BOOST_CHECK_EQUAL_COLLECTIONS( object9.begin(), object9.end(),
         ordered_hi_values, ordered_hi_values + values_count / 2u );

        object9 = int_hat();
        BOOST_CHECK_EQUAL( object9.begin(), object9.end() );

        object9 = object7;
        BOOST_CHECK_EQUAL_COLLECTIONS( reversed_values, reversed_values +
         values_count, object9.rbegin(), object9.rend() );
    }
}

// Auxillary object (allocator, comparator, RNG engine) checking test
// (It's not too impressive for allocators since all the allocator types
//  I've found make all objects of the same type equivalent.)
BOOST_AUTO_TEST_CASE( hat_auxillary_checking_test )
{
    // Standard allocator
    {
        int_hat const  object1;

        BOOST_CHECK_EQUAL( object1.get_allocator(), std::allocator<int>() );
    }

    // Custom allocator
    {
        hat<int, mt19937, std::less<int>, fast_allocator> const  object2;

        BOOST_CHECK_EQUAL( object2.get_allocator(), fast_allocator() );
    }

    // Custom comparator (note that since key_type == value_type,
    // that key_compare == value_compare)
    {
        // different comparators
        // (Using temporary objects in "object[3|4]" instead of the three
        //  full objects got me the function-declaration v. object-construction
        //  confusion error.)
        mt19937 const                  default_rng;
        complexd_compare_either const  comp_rect( by_rectangular );
        complexd_compare_either const  comp_polar( by_polar );
        complex_either_hat const       object3( default_rng, comp_rect );
        complex_either_hat             object4( default_rng, comp_polar );
        complexd const                 c1( 3.0, 4.0 ), c2( 4.0, 3.0 );

        BOOST_CHECK( object3.key_comp() != object4.key_comp() );
        BOOST_CHECK( object3.key_comp()(c1, c2) != object4.key_comp()(c1, c2) );

        // Due to the implementation, the value-comparison type is compatible
        // with the key-comparison type only as far as using the application
        // operator is concerned; it doesn't carry over any other interface.
        BOOST_CHECK_EQUAL( object3.key_comp()(c1, c2) == object4.key_comp()(c1,
         c2), object3.value_comp()(c1, c2) == object4.value_comp()(c1, c2) );

        // change to make same
        object4 = object3;
        BOOST_CHECK_EQUAL( object4.key_comp(), object3.key_comp() );
        BOOST_CHECK( object4.key_comp()(c1, c2) == object3.key_comp()(c1, c2) );

        BOOST_CHECK_EQUAL( object3.key_comp()(c1, c2) == object4.key_comp()(c1,
         c2), object3.value_comp()(c1, c2) == object4.value_comp()(c1, c2) );
    }

    // Custom RNG (and show some inner access)
    {
        // different engines
        int_hat const  object5( mt19937(static_cast<uint32_t>( 23u )) );
        int_hat        object6( mt19937(static_cast<uint32_t>( 45u )) );

        BOOST_CHECK( object5.rng_engine() != object6.rng_engine() );

        // change to make same
        object6 = object5;
        BOOST_CHECK_EQUAL( object6.rng_engine(), object5.rng_engine() );

        // Make different again (with inner access)
        object6.rng_engine().seed( static_cast<uint32_t>(45u) );
        BOOST_CHECK( object5.rng_engine() != object6.rng_engine() );
    }
}

BOOST_AUTO_TEST_SUITE_END()


#pragma mark -
#pragma mark Set Routine Suite

// The set-container routines test suite
BOOST_AUTO_TEST_SUITE( hat_set_routine_suite )

// Searching test
BOOST_AUTO_TEST_CASE( hat_search_test )
{
    using std::distance;
    using std::make_pair;

    int const           block1[] = { -3, -6, 2, 0, 15, 2, 7, -6, 9 };
    int const           ordered1[] = { -6, -3, 0, 2, 7, 9, 15 };
    std::size_t const   block1_s = 9u, ordered1_s = 7u;
    int_hat const       hat1( block1, block1 + block1_s );

    BOOST_CHECK_EQUAL_COLLECTIONS( hat1.begin(), hat1.end(), ordered1, ordered1
     + ordered1_s );

    // Find and count (there's no duplicates)
    BOOST_CHECK_EQUAL( 1, distance(hat1.begin(), hat1.find( -3 )) );
    BOOST_CHECK_EQUAL( 1u, hat1.count(-3) );
    BOOST_CHECK_EQUAL( 3, distance(hat1.begin(), hat1.find( 2 )) );
    BOOST_CHECK_EQUAL( 1u, hat1.count(2) );

    BOOST_CHECK_EQUAL( hat1.end(), hat1.find(8) );
    BOOST_CHECK_EQUAL( 0u, hat1.count(8) );

    // Bounds
    int_hat::iterator  bounds_l = hat1.lower_bound( 0 );  // existing element
    int_hat::iterator  bounds_u = hat1.upper_bound( 0 );

    BOOST_CHECK_EQUAL( 2, distance(hat1.begin(), bounds_l) );  // at "0"
    BOOST_CHECK_EQUAL( 3, distance(hat1.begin(), bounds_u) );  // at "2"
    BOOST_CHECK_EQUAL( make_pair(bounds_l, bounds_u), hat1.equal_range(0) );

    bounds_l = hat1.lower_bound( 8 );  // between elements
    bounds_u = hat1.upper_bound( 8 );
    BOOST_CHECK_EQUAL( 5, distance(hat1.begin(), bounds_l) );  // at "9"
    BOOST_CHECK_EQUAL( 5, distance(hat1.begin(), bounds_u) );  // at "9"
    BOOST_CHECK_EQUAL( make_pair(bounds_l, bounds_u), hat1.equal_range(8) );

    bounds_l = hat1.lower_bound( -8 );  // before first element
    bounds_u = hat1.upper_bound( -8 );
    BOOST_CHECK_EQUAL( 0, distance(hat1.begin(), bounds_l) );  // at "-6"
    BOOST_CHECK_EQUAL( 0, distance(hat1.begin(), bounds_u) );  // at "-6"
    BOOST_CHECK_EQUAL( make_pair(bounds_l, bounds_u), hat1.equal_range(-8) );

    bounds_l = hat1.lower_bound( 15 );  // at last element
    bounds_u = hat1.upper_bound( 15 );
    BOOST_CHECK_EQUAL( 6, distance(hat1.begin(), bounds_l) );  // at "15"
    BOOST_CHECK_EQUAL( bounds_u, hat1.end() );
    BOOST_CHECK_EQUAL( make_pair(bounds_l, bounds_u), hat1.equal_range(15) );

    bounds_l = hat1.lower_bound( 21 );  // after last element
    bounds_u = hat1.upper_bound( 21 );
    BOOST_CHECK_EQUAL( hat1.end(), bounds_l );
    BOOST_CHECK_EQUAL( bounds_u, hat1.end() );
    BOOST_CHECK_EQUAL( make_pair(bounds_l, bounds_u), hat1.equal_range(21) );
}

// Element insert & removal test
BOOST_AUTO_TEST_CASE( hat_container_routine_test )
{
    using std::size_t;
    using std::distance;

    int const      block1[] = { -3, -6, 2, 0, 15, 2, 7, -6, 9 };
    int const      ordered1[] = { -6, -3, 0, 2, 7, 9, 15 };
    size_t const   block1_s = sizeof(block1) / sizeof(block1[0]);
    size_t const   ordered1_s = sizeof(ordered1) / sizeof(ordered1[0]);
    int_hat        hat1( block1, block1 + block1_s );

    BOOST_CHECK_EQUAL_COLLECTIONS( hat1.begin(), hat1.end(), ordered1, ordered1
     + ordered1_s );

    // Inserting with success flag
    std::pair<int_hat::iterator, bool>  inserted = hat1.insert( 7 );

    BOOST_CHECK( !inserted.second );
    BOOST_CHECK_EQUAL( 4, distance(hat1.begin(), inserted.first) );

    inserted = hat1.insert( -7 );  // now {-7,-6,-3,0,2,7,9,15}
    BOOST_CHECK( inserted.second );
    BOOST_CHECK_EQUAL( 0, distance(hat1.begin(), inserted.first) );

    inserted = hat1.insert( 4 );  // now {-7,-6,-3,0,2,4,7,9,15}
    BOOST_CHECK( inserted.second );
    BOOST_CHECK_EQUAL( 5, distance(hat1.begin(), inserted.first) );

    inserted = hat1.insert( 32 );  // now {-7,-6,-3,0,2,4,7,9,15,32}
    BOOST_CHECK( inserted.second );
    BOOST_CHECK_EQUAL( 9, distance(hat1.begin(), inserted.first) );

    inserted = hat1.insert( 32 );  // no change
    BOOST_CHECK( !inserted.second );
    BOOST_CHECK_EQUAL( 9, distance(hat1.begin(), inserted.first) );

    // Inserting with a hint
    int_hat::iterator &  i = inserted.first;
    int_hat::size_type   s = distance( hat1.begin(), hat1.end() );

    i = hat1.insert( hat1.find(9), 14 );  // now {-7,-6,-3,0,2,4,7,9,14,15,32}
    BOOST_CHECK_EQUAL( 1, distance(hat1.find( 9 ), i) );  // good hint
    BOOST_CHECK_EQUAL( ++s, distance(hat1.begin(), hat1.end()) );

    i = hat1.insert( hat1.find(0), 22 );
        // now {-7,-6,-3,0,2,4,7,9,14,15,22,32}
    BOOST_CHECK_EQUAL( 7, distance(hat1.find( 0 ), i) );  // bad hint
    BOOST_CHECK_EQUAL( ++s, distance(hat1.begin(), hat1.end()) );

    i = hat1.insert( hat1.find(0), 2 );  // no change
    BOOST_CHECK_EQUAL( 1, distance(hat1.find( 0 ), i) );  // good hint
    BOOST_CHECK_EQUAL( s, distance(hat1.begin(), hat1.end()) );

    i = hat1.insert( hat1.find(14), 4 );  // no change
    BOOST_CHECK_EQUAL( 3, distance(i, hat1.find( 14 )) );  // bad hint
    BOOST_CHECK_EQUAL( s, distance(hat1.begin(), hat1.end()) );

    // Inserting a range
    int const     block2[] = { -12, -3, 4, 11, 22, 40 };
    size_t const  block2_s = sizeof(block2) / sizeof(block2[0]);
    size_t const  unique_block2 = 3u;

    hat1.insert( block2, block2 + block2_s );
    BOOST_CHECK_EQUAL( s += unique_block2, distance(hat1.begin(), hat1.end()) );
        // now {-12,-7,-6,-3,0,2,4,7,9,11,14,15,22,32,40}

    // Removing 1 element
    hat1.erase( hat1.find(-3) );  // now {-12,-7,-6,0,2,4,7,9,11,14,15,22,32,40}
    BOOST_CHECK_EQUAL( hat1.end(), hat1.find(-3) );
    BOOST_CHECK_EQUAL( --s, distance(hat1.begin(), hat1.end()) );

    BOOST_CHECK_EQUAL( hat1.end(), hat1.find(5) );
    BOOST_CHECK_EQUAL( 0u, hat1.erase(5) );  // no change
    BOOST_CHECK_EQUAL( hat1.end(), hat1.find(5) );
    BOOST_CHECK_EQUAL( s, distance(hat1.begin(), hat1.end()) );

    BOOST_CHECK_EQUAL( hat1.begin(), hat1.find(-12) );
    BOOST_CHECK_EQUAL( 1u, hat1.erase(-12) );
        // now {-7,-6,0,2,4,7,9,11,14,15,22,32,40}
    BOOST_CHECK_EQUAL( hat1.end(), hat1.find(-12) );
    BOOST_CHECK_EQUAL( --s, distance(hat1.begin(), hat1.end()) );

    // Removing a range
    hat1.erase( hat1.find(11), hat1.find(15) );
        // now {-7,-6,0,2,4,7,9,15,22,32,40}
    BOOST_CHECK_EQUAL( hat1.end(), hat1.find(11) );
    BOOST_CHECK_EQUAL( hat1.end(), hat1.find(14) );
    BOOST_CHECK_EQUAL( 7, distance(hat1.begin(), hat1.find( 15 )) );
    BOOST_CHECK_EQUAL( s -= 2, distance(hat1.begin(), hat1.end()) );

    hat1.erase( hat1.find(22), hat1.end() );  // now {-7,-6,0,2,4,7,9,15}
    BOOST_CHECK_EQUAL( hat1.end(), hat1.find(22) );
    BOOST_CHECK_EQUAL( hat1.end(), hat1.find(32) );
    BOOST_CHECK_EQUAL( hat1.end(), hat1.find(40) );
    BOOST_CHECK_EQUAL( s -= 3, distance(hat1.begin(), hat1.end()) );

    hat1.clear();  // now {}
    BOOST_CHECK_EQUAL( hat1.begin(), hat1.end() );
    BOOST_CHECK_EQUAL( s = 0, distance(hat1.begin(), hat1.end()) );

    // Swapping
    int_hat  hat2( block2, block2 + block2_s );

    BOOST_CHECK_EQUAL_COLLECTIONS( hat2.begin(), hat2.end(), block2, block2 +
     block2_s );

    hat1.swap( hat2 );
    BOOST_CHECK_EQUAL( hat2.begin(), hat2.end() );
    BOOST_CHECK_EQUAL_COLLECTIONS( hat1.begin(), hat1.end(), block2, block2 +
     block2_s );

    swap( hat2, hat1 );
    BOOST_CHECK_EQUAL( hat1.begin(), hat1.end() );
    BOOST_CHECK_EQUAL_COLLECTIONS( hat2.begin(), hat2.end(), block2, block2 +
     block2_s );
}

// Sizing test
BOOST_AUTO_TEST_CASE( hat_size_inspections_test )
{
    // Empty hat
    int_hat  hat1;

    BOOST_CHECK( hat1.empty() );
    BOOST_CHECK_EQUAL( 0u, hat1.size() );
    BOOST_CHECK( hat1.capacity() >= hat1.size() );

    // Extra
    BOOST_CHECK( hat1.max_size() > 0u );

    // Insert values and check
    int_iterator  count( 0 );

    hat1.insert( count, count + 4 );
    BOOST_CHECK( !hat1.empty() );
    BOOST_CHECK_EQUAL( 4u, hat1.size() );
    BOOST_CHECK( hat1.capacity() >= hat1.size() );
    count += 4;

    // Use up reserve...
    while ( *count < hat1.capacity() )
    {
        hat1.insert( *count++ );
    }

    // ...And expand it
    int const  old_capacity = *count;

    BOOST_CHECK_EQUAL( old_capacity, hat1.capacity() );
    hat1.reserve( old_capacity + 10 );
    BOOST_CHECK( hat1.capacity() - old_capacity >= 10 );
    hat1.insert( *count++ );
    BOOST_CHECK( hat1.size() > old_capacity );
    BOOST_CHECK( !hat1.empty() );

    // Can't shrink capacity
    hat1.reserve( old_capacity + 9 );
    BOOST_CHECK( hat1.capacity() - old_capacity >= 10 );

    // Remove values and check
    BOOST_CHECK_EQUAL( hat1.size(), old_capacity + 1 );

    hat1.erase( 3 );
    BOOST_CHECK( !hat1.empty() );
    BOOST_CHECK_EQUAL( hat1.size(), old_capacity );
    BOOST_CHECK( hat1.capacity() - old_capacity >= 10 );

    hat1.clear();
    BOOST_CHECK( hat1.empty() );
    BOOST_CHECK_EQUAL( hat1.size(), 0u );
    BOOST_CHECK( hat1.capacity() - old_capacity >= 10 );
}

// Equality & Ordering operators test
BOOST_AUTO_TEST_CASE( hat_comparison_operators_test )
{
    int const  block1[] = { 1, 5, 9 }, block2[] = { 8, 0, 4 };
    int const  block3[] = { 4, 0 }, block4[] = { 0, 8 };

    int_hat const  h1( block1, block1 + 3 ), h2( block2, block2 + 3 );
    int_hat const  h3( block3, block3 + 2 ), h4( block4, block4 + 2 );
    int_hat const  h5( h4.rbegin(), h4.rend() );

    // Equality: same element type, same and different lengths
    BOOST_CHECK( h1 == h1 );
    BOOST_CHECK( h1 != h2 );
    BOOST_CHECK( h1 != h3 );
    BOOST_CHECK( h2 != h3 );
    BOOST_CHECK( h4 == h4 );
    BOOST_CHECK( h5 == h4 );

    // Equality: differing element types, same and different lengths
    double_hat const  h6( h5.begin(), h5.end() );

    BOOST_CHECK( h6 == h5 );
    BOOST_CHECK( h6 != h3 );
    BOOST_CHECK( h1 != h6 );

    // Equality: differing RNGs, comparison types, or allocators
    // (Note that complexd can't be compared with int, only double.)
    hat< complexd, boost::hellekalek1995, complexd_compare_ma,
     fast_pool_allocator<complexd> > const  h7( h6.begin(), h6.end() );

    BOOST_CHECK( h7 == h6 );
    BOOST_CHECK( double_hat(h3.begin(), h3.end()) != h7 );
    BOOST_CHECK( double_hat(h2.rbegin(), h2.rend()) != h7 );

    // Ordering: same element type
    BOOST_CHECK( h3 < h1 );
    BOOST_CHECK( h1 > h2 );
    BOOST_CHECK( h3 <= h2 );
    BOOST_CHECK( h4 >= h3 );

    // Ordering: differing element types
    BOOST_CHECK( h5 <= h6 );
    BOOST_CHECK( h6 >= h3 );
    BOOST_CHECK( h6 < h1 );
    BOOST_CHECK( h6 > h2 );

    // Can't use complex types, since they don't have a native operator<
}

BOOST_AUTO_TEST_SUITE_END()


#pragma mark -
#pragma mark Random-Selection Routine Suite

// The hat-selection routines test suite
BOOST_AUTO_TEST_SUITE( hat_selection_routine_suite )

// Weight-checking tests
BOOST_AUTO_TEST_CASE( hat_weight_test )
{
    // Empty hat
    complex_xy_hat  hat1;

    BOOST_CHECK_EQUAL( hat1.total_weight(), 0u );
    BOOST_CHECK_EQUAL( hat1.get_weight(complexd(1.0, 1.0)), 0u );

    BOOST_CHECK( hat1.empty() );

    // Extra
    BOOST_CHECK( hat1.max_total_weight() > 0u );
    BOOST_CHECK_EQUAL( hat1.max_weight(), 1u );

    // Add weight
    BOOST_CHECK( hat1.set_weight(complexd( 1.0, 1.0 ), 1u) );
    BOOST_CHECK( hat1.set_weight(complexd( 0.0, -11.0 ), 1u) );
    BOOST_CHECK_EQUAL( hat1.total_weight(), 2u );
    BOOST_CHECK_EQUAL( hat1.get_weight(complexd(1.0, 1.0)), 1u );
    BOOST_CHECK_EQUAL( hat1.get_weight(complexd(0.0, -11.0)), 1u );
    BOOST_CHECK_EQUAL( hat1.get_weight(complexd(6.0, 0.0)), 0u );
    BOOST_CHECK_EQUAL( hat1.get_weight(complexd(-0.5, 8.0)), 0u );

    BOOST_CHECK_EQUAL( 2u, hat1.size() );

    // Can't make weights above 1
    BOOST_CHECK( !hat1.set_weight(complexd( 6.0, 0.0 ), 5u) );
    BOOST_CHECK_EQUAL( hat1.total_weight(), 2u );
    BOOST_CHECK_EQUAL( hat1.get_weight(complexd(1.0, 1.0)), 1u );
    BOOST_CHECK_EQUAL( hat1.get_weight(complexd(0.0, -11.0)), 1u );
    BOOST_CHECK_EQUAL( hat1.get_weight(complexd(6.0, 0.0)), 0u );
    BOOST_CHECK_EQUAL( hat1.get_weight(complexd(-0.5, 8.0)), 0u );

    BOOST_CHECK_EQUAL( 2u, hat1.size() );

    // Lose weight
    BOOST_CHECK( hat1.set_weight(complexd( 0.0, -11.0 ), 0u) );
    BOOST_CHECK_EQUAL( hat1.total_weight(), 1u );
    BOOST_CHECK_EQUAL( hat1.get_weight(complexd(1.0, 1.0)), 1u );
    BOOST_CHECK_EQUAL( hat1.get_weight(complexd(0.0, -11.0)), 0u );
    BOOST_CHECK_EQUAL( hat1.get_weight(complexd(6.0, 0.0)), 0u );
    BOOST_CHECK_EQUAL( hat1.get_weight(complexd(-0.5, 8.0)), 0u );

    BOOST_CHECK_EQUAL( 1u, hat1.size() );

    hat1.clear();
    BOOST_CHECK_EQUAL( hat1.total_weight(), 0u );
    BOOST_CHECK( hat1.empty() );
}

// Random-selection tests
BOOST_AUTO_TEST_CASE( hat_select_test )
{
    // Empty hat
    int_hat  hat1;
    int      dummy = 0;

    std::srand( static_cast<unsigned>(std::time( NULL )) );
    hat1.rng_engine().seed( static_cast<uint32_t>(std::rand()) );

    BOOST_CHECK( hat1.empty() );
    BOOST_CHECK_EQUAL( hat1.pick(), hat1.end() );
    BOOST_CHECK( !hat1.draw(dummy) );
    BOOST_CHECK( !hat1.discard() );
    BOOST_CHECK( hat1.empty() );

    // Single element
    hat1.insert( -1 );
    BOOST_CHECK_EQUAL( 1u, hat1.size() );
    BOOST_CHECK_EQUAL( -1, *hat1.pick() );
    BOOST_CHECK_EQUAL( -1, hat1.sample() );

    while ( hat1.draw(dummy) )
        ;
    BOOST_CHECK_EQUAL( dummy, -1 );
    BOOST_CHECK( hat1.empty() );

    hat1.insert( -2 );
    BOOST_CHECK_EQUAL( 1u, hat1.size() );
    while ( hat1.discard() )
        ;
    BOOST_CHECK( hat1.empty() );

    // Many random sample trials, hopefully RNG is uniform
    unsigned long const           trial_limit = 10000ul;
    std::map<int, unsigned long>  trial_results;
    int const                     entry_limit = 10;
    double const                  error_tolerance_percentage = 10.0;

    hat1.insert( int_iterator(0), int_iterator(entry_limit) );

    for ( unsigned long  i = 0ul ; i < trial_limit ; ++i )
    {
        ++trial_results[ *hat1.pick() ];
    }

    for ( int  i = 0 ; i < entry_limit ; ++i )
    {
        double const  actual( trial_results[i] ),
                      expected( double(trial_limit) / entry_limit );
        double const  diff = std::abs( actual - expected ), da = diff / actual,
                      de = diff / expected;
        double const  error = 100.0 * std::min( da, de );
          // use "max" for leinancy

        BOOST_CHECK_MESSAGE( error < error_tolerance_percentage,
         "for pick trial #" << i + 1 << ", the actual results (" <<
         trial_results[i] << ") differ from the expected ones (" << expected <<
         ") by more than " << error_tolerance_percentage <<
         "%.  (It's actually off by " << error << "%.)" );
    }

    // Use another sampling method for trials
    trial_results.clear();
    for ( unsigned long  i = 0ul ; i < trial_limit ; ++i )
    {
        ++trial_results[ hat1.sample() ];
    }

    for ( int  i = 0 ; i < entry_limit ; ++i )
    {
        double const  actual( trial_results[i] ),
                      expected( double(trial_limit) / entry_limit );
        double const  diff = std::abs( actual - expected ), da = diff / actual,
                      de = diff / expected;
        double const  error = 100.0 * std::min( da, de );
          // use "max" for leinancy

        BOOST_CHECK_MESSAGE( error < error_tolerance_percentage,
         "for sample trial #" << i + 1 << ", the actual results (" <<
         trial_results[i] << ") differ from the expected ones (" << expected <<
         ") by more than " << error_tolerance_percentage <<
         "%.  (It's actually off by " << error << "%.)" );
    }

    // Sample (possibly) and remove
    BOOST_CHECK_EQUAL( hat1.size(), entry_limit );

    BOOST_CHECK( hat1.draw(dummy) );
    BOOST_CHECK_EQUAL( hat1.size(), entry_limit - 1 );
    BOOST_CHECK( (dummy >= 0) && (dummy < entry_limit) );
    BOOST_CHECK_EQUAL( hat1.end(), hat1.find(dummy) );
    BOOST_CHECK_EQUAL( hat1.get_weight(dummy), 0u );

    BOOST_CHECK( hat1.discard() );
    BOOST_CHECK_EQUAL( hat1.size(), entry_limit - 2 );
}

BOOST_AUTO_TEST_SUITE_END()
