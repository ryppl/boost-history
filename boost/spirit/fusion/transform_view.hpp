/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_TRANSFORM_VIEW_HPP)
#define BOOST_FUSION_TRANSFORM_VIEW_HPP

#include <boost/spirit/fusion/iterator.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  transform_iterator
    //
    //      Transforms an iterator's dereference result given a function
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename IteratorT, typename F>
    struct transform_iterator;

    namespace tuple_detail
    {
        template <typename IteratorT, typename F>
        struct next_transform_iterator
        {
            typedef transform_iterator<
                typename next_iterator<IteratorT>::type, F>
            type;
        };

        template <typename IteratorT, typename F>
        struct deref_transform_iterator
        {
            typedef typename F::template result<
                typename deref_iterator<IteratorT>::type>::type
            type;
        };
    }

    template <typename IteratorT, typename F>
    struct transform_iterator : iterator<transform_iterator<IteratorT, F> >
    {
        typedef IteratorT iterator_type;
        typedef F function_type;

        typedef typename tuple_detail::
            deref_transform_iterator<IteratorT, F> deref_type;
        typedef typename tuple_detail::
            next_transform_iterator<IteratorT, F> next_type;

        transform_iterator(IteratorT const& iter, F const& f)
            : iter(iter), f(f) {}

        template <typename IterT>
        static typename next_iterator<IterT>::type
        next(IterT const& i)
        {
            typedef typename next_iterator<IterT>::type result;
            return result(boost::fusion::next(i.iter), i.f);
        }

        template <typename IterT>
        static typename deref_iterator<IterT>::type
        deref(IterT const& i)
        {
            return i.f(*i.iter);
        }

        IteratorT iter;
        F f;
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  transform_view
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ViewT, typename F>
    struct transform_view
    {
        typedef transform_iterator<typename ViewT::begin_type, F> begin_type;
        typedef transform_iterator<typename ViewT::end_type, F> end_type;

        transform_view(ViewT const& view)
            : view(view), f() {}

        transform_view(ViewT const& view, F const& f)
            : view(view), f(f) {}

        begin_type
        begin() const
        {
            return begin_type(view.begin(), f);
        }

        end_type
        end() const
        {
            return end_type(view.end(), f);
        }

        ViewT view;
        F f;
    };

}} // namespace boost::fusion

#endif


