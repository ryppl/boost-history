/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_TRANSFORM_HPP)
#define BOOST_FUSION_TRANSFORM_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/spirit/fusion/transform_view.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  transform
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ViewT, typename F>
    struct transform_result
    {
        typedef transform_view<ViewT, F> type;
    };

    template <typename ViewT, typename F>
    inline transform_view<ViewT, F>
    transform(ViewT const& view, F const& f)
    {
        return transform_view<ViewT, F>(view, f);
    }

}} // namespace boost::fusion

#endif

