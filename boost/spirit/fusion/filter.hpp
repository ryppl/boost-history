/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_FILTER_HPP)
#define BOOST_FUSION_FILTER_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/spirit/fusion/filter_view.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  filter
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename PredT, typename ViewT>
    struct filter_result
    {
        typedef filter_view<ViewT, PredT> type;
    };

    template <typename PredT, typename ViewT>
    inline filter_view<ViewT, PredT>
    filter(ViewT const& view)
    {
        return filter_view<ViewT, PredT>(view);
    }

}} // namespace boost::fusion

#endif

