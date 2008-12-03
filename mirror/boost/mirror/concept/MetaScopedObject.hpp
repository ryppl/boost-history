/**
 * \file boost/mirror/concept/MetaScopedObject.hpp
 *
 * Meta function checking whether a given type satisfies the 
 * requirements of the MetaScopedObject concept
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_CONCEPT_META_SCOPED_OBJECT_HPP
#define BOOST_MIRROR_CONCEPT_META_SCOPED_OBJECT_HPP

#include <boost/mirror/concept/MetaObject.hpp>

namespace boost {
namespace mirror {

template <typename T>
struct is_MetaScopedObject 
{
	BOOST_MIRROR_CONCEPT_TESTER_BEGIN

	template <class X> 
	static match test(
		X*, 
		// check for scope typedef
		typename X::scope* _x = 0
	);

	typedef is_MetaObject<T> other_constraints;

	BOOST_MIRROR_CONCEPT_TESTER_END

};


} // namespace mirror
} // namespace boost

#endif //include guard

