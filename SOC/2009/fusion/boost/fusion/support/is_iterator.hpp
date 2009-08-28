/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_IS_ITERATOR_HPP
#define BOOST_FUSION_SUPPORT_IS_ITERATOR_HPP

#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/iterator_base.hpp>

#include <boost/type_traits/is_base_of.hpp>

//TODO doc!!!

namespace boost { namespace fusion
{
    namespace extension
    {
        template<typename>
        struct is_iterator_impl
        {
            template <typename T>
            struct apply
              : is_base_of<iterator_root, typename detail::identity<T>::type>
            {};
        };
    }

    namespace traits
    {
        template <typename T>
        struct is_iterator
          : extension::is_iterator_impl<
                typename fusion::traits::tag_of<T>::type
            >::template apply<T>::type
        {};
    }
}}

#endif
