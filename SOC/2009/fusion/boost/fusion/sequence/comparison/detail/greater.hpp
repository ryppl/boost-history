/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_COMPARISON_DETAIL_GREATER_HPP
#define BOOST_FUSION_SEQUENCE_COMPARISON_DETAIL_GREATER_HPP

#include <boost/mpl/bool.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/equal_to.hpp>

namespace boost { namespace fusion { namespace detail
{
    template <typename Seq1, typename Seq2>
    struct sequence_greater
    {
        typedef typename result_of::end<Seq1>::type end1_type;
        typedef typename result_of::end<Seq2>::type end2_type;

        template <typename It1, typename I2>
        static bool
        call(It1 const&, I2 const&, mpl::true_)
        {
            return false;
        }

        template <typename It1, typename I2>
        static bool
        call(It1 const& a, It2 const& b, mpl::false_)
        {
            return fusion::deref(a) > fusion::deref(b)
                || !(fusion::deref(b) > fusion::deref(a))
                && call(fusion::next(a), fusion::next(b));
        }

        template <typename It1, typename It2>
        static bool
        call(It1 const& a, It2 const& b)
        {
            typename result_of::equal_to<It1, end1_type>::type eq;
            return call(a, b, eq);
        }
    };
}}}

#endif
