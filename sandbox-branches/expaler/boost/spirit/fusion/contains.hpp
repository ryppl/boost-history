/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_CONTAINS_HPP)
#define BOOST_FUSION_CONTAINS_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/spirit/fusion/filter_view.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/not.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  contains
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename ViewT>
    struct contains_result
    {
        typedef
            mpl::not_<
                is_same<
                    typename tuple_detail::filtered_next<
                        typename ViewT::begin_type
                      , typename ViewT::end_type
                      , is_same<mpl::_, T>
                    >::type
                  , typename ViewT::end_type
                >
            >
        type;
    };

    template <typename T, typename ViewT>
    inline typename contains_result<T, ViewT>::type
    contains(ViewT const& view)
    {
        typedef typename contains_result<T, ViewT>::type result;
        return result();
    }

}} // namespace boost::fusion

#endif

