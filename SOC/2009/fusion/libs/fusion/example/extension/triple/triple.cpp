/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

/*=============================================================================
    An implementation of a std::pair like triple<T0, T1, T2>
    We use fusion::sequence_facade and fusion::iterator_facade
    to make our triple a fully conforming Boost.Fusion random
    traversal sequence.
==============================================================================*/

#include <boost/config.hpp>
#include <boost/detail/lightweight_test.hpp>

#include <boost/fusion/sequence.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/support.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_same.hpp>
#include <string>

namespace mpl = boost::mpl;
namespace fusion = boost::fusion;

namespace demo
{
    template<typename SeqRef, int N>
    struct triple_iterator
      : fusion::iterator_facade<
            triple_iterator<SeqRef, N>
          , fusion::random_access_traversal_tag
        >
    {
        typedef mpl::int_<N> index;
        typedef SeqRef seq_type;
        //TODO
        typedef typename
            fusion::detail::remove_reference<SeqRef>::type
        seq_nonref_type;

        triple_iterator(SeqRef seq)
          : seq(seq)
        {}

        SeqRef seq;

        template<typename ItRef>
        struct value_of;

        template<typename ItRef>
        struct value_of<ItRef&>
          : value_of<ItRef>
        {};

#ifndef BOOST_NO_RVALUE_REFERENCES
        template<typename ItRef>
        struct value_of<ItRef&&>
          : value_of<ItRef>
        {};
#endif

        template<typename SeqRef_>
        struct value_of<triple_iterator<SeqRef_, 0> >
        {
            typedef typename seq_nonref_type::t0_type type;
        };

        template<typename SeqRef_>
        struct value_of<triple_iterator<SeqRef_, 1> >
        {
            typedef typename seq_nonref_type::t1_type type;
        };

        template<typename SeqRef_>
        struct value_of<triple_iterator<SeqRef_, 2> >
        {
            typedef typename seq_nonref_type::t2_type type;
        };

        template<typename ItRef>
        struct deref;

        template<typename ItRef>
        struct deref<ItRef const>
          : deref<ItRef>
        {};

        template<typename ItRef>
        struct deref<ItRef&>
          : deref<ItRef>
        {};

#ifndef BOOST_NO_RVALUE_REFERENCES
        template<typename ItRef>
        struct deref<ItRef&&>
          : deref<ItRef>
        {};
#endif

        template<typename SeqRef_>
        struct deref<triple_iterator<SeqRef_, 0> >
        {
            typedef typename
                mpl::if_<
                    boost::is_const<seq_nonref_type>
                  , typename seq_nonref_type::t0_type const&
                  , typename seq_nonref_type::t0_type&
                >::type
            type;

            static type
            call(triple_iterator<SeqRef, 0> const& iter)
            {
                return iter.seq.t0;
            }
        };

        template<typename SeqRef_>
        struct deref<triple_iterator<SeqRef_, 1> >
        {
            typedef typename
                mpl::if_<
                    boost::is_const<seq_nonref_type>
                  , typename seq_nonref_type::t1_type const&
                  , typename seq_nonref_type::t1_type&
                >::type
            type;

            static type
            call(triple_iterator<SeqRef, 1> const& iter)
            {
                return iter.seq.t1;
            }
        };

        template<typename SeqRef_>
        struct deref<triple_iterator<SeqRef_, 2> >
        {
            typedef typename
                mpl::if_<
                    boost::is_const<seq_nonref_type>
                  , typename seq_nonref_type::t2_type const&
                  , typename seq_nonref_type::t2_type&
                >::type
            type;

            static type
            call(triple_iterator<SeqRef, 2> const& iter)
            {
                return iter.seq.t2;
            }
        };

        template<typename ItRef1, typename ItRef2>
        struct distance
          : mpl::minus<
                typename fusion::detail::remove_reference<ItRef2>::type::index
              , typename fusion::detail::remove_reference<ItRef1>::type::index
            >
        {};

        template<typename ItRef, typename M>
        struct advance
        {
            typedef
                triple_iterator<
                    seq_type,
                    N + M::value
                >
            type;

            static type
            call(ItRef it)
            {
                return type(it.seq);
            }
        };

        template<typename ItRef>
        struct next
          : advance<ItRef, mpl::int_<1> >
        {};

        template<typename ItRef>
        struct prior
          : advance<ItRef, mpl::int_<-1> >
        {};
    };

