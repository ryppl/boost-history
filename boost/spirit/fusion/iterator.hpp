/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_ITERATOR_HPP)
#define BOOST_FUSION_ITERATOR_HPP

#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    template <typename IteratorT>
    struct next_iterator;

    namespace tuple_detail
    {
        template <typename BeginT, typename EndT>
        struct distance;

        template <typename BeginT, typename EndT>
        struct next_distance
        {
            typedef typename mpl::next<
                typename distance<
                    typename next_iterator<BeginT>::type
                  , EndT
                >::type
            >::type type;
        };

        template <typename BeginT, typename EndT>
        struct distance
        {
            typedef typename
                mpl::if_<
                    is_same<BeginT, EndT>
                  , mpl::identity<mpl::int_<0> >
                  , next_distance<BeginT, EndT>
                >::type
            result;

            typedef typename result::type type;
        };
    }

    template <typename DerivedT>
    struct iterator
    {
        template <typename EndT>
        struct distance : tuple_detail::distance<DerivedT, EndT> {};
    };

    template <typename IteratorT>
    struct next_iterator
    {
        typedef typename IteratorT::next_type next_type;
        typedef typename next_type::type type;
    };

    template <typename DerivedT>
    typename next_iterator<DerivedT>::type
    next(iterator<DerivedT> const& i)
    {
        return DerivedT::next(static_cast<DerivedT const&>(i));
    }

    template <typename IteratorT>
    struct deref_iterator
    {
        typedef typename IteratorT::deref_type deref_type;
        typedef typename deref_type::type type;
    };

    template <typename DerivedT>
    typename deref_iterator<DerivedT>::type
    operator*(iterator<DerivedT> const& i)
    {
        return DerivedT::deref(static_cast<DerivedT const&>(i));
    }

    template <typename BeginT, typename EndT>
    struct distance : BeginT::template distance<EndT> {};

}} // namespace boost::fusion

#endif


