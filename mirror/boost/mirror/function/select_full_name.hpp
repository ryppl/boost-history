/**
 * \file boost/mirror/function/select_full_name.hpp
 * Functor that returns the full_name when given an 
 * appropriate meta-object.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_FUNCTION_SELECT_FULL_NAME_HPP
#define BOOST_MIRROR_FUNCTION_SELECT_FULL_NAME_HPP

namespace boost {
namespace mirror {

struct select_full_name
{
	template <class MetaObject>
	inline const cts::bchar* operator()(MetaObject) const
	{
		return MetaObject::full_name();
	}
};

} // namespace mirror
} // namespace boost

#endif //include guard

