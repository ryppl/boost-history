/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_APPEND_HPP)
#define BOOST_FUSION_APPEND_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/spirit/fusion/joint_view.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  append
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename View1, typename View2>
    struct append_result
    {
        typedef joint_view<View1, View2> type;
    };

    template <typename View1, typename View2>
    inline joint_view<View1, View2>
    append(View1 const& view1, View2 const& view2)
    {
        return joint_view<View1, View2>(view1, view2);
    }

}} // namespace boost::fusion

#endif

