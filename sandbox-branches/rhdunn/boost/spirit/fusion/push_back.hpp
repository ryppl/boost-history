/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_PUSH_BACK_HPP)
#define BOOST_FUSION_PUSH_BACK_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/spirit/fusion/single_view.hpp>
#include <boost/spirit/fusion/joint_view.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  push_back
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ViewT, typename T>
    struct push_back_result
    {
        typedef joint_view<ViewT, typename make_single_view<T>::type> type;
    };

    template <typename ViewT, typename T>
    inline typename push_back_result<ViewT, T>::type
    push_back(ViewT const& view, T const& x)
    {
        typedef typename make_single_view<T>::type insert_type;
        typedef joint_view<ViewT, insert_type> result;
        return result(view, insert_type(x));
    }

}} // namespace boost::fusion

#endif

