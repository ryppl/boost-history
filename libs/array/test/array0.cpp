/* tests for using class array<> specialization for size 0
 * (C) Copyright Alisdair Meredith 2006.
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <string>
#include <iostream>
#include <boost/array.hpp>

namespace {
unsigned int failed_tests = 0;

void    fail_test( const char * reason ) {
    ++failed_tests;
    std::cerr << "Test failure " << failed_tests << ": " << reason << std::endl;
}

template< class T >
void    BadValue( const T &  )
{
    fail_test( "Unexpected value" );
}

template< class T >
void    RunTests()
{
    typedef boost::array< T, 0 >    test_type;

    //  Test value and aggegrate initialization
    test_type                   test_case   =   {};
    const boost::array< T, 0 >  const_test_case = test_type();

    test_case.fill ( T() );

    //  front/back and operator[] must compile, but calling them is undefined
    //  Likewise, all tests below should evaluate to false, avoiding undefined behaviour
    if( !test_case.empty() ) {
        BadValue( test_case.front() );
    }

    if( !const_test_case.empty() ) {
        BadValue( const_test_case.back() );
    }

    if( test_case.size() > 0 ) {
        BadValue( test_case[ 0 ] );
    }

    if( const_test_case.max_size() > 0 ) {
        BadValue( const_test_case[ 0 ] );
    }

    //  Assert requirements of TR1 6.2.2.4
    if( test_case.begin() != test_case.end() ) {
        fail_test( "Not an empty range" );
    }
    if( const_test_case.begin() != const_test_case.end() ) {
        fail_test( "Not an empty range" );
    }

    if( test_case.begin() == const_test_case.begin() ) {
        fail_test( "iterators for different containers are not distinct" );
    }

    if( test_case.data() == const_test_case.data() ) {
    //  Value of data is unspecified in TR1, so no requirement this test pass or fail
    //  However, it must compile!
    }


    //  Check can safely use all iterator types with std algorithms
    std::for_each( test_case.begin(), test_case.end(), BadValue< T > );
    std::for_each( test_case.rbegin(), test_case.rend(), BadValue< T > );
    std::for_each( const_test_case.begin(), const_test_case.end(), BadValue< T > );
    std::for_each( const_test_case.rbegin(), const_test_case.rend(), BadValue< T > );

    //  Check swap is well formed
    std::swap( test_case, test_case );

    //  Check assigment operator and overloads are well formed
    test_case   =   const_test_case;

    //  Confirm at() throws the std lib defined exception
    try {
        BadValue( test_case.at( 0 ) );
    } catch ( const std::out_of_range & ) {
    }

    try {
        BadValue( const_test_case.at( 0 ) );
    } catch ( const std::out_of_range & ) {
    }
}

}

int main()
{
    RunTests< bool >();
    RunTests< void * >();
    RunTests< long double >();
    RunTests< std::string >();
    return failed_tests;
}

