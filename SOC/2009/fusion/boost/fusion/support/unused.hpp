/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_UNUSED_HPP
#define BOOST_FUSION_SUPPORT_UNUSED_HPP

#include <boost/fusion/support/internal/base.hpp>
#ifdef BOOST_MSVC
#   pragma warning(push)
// multiple assignment operators specified warning
#   pragma warning(disable: 4522)
#endif

namespace boost { namespace fusion
{
    struct unused_type
    {
        unused_type()
        {}

        template<typename T>
        unused_type(T const&)
        {}

        template<typename T>
        unused_type const&
        operator=(T const&) const
        {
            return *this;
        }

        template<typename T>
        unused_type&
        operator=(T const&)
        {
            return *this;
        }

        unused_type const&
        operator=(unused_type const&) const
        {
            return *this;
        }

        unused_type&
        operator=(unused_type const&)
        {
            return *this;
        }
    };

    unused_type const unused = unused_type();
}}

#if defined(BOOST_MSVC)
#   pragma warning(pop)
#endif

#endif
