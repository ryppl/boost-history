/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_FOR_EACH_HPP)
#define BOOST_FUSION_FOR_EACH_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  for_each
    //
    ///////////////////////////////////////////////////////////////////////////
    namespace tuple_detail
    {
        template <typename FirstT, typename LastT, typename F>
        inline void
        for_each(FirstT const&, LastT const&, F const&, mpl::true_)
        {
        }

        template <typename FirstT, typename LastT, typename F>
        inline void
        for_each(FirstT const& first, LastT const& last, F const& f, mpl::false_)
        {
            f(*first);
            for_each(next(first), last, f
                , is_same<typename next_iterator<FirstT>::type, LastT>());
        }
    }

    template <typename ViewT, typename F>
    struct for_each_result
    {
        typedef void type;
    };

    template <typename ViewT, typename F>
    inline void
    for_each(ViewT const& view, F const& f)
    {
        tuple_detail::for_each(view.begin(), view.end(), f
            , is_same<typename ViewT::begin_type, typename ViewT::end_type>());
    }

}} // namespace boost::fusion

#endif

