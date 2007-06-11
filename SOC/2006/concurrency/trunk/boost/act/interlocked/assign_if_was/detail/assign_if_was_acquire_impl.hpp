/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_IF_WAS_DETAIL_ASSIGN_IF_WAS_ACQ_IMPL_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_IF_WAS_DETAIL_ASSIGN_IF_WAS_ACQ_IMPL_HPP

#include <boost/config.hpp>

#ifdef BOOST_WINDOWS

#include <windows.h>

#if WINVER >= 0x0600

#include <boost/act/interlocked/assign_if_was/detail/assign_if_was_acquire_vista.hpp>

#endif

#endif

#endif
