/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_REVERSE_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_REVERSE_HPP

#include <boost/fusion/view/reverse_view/reverse_view.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Sequence>
        struct reverse
        {
            typedef reverse_view<Sequence> type;
        };
    }

    template <typename Sequence>
    inline reverse_view<Sequence const>
    reverse(Sequence const& view)
    {
        return reverse_view<Sequence const>(view);
    }
}}

#endif
