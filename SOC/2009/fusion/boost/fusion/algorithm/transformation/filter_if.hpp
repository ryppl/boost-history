/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_FILTER_IF_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_FILTER_IF_HPP

#include <boost/config.hpp>
#include <boost/fusion/view/filter_view/filter_view.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/support/internal/workaround.hpp>
#include <boost/mpl/bool.hpp>
#ifdef BOOST_NO_RVALUE_REFERENCES
#   include <boost/detail/workaround.hpp>
#   if defined(BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS) ||\
        BOOST_WORKAROUND(__GNUC__,<4)
#       include <boost/type_traits/is_const.hpp>
#       include <boost/utility/enable_if.hpp>
#   endif
#endif

namespace boost { namespace fusion
{
    namespace result_of
    {
        template<
            typename Seq
          , typename Pred
          , typename PredIsMetafunction=mpl::true_
        >
        struct filter_if
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

            typedef filter_view<Seq, Pred, PredIsMetafunction> type;
        };
    }
    
    template<typename Pred, typename Seq>
    inline typename
        result_of::filter_if<BOOST_FUSION_R_ELSE_CLREF(Seq), Pred>::type
    filter_if(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return typename result_of::filter_if<
            BOOST_FUSION_R_ELSE_CLREF(Seq), Pred
        >::type(BOOST_FUSION_FORWARD(Seq,seq));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template<typename Pred, typename Seq>
    inline BOOST_FUSION_EXPLICIT_TEMPLATE_NON_CONST_ARG_OVERLOAD(
            result_of::filter_if<,Seq,&, Pred>)
    filter_if(Seq& seq)
    {
        return typename result_of::filter_if<Seq&, Pred>::type(seq);
    }
#endif

    template<typename Pred, typename PredIsMetafunction, typename Seq>
    inline typename result_of::filter_if<
        BOOST_FUSION_R_ELSE_CLREF(Seq)
      , Pred
      , PredIsMetafunction
    >::type
    filter_if(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return typename result_of::filter_if<
            BOOST_FUSION_R_ELSE_CLREF(Seq)
          , Pred
          , PredIsMetafunction
        >::type(BOOST_FUSION_FORWARD(Seq,seq));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template<typename Pred, typename PredIsMetafunction, typename Seq>
#   if defined(BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS) ||\
        BOOST_WORKAROUND(__GNUC__,<4)
    inline typename lazy_disable_if<
        is_const<Seq>
      , result_of::filter_if<Seq&,Pred,PredIsMetafunction>
    >::type
#   else
    inline typename result_of::filter_if<Seq&,Pred,PredIsMetafunction>::type
#   endif
    filter_if(Seq& seq)
    {
        return typename result_of::filter_if<
            Seq&
          , Pred
          , PredIsMetafunction
        >::type(seq);
    }
#endif
}}

#endif
