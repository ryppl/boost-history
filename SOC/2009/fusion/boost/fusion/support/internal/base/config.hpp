/*==============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_BASE_CONFIG_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_BASE_CONFIG_HPP

#if !defined(BOOST_FUSION_NO_RVALUE_REFERENCES) &&                              \
    defined(BOOST_NO_RVALUE_REFERENCES)
#   define BOOST_FUSION_NO_RVALUE_REFERENCES
#endif

#ifndef BOOST_FUSION_UNROLLED_DEPTH
#   define BOOST_FUSION_UNROLLED_DEPTH 6
#endif

#endif
