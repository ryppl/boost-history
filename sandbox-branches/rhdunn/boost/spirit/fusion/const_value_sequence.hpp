/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_CONST_VALUE_SEQUENCE_HPP)
#define BOOST_FUSION_CONST_VALUE_SEQUENCE_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  const_value_iterator
    //
    //      Wraps an MPL-like constant value iterator (e.g. iterators to
    //      mpl::vector_c and mpl::list_c).
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename IteratorT>
    struct const_value_iterator;

    namespace tuple_detail
    {
        template <typename IteratorT>
        struct constant_value_type_of
        {
            typedef typename IteratorT::type const_;
            typedef typename const_::value_type type;
        };

        template <typename IteratorT>
        struct next_const_value_iterator
        {
            typedef const_value_iterator<typename IteratorT::next> type;
        };
    }

    template <typename IteratorT>
    struct const_value_iterator
        : iterator<const_value_iterator<IteratorT> >
    {
        typedef tuple_detail::constant_value_type_of<IteratorT> deref_type;
        typedef tuple_detail::next_const_value_iterator<IteratorT> next_type;

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
            typedef typename
                tuple_detail::constant_value_type_of<IteratorT>::const_
            const_;
            return const_::value;
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  const_value_sequence
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename SequenceT>
    struct const_value_sequence
    {
        typedef const_value_iterator<typename mpl::begin<SequenceT>::type> begin_type;
        typedef const_value_iterator<typename mpl::end<SequenceT>::type> end_type;

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


