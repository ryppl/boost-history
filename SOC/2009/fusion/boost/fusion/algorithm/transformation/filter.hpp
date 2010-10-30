/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_FILTER_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_FILTER_HPP

#include <boost/fusion/support/internal/base.hpp>
#include <boost/fusion/algorithm/transformation/filter_if.hpp>
#include <boost/fusion/support/internal/workaround.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template<typename Seq, typename T>
        struct filter
          : filter_if<Seq, is_same<mpl::_1, T> >
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>))
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>))
        };
    }

    template<typename T, typename Seq>
    inline typename result_of::filter<BOOST_FUSION_R_ELSE_CLREF(Seq), T>::type
    filter(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return filter_if<is_same<mpl::_1, T> >(BOOST_FUSION_FORWARD(Seq,seq));
    }

#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
    template<typename T, typename Seq>
    inline BOOST_FUSION_EXPLICIT_TEMPLATE_NON_CONST_ARG_OVERLOAD(
            result_of::filter<,Seq,&, T>)
    filter(Seq& seq)
    {
        return filter_if<is_same<mpl::_1, T> >(seq);
    }
#endif
}}

#endif
