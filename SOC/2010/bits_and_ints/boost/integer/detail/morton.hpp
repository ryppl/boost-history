//  Boost integer/detail/morton.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_DETAIL_MORTON_INCLUDED
#define BOOST_DETAIL_MORTON_INCLUDED

#include <boost/cstdint.hpp>

/*
 *	Some inner types used by morton type.
 */
namespace boost {
namespace integer_detail {

/*
 * The parameter size must be equal to 8, 16, 32 or 64.
 */
template <std::size_t size>
struct morton_type
{};

template <>
struct morton_type<8>
{
	typedef uint8_t type;
};

template <>
struct morton_type<16>
{
	typedef uint16_t type;
};

template <>
struct morton_type<32>
{
	typedef uint32_t type;
};

template <>
struct morton_type<64>
{
	typedef uint64_t type;
};

} // integer_detail
} // boost

#endif