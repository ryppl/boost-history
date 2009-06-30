/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ITERATOR_DEREF_HPP
#define BOOST_FUSION_ITERATOR_DEREF_HPP

#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/support/tag_of.hpp>

namespace boost { namespace fusion
{
    // Special tags:
    struct iterator_facade_tag; // iterator facade tag

    namespace extension
    {
        template <typename Tag>
        struct deref_impl;

        template <>
        struct deref_impl<iterator_facade_tag>
        {
            template <typename Iterator>
            struct apply
              : detail::remove_reference<Iterator>::type::template deref<Iterator>
            {};
       };
    }

    namespace result_of
    {
        template <typename Iterator>
        struct deref
          : extension::deref_impl<typename traits::tag_of<Iterator>::type>::
                template apply<typename detail::add_lref<Iterator>::type>
        {};
    }

    template <typename Iterator>
    typename result_of::deref<Iterator const&>::type
    deref(Iterator const& i)
    {
        return result_of::deref<Iterator const&>::call(i);
    }

    template <typename Iterator>
    typename result_of::deref<Iterator const&>::type
    operator*(iterator_base<Iterator> const& i)
    {
        return fusion::deref(i.cast());
    }
}}

#endif
