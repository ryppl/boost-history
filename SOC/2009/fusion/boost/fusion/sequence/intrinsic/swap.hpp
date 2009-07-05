/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_SWAP_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_SWAP_HPP

#include <boost/config.hpp>
#ifndef BOOST_NO_RVALUE_REFERENCES
#   include <boost/fusion/view/zip_view.hpp>
#   include <boost/fusion/algorithm/iteration/for_each.hpp>
#   include <boost/fusion/sequence/intrinsic/front.hpp>
#   include <boost/fusion/sequence/intrinsic/back.hpp>
#endif

#include <algorithm>

namespace boost { namespace fusion {
#ifdef BOOST_NO_RVALUE_REFERENCES
    namespace detail
    {
        struct swap
        {
            template<typename Elem>
            struct result
            {
                typedef void type;
            };

            template<typename Elem>
            void operator()(Elem const& e) const
            {
                using std::swap;
                swap(front(e), back(e));
            }
        };
    }
#endif

    namespace result_of
    {
        template<typename Seq1, typename Seq2>
        struct swap
        {
            typedef void type;
        };
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template<typename Seq1, typename Seq2>
    void
    swap(Seq1& lhs, Seq2& rhs)
    {
        typedef vector<Seq1&, Seq2&> references;
        for_each(zip_view<references>(references(lhs, rhs)), detail::swap());
    }
#else
    using std::swap;
#endif
}}

#endif
