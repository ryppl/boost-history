/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_FIND_HPP)
#define BOOST_FUSION_FIND_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/spirit/fusion/filter_view.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  find
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename ViewT>
    struct find_result
    {
        typedef typename
            tuple_detail::filtered_next<
                typename ViewT::begin_type
              , typename ViewT::end_type
              , is_same<mpl::_, T>
            >::type
        type;
    };

    template <typename T, typename ViewT>
    inline typename find_result<T, ViewT>::type
    find(ViewT const& view)
    {
        typedef
            tuple_detail::filtered_next<
                typename ViewT::begin_type
              , typename ViewT::end_type
              , is_same<mpl::_, T>
            >
        filter;

        return filter::get(view.begin());
    }

}} // namespace boost::fusion

#endif

