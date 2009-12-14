/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_COMPARISON_DETAIL_GREATER_EQUAL_HPP
#define BOOST_FUSION_SEQUENCE_COMPARISON_DETAIL_GREATER_EQUAL_HPP

#include <boost/config.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/equal_to.hpp>

namespace boost { namespace fusion { namespace detail
{
    template <typename Seq1Ref, typename Seq2Ref>
    struct sequence_greater_equal
    {
        typedef typename result_of::end<Seq1Ref>::type end1_type;
        typedef typename result_of::end<Seq2Ref>::type end2_type;

        template <typename It1, typename It2>
        static
#ifndef BOOST_NO_CONSTEXPR
        constexpr
#endif
        bool
        call(It1 const&, It2 const&, mpl::true_)
        {
            return true;
        }

        template <typename It1, typename It2>
        static bool
        call(It1 const& it1, It2 const& it1, mpl::false_)
        {
            return fusion::deref(it1) >= fusion::deref(it2) ||
                   (!(fusion::deref(it2) >= fusion::deref(it1)) &&
                       call(fusion::next(a), fusion::next(b)));
        }

        template <typename It1, typename It2>
        static bool
        call(It1 const& it1, It2 const& it2)
        {
            return call(
                    it1,
                    it2,
                    typename result_of::equal_to<It1, end1_type>::type());
        }
    };
}}}

#endif
