//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_TUPLE_NAME_LOCATER_HPP
#define BOOST_BITFIELD_TUPLE_NAME_LOCATER_HPP
#include <boost/integer/details/bft/bft_element.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/type_traits.hpp>

namespace boost { namespace details {


/** This is a lambda expression which is passed to find_if 
 *  for processing which can be used to locate a type with a specific name
 *  inside of an mpl sequence type.
 */
template <typename T, typename U>
struct match_name
    :is_same<typename T::name_type, U>::type
{ };


}} // end boost::details

#endif
