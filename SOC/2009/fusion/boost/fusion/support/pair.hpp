/*=============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2006 Tobias Schwinger
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_PAIR_HPP
#define BOOST_FUSION_SUPPORT_PAIR_HPP

#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/deduce.hpp>

#include <boost/preprocessor/empty.hpp>
#if defined(BOOST_NO_VARIADIC_TEMPLATES) && defined(BOOST_NO_RVALUE_REFERENCES)
#   include <boost/call_traits.hpp>
#endif

namespace boost { namespace fusion
{
    // A half runtime pair where the first type does not have data
    template <typename First, typename Second>
    struct pair
    {
        typedef First first_type;
        typedef Second second_type;

#define BOOST_FUSION_PAIR_CTOR(COMBINATION,_)\
        template<class OtherSecond>\
        pair(pair<First,OtherSecond> COMBINATION pair_)\
          : second(static_cast<pair<First,OtherSecond> COMBINATION>\
                (pair_).second)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_PAIR_CTOR,_)

#undef BOOST_FUSION_PAIR_CTOR

#ifdef BOOST_NO_VARIADIC_TEMPLATES
        pair()
          : second()
        {}

#   ifdef BOOST_NO_RVALUE_REFERENCES
        pair(typename call_traits<second_type>::param_type second)
          : second(second)
        {}
#   else
        template<typename Arg>
        pair(Arg&& arg);
          : second(std::forward<Arg>(arg))
        {}
#   endif
#else
        template<typename... Args>
        pair(BOOST_FUSION_R_ELSE_CLREF(Args)... args)
          : second(BOOST_FUSION_FORWARD(Args,args)...)
        {}
#endif

        template <typename Second2>
        pair& operator=(pair<First, Second2>
            BOOST_FUSION_R_ELSE_CLREF(BOOST_PP_EMPTY()) rhs)
        {
            second = rhs.second;
            return *this;
        }

        second_type second;
    };

    namespace result_of
    {
        template<typename First, typename Second>
        struct make_pair
        {
            typedef
                fusion::pair<
                    First
                  , typename traits::deduce<Second>::type
                >
            type;
        };

        template<typename Pair>
        struct first
        {
            typedef typename Pair::begin_type type;
        };

        template<typename Pair>
        struct second
        {
            typedef typename Pair::second_type type;
        };
    }

    template <typename First, typename Second>
    inline typename
        result_of::make_pair<First,BOOST_FUSION_R_ELSE_CLREF(Second)>::type
    make_pair(BOOST_FUSION_R_ELSE_CLREF(Second) second)
    {
        return typename
            result_of::make_pair<
                First
              , BOOST_FUSION_R_ELSE_CLREF(Second)
            >::type(BOOST_FUSION_FORWARD(Second,second));
    }

    template <typename OStream, typename First, typename Second>
    inline BOOST_FUSION_R_ELSE_LREF(OStream)
    operator<<(
            BOOST_FUSION_R_ELSE_LREF(OStream) os,
            pair<First, Second> const& p)
    {
        os << p.second;
        return os;
    }

    template <typename IStream, typename First, typename Second>
    inline BOOST_FUSION_R_ELSE_LREF(IStream)
    operator>>(
            BOOST_FUSION_R_ELSE_LREF(IStream) is
          , pair<First, Second> BOOST_FUSION_R_ELSE_LREF(BOOST_PP_EMPTY()) p)
    {
        is >> static_cast<
            pair<First, Second>BOOST_FUSION_R_ELSE_LREF(BOOST_PP_EMPTY())
        >(p).second;
        return is;
    }

    template <typename First, typename SecondLhs, typename SecondRhs>
    inline bool
    operator==(pair<First, SecondLhs> const& lhs,
            pair<First, SecondRhs> const& rhs)
    {
        return lhs.second == rhs.second;
    }

    template <typename First, typename SecondLhs, typename SecondRhs>
    inline bool
    operator!=(pair<First, SecondLhs> const& lhs,
            pair<First, SecondRhs> const& rhs)
    {
        return lhs.second != rhs.second;
    }
}}

#endif
