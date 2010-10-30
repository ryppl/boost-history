/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_ITERATOR_BASE_HPP
#define BOOST_FUSION_SUPPORT_ITERATOR_BASE_HPP

#include <boost/fusion/support/internal/base.hpp>

namespace boost { namespace fusion
{
    struct iterator_root
    {};

    template<typename It>
    struct iterator_base
      : iterator_root
    {
        It&
        cast()
        {
            return static_cast<It&>(*this);
        }

        It const&
        cast() const
        {
            return static_cast<It const&>(*this);
        }
    };
}}

#endif
