/**
 * \file boost/mirror/concept/MetaScope.hpp
 *
 * Meta function checking whether a given type satisfies the 
 * requirements of the MetaScope concept
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_CONCEPT_META_SCOPE_HPP
#define BOOST_MIRROR_CONCEPT_META_SCOPE_HPP

#include <boost/mirror/concept/MetaNamedObject.hpp>

namespace boost {
namespace mirror {


template <typename T>
struct is_MetaScope
{
        BOOST_MIRROR_CONCEPT_TESTER_BEGIN

        template <class X>
        static match test(
                X*
        );

        typedef typename is_MetaNamedObject<T>::type
		other_constraints;

        BOOST_MIRROR_CONCEPT_TESTER_END

};



} // namespace mirror
} // namespace boost

#endif //include guard

