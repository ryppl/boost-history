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

	template <class FullOrBase, typename CharT>
	class append_arg_typename
	{
	private:
		typedef type_name_decorator_literals<CharT> lits;
	public:
		template <int I>
		inline void operator()(::boost::mpl::identity< ::boost::mpl::int_<I> >)
		{
			static ::std::basic_string<CharT> comma(lits::get(lits::comma()));
			//TODO: list.append();
			list.append(comma);
		}

		template <typename T>
		inline void operator()(::boost::mpl::identity<T>)
		{
			static ::std::basic_string<CharT> comma(lits::get(lits::comma()));
			list.append(BOOST_MIRRORED_TYPE(T)::get_name(
				FullOrBase(),
				::std::char_traits<CharT>()
			));
			list.append(comma);
		}

		inline append_arg_typename(::std::basic_string<CharT>& _list)
		: list(_list)
		{ }
	private:
		::std::basic_string<CharT>& list;
	};

	template <
		typename ArgTypeList, 
		class FullOrBase, 
		typename CharT,
		class Size
	>
	static void do_append_args(
		ArgTypeList*, 
		::std::basic_string<CharT>& str, 
		FullOrBase full_or_base,
		::std::char_traits<CharT> _cht,
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
		>(append_arg_typename<FullOrBase, CharT>(str));
		//
		// append the last argument
		typedef typename mpl::back<ArgTypeList>::type last_arg_type;
		str.append(BOOST_MIRRORED_TYPE(last_arg_type)::get_name(
			full_or_base, 
			_cht
		));
	}

	// overload for empty argument list
	template <typename ArgTypeList, class FullOrBase, typename CharT>
	inline static void do_append_args(
		ArgTypeList*, 
		::std::basic_string<CharT>& str, 
		FullOrBase,
		::std::char_traits<CharT>,
		mpl::int_<0>
	)
	{ }
protected:
	template <typename ArgTypeList, class FullOrBase, typename CharT>
	inline static void append_args(
		ArgTypeList*, 
		::std::basic_string<CharT>& str, 
		FullOrBase full_or_base,
		::std::char_traits<CharT> cht
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
			cht,
			size
		);
	}
};

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

