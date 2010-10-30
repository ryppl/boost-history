/*==============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_CONSTEXPR_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_CONSTEXPR_HPP

#ifdef BOOST_NO_CONSTEXPR
#   define BOOST_FUSION_CONSTEXPR
#else
#   define BOOST_FUSION_CONSTEXPR constexpr
#endif

#endif
