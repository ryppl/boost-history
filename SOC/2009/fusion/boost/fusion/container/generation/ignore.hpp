/*==============================================================================
    Copyright (c) 2001 Doug Gregor
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_GENERATION_IGNORE_HPP
#define BOOST_FUSION_CONTAINER_GENERATION_IGNORE_HPP

#include <boost/fusion/support/internal/base.hpp>

namespace boost { namespace fusion
{
    //  Swallows any assignment (by Doug Gregor)
    namespace detail
    {
        struct swallow_assign
        {
            template<typename T>
            swallow_assign const&
            operator=(T const&) const
            {
                return *this;
            }
        };
    }

    //  "ignore" allows tuple positions to be ignored when using "tie".
    detail::swallow_assign const ignore = detail::swallow_assign();
}}

#endif
