/*=============================================================================
    Copyright (c) 2001-2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_TUPLE_VIEW_HPP)
#define BOOST_FUSION_TUPLE_VIEW_HPP

#include <boost/ref.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/spirit/fusion/tuple.hpp>
#include <boost/spirit/fusion/iterator.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  tuple_iterator
    //
    ///////////////////////////////////////////////////////////////////////////
    template <int N, typename TupleT>
    class tuple_iterator : public iterator<tuple_iterator<N, TupleT> >
    {
    public:

        typedef add_reference<
            typename tuple_element<N, TupleT>::type
        > deref_type;

        typedef mpl::identity<tuple_iterator<N+1, TupleT> > next_type;

        BOOST_STATIC_CONSTANT(int, n = N);

        template <typename EndT>
        struct distance
        {
            BOOST_STATIC_CONSTANT(int, dist = EndT::n-N);
            typedef mpl::int_<dist> type;
        };

        tuple_iterator(TupleT& t)
            : ref(t) {}

        TupleT&
        get_tuple() const
        {
            return ref;
        }

        template <typename IterT>
        static typename next_iterator<IterT>::type
        next(IterT const& t)
        {
            typedef typename next_iterator<IterT>::type result;
            return result(t.get_tuple());
        }

        template <typename IterT>
        static typename deref_iterator<IterT>::type
        deref(IterT const& t)
        {
            return get<N>(t.get_tuple());
        }

    private:

        reference_wrapper<TupleT> ref;
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  tuple_view
    //
    ///////////////////////////////////////////////////////////////////////////
    template <
        typename TupleT
      , int Begin = 0
      , int End = tuple_size<TupleT>::value
    >
    class tuple_view
    {
    public:

        typedef tuple_iterator<Begin, TupleT> begin_type;
        typedef tuple_iterator<End, TupleT> end_type;

        tuple_view(TupleT& t)
            : ref(t) {}

        begin_type
        begin() const
        {
            return begin_type(ref);
        }

        end_type
        end() const
        {
            return end_type(ref);
        }

    private:

        reference_wrapper<TupleT> ref;
    };

}} // namespace boost::fusion

#endif


