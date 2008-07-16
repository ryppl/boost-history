/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden
    Copyright (c) 2008 Stjepan Rajko

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_SEQUENCE_CLASS_LAZY_SEQUENCE_10022005_0602)
#define FUSION_SEQUENCE_CLASS_LAZY_SEQUENCE_10022005_0602

#include <boost/fusion/sequence/sequence_facade.hpp>
#include <boost/fusion/iterator/iterator_facade.hpp>

#include <boost/mpl/int.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/minus.hpp>

#include <boost/utility/result_of.hpp>

#include <boost/function_types/is_function.hpp>

namespace boost { namespace fusion {

    template<typename Seq, int N>
    struct lazy_sequence_iterator
        : fusion::iterator_facade<lazy_sequence_iterator<Seq, N>, fusion::random_access_traversal_tag>
    {
        typedef mpl::int_<N> index;
        typedef Seq sequence_type;

        lazy_sequence_iterator(Seq& seq)
            : seq_(seq) {}

        Seq& seq_;

        template<typename T>
        struct value_of;

        template<typename Sq>
        struct value_of<lazy_sequence_iterator<Sq, N> >
            : mpl::identity<typename boost::result_of<typename Seq::generator_type(mpl::int_<N>)>::type>
        {};

        template<typename T>
        struct deref;

        template <typename Sq, int N_>
        struct deref<lazy_sequence_iterator<Sq, N_> >
        {
            typedef typename boost::result_of<typename Seq::generator_type(mpl::int_<N_>)>::type type;

            static type
            call(lazy_sequence_iterator<Sq, N_> const& iter)
            {
                return iter.seq_.generator(mpl::int_<N_>());
            }
        };

        template<typename It>
        struct next
        {
            typedef lazy_sequence_iterator<
                typename It::sequence_type, It::index::value + 1> type;

            static type call(It const& it)
            {
                return type(it.seq_);
            }
        };

        template<typename It>
        struct prior
        {
            typedef lazy_sequence_iterator<
                typename It::sequence_type, It::index::value - 1> type;

            static type call(It const& it)
            {
                return type(it.seq_);
            }
        };

        template<typename It1, typename It2>
        struct distance
        {
            typedef typename mpl::minus<typename It2::index, typename It1::index>::type type;

            static type call(It1 const& it1, It2 const& it2)
            {
                return type();
            }
        };

        template<typename It, typename M>
        struct advance
        {
            typedef lazy_sequence_iterator<
                typename It::sequence_type,
                It::index::value + M::value> type;

            static type call(It const& it)
            {
                return type(it.seq_);
            }
        };
    };

    template<typename Generator, int Size>
    struct lazy_sequence
        : fusion::sequence_facade<lazy_sequence<Generator, Size>, fusion::random_access_traversal_tag>
    {
        lazy_sequence()
            : generator()
        {}

        lazy_sequence(const Generator &generator)
            : generator(generator)
        {}

        template<typename Sq>
        struct begin
        {
            typedef lazy_sequence_iterator<
                Sq, 0> type;

            static type call(Sq& sq)
            {
                return type(sq);
            }
        };

        template<typename Sq>
        struct end
        {
            typedef lazy_sequence_iterator<
                Sq, Size> type;

            static type call(Sq& sq)
            {
                return type(sq);
            }
        };

        template<typename Sq>
        struct size
            : mpl::int_<Size>
        {};

        template<typename Sq, typename N>
        struct value_at
        {
            typedef typename boost::result_of<Generator(N)>::type type;
        };

        template<typename Sq, typename N>
        struct at
        {
            typedef typename boost::result_of<Generator(N)>::type type;

            static type call(Sq& sq)
            {
                return sq.generator(N());
            }
        };
        
        typedef Generator generator_type;
        generator_type generator;
    };
    
    namespace result_of
    {
        template<typename G, typename Size>
        struct make_lazy_sequence
        {
            typedef lazy_sequence<
                typename
                    mpl::if_<
                        function_types::is_function<G>,
                        G *,
                        G
                    >::type,
                Size::value
            > type;
        };
    }
    
    template<typename G, typename Size>
    typename result_of::make_lazy_sequence<G, Size>::type make_lazy_sequence(const G &g, Size size)
    {
        return typename result_of::make_lazy_sequence<G, Size>::type(g);
    }
} }

#endif