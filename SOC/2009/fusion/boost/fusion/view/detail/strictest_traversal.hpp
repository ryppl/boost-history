/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_DETAIL_STRICTEST_TRAVERSAL_HPP
#define BOOST_FUSION_VIEW_DETAIL_STRICTEST_TRAVERSAL_HPP

#include <boost/fusion/algorithm/iteration/fold.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/internal/ref.hpp>

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        struct strictest_traversal_impl
        {
            template<typename Sig>
            struct result;

            template<typename Self, typename StrictestSoFar, typename NextSeq>
            struct result<Self(StrictestSoFar, NextSeq)>
            {
                typedef typename traits::category_of<NextSeq>::type next_tag;
                typedef typename
                    identity<StrictestSoFar>::type
                strictest_so_far_identity;

                typedef typename
                    mpl::if_<
                        is_base_of<strictest_so_far_identity,next_tag>
                      , strictest_so_far_identity
                      , next_tag
                    >::type
                type;
            };

            template<typename NextSeq,typename StrictestSoFar>
            typename result<
                strictest_traversal_impl(NextSeq const&, StrictestSoFar const&)
            >::type
            operator()(NextSeq const&,StrictestSoFar const&);
        };

        template<typename Seq>
        struct strictest_traversal
          : result_of::fold<
                Seq
              , fusion::random_access_traversal_tag
              , strictest_traversal_impl
            >
        {};
    }
}}

#endif
