// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_PP_VECTOR_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_PP_VECTOR_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/container/vector/detail/vector_iterator.hpp>
#include <boost/fusion/container/vector/detail/pp/vector_n_chooser.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/add_const.hpp>

#include <boost/fusion/container/detail/pp/forward_ctor.hpp>

namespace boost { namespace fusion
{
    struct fusion_sequence_tag;

    VARIADIC_TEMPLATE(FUSION_MAX_VECTOR_SIZE)
    struct vector
      : sequence_base<vector<EXPAND_TEMPLATE_ARGUMENTS(FUSION_MAX_VECTOR_SIZE)> >
    {
        typedef typename
            detail::vector_n_chooser<
                EXPAND_TEMPLATE_ARGUMENTS(FUSION_MAX_VECTOR_SIZE)
            >::type
        vector_n;

        typedef typename vector_n::fusion_tag fusion_tag;
        typedef typename vector_n::tag tag;
        typedef typename vector_n::size size;
        typedef typename vector_n::category category;
        typedef typename vector_n::is_view is_view;

        typedef typename vector_n::types types;
        typedef vector<EXPAND_TEMPLATE_ARGUMENTS(FUSION_MAX_VECTOR_SIZE)> self_type;

        vector()
        {}

#define VECTOR_CTOR(COMBINATION,_)\
        vector(self_type COMBINATION vec)\
          : data(BOOST_FUSION_FORWARD(self_type COMBINATION,vec).data)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(VECTOR_CTOR,_);

#undef VECTOR_CTOR

#define BOOST_FUSION_SEQ_NAME vector
#define BOOST_FUSION_MAX_SEQ_SIZE FUSION_MAX_VECTOR_SIZE
        #include <boost/fusion/container/detail/pp/forward_ctor.hpp>
#undef BOOST_FUSION_MAX_SEQ_SIZE
#undef BOOST_FUSION_SEQ_NAME

        template <typename Seq>
        vector&
        operator=(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
        {
            data = BOOST_FUSION_FORWARD(Seq,seq);
            return *this;
        }

        template <typename I>
        typename detail::add_lref<typename mpl::at<types, I>::type>::type
        at_impl(I)
        {
            return data.at_impl(I());
        }

        template <typename I>
        typename detail::add_lref<
            typename add_const<typename mpl::at<types, I>::type>::type
        >::type
        at_impl(I) const
        {
            return data.at_impl(I());
        }

        vector_n data;
    };
}}

#endif
