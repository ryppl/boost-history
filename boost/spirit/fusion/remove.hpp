/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_REMOVE_HPP)
#define BOOST_FUSION_REMOVE_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/spirit/fusion/single_view.hpp>
#include <boost/spirit/fusion/joint_view.hpp>
#include <boost/spirit/fusion/slice_view.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  remove
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ViewT, typename PositionT>
    struct remove_result
    {
        BOOST_STATIC_ASSERT((
            !is_same<PositionT, typename ViewT::end_type>::value));

        typedef
            slice_view<typename ViewT::begin_type, PositionT>
        left_type;

        typedef
            slice_view<
                typename next_iterator<PositionT>::type
              , typename ViewT::end_type>
        right_type;

        typedef joint_view<left_type, right_type> type;
    };

    template <typename ViewT, typename PositionT>
    inline typename remove_result<ViewT, PositionT>::type
    remove(ViewT const& view, PositionT const& pos)
    {
        typedef remove_result<ViewT, PositionT> remover;

        typedef typename remover::left_type left_type;
        typedef typename remover::right_type right_type;
        typedef typename remover::type result;

        left_type left(view.begin(), pos);
        right_type right(next(pos), view.end());
        return result(left, right);
    }

}} // namespace boost::fusion

#endif

