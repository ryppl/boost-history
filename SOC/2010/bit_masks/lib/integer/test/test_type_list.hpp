//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

/** This is a header file used for creating a list of all of the different
 *  types which I wish to run all tests against.
 *
 *  Basically the idea is to use boost for_each to test all of the different
 *  types inside the list by passing it off to a function template.
 *
 */


#ifndef BOOST_BITS_AND_INTS_TESTING_TYPE_LIST_HPP
#define BOOST_BITS_AND_INTS_TESTING_TYPE_LIST_HPP

// standard library includes
#include <cstddef>


// boost includes
#include <boost/cstdint.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/for_each.hpp>

using namespace boost;

/*
integer types from boost integer:

int_least8_t
int_least16_t
int_least32_t
uint_least8_t
uint_least16_t
uint_least32_t


The following types are available only if, after including <boost/cstdint.hpp>,
the macro BOOST_NO_INT64_T is not defined:

int_least64_t
uint_least64_t

The following fastest minimum-width integer types are provided for all platforms:

int_fast8_t
int_fast16_t
int_fast32_t
uint_fast8_t
uint_fast16_t
uint_fast32_t

The following types are available only if, after including <boost/cstdint.hpp>,
the macro BOOST_NO_INT64_T is not defined:

int_fast64_t
uint_fast64_t

/////
typedef low_bits<T, 3> lbits
*/

// testing type list.
typedef boost::mpl::list<
                            char,
                            unsigned char,
                            short,
                            unsigned short,
                            long,
                            unsigned long,
                            int,
                            unsigned int
#if 0
                            ,
                            long long,
                            unsigned long long,
#endif
                        > test_types;    


            // ____________ standard types __________________ 
typedef boost::mpl::list<
                            // char16_t,
                            // char32_t,
                            wchar_t,
                            std::ptrdiff_t,
                            std::size_t//,
                            // std::max_align_t,
                            // std::nullptr_t,
                        > test_types_2;

            // ____________ Boost cstdint types __________________
typedef boost::mpl::list<
                            boost::int_least8_t,
                            boost::int_least16_t,
                            boost::int_least32_t,
                            boost::uint_least8_t,
                            boost::uint_least16_t,
                            boost::uint_least32_t,
#if 0
                            boost::int_least64_t,
                            boost::uint_least64_t,
#endif
                            boost::int_fast8_t,
                            boost::int_fast16_t,
                            boost::int_fast32_t,
                            boost::uint_fast8_t,
                            boost::uint_fast16_t,
                            boost::uint_fast32_t
#if 0
                            ,
                            boost::int_fast64_t,
                            boost::uint_fast64_t
#endif
                        > test_types_3;                        



#endif
