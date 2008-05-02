/**
 * \file boost/mirror/detail/ptr_ref_type_name.hpp
 * Helpers for composing pointer and reference type names
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_PTRREF_TYPE_NAME_HPP
#define BOOST_MIRROR_META_DETAIL_PTRREF_TYPE_NAME_HPP

#include <assert.h>
#include <boost/mirror/detail/nontrivial_type_name.hpp>

namespace boost {
namespace mirror {
namespace detail {

template <class MetaType, bool BaseName, bchar Token, bchar Token2>
struct static_ptr_ref_type_name_base
{
protected:
	typedef nontrivial_type_base_or_full_name<MetaType, BaseName>
		name_info;

	BOOST_STATIC_CONSTANT(int, difference = 3);

	BOOST_STATIC_CONSTANT(
		int,
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
		// append the " * " or " & "
		assert(cur_pos == (the_name + name_info::name_length));
		*(cur_pos++) = BOOST_STR_LIT(' ');
		*(cur_pos++) = Token;
		*(cur_pos++) = Token2;
		//
		// finalize the string
		assert(cur_pos == (the_name + name_length));
		*cur_pos = BOOST_STR_LIT('\0');
	}
};

template <class MetaType, typename Dummy, bool BaseName>
struct static_pointer_type_name_base
: static_ptr_ref_type_name_base<
	MetaType, 
	BaseName, 
	BOOST_STR_LIT(' '),
	BOOST_STR_LIT('*')
>{ };

template <class MetaType, typename Dummy, bool BaseName>
struct static_reference_type_name_base
: static_ptr_ref_type_name_base<
	MetaType, 
	BaseName, 
	BOOST_STR_LIT(' '),
	BOOST_STR_LIT('&')
>{ };

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

