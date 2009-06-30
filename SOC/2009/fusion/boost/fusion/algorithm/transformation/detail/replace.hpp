/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_REPLACE_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_REPLACE_HPP

namespace boost { namespace fusion { namespace detail
{
    template <typename OldValue>
    struct replacer
    {
        template <typename OldValue_>
        replacer(BOOST_FUSION_R_ELSE_LREF(OldValue_) old_value,int)
          : old_value(old_value)
        {}

        template <typename U>
        bool
        operator()(BOOST_FUSION_R_ELSE_LREF(U) x) const
        {
            return x==old_value;
        }

        OldValue old_value;
    };
}}}

#endif
