/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_FILTER_VIEW_HPP)
#define BOOST_FUSION_FILTER_VIEW_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  filter_iterator
    //
    //      Transforms an iterator's dereference result given a function
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename FirstT, typename LastT, typename PredT>
    struct filter_iterator;

    namespace tuple_detail
    {
        template <typename IterT, typename PredT>
        struct apply_filter
        {
            typedef typename
                mpl::apply1<PredT
                  , typename remove_reference<
                        typename deref_iterator<IterT>::type>::type
                >::type
            type;
        };

        template <typename FirstT, typename LastT, typename PredT>
        struct filtered_next_
        {
            typedef typename FirstT::type first;

            typedef
                mpl::or_<is_same<first, LastT>, apply_filter<first, PredT> >
            filter;

            typedef
                filtered_next_<next_iterator<first>, LastT, PredT>
            next_result;

            typedef typename
                mpl::if_<filter, FirstT, next_result>::type
            result;

            typedef typename result::type type;

            template <typename IterT>
            static type
            get(IterT const& iter, mpl::true_)
            {
                return iter;
            };

            template <typename IterT>
            static type
            get(IterT const& iter, mpl::false_)
            {
                return next_result::get(next(iter));
            };

            static type
            get(first const& iter)
            {
                return get(iter, filter());
            }
        };

        template <typename FirstT, typename LastT, typename PredT>
        struct filtered_next
        {
            typedef
                filtered_next_<
                    mpl::identity<FirstT>
                  , LastT
                  , typename mpl::lambda<PredT>::type
                >
            result;

            typedef typename result::type type;

            static type
            get(FirstT const& iter)
            {
                return result::get(iter);
            }
        };

        template <typename FirstT, typename LastT, typename PredT>
        struct next_filter_iterator
        {
            typedef filtered_next_<
                next_iterator<FirstT>
              , LastT
              , typename mpl::lambda<PredT>::type>
            result;
            typedef filter_iterator<typename result::type, LastT, PredT> type;

            static type
            get(FirstT const& i)
            {
                return type(result::get(next(i)));
            }
        };

        template <typename FirstT, typename LastT, typename PredT>
        struct deref_filter_iterator
        {
            typedef typename deref_iterator<FirstT>::type type;
        };
    }

    template <typename FirstT, typename LastT, typename PredT>
    struct filter_iterator : iterator<filter_iterator<FirstT, LastT, PredT> >
    {
        typedef tuple_detail::
            filtered_next<FirstT, LastT, PredT>
        begin_filter;

        typedef typename begin_filter::type begin_type;
        typedef LastT end_type;
        typedef PredT predicate_type;

        typedef typename tuple_detail::
            deref_filter_iterator<begin_type, LastT, PredT> deref_type;
        typedef typename tuple_detail::
            next_filter_iterator<begin_type, LastT, PredT> next_type;

        filter_iterator(FirstT const& iter)
            : iter(begin_filter::get(iter)) {}

        template <typename IterT>
        static typename next_iterator<IterT>::type
        next(IterT const& i)
        {
            return next_type::get(i.iter);
        }

        template <typename IterT>
        static typename deref_iterator<IterT>::type
        deref(IterT const& i)
        {
            return *i.iter;
        }

        begin_type iter;
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  filter_view
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ViewT, typename PredT>
    struct filter_view
    {
        typedef filter_iterator<
            typename ViewT::begin_type
          , typename ViewT::end_type
          , PredT> begin_type;

        typedef filter_iterator<
            typename ViewT::end_type
          , typename ViewT::end_type
          , PredT> end_type;

        filter_view(ViewT const& view)
            : view(view) {}

        begin_type
        begin() const
        {
            return begin_type(view.begin());
        }

        end_type
        end() const
        {
            return end_type(view.end());
        }

        ViewT view;
    };

}} // namespace boost::fusion

#endif