    template<typename T0, typename T1, typename T2>
    struct triple
      : fusion::sequence_facade<
            triple<T0, T1, T2>
          , fusion::random_access_traversal_tag
        >
    {
        typedef T0 t0_type;
        typedef  T1 t1_type;
        typedef T2 t2_type;

        triple(T0 const& t0, T1 const& t1, T2 const& t2)
          : t0(t0), t1(t1), t2(t2)
        {}

        template<typename SeqRef>
        struct begin
        {
            typedef demo::triple_iterator<SeqRef, 0> type;

            static type
            call(SeqRef seq)
            {
                return type(seq);
            }
        };

        template<typename SeqRef>
        struct end
        {
            typedef demo::triple_iterator<SeqRef, 3> type;

            static type call(SeqRef seq)
            {
                return type(seq);
            }
        };

        template<typename SeqRef>
        struct size
          : mpl::int_<3>
        {};

        template<typename SeqRef, typename N>
        struct value_at
          : value_at<SeqRef, mpl::int_<N::value> >
        {};

        template<typename SeqRef>
        struct value_at<SeqRef, mpl::int_<0> >
        {
            typedef T0 type;
        };

        template<typename SeqRef>
        struct value_at<SeqRef, mpl::int_<1> >
        {
            typedef T1 type;
        };

        template<typename SeqRef>
        struct value_at<SeqRef, mpl::int_<2> >
        {
            typedef T2 type;
        };

        template<typename SeqRef, typename N>
        struct at
          : at<SeqRef, mpl::int_<N::value> >
        {};

        template<typename SeqRef>
        struct at<SeqRef, mpl::int_<0> >
        {
            typedef typename fusion::detail::remove_reference<SeqRef>::type seq;

            typedef typename
                mpl::if_<boost::is_const<seq>, T0 const&, T0&>::type
            type;

            static type
            call(SeqRef seq)
            {
                return seq.t0;
            }
        };

        template<typename SeqRef>
        struct at<SeqRef, mpl::int_<1> >
        {
            typedef typename fusion::detail::remove_reference<SeqRef>::type seq;

            typedef typename
                mpl::if_<boost::is_const<seq>, T1 const&, T1&>::type
            type;

            static type
            call(SeqRef seq)
            {
                return seq.t1;
            }
        };

        template<typename SeqRef>
        struct at<SeqRef, mpl::int_<2> >
        {
            typedef typename fusion::detail::remove_reference<SeqRef>::type seq;

            typedef typename
                mpl::if_<boost::is_const<seq>, T2 const&, T2&>::type
            type;

            static type
            call(SeqRef seq)
            {
                return seq.t2;
            }
        };

        T0 t0;
        T1 t1;
        T2 t2;
    };
}

int main()
{
    typedef demo::triple<int, char, std::string> my_triple;
    my_triple t(101, 'a', "hello");

    BOOST_TEST(*fusion::begin(t) == 101);
    BOOST_TEST(*fusion::next(fusion::begin(t)) == 'a');
    BOOST_TEST(*fusion::prior(fusion::end(t)) == "hello");
    BOOST_TEST(fusion::distance(fusion::begin(t), fusion::end(t)) == 3);
    BOOST_TEST(fusion::size(t) == 3);

    BOOST_MPL_ASSERT((boost::is_same<int, fusion::result_of::value_at_c<my_triple, 0>::type>));
    BOOST_MPL_ASSERT((boost::is_same<char, fusion::result_of::value_at_c<my_triple, 1>::type>));
    BOOST_MPL_ASSERT((boost::is_same<std::string, fusion::result_of::value_at_c<my_triple, 2>::type>));

    BOOST_TEST(fusion::at_c<0>(t) == 101);
    BOOST_TEST(fusion::at_c<1>(t) == 'a');
    BOOST_TEST(fusion::at_c<2>(t) == "hello");

    return boost::report_errors();
}
