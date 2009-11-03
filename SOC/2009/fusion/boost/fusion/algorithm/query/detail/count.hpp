/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_DETAIL_COUNT_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_DETAIL_COUNT_HPP

#include <boost/config.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_convertible.hpp>

namespace boost { namespace fusion { namespace detail
{
    template <bool is_convertible>
    struct compare_convertible;

    // T1 is convertible to T2 or vice versa
    template <>
    struct compare_convertible<true>
    {
        template <typename T1, typename T2>
        static bool
        call(BOOST_FUSION_R_ELSE_CLREF(T1) x, BOOST_FUSION_R_ELSE_CLREF(T2) y)
        {
            return BOOST_FUSION_FORWARD(T1,x) == BOOST_FUSION_FORWARD(T2,y);
        }
    };

    // T1 is NEITHER convertible to T2 NOR vice versa
    template <>
    struct compare_convertible<false>
    {
        template <typename T1, typename T2>
        static
#ifndef BOOST_NO_CONSTEXPR
        constexpr
#endif
        bool
        call(BOOST_FUSION_R_ELSE_CLREF(T1) x, BOOST_FUSION_R_ELSE_CLREF(T2) y)
        {
            return false;
        }
    };

    template <typename T1Ref>
    struct count_helper
    {
        count_helper(T1Ref x)
          : x(x)
        {}

        template <typename T2>
        bool
        operator()(BOOST_FUSION_R_ELSE_CLREF(T2) y)const
        {
            typedef typename remove_reference<T1Ref>::type T1_nonref;
#ifdef BOOST_NO_RVALUE_REFERENCES
            typedef T2 T2_nonref;
#else
            typedef typename remove_reference<T2>::type T2_nonref;
#endif

            return compare_convertible<
                mpl::or_<
                    is_convertible<T1_nonref, T2_nonref>
                  , is_convertible<T2_nonref, T1_nonref>
                >::value
            >::call(static_cast<T1Ref>(x),BOOST_FUSION_FORWARD(T2,y));
        }

        T1Ref x;
    };
}}}

#endif
