/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_COUNT_IF_HPP)
#define BOOST_FUSION_COUNT_IF_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/spirit/fusion/filter_view.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  count_if
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename PredT, typename ViewT>
    struct count_if_result :
        distance<
            typename filter_view<ViewT, PredT>::begin_type
          , typename filter_view<ViewT, PredT>::end_type
        >
    {};

    template <typename PredT, typename ViewT>
    inline typename count_if_result<PredT, ViewT>::type
    count_if(ViewT const& view)
    {
        typedef typename count_if_result<PredT, ViewT>::type result;
        return result();
    }

}} // namespace boost::fusion

#endif

