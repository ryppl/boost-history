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

template <class meta_type, typename dummy, bool base_type>
struct static_volatile_type_name_base
{
protected:
	typedef nontrivial_type_base_or_full_name<meta_type, base_type>
		name_info;

	BOOST_STATIC_CONSTANT(int, difference = 9);

	BOOST_STATIC_CONSTANT(
		int,
		name_length = 
		name_info::name_length + difference
	);

	static void init_name(bchar* the_name)
	{
		bchar* cur_pos = the_name;
		//
		// copy the name of the base type
		bstrcpy(cur_pos, name_info::name());
		cur_pos += name_info::name_length;
		//
		// append the volatile keyword
		bstrcpy(cur_pos, BOOST_STR_LIT(" volatile"));
		cur_pos += difference;
		//
		// finalize the string
		assert(cur_pos == (the_name + name_length));
		*cur_pos = BOOST_STR_LIT('\0');
	}
};

template <class meta_type>
struct static_volatile_type_name : static_nontrivial_type_name<
	meta_type, void, static_volatile_type_name_base
>{ };


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

