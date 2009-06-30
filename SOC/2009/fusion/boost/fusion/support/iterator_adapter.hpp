// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_ITERATOR_ADAPTER_HPP
#define BOOST_FUSION_ITERATOR_ADAPTER_HPP

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

    template<typename Iterator, typename NewCategory>
    struct iterator_adapter
      : iterator_base<iterator_adapter<Iterator, NewCategory> >
    {
        typedef iterator_adapter_tag fusion_tag;
        typedef NewCategory category;
        typedef Iterator iterator_type;

        iterator_adapter(Iterator iterator)
            : iterator(BOOST_FUSION_FORWARD(Iterator,iterator)) {}
        Iterator iterator;
    };
}}

#endif
