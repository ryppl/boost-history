/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_REPLACE_IF_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_REPLACE_IF_HPP

#include <boost/fusion/view/transform_view/transform_view.hpp>
#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/detail/as_fusion_element.hpp>

#include <boost/type_traits/add_const.hpp>

#include <boost/fusion/algorithm/transformation/detail/replace_if.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename F, typename T>
        struct replace_if
        {
            typedef
                transform_view<
                //TODO ?!
                    typename detail::result_of_forward_as<
                        Seq
                      , typename add_const<
                            typename detail::remove_reference<Seq>::type
                        >::type
                    >::type
                  , detail::replacer_if<
                        typename detail::as_fusion_element<F>::type
                      , typename detail::as_fusion_element<T>::type
                    >
                >
            type;
        };
    }

    template <typename Seq, typename F, typename NewValue>
    inline typename result_of::replace_if<
        BOOST_FUSION_R_ELSE_LREF(Seq)
      , BOOST_FUSION_R_ELSE_LREF(F)
      , BOOST_FUSION_R_ELSE_LREF(NewValue)
    >::type
    replace_if(BOOST_FUSION_R_ELSE_LREF(Seq) seq,
            BOOST_FUSION_R_ELSE_LREF(F) pred,
            BOOST_FUSION_R_ELSE_LREF(NewValue) new_value)
    {
        typedef typename
            result_of::replace_if<
                BOOST_FUSION_R_ELSE_LREF(Seq)
              , BOOST_FUSION_R_ELSE_LREF(F)
              , BOOST_FUSION_R_ELSE_LREF(NewValue)
            >::type
        type;
        typedef
            detail::replacer_if<
                typename detail::as_fusion_element<
                    BOOST_FUSION_R_ELSE_LREF(F)
                >::type
              , typename detail::as_fusion_element<
                    BOOST_FUSION_R_ELSE_LREF(NewValue)
                >::type
            >
        replacer;

        return type(BOOST_FUSION_FORWARD(Seq,seq),
                replacer(BOOST_FUSION_FORWARD(F,pred),
                        BOOST_FUSION_FORWARD(NewValue,new_value)));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Seq, typename F, typename NewValue>
    inline typename result_of::replace_if<
        Seq&
      , F&
      , NewValue const&
    >::type
    replace_if(Seq& seq,
            F& pred,
            NewValue const& new_value)
    {
        typedef typename
            result_of::replace_if<
                Seq&
              , F&
              , NewValue const&
            >::type
        type;
        typedef
            detail::replacer_if<
                typename detail::as_fusion_element<F&>::type
              , typename detail::as_fusion_element<NewValue const&>::type
            >
        replacer;

        return type(seq, replacer(pred, new_value));
    }
#endif
}}

#endif
