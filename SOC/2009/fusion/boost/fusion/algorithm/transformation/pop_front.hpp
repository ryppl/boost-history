/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_POP_FRONT_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_POP_FRONT_HPP

#include <boost/fusion/view/iterator_range/iterator_range.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/next.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq>
        struct pop_front
        {
            typedef 
                iterator_range<
                    typename next<
                        typename begin<Seq>::type
                    >::type
                  , typename end<Seq>::type
                > 
            type;
        };
    }

    template <typename Seq>
    inline typename result_of::pop_front<Seq const&>::type
    pop_front(Seq const& seq)
    {
        typedef typename result_of::pop_front<Seq const&>::type result;
        return result(fusion::next(fusion::begin(seq)), fusion::end(seq));
    }
}}

#endif
