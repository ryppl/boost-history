/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_SINGLE_VIEW_SINGLE_VIEW_HPP
#define BOOST_FUSION_VIEW_SINGLE_VIEW_SINGLE_VIEW_HPP

#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/sequence_assign.hpp>
#include <boost/fusion/support/detail/as_fusion_element.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/fusion/view/single_view/detail/single_view_fwd.hpp>
#include <boost/fusion/view/single_view/detail/single_view_iterator.hpp>
#include <boost/fusion/view/single_view/detail/begin_impl.hpp>
#include <boost/fusion/view/single_view/detail/end_impl.hpp>
#include <boost/fusion/view/single_view/detail/deref_impl.hpp>
#include <boost/fusion/view/single_view/detail/next_impl.hpp>
#include <boost/fusion/view/single_view/detail/value_of_impl.hpp>
#include <boost/fusion/view/single_view/detail/equal_to_impl.hpp>

namespace boost { namespace fusion
{
    struct single_view_tag;
    struct forward_traversal_tag;
    struct fusion_sequence_tag;

    template <typename T>
    struct single_view
      : sequence_base<single_view<T> >
    {
        typedef single_view_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef forward_traversal_tag category;
        typedef mpl::true_ is_view;
        typedef mpl::int_<1> size;
        typedef T value_type;

        single_view()
          : val()
        {}

#define SINGLE_VIEW_CTOR(COMBINATION,_)\
        single_view(single_view COMBINATION view)\
          : val(BOOST_FUSION_FORWARD(single_view COMBINATION,view).val)\
        {}

        BOOST_FUSION_ALL_CV_REF_COMBINATIONS(SINGLE_VIEW_CTOR,_)

#undef SINGLE_VIEW_CTOR

        template<typename SeqAssign>
        single_view(BOOST_FUSION_R_ELSE_LREF(SeqAssign) seq,
            typename enable_if<
                 is_sequence_assign<BOOST_FUSION_R_ELSE_LREF(SeqAssign)>
            >::type* =NULL)
          : val(fusion::front(seq.get()))
        {}

        template<typename OtherT>
        explicit single_view(BOOST_FUSION_R_ELSE_CLREF(OtherT) val,
            typename disable_if<
                    is_sequence_assign<BOOST_FUSION_R_ELSE_CLREF(OtherT)>
            >::type* =NULL)
          : val(BOOST_FUSION_FORWARD(OtherT,val))
        {}

        template<typename OtherT>
        typename
            disable_if<
                is_sequence_assign<BOOST_FUSION_R_ELSE_CLREF(OtherT)>
              , single_view&
            >::type
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherT) val)
        {
            val=BOOST_FUSION_FORWARD(OtherT,val);
            return *this;
        }

        template<typename SeqAssign>
        typename
            enable_if<
                is_sequence_assign<BOOST_FUSION_R_ELSE_LREF(SeqAssign)>
              , single_view&
            >::type
        operator=(BOOST_FUSION_R_ELSE_LREF(SeqAssign) seq)
        {
            val=fusion::front(seq.get());
            return *this;
        }

        value_type val;
    };

    template <typename T>
    inline single_view<
        typename detail::as_fusion_element<BOOST_FUSION_R_ELSE_CLREF(T)>::type
    >
    make_single_view(BOOST_FUSION_R_ELSE_CLREF(T) v)
    {
        typedef
            single_view<
                typename detail::as_fusion_element<
                    BOOST_FUSION_R_ELSE_CLREF(T)
                >::type
            >
        view;

        return view(BOOST_FUSION_FORWARD(T,v));
    }
}}

#endif
