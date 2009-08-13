// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_ITERATOR_ITERATOR_FACADE_HPP
#define BOOST_FUSION_ITERATOR_ITERATOR_FACADE_HPP

#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/detail/advance.hpp>
#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/support/internal/dummy_tag.hpp>

namespace boost { namespace fusion
{
    struct void_;
    struct iterator_facade_tag;

    namespace extension
    {
        template<typename Tag>
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

        // default implementation
        template <typename ItRef, typename N>
        struct advance
          :  extension::advance_impl<
                 typename detail::get_dummy_tag<ItRef>::type
             >::template apply<ItRef,N>
        {};
    };
}}

#endif
