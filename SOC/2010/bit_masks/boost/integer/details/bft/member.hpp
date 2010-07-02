//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_MEMBER_FIELD_HPP
#define BOOST_MEMBER_FIELD_HPP
#include <cstddef>
#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/integer/bit_width.hpp>


namespace boost {



/** The purpose of the member struct is to provide a simple way of passing
 *  parameters into a bitfield_tuple or bit_mask_tuple. The use of this is
 *  specifically to associate a group of parameters to gather within a type
 *  and make it easier for the data structure to figure out what to do with
 *  those parameter.
 */
template <typename ReturnType, typename Name, std::size_t FieldWidth>
struct member;



} // end boost

#endif