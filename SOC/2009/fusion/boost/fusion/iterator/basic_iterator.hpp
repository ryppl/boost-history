/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ITERATOR_BASIC_ITERATOR_HPP
#define BOOST_FUSION_ITERATOR_BASIC_ITERATOR_HPP

#include <boost/fusion/iterator/iterator_facade.hpp>
#include <boost/fusion/support/internal/ref.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/int.hpp>
#include <boost/utility/addressof.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    namespace extension
    {
        template <typename>
        struct value_of_impl;

        template <typename>
        struct deref_impl;

        template <typename>
        struct value_of_data_impl;

        template <typename>
        struct key_of_impl;

        template <typename>
        struct deref_data_impl;
    }

    template<typename Tag, typename Category, typename SeqRef, int I>
    struct basic_iterator
      : iterator_facade<basic_iterator<Tag,Category,SeqRef,I>, Category>
    {
        BOOST_FUSION_MPL_ASSERT((detail::is_lrref<SeqRef>));

        typedef mpl::int_<I> index;
        typedef SeqRef seq_type;

        template <typename It>
        struct value_of
          : extension::value_of_impl<Tag>::template apply<It>
        {};

        template <typename It>
        struct deref
          : extension::deref_impl<Tag>::template apply<It>
        {};

        template <typename It>
        struct value_of_data
          : extension::value_of_data_impl<Tag>::template apply<It>
        {};

        template <typename It>
        struct key_of
          : extension::key_of_impl<Tag>::template apply<It>
        {};

        template <typename It>
        struct deref_data
          : extension::deref_data_impl<Tag>::template apply<It>
        {};

        template <typename It, typename N>
        struct advance
        {
            typedef
                basic_iterator<Tag, Category, SeqRef, I + N::value>
            type;

            static type
            call(It it)
            {
                return type(*it.seq,0);
            }
        };

        template <typename It>
        struct next
          : advance<It, mpl::int_<1> >
        {};

        template <typename It>
        struct prior
          : advance<It, mpl::int_<-1> >
        {};

        template <typename It1, typename It2>
        struct distance
          : mpl::minus<
                typename detail::remove_reference<It2>::type::index
              , typename detail::remove_reference<It1>::type::index
            >
        {};

        template <typename It1, typename It2>
        struct equal_to
        {
            typedef typename detail::remove_reference<It2>::type it2;

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

        template<typename OtherSeqRef>
        basic_iterator(basic_iterator<Tag,Category,OtherSeqRef,I> const& it)
          : seq(it.seq)
        {}

        basic_iterator(SeqRef seq, int)
          : seq(addressof(seq))
        {}

        template<typename OtherSeqRef>
        basic_iterator&
        operator=(basic_iterator<Tag,Category,OtherSeqRef,I> const& it)
        {
            seq=it.seq;
            return *this;
        }

        typename detail::remove_reference<SeqRef>::type* seq;
    };
}}

#endif
