// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_ITERATOR_BASIC_ITERATOR_HPP
#define BOOST_FUSION_ITERATOR_BASIC_ITERATOR_HPP

#include <boost/fusion/iterator/iterator_facade.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    namespace extension
    {
        template <typename Tag>
        struct value_of_impl;

        template <typename Tag>
        struct deref_impl;

        template <typename Tag>
        struct value_of_data_impl;

        template <typename Tag>
        struct key_of_impl;

        template <typename Tag>
        struct deref_data_impl;
    }

    template<
        typename Tag
      , typename Category
      , typename SeqRef
      , int Pos
    >
    struct basic_iterator
      : iterator_facade<
            basic_iterator<Tag,Category,SeqRef,Pos>
          , Category
        >
    {
        typedef mpl::int_<Pos> index;
        typedef SeqRef seq_type;

        template <typename ItRef>
        struct value_of
          : extension::value_of_impl<Tag>::template apply<ItRef>
        {};

        template <typename ItRef>
        struct deref
          : extension::deref_impl<Tag>::template apply<ItRef>
        {};

        template <typename ItRef>
        struct value_of_data
          : extension::value_of_data_impl<Tag>::template apply<ItRef>
        {};

        template <typename ItRef>
        struct key_of
          : extension::key_of_impl<Tag>::template apply<ItRef>
        {};

        template <typename ItRef>
        struct deref_data
          : extension::deref_data_impl<Tag>::template apply<ItRef>
        {};

        template <typename ItRef, typename N>
        struct advance
        {
            typedef
                basic_iterator<
                    Tag
                  , Category
                  , SeqRef
                  , Pos + N::value
                >
            type;

            static type
            call(ItRef it)
            {
                return type(*it.seq,0);
            }
        };

        template <typename ItRef>
        struct next
          : advance<ItRef, mpl::int_<1> >
        {};

        template <typename ItRef>
        struct prior
          : advance<ItRef, mpl::int_<-1> >
        {};

        template <typename It1Ref, typename It2Ref>
        struct distance
          : mpl::minus<
                typename detail::remove_reference<It2Ref>::type::index
              , typename detail::remove_reference<It1Ref>::type::index
            >
        {};

        template <typename It1Ref, typename It2Ref>
        struct equal_to
        {
            typedef typename detail::remove_reference<It2Ref>::type it2;

            typedef
                mpl::and_<
                    is_same<
                        typename detail::identity<SeqRef>::type
                      , typename detail::identity<typename it2::seq_type>::type
                    >
                  , mpl::equal_to<index,typename it2::index>
                >
            type;
        };

        template<typename OtherIt>
        basic_iterator(OtherIt const& it)
          : seq(it.seq)
        {}

        basic_iterator(SeqRef seq, int)
          : seq(&seq)
        {}

        template<typename OtherIt>
        basic_iterator&
        operator=(OtherIt const& it)
        {
            seq=it.seq;
            return *this;
        }

        typename detail::remove_reference<SeqRef>::type* seq;
    };
}}

#endif
