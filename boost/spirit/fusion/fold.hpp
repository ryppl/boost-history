/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_FOLD_HPP)
#define BOOST_FUSION_FOLD_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  fold
    //
    ///////////////////////////////////////////////////////////////////////////
    namespace tuple_detail
    {
        template <typename IteratorT, typename StateT, typename F>
        struct fold_apply
        {
            typedef typename F::template result<
                typename remove_reference<
                    typename deref_iterator<IteratorT>::type
                >::type, StateT
            >::type type;
        };

        template <typename BeginT, typename EndT, typename StateT, typename F>
        struct fold_result;

        template <typename BeginT, typename EndT, typename StateT, typename F>
        struct fold_result_next
        {
            typedef typename
                fold_result<
                    typename next_iterator<BeginT>::type
                  , EndT
                  , typename fold_apply<BeginT, StateT, F>::type
                  , F
                >::type
            type;
        };

        template <typename BeginT, typename EndT, typename StateT, typename F>
        struct fold_result
        {
            typedef typename
                mpl::if_<
                    is_same<BeginT, EndT>
                  , mpl::identity<StateT>
                  , fold_result_next<BeginT, EndT, StateT, F>
                >::type
            result;

            typedef typename result::type type;
        };

        // terminal case
        template <typename BeginT, typename EndT, typename StateT, typename F>
        inline StateT const&
        fold(BeginT const&, EndT const&, StateT const& state, F const&, mpl::true_)
        {
            return state;
        }

        // non-terminal case
        template <typename BeginT, typename EndT, typename StateT, typename F>
        inline typename fold_result<BeginT, EndT, StateT, F>::type
        fold(
            BeginT const& first
          , EndT const& last
          , StateT const& state
          , F const& f
          , mpl::false_)
        {
            return tuple_detail::fold(
                next(first)
              , last
              , f(*first, state)
              , f
              , is_same<typename next_iterator<BeginT>::type, EndT>()
            );
        }
    }

    template <typename ViewT, typename StateT, typename F>
    struct fold_result
    {
        typedef typename
            tuple_detail::fold_result<
                typename ViewT::begin_type
              , typename ViewT::end_type
              , StateT
              , F
            >::type
        type;
    };

    template <typename ViewT, typename StateT, typename F>
    inline typename fold_result<ViewT, StateT, F>::type
    fold(ViewT const& view, StateT const& state, F const& f)
    {
        return tuple_detail::fold(
            view.begin()
          , view.end()
          , state
          , f
          , is_same<typename ViewT::begin_type, typename ViewT::end_type>()
        );
    }

}} // namespace boost::fusion

#endif

