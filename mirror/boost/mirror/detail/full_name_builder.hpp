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

// char-type related 
#include <boost/char_type_switch/string.hpp>

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
			bstring& _str, 
			mpl::identity<meta_namespace<namespace_::_> >,
			mpl::identity<detail::registered_type_info<Type> >
		)
		{ }

		// append separator to anything else
		template <typename AnyScope, class AnyMO>
		inline static void append_separator(
			bstring& _str, 
			mpl::identity<AnyScope>,
			mpl::identity<AnyMO>
		)
		{
			static const bstring separator(BOOST_STR_LIT("::"));
			_str.append(separator);
		}


		// initializes the full names 
		inline static bstring init_name(mpl::true_ _full)
		{
			bstring res(Scope::get_name(_full));
			append_separator(
				res, 
				mpl::identity<Scope>(),
				mpl::identity<BaseMetaObject>()
			);
			res.append(BaseMetaObject::get_name(mpl::false_()));
			return res;
		}

		// initializes the base names 
		inline static const bstring& init_name(mpl::false_ _base)
		{
			return BaseMetaObject::get_name(_base);
		}
	public:
		// base of full name getter
		template <bool FullName>
		inline static const bstring& build_name(
			mpl::bool_<FullName> full_or_base,
			bstring& left,
			bstring& right,
			bstring& ex
		)
		{
			return get_name(full_or_base);
		}


		// base of full name getter
		template <bool FullName>
		inline static const bstring& get_name(
			mpl::bool_<FullName> full_or_base
		)
		{
			static bstring s_name(init_name(full_or_base));
			return s_name;
		}
	};
	
	
} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

