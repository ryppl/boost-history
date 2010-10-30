/*==============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2006 Tobias Schwinger
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_PAIR_HPP
#define BOOST_FUSION_SUPPORT_PAIR_HPP

#include <boost/fusion/support/internal/base.hpp>
#include <boost/fusion/support/deduce.hpp>
#include <boost/fusion/support/internal/constexpr.hpp>
#ifndef BOOST_FUSION_NO_RVALUE_REFERENCES
#   include <boost/fusion/support/internal/is_explicitly_convertible.hpp>
#endif

#include <boost/preprocessor/empty.hpp>
#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
#   include <boost/call_traits.hpp>
#else
#   include <boost/utility/enable_if.hpp>
#endif

namespace boost { namespace fusion
{
    // A half runtime pair where the first type does not have data
    template<typename First, typename Second>
    struct pair
    {
        typedef First first_type;
        typedef Second second_type;

#define BOOST_FUSION_PAIR_CTOR(MODIFIER,_)\
        template<class OtherSecond>\
        pair(pair<First,OtherSecond> MODIFIER pair_)\
          : second(static_cast<pair<First,OtherSecond> MODIFIER>\
                (pair_).second)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_PAIR_CTOR,_)

#undef BOOST_FUSION_PAIR_CTOR

        BOOST_FUSION_CONSTEXPR
        pair()
          : second()
        {}

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
        pair(typename call_traits<second_type>::param_type second)
          : second(second)
        {}
#   else
        template<typename Arg>
        pair(Arg&& arg,
            typename enable_if<
                detail::is_explicitly_convertible<Arg&&,second_type>
            >::type* =0)
          : second(std::forward<Arg>(arg))
        {}
#   endif
#else
        template<typename Arg>
        pair(BOOST_FUSION_R_ELSE_CLREF(Arg) arg,
            typename enable_if<
                detail::is_explicitly_convertible<Arg&&,second_type>
            >::type* =0)
          : second(BOOST_FUSION_FORWARD(Arg,arg))
        {}

        template<typename Arg1, typename Arg2, typename... Args>
        pair(BOOST_FUSION_R_ELSE_CLREF(Arg1) arg1
          , BOOST_FUSION_R_ELSE_CLREF(Arg2) arg2
          , BOOST_FUSION_R_ELSE_CLREF(Args)... args)
          : second(BOOST_FUSION_FORWARD(Arg1,arg1),
              BOOST_FUSION_FORWARD(Arg2,arg2),
              BOOST_FUSION_FORWARD(Args,args)...)
        {}
#endif

        pair&
        operator=(pair const& other_pair)
        {
            second = other_pair.second;
            return *this;
        }

#define BOOST_FUSION_PAIR_ASSIGN_OP(MODIFIER,_)\
        template<class OtherSecond>\
        pair&\
        operator=(pair<First,OtherSecond> MODIFIER other_pair)\
        {\
            second=static_cast<pair<First,OtherSecond> MODIFIER>(\
                other_pair).second;\
            return *this;\
        }

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(
            BOOST_FUSION_PAIR_ASSIGN_OP,_)

#undef BOOST_FUSION_PAIR_ASSIGN_OP

        second_type second;
    };

    namespace result_of
    {
        template<typename First, typename Second>
        struct make_pair
        {
            typedef
                pair<
                    First
                  , typename traits::deduce<Second>::type
                >
            type;
        };

        template<typename Key, typename T>
        struct pair_tie
        {
            typedef
                pair<
                    Key
                  , typename detail::deduce_ref<T>::type
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

    template<typename First, typename Second>
    inline typename
        result_of::make_pair<First,BOOST_FUSION_R_ELSE_CLREF(Second)>::type
    make_pair(BOOST_FUSION_R_ELSE_CLREF(Second) second)
    {
        return typename result_of::make_pair<
            First
          , BOOST_FUSION_R_ELSE_CLREF(Second)
        >::type(BOOST_FUSION_FORWARD(Second,second));
    }

    template<typename Key, typename T>
    typename result_of::pair_tie<Key, BOOST_FUSION_R_ELSE_CLREF(T)>::type
    pair_tie(BOOST_FUSION_R_ELSE_LREF(T) t)
    {
        return typename result_of::pair_tie<
            Key
          , BOOST_FUSION_R_ELSE_CLREF(T)
        >::type(BOOST_FUSION_FORWARD(T,t));
    }

    template<typename OStream, typename First, typename Second>
    inline BOOST_FUSION_R_ELSE_LREF(OStream)
    operator<<(BOOST_FUSION_R_ELSE_LREF(OStream) os,
        pair<First, Second> const& p)
    {
        os << p.second;
        return os;
    }

    template<typename IStream, typename First, typename Second>
    inline BOOST_FUSION_R_ELSE_LREF(IStream)
    operator>>(BOOST_FUSION_R_ELSE_LREF(IStream) is,
        pair<First, Second> BOOST_FUSION_R_ELSE_LREF(BOOST_PP_EMPTY()) p)
    {
        is >> static_cast<
            pair<First, Second>BOOST_FUSION_R_ELSE_LREF(BOOST_PP_EMPTY())
        >(p).second;
        return is;
    }

    template<typename First, typename SecondLhs, typename SecondRhs>
    inline bool
    operator==(pair<First, SecondLhs> const& lhs,
            pair<First, SecondRhs> const& rhs)
    {
        return lhs.second == rhs.second;
    }

    template<typename First, typename SecondLhs, typename SecondRhs>
    inline bool
    operator!=(pair<First, SecondLhs> const& lhs,
            pair<First, SecondRhs> const& rhs)
    {
        return lhs.second != rhs.second;
    }
}}

#endif
