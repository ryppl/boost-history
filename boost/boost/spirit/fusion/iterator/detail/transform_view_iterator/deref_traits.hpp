/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_ITERATOR_DETAIL_TRANSFORM_VIEW_ITERATOR_DEREF_TRAITS_HPP)
#define FUSION_ITERATOR_DETAIL_TRANSFORM_VIEW_ITERATOR_DEREF_TRAITS_HPP

#include <boost/spirit/fusion/detail/config.hpp>
#include <boost/spirit/fusion/iterator/deref.hpp>

namespace boost { namespace fusion
{
    struct transform_view_iterator_tag;

    namespace transform_view_detail {
        template<typename Iterator>
        struct deref_traits_impl {
            typedef typename
                meta::deref<typename Iterator::first_type>::type
            deref_type;

            typedef typename Iterator::transform_type transform_type;
            typedef typename fusion_apply1<transform_type,deref_type>::type type;

            static type
            call(Iterator const& i);
        };
        
        template<typename Iterator>
        deref_traits_impl<Iterator>::type deref_traits_impl<Iterator>::call(Iterator const& i) 
        {
            return i.f(*i.first);
        }
    }

    namespace meta
    {
        template <typename Tag>
        struct deref_impl;

        template <>
        struct deref_impl<transform_view_iterator_tag>
        {
            template <typename Iterator>
            struct apply : transform_view_detail::deref_traits_impl<Iterator>
            {};
        };
    }
}}

#endif


