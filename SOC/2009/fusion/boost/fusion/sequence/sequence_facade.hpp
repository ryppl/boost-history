// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SEQUENCE_SEQUENCE_FACADE_HPP
#define BOOST_FUSION_SEQUENCE_SEQUENCE_FACADE_HPP

#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/internal/dummy_tag.hpp>

#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion
{
    struct void_;
    struct sequence_facade_tag;

    namespace extension
    {
        template<typename Tag>
        struct at_key_impl;

        template<typename Tag>
        struct empty_impl;

        template<typename Tag>
        struct size_impl;

        template<typename Tag>
        struct has_key_impl;

        template<typename Tag>
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
        typedef sequence_facade_tag fusion_tag;
        typedef Derived derived_type;
        typedef Category category;
        typedef IsView is_view;

        template<typename SeqRef,typename Key>
        struct at_key
          : extension::at_key_impl<
                typename detail::get_dummy_tag<SeqRef>::type
            >::template apply<SeqRef,Key>
        {};

        template<typename SeqRef>
        struct empty
          : extension::empty_impl<
                typename detail::get_dummy_tag<SeqRef>::type
            >::template apply<SeqRef>
        {};

        template<typename SeqRef>
        struct size
          : extension::size_impl<
                typename detail::get_dummy_tag<SeqRef>::type
            >::template apply<SeqRef>
        {};

        template<typename SeqRef,typename Key>
        struct has_key
          : extension::has_key_impl<
                typename detail::get_dummy_tag<SeqRef>::type
            >::template apply<SeqRef,Key>
        {};

        template<typename SeqRef,typename Key>
        struct value_at_key
          : extension::value_at_key_impl<
                typename detail::get_dummy_tag<SeqRef>::type
            >::template apply<SeqRef,Key>
        {};

    };
}}

#endif
