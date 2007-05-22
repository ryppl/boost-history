#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_ACQUIRE_IMPL_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_ACQUIRE_IMPL_HPP

#include <boost/config.hpp>

#ifdef BOOST_WINDOWS

#include <windows.h>

#if WINVER >= 0x0600

#include <boost/act/interlocked/assign/detail/assign_acquire_vista.hpp>

#else // Else: No support for assign acquire (default to assign)

#include <boost/act/interlocked/assign/detail/assign_acquire_default.hpp>

#endif

#else // Else: No support for assign acquire (default to assign)

#include <boost/act/interlocked/assign/detail/assign_acquire_default.hpp>

#endif

#endif
