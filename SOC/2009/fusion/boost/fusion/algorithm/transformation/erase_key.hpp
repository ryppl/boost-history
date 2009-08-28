/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_ERASE_KEY_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_ERASE_KEY_HPP

#include <boost/fusion/algorithm/query/find_key.hpp>
#include <boost/fusion/algorithm/transformation/erase.hpp>
#ifdef BOOST_FUSION_ENABLE_STATIC_ASSERTS
#   include <boost/fusion/sequence/intrinsic/has_key.hpp>
#endif
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/workaround.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/fusion/algorithm/transformation/detail/filter_key_view.hpp>

//TODO doc singular?!
namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename Key>
        struct erase_key
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_associative<Seq>));
            BOOST_FUSION_MPL_ASSERT((has_key<Seq,Key>));

            typedef typename
                mpl::eval_if<
                    traits::is_forward<Seq>
                  , erase<
                        Seq
                      , typename find_key<Seq, Key>::type
                    >
                  , mpl::identity<
                        detail::filter_key_view<Seq,is_same<mpl::_1,Key> >
                    >
                >::type
            type;

            static type
            call_impl(Seq seq,mpl::true_ /*is_forward*/)
            {
                return fusion::erase(seq,fusion::find_key<Key>(seq));
            }

            static type
            call_impl(Seq seq,mpl::false_ /*is_forward*/)
            {
                return type(seq);
            }

            static type
            call(Seq seq)
            {
                return call_impl(seq,typename traits::is_forward<Seq>::type());
            }
        };
    }

    template <typename Key, typename Seq>
    inline typename
        result_of::erase_key<BOOST_FUSION_R_ELSE_CLREF(Seq), Key>::type
    erase_key(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return
            result_of::erase_key<BOOST_FUSION_R_ELSE_CLREF(Seq), Key>::call(
                seq);
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Key, typename Seq>
    inline BOOST_FUSION_EXPLICIT_TEMPLATE_NON_CONST_ARG_OVERLOAD(
            result_of::erase_key<,Seq,&, Key>)
    erase_key(Seq& seq)
    {
        return result_of::erase_key<Seq&, Key>::call(seq);
    }
#endif
}}

#endif
