/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_COMPARISON_DETAIL_NOT_EQUAL_TO_HPP
#define BOOST_FUSION_SEQUENCE_COMPARISON_DETAIL_NOT_EQUAL_TO_HPP

#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/support/internal/constexpr.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion { namespace detail
{
    template<typename Seq1Ref, typename Seq2Ref, bool same_size>
    struct sequence_not_equal_to
    {
        typedef typename result_of::end<Seq1Ref>::type end1_type;
        typedef typename result_of::end<Seq2Ref>::type end2_type;

        template<typename It1, typename It2>
        static BOOST_FUSION_CONSTEXPR
        bool
        call(It1 const&, It2 const&, mpl::true_)
        {
            return false;
        }

        template<typename It1, typename It2>
        static bool
        call(It1 const& it1, It2 const& it2, mpl::false_)
        {
            return fusion::deref(it1) != fusion::deref(it2) ||
                   call(fusion::next(it1), fusion::next(it2));
        }

        template<typename It1, typename It2>
        static bool
        call(It1 const& it1, It2 const& it2)
        {
            return call(
                    it1,
                    it2,
                    typename result_of::equal_to<It1, end1_type>::type());
        }
    };

    template<typename Seq1Ref, typename Seq2Ref>
    struct sequence_not_equal_to<Seq1Ref, Seq2Ref, false>
    {
        template<typename It1, typename It2>
        static bool
        call(It1 const&, It2 const&)
        {
            return true;
        }
    };
}}}

#endif
