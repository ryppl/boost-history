/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2007 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_DETAIL_ALL_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_DETAIL_ALL_HPP

#include <boost/config.hpp>
#include <boost/fusion/iterator/advance.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/deref.hpp>

#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion { namespace detail
{
    template<int N>
    struct unrolled_all
    {
        template <typename It0, typename F>
        static bool
        call(It0 const& it0, BOOST_FUSION_R_ELSE_CLREF(F) f)
        {
            if(!f(fusion::deref(it0)))
            {
                return false;
            }

            typedef typename result_of::next<It0 const&>::type It1;
            It1 it1 = fusion::next(it0);
            if(!f(fusion::deref(it1)))
            {
                return false;
            }

            typedef typename result_of::next<It1&>::type It2;
            It2 it2 = fusion::next(it1);
            if(!f(fusion::deref(it2)))
            {
                return false;
            }

            typedef typename result_of::next<It2&>::type It3;
            It3 it3 = fusion::next(it2);
            if(!f(fusion::deref(it3)))
            {
                return false;
            }

            return unrolled_all<N-4>::call(
                    fusion::next(it3),
                    BOOST_FUSION_FORWARD(F,f));
        }
    };

    template<>
    struct unrolled_all<3>
    {
        template <typename It0, typename F>
        static bool
        call(It0 const& it0, BOOST_FUSION_R_ELSE_CLREF(F) f)
        {
            if(!f(fusion::deref(it0)))
            {
                return false;
            }

            typedef typename result_of::next<It0 const&>::type It1;
            It1 it1 = fusion::next(it0);
            if(!f(fusion::deref(it1)))
            {
                return false;
            }

            typedef typename result_of::next<It1&>::type It2;
            It2 it2 = fusion::next(it1);
            return f(fusion::deref(it2));
        }
    };

    template<>
    struct unrolled_all<2>
    {
        template <typename It0, typename F>
        static bool
        call(It0 const& it0, BOOST_FUSION_R_ELSE_CLREF(F) f)
        {
            if(!f(fusion::deref(it0)))
            {
                return false;
            }

            typedef typename result_of::next<It0 const&>::type It1;
            It1 it1 = fusion::next(it0);
            return f(fusion::deref(it1));
        }
    };

    template<>
    struct unrolled_all<1>
    {
        template <typename It0, typename F>
        static bool
        call(It0 const& it0, BOOST_FUSION_R_ELSE_CLREF(F) f)
        {
            return f(fusion::deref(it0));
        }
    };

    template<>
    struct unrolled_all<0>
    {
        template <typename It0, typename F>
        static
#ifndef BOOST_NO_CONSTEXPR
        constexpr
#endif
        bool
        call(It0 const& it0, F const& f)
        {
            return true;
        }
    };
}}}

#endif
