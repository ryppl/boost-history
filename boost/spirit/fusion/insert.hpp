/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_INSERT_HPP)
#define BOOST_FUSION_INSERT_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/spirit/fusion/single_view.hpp>
#include <boost/spirit/fusion/joint_view.hpp>
#include <boost/spirit/fusion/slice_view.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  insert
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ViewT, typename PositionT, typename T>
    struct insert_result
    {
        typedef typename make_single_view<T>::type insert_type;

        typedef
            slice_view<typename ViewT::begin_type, PositionT>
        left_type;

        typedef
            slice_view<PositionT, typename ViewT::end_type>
        right_type;

        typedef joint_view<left_type, insert_type> left_insert_type;
        typedef joint_view<left_insert_type, right_type> type;
    };

    template <typename ViewT, typename PositionT, typename T>
    inline typename insert_result<ViewT, PositionT, T>::type
    insert(ViewT const& view, PositionT const& pos, T const& x)
    {
        typedef insert_result<ViewT, PositionT, T> inserter;

        typedef typename inserter::left_type left_type;
        typedef typename inserter::right_type right_type;
        typedef typename inserter::left_insert_type left_insert_type;
        typedef typename inserter::insert_type insert_type;
        typedef typename inserter::type result;

        left_type left(view.begin(), pos);
        right_type right(pos, view.end());
        left_insert_type left_insert(left, insert_type(x));

        return result(left_insert, right);
    }

}} // namespace boost::fusion

#endif

