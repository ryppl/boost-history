/*=============================================================================
    Copyright (c) 2001-2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_TYPE_SEQUENCE_HPP)
#define BOOST_FUSION_TYPE_SEQUENCE_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  type_iterator
    //
    //      Wraps an MPL-like type iterator (e.g. iterators to
    //      mpl::vector and mpl::list).
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename IteratorT>
    struct type_iterator;

    namespace tuple_detail
    {
        template <typename IteratorT>
        struct next_type_iterator
        {
            typedef type_iterator<typename IteratorT::next> type;
        };
    }

    template <typename IteratorT>
    struct type_iterator : iterator<type_iterator<IteratorT> >
    {
        typedef IteratorT deref_type;
        typedef tuple_detail::next_type_iterator<IteratorT> next_type;

        template <typename IterT>
        static typename next_iterator<IterT>::type
        next(IterT const& t)
        {
            typedef typename next_iterator<IterT>::type result;
            return result();
        }

        template <typename IterT>
        static typename deref_iterator<IterT>::type
        deref(IterT const& t)
        {
            typedef typename deref_iterator<IterT>::type result;
            return result();
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  type_sequence
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename SequenceT>
    struct type_sequence
    {
        typedef type_iterator<typename mpl::begin<SequenceT>::type> begin_type;
        typedef type_iterator<typename mpl::end<SequenceT>::type> end_type;

        begin_type
        begin() const
        {
            return begin_type();
        }

        end_type
        end() const
        {
            return end_type();
        }
    };

}} // namespace boost::fusion

#endif


