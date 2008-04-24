/**
 * \file boost/mirror/detail/array_type_name.hpp
 * Helpers for composing array type names
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_ARRAY_TYPE_NAME_HPP
#define BOOST_MIRROR_META_DETAIL_ARRAY_TYPE_NAME_HPP

#include <assert.h>
#include <boost/mirror/detail/nontrivial_type_name.hpp>

namespace boost {
namespace mirror {
namespace detail {


template <class meta_type, size_t array_size>
struct static_array_type_name_base
{
	BOOST_MIRROR_CONST_MEMBER_ATTRIB(
		size_t,
		base_name_length,
		(meta_type::base_name_length + 3)
	)

protected:
	static void init_base_name(bchar* the_base_name)
	{
		bchar* cur_pos = the_base_name;
		//
		// copy the name of the template
		bstrcpy(cur_pos, meta_type::base_name());
		cur_pos += meta_type::base_name_length;
		//
		// append the " [N]" 
		*(cur_pos++) = BOOST_STR_LIT(' ');
		*(cur_pos++) = BOOST_STR_LIT('[');
		*(cur_pos++) = BOOST_STR_LIT(']');
		//
		// finalize the string
		assert(cur_pos == (the_base_name + base_name_length));
		*cur_pos = BOOST_STR_LIT('\0');
	}
};

template <class meta_type, size_t array_size>
struct static_array_type_name : static_nontrivial_type_name<
	static_array_type_name_base<meta_type, array_size>
>{ };


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

