/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden
    Copyright (c) 2010 Christopher Schmidt
    Copyright (c) 2010 John Fletcher

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

/*==============================================================================
    An implementation of a std::pair like triple<T0, T1, T2>
    We use fusion::sequence_facade and fusion::iterator_facade
    to make our triple a fully conforming Boost.Fusion random
    traversal sequence.
==============================================================================*/

/* ---------------------------------------------------------------------------
   This file changes over to use a variadic tuple in place of the triple.
   This version works with gcc 4.4.3 with -std=gnu++0x and boost 1.42.0
   plus Christopher Schmidt's modifications (not yet in a boost release).

   John Fletcher J.P.Fletcher@aston.ac.uk

   ---------------------------------------------------------------------------*/

#include <boost/config.hpp>
#include <boost/detail/lightweight_test.hpp>
#ifndef BOOST_NO_VARIADIC_TEMPLATES
#include <tuple>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/support.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/greater_equal.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_scalar.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <string>

namespace mpl = boost::mpl;
namespace fusion = boost::fusion;

namespace variadic_tuple
{

// Test for tuple index size too large.
template<typename Sq, int N>
struct VARIADIC_TUPLE_INDEX_TOO_LARGE 
  : boost::mpl::greater_equal<boost::mpl::int_<N>,boost::mpl::int_<
  std::tuple_size<typename Sq::base_type>::value> >
{};

// Test for tuple index size too small.
template<typename Sq, int N>
struct VARIADIC_TUPLE_INDEX_TOO_SMALL
  : boost::mpl::less<boost::mpl::int_<N>,boost::mpl::int_<0> >
{};

// This has been added for the member functions defined below.
template<typename Sq, int N>
struct VARIADIC_TUPLE_RETURN_TYPE_N : boost::mpl::if_<
  VARIADIC_TUPLE_INDEX_TOO_LARGE<Sq,N>,
  boost::mpl::void_,
  std::tuple_element<N,typename Sq::base_type> >::type
{};

// This has been added for the member functions defined below.
// This does not have the checks for type in at the moment.
// It defines a member type internally instead of by inheritance.
template<typename Sq, typename Key, typename Key_Sq>
struct VARIADIC_TUPLE_RETURN_TYPE_KEY
{
  typedef typename std::tuple_element<
          boost::mpl::find<Key_Sq,Key>::type::pos::value,
          typename Sq::base_type >::type type;
};

