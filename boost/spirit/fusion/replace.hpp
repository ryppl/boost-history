/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_REPLACE_HPP)
#define BOOST_FUSION_REPLACE_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/spirit/fusion/single_view.hpp>
#include <boost/spirit/fusion/joint_view.hpp>
#include <boost/spirit/fusion/slice_view.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  replace
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ViewT, typename PositionT, typename T>
    struct replace_result
    {
        BOOST_STATIC_ASSERT((
            !is_same<PositionT, typename ViewT::end_type>::value));

        typedef typename make_single_view<T>::type insert_type;

        typedef
            slice_view<typename ViewT::begin_type, PositionT>
        left_type;

        typedef
            slice_view<
                typename next_iterator<PositionT>::type
              , typename ViewT::end_type>
        right_type;

        typedef joint_view<left_type, insert_type> left_replace_type;
        typedef joint_view<left_replace_type, right_type> type;
    };

    template <typename ViewT, typename PositionT, typename T>
    inline typename replace_result<ViewT, PositionT, T>::type
    replace(ViewT const& view, PositionT const& pos, T const& x)
    {
        typedef replace_result<ViewT, PositionT, T> replacer;

        typedef typename replacer::left_type left_type;
        typedef typename replacer::right_type right_type;
        typedef typename replacer::left_replace_type left_replace_type;
        typedef typename replacer::insert_type insert_type;
        typedef typename replacer::type result;

        left_type left(view.begin(), pos);
        right_type right(next(pos), view.end());
        left_replace_type left_replace(left, insert_type(x));

        return result(left_replace, right);
    }

}} // namespace boost::fusion

#endif

