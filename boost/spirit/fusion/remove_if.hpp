/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_REMOVE_IF_HPP)
#define BOOST_FUSION_REMOVE_IF_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/spirit/fusion/filter_view.hpp>
#include <boost/mpl/not.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  remove_if
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename PredT, typename ViewT>
    struct remove_if_result
    {
        typedef filter_view<ViewT, mpl::not_<PredT> > type;
    };

    template <typename PredT, typename ViewT>
    inline filter_view<ViewT, mpl::not_<PredT> >
    remove_if(ViewT const& view)
    {
        return filter_view<ViewT, mpl::not_<PredT> >(view);
    }

}} // namespace boost::fusion

#endif

