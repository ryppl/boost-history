//  (C) Copyright Eric Niebler 2004.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/*
 Revision history:
   25 August 2005 : Initial version.
*/

#include <boost/test/minimal.hpp>
#include "../../../boost/foreach.hpp"

///////////////////////////////////////////////////////////////////////////////
// define the container types, used by utility.hpp to generate the helper functions
typedef int container_type[5];
typedef int const const_container_type[5];
typedef int value_type;
typedef int &reference_type;
typedef int const &const_reference_type;

#include "./utility.hpp"

///////////////////////////////////////////////////////////////////////////////
// define come containers
//
int my_array[5] = { 1,2,3,4,5 };
int const (&my_const_array)[5] = my_array;

///////////////////////////////////////////////////////////////////////////////
// test_main
//   
int test_main( int, char*[] )
{
    // non-const containers by value
    BOOST_CHECK(sequence_equal_byval(my_array, "\1\2\3\4\5"));

    // const containers by value
    BOOST_CHECK(sequence_equal_byval(my_const_array, "\1\2\3\4\5"));

    return 0;
}
