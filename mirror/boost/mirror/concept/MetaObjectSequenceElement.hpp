/**
 * \file boost/mirror/concept/MetaObjectSequenceElement.hpp
 *
 * Meta function checking whether a given type satisfies the 
 * requirements of the MetaObjectSequenceElement concept
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_CONCEPT_META_OBJECT_SEQUENCE_ELEMENT_HPP
#define BOOST_MIRROR_CONCEPT_META_OBJECT_SEQUENCE_ELEMENT_HPP

#include <boost/mirror/concept/MetaObject.hpp>

namespace boost {
namespace mirror {


template <typename T>
struct is_MetaObjectSequenceElement
{
        BOOST_MIRROR_CONCEPT_TESTER_BEGIN

        template <class X>
        static match test(
                X*,
		typename X::container* _x1 = 0,
		typename X::position* _x2 = 0
        );

        typedef typename is_MetaObject<T>::type
		other_constraints;

        BOOST_MIRROR_CONCEPT_TESTER_END

};



} // namespace mirror
} // namespace boost

#endif //include guard

