/**
 * \file boost/mirror/concept/MetaType.hpp
 *
 * Meta function checking whether a given type satisfies the 
 * requirements of the MetaType concept
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_CONCEPT_META_TYPE_HPP
#define BOOST_MIRROR_CONCEPT_META_TYPE_HPP

#include <boost/mirror/concept/MetaNamedScopedObject.hpp>

namespace boost {
namespace mirror {

template <typename T>
struct is_MetaType
{
	BOOST_MIRROR_CONCEPT_TESTER_BEGIN

	template <class X> 
	static match test(
		X*, 
		// check for reflected_type typedef
		typename X::reflected_type* _x = 0
	);

	typedef is_MetaNamedScopedObject<T> other_constraints;

	BOOST_MIRROR_CONCEPT_TESTER_END

};


} // namespace mirror
} // namespace boost

#endif //include guard

