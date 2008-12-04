/**
 * \file boost/mirror/concept/MetaClassAttribute.hpp
 *
 * Meta function checking whether a given type satisfies the 
 * requirements of the MetaClassAttribute concept
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_CONCEPT_META_CLASS_ATTRIBUTE_HPP
#define BOOST_MIRROR_CONCEPT_META_CLASS_ATTRIBUTE_HPP

#include <boost/mirror/concept/MetaVariable.hpp>
#include <boost/mirror/concept/MetaClassMember.hpp>
#include <boost/mirror/concept/MetaObjectSequenceElement.hpp>

namespace boost {
namespace mirror {


template <typename T>
struct is_MetaClassAttribute
{
        BOOST_MIRROR_CONCEPT_TESTER_BEGIN

        template <class X>
        static match test(
                X*,
		typename X::type* x1 = 0,
		typename X::traits* x2 = 0,
                const typename X::type::reflected_type* _y1 = &X::get(
			*((typename X::scope::reflected_type*)0)
		)
        );

        typedef typename mpl::and_<
		typename is_MetaClassMember<T>::type,
		typename is_MetaVariable<T>::type,
		typename is_MetaObjectSequenceElement<T>::type
	>::type other_constraints;

        BOOST_MIRROR_CONCEPT_TESTER_END

};



} // namespace mirror
} // namespace boost

#endif //include guard

