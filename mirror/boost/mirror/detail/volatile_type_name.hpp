/**
 * \file boost/mirror/detail/volatile_type_name.hpp
 * Helpers for composing volatile type names
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_VOLATILE_TYPE_NAME_HPP
#define BOOST_MIRROR_META_DETAIL_VOLATILE_TYPE_NAME_HPP

#include <assert.h>
#include <boost/mirror/detail/nontrivial_type_name.hpp>

namespace boost {
namespace mirror {
namespace detail {

template <class meta_type>
struct static_volatile_type_name_base
{
	BOOST_MIRROR_CONST_MEMBER_ATTRIB(
		size_t,
		base_name_length,
		(meta_type::base_name_length + 9)
	)

protected:
	static void init_base_name(bchar* the_base_name)
	{
		bchar* cur_pos = the_base_name;
		//
		// copy the name of the base type
		bstrcpy(cur_pos, meta_type::base_name());
		cur_pos += meta_type::base_name_length;
		//
		// append the const keyword
		bstrcpy(cur_pos, BOOST_STR_LIT(" volatile"));
		cur_pos += 9;
		//
		// finalize the string
		assert(cur_pos == (the_base_name + base_name_length));
		*cur_pos = BOOST_STR_LIT('\0');
	}
};

template <class meta_type>
struct static_volatile_type_name : static_nontrivial_type_name<
	static_volatile_type_name_base<meta_type>
>{ };


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

