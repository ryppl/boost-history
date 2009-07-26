// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_DEDUCE_HPP
#define BOOST_FUSION_SUPPORT_DEDUCE_HPP

#include <boost/fusion/support/detail/as_fusion_element.hpp>

#include <boost/config.hpp>
#ifdef BOOST_NO_RVALUE_REFERENCES
#   include <boost/mpl/eval_if.hpp>
#   include <boost/mpl/identity.hpp>
#else
#   include <boost/mpl/if.hpp>
#endif

namespace boost { namespace fusion { namespace traits
{
    //TODO doc!
#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename T>
    struct deduce_impl
    {
        typedef T type;
    };

    template <typename T>
    struct deduce_impl<T const&>
    {
        typedef T type;
    };

    template <typename T>
    struct deduce_impl<T const volatile&>
    {
        typedef T type;
    };

    template <typename T>
    struct deduce
    {
        typedef typename detail::as_fusion_element<T>::type element;

        typedef typename
            mpl::eval_if<
                mpl::and_<
                    detail::is_lrref<T>
                  , is_same<typename detail::identity<T>::type,element>
                >
              , deduce_impl<T>
              , mpl::identity<element>
            >::type
        type;
    };
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
