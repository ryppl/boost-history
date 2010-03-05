/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_SEQUENCE_FACADE_HPP
#define BOOST_FUSION_SEQUENCE_SEQUENCE_FACADE_HPP

#include <boost/fusion/support/sequence_base.hpp>

#include <boost/mpl/apply.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion
{
    struct void_;
    struct sequence_facade_tag;

    namespace extension
    {
        template<typename>
        struct at_key_impl;

        template<typename>
        struct empty_impl;

        template<typename>
        struct size_impl;

        template<typename>
        struct has_key_impl;

        template<typename>
        struct value_at_key_impl;
    }

    template<
        typename Derived
      , typename Category
      , typename IsView = mpl::false_
    >
    struct sequence_facade
      : sequence_base<Derived>
    {
        typedef fusion_sequence_tag tag;
        typedef sequence_facade_tag fusion_tag;
        typedef Derived derived_type;
        typedef Category category;
        typedef IsView is_view;

        template<typename Seq,typename Key>
        struct at_key
          : extension::at_key_impl<
                typename mpl::apply1<mpl::always<void_>,Seq>::type
            >::template apply<Seq,Key>
        {};

        template<typename Seq>
        struct empty
          : extension::empty_impl<
                typename mpl::apply1<mpl::always<void_>,Seq>::type
            >::template apply<Seq>
        {};

        template<typename Seq>
        struct size
          : extension::size_impl<
                typename mpl::apply1<mpl::always<void_>,Seq>::type
            >::template apply<Seq>
        {};

        template<typename Seq,typename Key>
        struct has_key
          : extension::has_key_impl<
                typename mpl::apply1<mpl::always<void_>,Seq>::type
            >::template apply<Seq,Key>
        {};

        template<typename Seq,typename Key>
        struct value_at_key
          : extension::value_at_key_impl<
                typename mpl::apply1<mpl::always<void_>,Seq>::type
            >::template apply<Seq,Key>
        {};

    };
}}

#endif
