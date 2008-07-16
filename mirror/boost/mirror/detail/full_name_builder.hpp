/**
 * \file boost/mirror/detail/full_name_builder.hpp
 * 
 * Helper template that decorates classes having base_name function and
 * a scope meta-object with full_name function and full_name_length
 * constant
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_FULL_NAME_BUILDER_HPP
#define BOOST_MIRROR_META_DETAIL_FULL_NAME_BUILDER_HPP

// template meta programming
#include <boost/mpl/int.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/identity.hpp>


// forward declarations and common mirror defs
#include <boost/mirror/common_defs.hpp>
#include <boost/mirror/meta_data_fwd.hpp>

namespace boost {
namespace mirror {

namespace namespace_ {struct _;}

namespace detail {
	
	/** Helper template that builds the fully qualified names
	 *  from base names
	 */
	template <class Scope, class BaseMetaObject>
	struct full_name_builder : public BaseMetaObject
	{
	private:

		// don't prepend '::' to types on global scope
		template <typename Type>
		inline static void append_separator(
			::std::basic_string<char>& _str, 
			mpl::identity<meta_namespace<namespace_::_> >,
			mpl::identity<detail::registered_type_info<Type> >
		)
		{ }
		// don't prepend '::' to types on global scope
		template <typename Type>
		inline static void append_separator(
			::std::basic_string<wchar_t>& _str, 
			mpl::identity<meta_namespace<namespace_::_> >,
			mpl::identity<detail::registered_type_info<Type> >
		)
		{ }

		// append separator to anything else
		template <typename AnyScope, class AnyMO>
		inline static void append_separator(
			::std::string& _str, 
			mpl::identity<AnyScope>,
			mpl::identity<AnyMO>
		)
		{
			static const ::std::string separator("::");
			_str.append(separator);
		}

		// append separator to anything else
		template <typename AnyScope, class AnyMO>
		inline static void append_separator(
			::std::wstring& _str, 
			mpl::identity<AnyScope>,
			mpl::identity<AnyMO>
		)
		{
			static const ::std::wstring separator(L"::");
			_str.append(separator);
		}


		// initializes the full names
		template <typename CharT>
		inline static ::std::basic_string<CharT> init_name(
			mpl::true_ _full,
			::std::char_traits<CharT> _cht
		)
		{
			::std::basic_string<CharT> res(Scope::get_name(_full, _cht));
			append_separator(
				res, 
				mpl::identity<Scope>(),
				mpl::identity<BaseMetaObject>()
			);
			res.append(BaseMetaObject::get_name(mpl::false_(), _cht));
			return res;
		}

		// initializes the base names 
		template <typename CharT>
		inline static const ::std::basic_string<CharT>& init_name(
			mpl::false_ _base,
			::std::char_traits<CharT> _cht
		)
		{
			return BaseMetaObject::get_name(_base, _cht);
		}
	public:
		// base of full name getter
		template <bool FullName, typename CharT>
		inline static const ::std::basic_string<CharT>& build_name(
			mpl::bool_<FullName> full_or_base,
			::std::basic_string<CharT>& left,
			::std::basic_string<CharT>& right,
			::std::basic_string<CharT>& ex,
			::std::basic_string<CharT>& arg
		)
		{
			return get_name(full_or_base, ::std::char_traits<CharT>());
		}


		// base of full name getter
		template <bool FullName, typename CharT>
		inline static const ::std::basic_string<CharT>& get_name(
			mpl::bool_<FullName> full_or_base,
			::std::char_traits<CharT> _cht
		)
		{
			static ::std::basic_string<CharT> s_name(
				init_name(full_or_base, _cht)
			);
			return s_name;
		}
	};
	
	
} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

