/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_PUSH_FRONT_HPP)
#define BOOST_FUSION_PUSH_FRONT_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/spirit/fusion/single_view.hpp>
#include <boost/spirit/fusion/joint_view.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  push_front
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ViewT, typename T>
    struct push_front_result
    {
        typedef joint_view<typename make_single_view<T>::type, ViewT> type;
    };

    template <typename ViewT, typename T>
    inline typename push_front_result<ViewT, T>::type
    push_front(ViewT const& view, T const& x)
    {
        typedef typename make_single_view<T>::type insert_type;
        typedef joint_view<insert_type, ViewT> result;
        return result(insert_type(x), view);
    }

}} // namespace boost::fusion

#endif

