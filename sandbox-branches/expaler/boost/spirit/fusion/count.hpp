/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_COUNT_HPP)
#define BOOST_FUSION_COUNT_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/spirit/fusion/filter_view.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  count
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename ViewT>
    struct count_result : distance<
            typename filter_view<ViewT, is_same<mpl::_, T> >::begin_type
          , typename filter_view<ViewT, is_same<mpl::_, T> >::end_type
        >
    {};

    template <typename T, typename ViewT>
    inline typename count_result<T, ViewT>::type
    count(ViewT const& view)
    {
        typedef typename count_result<T, ViewT>::type result;
        return result();
    }

}} // namespace boost::fusion

#endif

