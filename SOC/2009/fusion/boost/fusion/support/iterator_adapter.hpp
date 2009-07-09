// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_ITERATOR_ADAPTER_HPP
#define BOOST_FUSION_SUPPORT_ITERATOR_ADAPTER_HPP

#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/iterator_base.hpp>

#include <boost/fusion/support/detail/iterator_adapter/deref_impl.hpp>
#include <boost/fusion/support/detail/iterator_adapter/value_of_impl.hpp>
#include <boost/fusion/support/detail/iterator_adapter/next_impl.hpp>
#include <boost/fusion/support/detail/iterator_adapter/prior_impl.hpp>
#include <boost/fusion/support/detail/iterator_adapter/equal_to_impl.hpp>
#include <boost/fusion/support/detail/iterator_adapter/distance_impl.hpp>
#include <boost/fusion/support/detail/iterator_adapter/advance_impl.hpp>

namespace boost { namespace fusion
{
    struct iterator_adapter_tag;

    template<typename It, typename NewCategory>
    struct iterator_adapter
      : iterator_base<iterator_adapter<It, NewCategory> >
    {
        typedef iterator_adapter_tag fusion_tag;
        typedef NewCategory category;
        typedef It iterator_type;

        template<typename OtherIt>
        iterator_adapter(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
          : it(BOOST_FUSION_FORWARD(OtherIt,it).it)
        {}

        iterator_adapter(It const& it, int)
          : it(it)
        {}

        template<typename OtherIt>
        iterator_adapter&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherIt) other_it)
        {
            it=BOOST_FUSION_FORWARD(OtherIt,other_it).it;
            return *this;
        }

        It it;
    };
}}

#endif
