/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_FILTER_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_FILTER_HPP

#include <boost/fusion/view/filter_view/filter_view.hpp>
#include <boost/fusion/support/ref.hpp>

#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename T>
        struct filter
        {
            typedef filter_view<Seq, is_same<mpl::_, T> > type;
        };
    }

    template <typename T, typename Seq>
    inline typename result_of::filter<BOOST_FUSION_R_ELSE_LREF(Seq), T>::type
    filter(BOOST_FUSION_R_ELSE_LREF(Seq) seq)
    {
        return
            filter_view<
                BOOST_FUSION_R_ELSE_LREF(Seq)
              , is_same<mpl::_, T>
            >(BOOST_FUSION_FORWARD(Seq,seq));
    }
}}

#endif
