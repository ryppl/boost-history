// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_DEDUCE_HPP
#define BOOST_FUSION_SUPPORT_DEDUCE_HPP

#include <boost/fusion/support/detail/as_fusion_element.hpp>

#include <boost/config.hpp>

namespace boost { namespace fusion { namespace traits
{
    //TODO doc!
#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename T>
    struct deduce
      : detail::as_fusion_element<T>
    {};
#else
    template <typename T>
    struct deduce
    {
        typedef typename detail::as_fusion_element<T>::type element;

        typedef typename
            mpl::if_<
                mpl::and_<
                    detail::is_lrref<T>
                  , is_same<typename detail::identity<T>::type,element>
                >
              , T
              , element
            >::type
        type;
    };
#endif
}}}

#endif
