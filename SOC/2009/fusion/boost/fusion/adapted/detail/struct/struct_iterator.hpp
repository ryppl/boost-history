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

#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/config/no_tr1/utility.hpp>

namespace boost { namespace fusion
{
    struct random_access_traversal_tag;

    template <typename SeqRef, int Pos>
    struct struct_iterator
      : iterator_facade<
            struct_iterator<SeqRef, Pos>
          , random_access_traversal_tag
        >
    {
        //BOOST_FUSION_INDEX_CHECK(N,struct_size<SeqRef>::value);

        typedef typename detail::identity<SeqRef>::type identity_struct;

        template <typename ItRef>
        struct value_of
          : extension::struct_member<identity_struct, Pos>
        {};

        template <typename ItRef>
        struct deref
        {
            typedef typename
                detail::forward_as<
                    SeqRef
                  , typename extension::struct_member<
                        identity_struct
                      , Pos
                    >::type
                >::type
            type;

            static type
            call(ItRef it)
            {
                return extension::struct_member<identity_struct, Pos>::call(
                        *it.struct_);
            }
        };

        template <typename ItRef, typename N>
        struct advance
        {
            typedef
                struct_iterator<SeqRef, Pos + N::value>
            type;

            static type
            call(ItRef it)
            {
                return type(*it.struct_,0);
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
                mpl::int_<Pos>
              , typename detail::remove_reference<It1Ref>::type::index
            >
        {
        };

        typedef mpl::int_<Pos> index;

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