    template<typename SeqRef, int NN>
    struct vtuple_iterator
        : fusion::iterator_facade<
          vtuple_iterator<SeqRef, NN>
        , fusion::random_access_traversal_tag
    >
    {
        typedef mpl::int_<NN> index;
        typedef SeqRef sequence_type;
        typedef typename
            boost::remove_reference<SeqRef>::type
        seq_nonref_type;

        vtuple_iterator(SeqRef seq)
            : seq(seq) {}

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

        template<typename It>
        struct value_of<It const>
          : value_of<It>
        {};

      template<typename SeqRef_, int N>
        struct value_of<vtuple_iterator<SeqRef_, N> >
        {
            typedef typename
            boost::remove_reference<SeqRef_>::type
            seq_;
            BOOST_MPL_ASSERT_NOT((VARIADIC_TUPLE_INDEX_TOO_LARGE<seq_,N>));
	    typedef typename std::tuple_element<N,
                    typename seq_::base_type>::type type;
        };

      template<typename T>
        struct deref;

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

        template<typename It>
        struct deref<It const>
          : deref<It>
        {};

      template<typename SeqRef_, int N>
        struct deref<vtuple_iterator<SeqRef_, N> >
        {
            typedef typename
            boost::remove_reference<SeqRef_>::type
            seq_;
            BOOST_MPL_ASSERT_NOT((VARIADIC_TUPLE_INDEX_TOO_LARGE<seq_,N>));
	    typedef typename std::tuple_element<N,typename seq_::base_type>::type typeN;
            typedef typename
                mpl::if_<
                    boost::is_const<seq_>
                  , typeN const&
                  , typeN
                >::type
            type;

            static type
            call(vtuple_iterator<SeqRef, N> const& iter)
            {
                return std::get<N>(iter.seq);
            }
        };

        template<typename ItRef1, typename ItRef2>
        struct distance
          : mpl::minus<
                typename boost::remove_reference<ItRef2>::type::index
              , typename boost::remove_reference<ItRef1>::type::index
            >
        {};

        template<typename ItRef, typename M>
        struct advance
        {
            typedef
                vtuple_iterator<
                    sequence_type,
                    NN + M::value
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

    template<typename... Types>
    struct vtuple
      : fusion::sequence_facade<vtuple<Types...>, fusion::random_access_traversal_tag>,
        public std::tuple<Types...>
    {
        typedef vtuple<Types...> This_type;
        typedef std::tuple<Types...> base_type;

        vtuple()
            : base_type() {}

        template<typename... Args>
        vtuple(const Args&... args) : base_type(args...) { }

        template<typename... Args>
        vtuple(Args&... args) : base_type(args...) { }

        template<typename T>
        explicit vtuple(T const& rhs)
            : base_type(rhs) {}

        template<typename T>
        vtuple&
        operator=(T const& rhs)
        {
            base_type::operator=(rhs);
            return *this;
        }

        // I want to make member functions which will return the same things as 
        // the nonmember functions....
        // I am having trouble with the grammar....
        // The problem was not the grammar, but with the null constructor.
        // I have taken this out of use. What is needed is something to sort this out
        // to compile conditionally for cases with at least one type...
        // This now works so the default constructor can go back in.
        typename VARIADIC_TUPLE_RETURN_TYPE_N<This_type,0>::type
        first() const
        {
           BOOST_MPL_ASSERT_NOT((VARIADIC_TUPLE_INDEX_TOO_LARGE<This_type,0>));
           return std::get<0>(*this);
        }
 
        typename VARIADIC_TUPLE_RETURN_TYPE_N<This_type,1>::type
        second() const
        {
           BOOST_MPL_ASSERT_NOT((VARIADIC_TUPLE_INDEX_TOO_LARGE<This_type,1>));
           return std::get<1>(*this);
        }

        typename VARIADIC_TUPLE_RETURN_TYPE_N<This_type,2>::type
        third() const
        {
           BOOST_MPL_ASSERT_NOT((VARIADIC_TUPLE_INDEX_TOO_LARGE<This_type,2>));
           return std::get<2>(*this);
        }

        // Generalised for any element in the sequence.
        template<int N>
        typename VARIADIC_TUPLE_RETURN_TYPE_N<This_type,N>::type
        any() const
        {
           BOOST_MPL_ASSERT_NOT((VARIADIC_TUPLE_INDEX_TOO_LARGE<This_type,N>));
           return std::get<N>(*this);
        }

        // Generalised for any element in the sequence by key.
        template<typename Key,typename Key_Sequence>
        typename VARIADIC_TUPLE_RETURN_TYPE_KEY<This_type,Key,Key_Sequence>::type
        any_key() const
        {
           typedef typename boost::mpl::end<Key_Sequence>::type finish;
       typedef typename boost::mpl::find<Key_Sequence,Key>::type pos_type;
	   // This checks that the key is in the sequence
           BOOST_STATIC_ASSERT((!boost::is_same<pos_type,finish>::value)); 
       // This checks against the size of the tuple
           BOOST_MPL_ASSERT_NOT((VARIADIC_TUPLE_INDEX_TOO_LARGE<This_type,pos_type::pos::value>));

           return std::get<pos_type::pos::value>(*this);
      }

      // Find the postion of a given key.
      // I have had to include the Key_Sequence parameter as the actual sequence
      // is not available when this is compiled - as far as I can work out.
      // This has in fact liberated the top level code to supply any key sequence.
      // It does not have to be the same length as the sequence.
      // Any problem cases will not compile because of the tests here.
      template<typename Key,typename Key_Sequence>
      int pos()
      {
        typedef typename boost::mpl::end<Key_Sequence>::type finish;
    typedef typename boost::mpl::find<Key_Sequence,Key>::type pos_type;
    // This checks that the key is in the sequence
        BOOST_STATIC_ASSERT((!boost::is_same<pos_type,finish>::value)); 
    // This checks against the size of the tuple
        BOOST_MPL_ASSERT_NOT((VARIADIC_TUPLE_INDEX_TOO_LARGE<This_type,pos_type::pos::value>));
        return pos_type::pos::value;
      }
       
      // location function for the enum method
      template<typename Enums, Enums Key>
      int locate()
      {
        BOOST_MPL_ASSERT_NOT((VARIADIC_TUPLE_INDEX_TOO_LARGE<This_type,
                              boost::mpl::integral_c<Enums,Key>::value>));
    return boost::mpl::integral_c<Enums,Key>::value;
      }
      // Access function for the enum method
      /*      template<typename Enums, Enums Key>
      typename VARIADIC_TUPLE_RETURN_TYPE_N<This_type,
               boost::mpl::integral_c<Enums,Key>::value>::type
      at()
      {
        BOOST_MPL_ASSERT_NOT((VARIADIC_TUPLE_INDEX_TOO_LARGE<This_type,
                              boost::mpl::integral_c<Enums,Key>::value>));
    return std::get<boost::mpl::integral_c<Enums,Key>::value >(*this);
      }
      */
        template<typename Sq>
        struct begin
        {
            typedef variadic_tuple::vtuple_iterator<
                Sq, 0> type;

            static type call(Sq sq)
            {
                return type(sq);
            }
        };

        template<typename Sq>
        struct end
        {
           typedef typename
            boost::remove_reference<Sq>::type
            seq;
            typedef variadic_tuple::vtuple_iterator<
                Sq, 
                std::tuple_size<typename seq::base_type>::value
                > type;
            static type call(Sq sq)
            {
                return type(sq);
            }
        };

        template<typename Sq>
        struct size
    //  : mpl::int_<
    //    std::tuple_size<typename Sq::sequence_type::base_type>::value
        //    >
        {
            typedef typename
            boost::remove_reference<Sq>::type
            seq;
            typedef typename mpl::int_<
          std::tuple_size<typename seq::base_type>::value>::type type;
        };
        template<typename Sq, typename N>
        struct value_at
            : value_at<Sq, mpl::int_<N::value> >
        {};

        template<typename Sq, int N>
        struct value_at<Sq, mpl::int_<N> >
        {
            typedef typename
            boost::remove_reference<Sq>::type
            seq;
            BOOST_MPL_ASSERT_NOT((VARIADIC_TUPLE_INDEX_TOO_LARGE<seq,N>));
          typedef typename std::tuple_element<N,
              typename seq::base_type>::type type;
        };
      
        template<typename Sq, typename N>
        struct at
            : at<Sq, mpl::int_<N::value> >
        {};
      
      
      template<typename Sq, int N>
        struct at<Sq, mpl::int_<N> >
        {
            typedef typename
            boost::remove_reference<Sq>::type
            seq;
             BOOST_MPL_ASSERT_NOT((VARIADIC_TUPLE_INDEX_TOO_LARGE<seq,N>));
          typedef typename std::tuple_element<N,
              typename seq::base_type>::type typeN;
              typedef typename mpl::if_<
            boost::is_const<seq>,
                    typeN const&,
                    typeN&>::type type;
            static type call(Sq seq)
            {
                return std::get<N>(seq);
            }
        };
      
   };
}

int main()
{
    typedef variadic_tuple::vtuple<int> my_mono;
    typedef variadic_tuple::vtuple<int, char> my_duo;
    typedef variadic_tuple::vtuple<int, char, std::string> my_tuple;
    my_tuple t(101, 'a', "hello");
    BOOST_TEST(*fusion::begin(t) == 101);
    BOOST_TEST(*fusion::next(fusion::begin(t)) == 'a');
    BOOST_TEST(*fusion::prior(fusion::end(t)) == "hello");
    BOOST_TEST(fusion::distance(fusion::begin(t), fusion::end(t)) == 3);
    BOOST_TEST(fusion::size(t) == 3);
    BOOST_MPL_ASSERT((boost::is_same<int, fusion::result_of::value_at_c<my_tuple, 0>::type>));
    BOOST_MPL_ASSERT((boost::is_same<char, fusion::result_of::value_at_c<my_tuple, 1>::type>));
    BOOST_MPL_ASSERT((boost::is_same<std::string, fusion::result_of::value_at_c<my_tuple, 2>::type>));
    BOOST_TEST(fusion::at_c<0>(t) == 101);
    BOOST_TEST(fusion::at_c<1>(t) == 'a');
    BOOST_TEST(fusion::at_c<2>(t) == "hello");
    return boost::report_errors();
}
#else
int main()
{
    return boost::report_errors();
}
#endif
