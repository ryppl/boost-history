/**
 * \file boost/mirror/detail/function_type_name.hpp
 *
 * Helpers for composing function typenames
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_FUNCTION_TYPE_NAME_HPP
#define BOOST_MIRROR_META_DETAIL_FUNCTION_TYPE_NAME_HPP

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/identity.hpp>
//
#include <boost/mirror/meta_data_fwd.hpp>
#include <boost/mirror/detail/decorated_type_name.hpp>

namespace boost {
namespace mirror {
namespace detail {

template <
	typename RetValType, 
	typename CallingConvention, 
	typename ArgTypeList
>
struct function_type_name_base
{
private:
	template <class FullOrBase>
	class append_arg_typename
	{
	public:
		template <typename T>
		inline void operator()(::boost::mpl::identity<T>)
		{
			static bstring comma(BOOST_STR_LIT(", "));
			list.append(BOOST_MIRRORED_TYPE(T)::get_name(FullOrBase()));
			list.append(comma);
		}

		inline append_arg_typename(bstring& _list)
		: list(_list)
		{ }
	private:
		bstring& list;
	};

	template <typename T>
	struct to_identity
	{
		typedef mpl::identity<T> type;
	};

	template <typename TypeList, class FullOrBase>
	inline static void append_args(
		TypeList*, 
		bstring& str, 
		FullOrBase full_or_base
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
		typedef mpl::back<TypeList>::type last_arg_type;
		str.append(BOOST_MIRRORED_TYPE(last_arg_type)::get_name(full_or_base));
	}

	// overload for empty argument list
	template <class FullOrBase>
	inline static void append_args(mpl::vector<>*, bstring& str, FullOrBase)
	{
		str.append(bstring(BOOST_STR_LIT("void")));
	}

protected:
	template <bool FullName>
	inline static bstring init_name(mpl::bool_<FullName> full_or_base)
	{
		static bstring space(BOOST_STR_LIT(" "));
		static bstring l_par(BOOST_STR_LIT("("));
		static bstring r_par(BOOST_STR_LIT(")"));
		static bstring aster(BOOST_STR_LIT("*"));
		//
		// the return value type
		typedef BOOST_MIRRORED_TYPE(RetValType) meta_RV;
		bstring res(meta_RV::get_name(full_or_base));
		//
		// the calling convention
		res.append(space);
		res.append(l_par);
		res.append(CallingConvention::name());
		res.append(aster);
		res.append(r_par);
		//
		// argument list
		res.append(l_par);
		append_args(((ArgTypeList*)0), res, full_or_base);
		res.append(r_par);
		return res;
	}
};

template <
	typename RetValType, 
	typename CallingConvention, 
	typename ArgTypeList
>
struct function_type_name
: decorated_type_name_finisher<
	function_type_name_base<RetValType, CallingConvention, ArgTypeList>
> { };



} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

