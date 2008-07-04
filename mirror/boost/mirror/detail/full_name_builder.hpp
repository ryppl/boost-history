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

// forward declarations and common mirror defs
#include <boost/mirror/common_defs.hpp>
#include <boost/mirror/meta_data_fwd.hpp>

// char-type related 
#include <boost/char_type_switch/string.hpp>

namespace boost {
namespace mirror {

namespace detail {
	
	/** Helper template that builds the fully qualified names
	 *  from base names
	 */
	template <class Scope, class BaseMetaObject>
	struct full_name_builder : public BaseMetaObject
	{
	private:
		// initializes the full names 
		inline static bstring init_full_name(void)
		{
			bstring res(Scope::full_name());
			res.append(bstring(BOOST_STR_LIT("::")));
			res.append(bstring(BaseMetaObject::base_name()));
			return res;
		}
	public:
		// full name getter
		inline static const bchar* full_name(void)
		{
			static bstring s_full_name(init_full_name());
			return s_full_name.c_str();
		}
	
		// the full name length
		typedef typename ::boost::mpl::plus<
			typename Scope::full_name_length,
			typename ::boost::mpl::int_<2>::type,
			typename BaseMetaObject::base_name_length
		>::type full_name_length;
	};
	
	
} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

