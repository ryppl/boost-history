/**
 * \file boost/mirror/concept/MetaNamedScopedObject.hpp
 *
 * Meta function checking whether a given type satisfies the 
 * requirements of the MetaNamedObject concept
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_CONCEPT_META_NAMED_SCOPED_OBJECT_HPP
#define BOOST_MIRROR_CONCEPT_META_NAMED_SCOPED_OBJECT_HPP

#include <boost/mirror/concept/MetaNamedObject.hpp>
#include <boost/mirror/concept/MetaScopedObject.hpp>

namespace boost {
namespace mirror {


template <typename T>
struct is_MetaNamedScopedObject
{
        BOOST_MIRROR_CONCEPT_TESTER_BEGIN

        template <class X>
        static match test(
                X*,
                // check for full_name member function
                const ::boost::cts::bstring* _y1 = &X::full_name()
        );

        typedef typename mpl::and_<
		typename is_MetaScopedObject<T>::type,
		typename is_MetaNamedObject<T>::type
	>::type other_constraints;

        BOOST_MIRROR_CONCEPT_TESTER_END

};



} // namespace mirror
} // namespace boost

#endif //include guard

