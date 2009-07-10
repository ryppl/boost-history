/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_STRUCT_ITERATOR_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_STRUCT_ITERATOR_HPP

#include <boost/fusion/support/assert.hpp>
#include <boost/fusion/iterator/iterator_facade.hpp>

#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/config/no_tr1/utility.hpp>

namespace boost { namespace fusion
{
    struct random_access_traversal_tag;

    template <typename SeqRef, int N>
    struct struct_iterator
      : iterator_facade<struct_iterator<SeqRef, N>, random_access_traversal_tag>
    {
        //BOOST_FUSION_INDEX_CHECK(N,struct_size<SeqRef>::value);

        template <typename ItRef>
        struct value_of
          : extension::struct_member<SeqRef, N>
        {};

        template <typename ItRef>
        struct deref
        {
            typedef typename detail::identity<SeqRef>::type identity_struct;

            typedef typename
                detail::forward_as<
                    SeqRef
                  , typename extension::struct_member<identity_struct, N>::type
                >::type
            type;

            static type
            call(ItRef it)
            {
                return extension::struct_member<identity_struct, N>::call(
                        *it.struct_);
            }
        };

        template <typename ItRef, typename N_>
        struct advance
        {
            typedef typename detail::remove_reference<ItRef>::type it;

            typedef
                struct_iterator<
                    typename it::struct_type
                  , it::index::value + N_::value
                >
            type;

            static type
            call(ItRef it)
            {
                return type(*it.struct_,0);
            }
        };

        typedef mpl::int_<N> index;
        typedef SeqRef struct_type;

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
        {
        };

        template<typename OtherIt>
        struct_iterator(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
          : struct_(it.struct_)
        {}

        struct_iterator(SeqRef struct_,int)
          : struct_(&struct_)
        {}

        template<typename OtherIt>
        struct_iterator&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
        {
            struct_=it.struct_;
            return *this;
        }

        typename detail::remove_reference<SeqRef>::type* struct_;
    };
}}

#endif
