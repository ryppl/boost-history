/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_SEQUENCE_DETAIL_GENERATE_HPP)
#define FUSION_SEQUENCE_DETAIL_GENERATE_HPP

#include <boost/spirit/fusion/sequence/tuple.hpp>
#include <boost/spirit/fusion/iterator/value_of.hpp>
#include <boost/spirit/fusion/iterator/next.hpp>
#include <boost/spirit/fusion/iterator/equal_to.hpp>
#include <boost/preprocessor/repetition/enum_shifted.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/mpl/apply_if.hpp>
#include <boost/mpl/identity.hpp>

#define FUSION_DEREF_ITERATOR(z, n, data)                                       \
    typename checked_deref_iterator<                                            \
        BOOST_PP_CAT(T, n), Last>::type

#define FUSION_NEXT_ITERATOR(z, n, data)                                        \
    typename BOOST_PP_CAT(T, n) =                                               \
        typename checked_next_iterator<                                         \
            BOOST_PP_CAT(T, BOOST_PP_DEC(n)), Last>::type

namespace boost { namespace fusion { namespace detail
{
    template <typename First, typename Last>
    struct checked_deref_iterator
    {
        typedef typename
            mpl::apply_if<
                equal_to<First, Last>
              , mpl::identity<void_t>
              , value_of<First>
            >::type
        type;
    };

    template <typename First, typename Last>
    struct checked_next_iterator
    {
        typedef typename
            mpl::apply_if<
                equal_to<First, Last>
              , mpl::identity<Last>
              , result_of_next<First>
            >::type
        type;
    };

    template <
        typename First
      , typename Last
      , typename T0 = First
      , BOOST_PP_ENUM_SHIFTED(FUSION_MAX_TUPLE_SIZE, FUSION_NEXT_ITERATOR, _)
    >
    struct result_of_generate
    {
        typedef tuple<BOOST_PP_ENUM(FUSION_MAX_TUPLE_SIZE
            , FUSION_DEREF_ITERATOR, _)> type;
    };

    template <typename First, typename Last>
    inline typename result_of_generate<First, Last>::type
    generate(First const& first, Last const&)
    {
        typedef typename result_of_generate<First, Last>::type result;
        return result(first);
    }
}}}

#undef FUSION_DEREF_ITERATOR
#undef FUSION_NEXT_ITERATOR
#endif


