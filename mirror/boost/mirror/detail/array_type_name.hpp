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
#include <boost/mirror/detail/static_int_to_str.hpp>

namespace boost {
namespace mirror {
namespace detail {


template <class meta_type, class array_size, bool base_name>
struct static_array_type_name_base
{
protected:
	typedef typename detail::static_int_to_str<array_size::value>
		size_as_string;

	typedef nontrivial_type_base_or_full_name<meta_type, base_name>
		name_info;

	BOOST_STATIC_CONSTANT(size_t, difference = 3 + size_as_string::length::value);

	BOOST_STATIC_CONSTANT(
		size_t,
		name_length = 
		name_info::name_length + difference
	);

	static void init_name(bchar* the_name)
	{
		bchar* cur_pos = the_name;
		//
		// copy the name of the template
		bstrcpy(cur_pos, name_info::name());
		cur_pos += name_info::name_length;
		//
		// append the " [" 
		*(cur_pos++) = BOOST_STR_LIT(' ');
		*(cur_pos++) = BOOST_STR_LIT('[');
		// append the index
		size_as_string::convert(cur_pos, size_as_string::length::value + 1);
		cur_pos += size_as_string::length::value;
		// append the "]" 
		*(cur_pos++) = BOOST_STR_LIT(']');
		//
		// finalize the string
		assert(cur_pos == (the_name + name_length));
		*cur_pos = BOOST_STR_LIT('\0');
	}
};

template <class meta_type, bool base_name>
struct static_array_type_name_base<meta_type, mpl::int_<-1>, base_name>
{
protected:
	typedef nontrivial_type_base_or_full_name<meta_type, base_name>
		name_info;

	BOOST_STATIC_CONSTANT(
		size_t,
		name_length = 
		name_info::name_length + 3
	);

	static void init_name(bchar* the_name)
	{
		bchar* cur_pos = the_name;
		//
		// copy the name of the template
		bstrcpy(cur_pos, name_info::name());
		cur_pos += name_info::name_length;
		//
		// append the " []" 
		*(cur_pos++) = BOOST_STR_LIT(' ');
		*(cur_pos++) = BOOST_STR_LIT('[');
		*(cur_pos++) = BOOST_STR_LIT(']');
		//
		// finalize the string
		assert(cur_pos == (the_name + name_length));
		*cur_pos = BOOST_STR_LIT('\0');
	}
};


template <class meta_type, int array_size>
struct static_array_type_name : static_nontrivial_type_name<
	meta_type, mpl::int_<array_size>, static_array_type_name_base
>{ };

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

