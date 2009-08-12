/*=============================================================================
    Copyright (c) 2005 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_LIST_HPP
#define BOOST_FUSION_CONTAINER_LIST_LIST_HPP

#include <boost/fusion/container/list/list_fwd.hpp>
#include <boost/fusion/container/vector/vector.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/ref.hpp>

#include <boost/fusion/container/list/detail/list/begin_impl.hpp>
#include <boost/fusion/container/list/detail/list/end_impl.hpp>
#include <boost/fusion/container/list/detail/list/at_impl.hpp>
#include <boost/fusion/container/list/detail/list/value_at_impl.hpp>
#include <boost/fusion/container/list/detail/list/deref_impl.hpp>
#include <boost/fusion/container/list/detail/list/value_of_impl.hpp>

#include <boost/fusion/container/detail/forward_ctor.hpp>

namespace boost { namespace fusion
{
    struct fusion_sequence_tag;

    VARIADIC_TEMPLATE(FUSION_MAX_LIST_SIZE)
    struct list
      : sequence_base<list<EXPAND_TEMPLATE_ARGUMENTS(FUSION_MAX_LIST_SIZE)> >
    {
        typedef bidirectional_traversal_tag category;
        typedef list_tag fusion_tag;
        typedef fusion_sequence_tag tag;
        typedef mpl::false_ is_view;

        typedef
            vector<EXPAND_TEMPLATE_ARGUMENTS(FUSION_MAX_LIST_SIZE)>
        storage_type;
        typedef typename storage_type::size size;

        list()
        {}

#define LIST_CTOR(COMBINATION,_)\
        list(list COMBINATION list_)\
          : data(BOOST_FUSION_FORWARD(list COMBINATION,list_).data)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(LIST_CTOR,_)

#undef LIST_CTOR

#define BOOST_FUSION_SEQ_NAME list
#define BOOST_FUSION_MAX_SEQ_SIZE FUSION_MAX_LIST_SIZE
#include <boost/fusion/container/detail/forward_ctor.hpp>
#undef BOOST_FUSION_MAX_SEQ_SIZE
#undef BOOST_FUSION_SEQ_NAME

        template <typename Seq>
        list&
        operator=(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
        {
            data = BOOST_FUSION_FORWARD(Seq, seq);
            return *this;
        }

        storage_type& get_data() { return data; }
        storage_type const& get_data() const { return data; }

    private:
        storage_type data;
    };
}}

#endif
