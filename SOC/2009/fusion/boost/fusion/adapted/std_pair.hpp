/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_STD_PAIR_HPP
#define BOOST_FUSION_ADAPTED_STD_PAIR_HPP

#include <boost/config.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/support/tag_of_fwd.hpp>
#include <boost/fusion/support/internal/ref.hpp>

#include <boost/mpl/tag.hpp>
#include <boost/mpl/int.hpp>
#include <boost/config/no_tr1/utility.hpp>

namespace boost
{
    namespace fusion { namespace traits
    {
#ifdef BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS
#   define BOOST_FUSION_STD_PAIR_TAG_OF_SPECIALIZATION(MODIFIER, _)             \
        template <typename T1, typename T2>                                     \
        struct tag_of<std::pair<T1, T2> MODIFIER, void>                         \
        {                                                                       \
            typedef struct_tag type;                                            \
        };
#else
#   define BOOST_FUSION_STD_PAIR_TAG_OF_SPECIALIZATION(MODIFIER, _)             \
        template <typename T1, typename T2>                                     \
        struct tag_of<std::pair<T1, T2> MODIFIER>                               \
        {                                                                       \
            typedef struct_tag type;                                            \
        };
#endif

        BOOST_FUSION_ALL_CV_REF_NON_REF_COMBINATIONS(
                BOOST_FUSION_STD_PAIR_TAG_OF_SPECIALIZATION,name)

#undef BOOST_FUSION_STD_PAIR_TAG_OF_SPECIALIZATION
    }}

    namespace mpl
    {
        template<typename>
        struct sequence_tag;

        template <typename T1, typename T2>
        struct sequence_tag<std::pair<T1, T2> >
        {
            typedef fusion::fusion_sequence_tag type;
        };

        template <typename T1, typename T2>
        struct sequence_tag<std::pair<T1, T2> const>
        {
            typedef fusion::fusion_sequence_tag type;
        };
    }

    namespace fusion { namespace extension
    {
        template <typename T1, typename T2>
        struct struct_member<std::pair<T1, T2>, 0>
        {
            typedef T1 type;

            template<typename Pair>
            static typename detail::forward_as<Pair&, type>::type
            call(Pair& pair)
            {
                return pair.first;
            }
        };

        template <typename T1, typename T2>
        struct struct_member<std::pair<T1, T2>, 1>
        {
            typedef T2 type;

            template<typename Pair>
            static typename detail::forward_as<Pair&, type>::type
            call(Pair& pair)
            {
                return pair.second;
            }
        };

        template <typename T1, typename T2>
        struct struct_size<std::pair<T1, T2> >
          : mpl::int_<2>
        {};
    }}
}

#endif
