/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ITERATOR_ITERATOR_FACADE_HPP
#define BOOST_FUSION_ITERATOR_ITERATOR_FACADE_HPP

#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>

namespace boost { namespace fusion
{
    struct void_;
    struct iterator_facade_tag;

    namespace extension
    {
        template<typename>
        struct advance_impl;
    }

    template <typename Derived, typename Category>
    struct iterator_facade
      : iterator_base<Derived>
    {
        typedef iterator_facade_tag fusion_tag;
        typedef Derived derived_type;
        typedef Category category;

        //TODO doc

        template <typename It, typename N>
        struct advance
          :  extension::advance_impl<
                 typename mpl::apply1<mpl::always<void_>,It>::type
             >::template apply<It,N>
        {};
    };
}}

#endif
