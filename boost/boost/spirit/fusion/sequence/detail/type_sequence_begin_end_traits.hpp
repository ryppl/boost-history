/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_SEQUENCE_DETAIL_TYPE_SEQUENCE_BEGIN_END_TRAITS_HPP)
#define FUSION_SEQUENCE_DETAIL_TYPE_SEQUENCE_BEGIN_END_TRAITS_HPP

#include <boost/spirit/fusion/detail/config.hpp>
#include <boost/mpl/begin_end.hpp>

namespace boost { namespace fusion
{
    template <typename Iterator>
    struct type_sequence_iterator;

    struct type_sequence_tag;

    template <typename SequenceT>
    struct type_sequence;

    namespace type_sequence_detail
    {
        template <typename Sequence>
        struct begin_traits_impl
        {
            typedef type_sequence_iterator<
                    typename mpl::begin<typename Sequence::sequence_type>::type>
            type;

            static type
            call(Sequence);
        };

        template <typename Sequence>
        begin_traits_impl<Sequence>::type begin_traits_impl<Sequence>::call(Sequence) 
        {
            FUSION_RETURN_DEFAULT_CONSTRUCTED;
        }

        template <typename Sequence>
        struct end_traits_impl
        {
            typedef type_sequence_iterator<
                    typename mpl::end<typename Sequence::sequence_type>::type>
            type;

            static type
            call(Sequence);
        };

        template <typename Sequence>
        end_traits_impl<Sequence>::type end_traits_impl<Sequence>::call(Sequence) 
        {
            FUSION_RETURN_DEFAULT_CONSTRUCTED;
        }
    }

    namespace meta
    {
        template <typename Tag>
        struct begin_impl;

        template <>
        struct begin_impl<type_sequence_tag>
        {
            template <typename Sequence>
            struct apply : type_sequence_detail::begin_traits_impl<Sequence>
            {};
        };

        template <typename Tag>
        struct end_impl;

        template <>
        struct end_impl<type_sequence_tag>
        {
            template <typename Sequence>
            struct apply : type_sequence_detail::end_traits_impl<Sequence>
            {};
        };
    }
}}

namespace boost { namespace mpl
{
    template <typename Tag>
    struct begin_impl;

    template <typename Tag>
    struct end_impl;

    template <>
    struct begin_impl<fusion::type_sequence_tag>
        : fusion::meta::begin_impl<fusion::type_sequence_tag> {};

    template <>
    struct end_impl<fusion::type_sequence_tag>
        : fusion::meta::end_impl<fusion::type_sequence_tag> {};
}}

#endif


