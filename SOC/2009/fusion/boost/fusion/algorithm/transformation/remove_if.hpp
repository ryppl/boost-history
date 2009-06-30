/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_REMOVE_IF_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_REMOVE_IF_HPP

#include <boost/fusion/view/filter_view/filter_view.hpp>
#include <boost/mpl/not.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Sequence, typename Pred>
        struct remove_if
        {
            typedef filter_view<Sequence, mpl::not_<Pred> > type;
        };
    }

    template <typename Pred, typename Sequence>
    inline typename result_of::remove_if<Sequence const, Pred>::type
    remove_if(Sequence const& seq)
    {
        typedef typename
            result_of::remove_if<Sequence const, Pred>::type
        result_type;
        return result_type(seq);
    }
}}

#endif
