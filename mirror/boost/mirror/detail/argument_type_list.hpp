/**
 * \file boost/mirror/detail/argument_type_list.hpp
 *
 * Helpers for composing function and template arg lists
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_ARGUMENT_TYPE_LIST_HPP
#define BOOST_MIRROR_META_DETAIL_ARGUMENT_TYPE_LIST_HPP

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/remove_if.hpp>
//
#include <boost/mirror/meta_data_fwd.hpp>
#include <boost/mirror/detail/decorated_type_name.hpp>

namespace boost {
namespace mirror {
namespace detail {

/** template meta-function that can be used to distinguish "null" 
 *  template param types
 */
template <typename T>
struct is_typelist_null_type : ::boost::false_type { };

template <typename ArgTypeList>
struct template_args_type_list_wo_nulls
{
	typedef typename ::boost::mpl::remove_if<
			ArgTypeList,
			is_typelist_null_type< ::boost::mpl::_ >
	>::type type;
};

/** Helper class implementing a function that can be used 
 *  to append the list of type names to the given string.
 */
struct argument_type_list_builder
{
private:
	template <typename T>
	struct to_identity
	{
		typedef mpl::identity<T> type;
	};

	template <class FullOrBase>
	class append_arg_typename
	{
	public:
		template <typename T>
		inline void operator()(::boost::mpl::identity<T>)
		{
			static cts::bstring comma(BOOST_CTS_LIT(", "));
			list.append(BOOST_MIRRORED_TYPE(T)::get_name(FullOrBase()));
			list.append(comma);
		}

		inline append_arg_typename(cts::bstring& _list)
		: list(_list)
		{ }
	private:
		cts::bstring& list;
	};

	template <typename ArgTypeList, class FullOrBase, class Size>
	static void do_append_args(
		ArgTypeList*, 
		cts::bstring& str, 
		FullOrBase full_or_base,
		Size
	)
	{
		// append type names and a comma for all
		// arguments bar the last
		//
		// transform the typelist
		typedef typename mpl::transform<
			typename mpl::pop_back<
				ArgTypeList
			>::type,
			to_identity<mpl::_>
		>:: type args_wo_last;
		// 
		// call the functor
		::boost::mpl::for_each<
			args_wo_last
		>(append_arg_typename<FullOrBase>(str));
		//
		// append the last argument
		typedef typename mpl::back<ArgTypeList>::type last_arg_type;
		str.append(BOOST_MIRRORED_TYPE(last_arg_type)::get_name(full_or_base));
	}

	// overload for empty argument list
	template <typename ArgTypeList, class FullOrBase>
	inline static void do_append_args(
		ArgTypeList*, 
		cts::bstring& str, 
		FullOrBase,
		mpl::int_<0>
	)
	{ }
protected:
	template <typename ArgTypeList, class FullOrBase>
	inline static void append_args(
		ArgTypeList*, 
		cts::bstring& str, 
		FullOrBase full_or_base
	)
	{
		// remove "null" types from the typelist
		typedef typename template_args_type_list_wo_nulls<
			ArgTypeList
		>::type arg_type_list;
		arg_type_list* atlp(0);
		//
		// calculate the size
		typename ::boost::mpl::int_<
			::boost::mpl::size<arg_type_list>::value
		>::type size;
		//
		do_append_args(
			atlp, 
			str, 
			full_or_base,
			size
		);
	}
};

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

