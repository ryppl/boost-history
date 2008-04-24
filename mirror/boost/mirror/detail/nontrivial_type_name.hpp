/**
 * \file boost/mirror/detail/nontrivial_type_name.hpp
 * Helpers for composing a nontrivial typenames
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_NONTRIVIAL_TYPE_NAME_HPP
#define BOOST_MIRROR_META_DETAIL_NONTRIVIAL_TYPE_NAME_HPP

namespace boost {
namespace mirror {
namespace detail {

template <class implementation>
struct static_nontrivial_type_name : implementation
{
	static const bchar* base_name(void)
	{
		static bchar the_base_name[implementation::base_name_length+1] 
			= {BOOST_STR_LIT("")};
		if(!the_base_name[0]) 
			implementation::init_base_name(the_base_name);
		return the_base_name;
	}
};

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

