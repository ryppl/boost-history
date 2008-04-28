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
#include <boost/mpl/remove_if.hpp>

#include <boost/mirror/detail/nontrivial_type_name.hpp>

namespace boost {
namespace mirror {
namespace detail {

/** The length is calculated assuming that the string is
 *  going to be formatted like this:
 *  template_name|< |T1|, |T2|, |...|Tn| > (without the 
 *  delimiting '|'s.
 */
template <class typelist, bool base_name>
struct static_template_name_length
{
	template <typename a_type>
	struct get_type_name_length_type
	{
		typedef BOOST_MIRROR_REFLECT_TYPE(a_type) meta_type;
	
		typedef nontrivial_type_base_or_full_name<meta_type, base_name>
			name_info;

		typedef typename mpl::int_<
			name_info::name_length
		> type;
	};

	typedef typename mpl::accumulate<
		typelist,
		mpl::int_<2>,
		mpl::plus<
			mpl::_1,
			mpl::plus<
				get_type_name_length_type<mpl::_2>,
				mpl::int_<2>
			>
		>
	>::type type;
};

/** Specializations of this template
 */

template <typename a_type>
struct is_typelist_null_type : ::boost::false_type { };

template <class full_typelist>
struct template_with_null_args_type_list
{
	/** A typelist that contains all types from full_type_list
	 *  except those that are typelist_null_types
	 */
	typedef typename mpl::remove_if<
		full_typelist,
		is_typelist_null_type<mpl::_1>
	>::type type;
};

template <class meta_type, class full_typelist, bool base_name>
struct static_template_name_base
{
protected:
	typedef typename template_with_null_args_type_list<
		full_typelist
	>::type typelist;

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
		typedef nontrivial_type_base_or_full_name<meta_type, base_name>
			local_name_info;
		bstrcpy(cur_pos, local_name_info::name());
		cur_pos += local_name_info::name_length;
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

	typedef nontrivial_type_base_or_full_name<meta_type, base_name>
		name_info;

	typedef typename static_template_name_length<typelist, base_name>::type
		template_param_list_length_type;

	BOOST_MIRROR_CONST_MEMBER_ATTRIB(
		size_t, 
		difference, 
		template_param_list_length_type::value
	)

	BOOST_MIRROR_CONST_MEMBER_ATTRIB(
		size_t,
		name_length,
		name_info::name_length + difference
	)

	static void init_name(bchar* the_name)
	{
		bchar* cur_pos = the_name;
		//
		// copy the name of the template
		bstrcpy(cur_pos, name_info::name());
		cur_pos += name_info::name_length;
		//
		// append the leading "< "
		assert(cur_pos+2 < (the_name + name_length));
		bstrcpy(cur_pos, BOOST_STR_LIT("< "));
		cur_pos += 2;
		//
		// append the typenames
		cur_pos = append_type_name(cur_pos, last_type_pos());
		//
		// append the final " >"
		bstrcpy(cur_pos, BOOST_STR_LIT(" >"));
		cur_pos += 2;
		//
		// finalize the string
		assert(cur_pos == (the_name + name_length));
		*cur_pos = BOOST_STR_LIT('\0');
	}
};

template <class meta_type, class typelist>
struct static_template_name : static_nontrivial_type_name<
	meta_type, typelist, static_template_name_base
>{ };


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

