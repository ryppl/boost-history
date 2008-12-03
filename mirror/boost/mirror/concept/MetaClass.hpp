/**
 * \file boost/mirror/concept/MetaClass.hpp
 *
 * Meta function checking whether a given type satisfies the 
 * requirements of the MetaClass concept
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_CONCEPT_META_CLASS_HPP
#define BOOST_MIRROR_CONCEPT_META_CLASS_HPP

#include <boost/mirror/concept/MetaType.hpp>
#include <boost/mirror/concept/MetaScope.hpp>

namespace boost {
namespace mirror {

template <typename T>
struct is_MetaClass
{
	BOOST_MIRROR_CONCEPT_TESTER_BEGIN

	template <class X> 
	static match test(
		X*, 
		typename X::base_classes* _x1 = 0,
		typename X::attributes * _x2 = 0,
		typename X::all_attributes * _x3 = 0
	);

	typedef typename mpl::and_<
		typename is_MetaType<T>::type,
		typename is_MetaScope<T>::type
	>::type other_constraints;

	BOOST_MIRROR_CONCEPT_TESTER_END

};


} // namespace mirror
} // namespace boost

#endif //include guard

