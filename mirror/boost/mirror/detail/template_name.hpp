/**
 * \file boost/mirror/detail/template_name.hpp
 * Helpers for composing a template name
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_TEMPLATE_NAME_HPP
#define BOOST_MIRROR_META_DETAIL_TEMPLATE_NAME_HPP

#include <assert.h>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>

#include <boost/mirror/detail/nontrivial_type_name.hpp>

namespace boost {
namespace mirror {
namespace detail {

template <typename base_type>
struct get_base_type_name_length_type
{
	typedef typename mpl::int_<
		BOOST_MIRROR_REFLECT_TYPE(base_type)::base_name_length
	> type;
};

/** The length is calculated assuming that the string is
 *  going to be formatted like this:
 *  template_name|< |T1|, |T2|, |...|Tn| > (without the 
 *  delimiting '|'s.
 */
template <class meta_type, class typelist, int template_name_length>
struct static_template_name_length
{
	typedef typename mpl::accumulate<
		typelist,
		mpl::int_<template_name_length+2>,
		mpl::plus<
			mpl::_1,
			mpl::plus<
				get_base_type_name_length_type<mpl::_2>,
				mpl::int_<2>
			>
		>
	>::type type;
};


template <class meta_type, class typelist, int template_name_length>
struct static_template_name_base
{
	BOOST_MIRROR_CONST_MEMBER_ATTRIB(
		size_t,
		base_name_length,
		(static_template_name_length<
			meta_type, 
			typelist,
			template_name_length
		>::type::value)
	)

protected:
	/** The 'position' of the last type in the template
	 *  type list.
	 */
	typedef typename mpl::int_<
		mpl::size< typelist	>::value - 1
	> last_type_pos;

	template <int I>
	static bchar* do_append_type_name(bchar* cur_pos, mpl::int_<I>)
	{
		typedef typename mpl::at<typelist, mpl::int_<I> >::type type;
		typedef BOOST_MIRROR_REFLECT_TYPE(type) meta_type;
		bstrcpy(cur_pos, meta_type::base_name());
		cur_pos += meta_type::base_name_length;
		return cur_pos;
	}

	static bchar* append_type_name(bchar* cur_pos, mpl::int_<0> type_pos)
	{
		return do_append_type_name(cur_pos, type_pos);
	}

	template <int I>
	static bchar* append_type_name(bchar* cur_pos, mpl::int_<I> type_pos)
	{
		cur_pos = append_type_name(cur_pos, mpl::int_<I - 1>());
		bstrcpy(cur_pos, BOOST_STR_LIT(", "));
		cur_pos += 2;
		return do_append_type_name(cur_pos, type_pos);
	}

	static void init_base_name(bchar* the_base_name)
	{
		bchar* cur_pos = the_base_name;
		//
		// copy the name of the template
		bstrcpy(cur_pos, meta_type::template_base_name());
		cur_pos += template_name_length;
		//
		// append the leading "< "
		assert(cur_pos+2 < (the_base_name + base_name_length));
		bstrcpy(cur_pos, BOOST_STR_LIT("< "));
		cur_pos += 2;
		//
		// append the typenames
		cur_pos = append_type_name(cur_pos, last_type_pos());
		//
		// append the final " >"
		assert(cur_pos+2 == (the_base_name + base_name_length));
		bstrcpy(cur_pos, BOOST_STR_LIT(" >"));
		cur_pos += 2;
		//
		// finalize the string
		assert(cur_pos == (the_base_name + base_name_length));
		*cur_pos = BOOST_STR_LIT('\0');
	}
};

template <class meta_type, class typelist, int template_name_length>
struct static_template_name : static_nontrivial_type_name<
	static_template_name_base<meta_type, typelist, template_name_length>
>{ };


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

