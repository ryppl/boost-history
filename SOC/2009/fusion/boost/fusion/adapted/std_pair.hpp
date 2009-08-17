// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_ADAPTED_STD_PAIR_HPP
#define BOOST_FUSION_ADAPTED_STD_PAIR_HPP

#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/support/tag_of_fwd.hpp>
#include <boost/fusion/support/internal/ref.hpp>

#include <boost/mpl/int.hpp>
#include <boost/config/no_tr1/utility.hpp>

#define BOOST_FUSION_STD_PAIR_TAG_OF_SPECIALIZATION(COMBINATION, _)             \
    template <typename T1, typename T2>                                         \
    struct tag_of<std::pair<T1, T2> COMBINATION>                                \
    {                                                                           \
        typedef struct_tag type;                                                \
    };

namespace boost { namespace fusion { namespace traits                           \
{                                                                               \
    BOOST_FUSION_ALL_CV_REF_NON_REF_COMBINATIONS(                               \
            BOOST_FUSION_STD_PAIR_TAG_OF_SPECIALIZATION,name)                   \
}}}                                                                             \

#undef BOOST_FUSION_STD_PAIR_TAG_OF_SPECIALIZATION

namespace boost { namespace fusion { namespace extension
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
}}}

#endif
