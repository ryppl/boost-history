/*==============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_BASE_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_BASE_HPP

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>

#if !defined(BOOST_FUSION_NO_RVALUE_REFERENCES) &&                              \
    BOOST_WORKAROUND(__GNUC__,==4)&&                                            \
    BOOST_WORKAROUND(__GNUC_MINOR__,<5)
#   error The c++0x extension of your compiler is not supported!
#endif

#include <boost/fusion/support/internal/base/config.hpp>
#include <boost/fusion/support/internal/base/ref.hpp>
#include <boost/fusion/support/internal/base/assert.hpp>

#endif
