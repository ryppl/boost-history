/*=============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2006 Tobias Schwinger

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_PAIR_HPP
#define BOOST_FUSION_SUPPORT_PAIR_HPP

#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/detail/as_fusion_element.hpp>

namespace boost { namespace fusion
{
    // A half runtime pair where the first type does not have data
    template <typename First, typename Second>
    struct pair
    {
        pair()
          : second()
        {}

#define PAIR_CTOR(COMBINATION,_)\
        template<typename OtherFirst,typename OtherSecond>\
        pair(pair<OtherFirst,OtherSecond> COMBINATION pair_)\
          : second(static_cast<pair<OtherFirst,OtherSecond> COMBINATION>(pair_)\
                  .second)\
        {}

        BOOST_FUSION_ALL_CV_REF_COMBINATIONS(PAIR_CTOR,_)

#undef PAIR_CTOR


#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   ifdef BOOST_NO_RVALUE_REFERENCES
        pair(Second const& second)
          : second(second)
        {}
#   else
        template<typename Arg>
        pair(Arg&& arg)
          : second(std::forward<Arg>(arg))
        {}
#   endif
#else
        template<typename Arg1,typename... Args>
        pair(Arg1&& arg1, Args&&... args)
          : second(std::forward<Arg1>(arg1),std::forward<Args>(args)...)
        {}
#endif

        template <typename Second2>
        pair& operator=(pair<First, Second2> const& rhs)
        {
            second = rhs.second;
            return *this;
        }

#ifndef BOOST_NO_RVALUE_REFERENCES
        template <typename Second2>
        pair& operator=(pair<First, Second2>&& rhs)
        {
            second = std::forward<Second2>(rhs.second);
            return *this;
        }
#endif

        typedef First first_type;
        typedef Second second_type;
        Second second;
    };

    namespace result_of
    {
        template<typename First, typename Second>
        struct make_pair
        {
            typedef fusion::pair<First,
                        typename detail::as_fusion_element<Second>::type> type;
        };

        template<class Pair>
        struct first
        {
            typedef typename Pair::first_type type;
        };

        template<class Pair>
        struct second
        {
            typedef typename Pair::second_type type;
        };
    }

    template <typename First, typename Second>
    inline typename
        result_of::make_pair<First,BOOST_FUSION_R_ELSE_CLREF(Second)>::type
    make_pair(BOOST_FUSION_R_ELSE_CLREF(Second) val)
    {
        return typename result_of::
            make_pair<First,BOOST_FUSION_R_ELSE_CLREF(Second)>::type(val);
    }

    template <typename OStream, typename First, typename Second>
    inline OStream&
    operator<<(OStream& os, pair<First, Second> const& p)
    {
        os << p.second;
        return os;
    }

    //TODO cschmidt: rref?!
    template <typename IStream, typename First, typename Second>
    inline IStream&
    operator>>(IStream& is, pair<First, Second> p)
    {
        is >> p.second;
        return is;
    }

    template <typename First, typename SecondL, typename SecondR>
    inline bool
    operator==(pair<First, SecondL> const& l, pair<First, SecondR> const& r)
    {
        return l.second == r.second;
    }

    template <typename First, typename SecondL, typename SecondR>
    inline bool
    operator!=(pair<First, SecondL> const& l, pair<First, SecondR> const& r)
    {
        return l.second != r.second;
    }
}}

#endif
