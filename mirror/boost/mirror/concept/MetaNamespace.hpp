/**
 * \file boost/mirror/concept/MetaNamespace.hpp
 *
 * Meta function checking whether a given type satisfies the 
 * requirements of the MetaNamespace concept
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_CONCEPT_META_NAMESPACE_HPP
#define BOOST_MIRROR_CONCEPT_META_NAMESPACE_HPP

#include <boost/mirror/concept/MetaNamedScopedObject.hpp>

namespace boost {
namespace mirror {

template <typename T>
struct is_MetaNamespace
{
	BOOST_MIRROR_CONCEPT_TESTER_BEGIN

	template <class X> 
	static match test(
		X*, 
		// check for ancestors typedef
		typename X::ancestors* _x = 0
	);

	typedef typename mpl::and_<
		is_MetaNamedScopedObject<T>,
		is_MetaScope<T>
	>::type other_constraints;

	BOOST_MIRROR_CONCEPT_TESTER_END

};


} // namespace mirror
} // namespace boost

#endif //include guard

