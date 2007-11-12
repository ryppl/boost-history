/* Copyright 2006 Joaquín M López Muñoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#ifndef BOOST_FLYWEIGHT_DETAIL_TEMPLATE_CTORS_UNDEF_HPP
#define BOOST_FLYWEIGHT_DETAIL_TEMPLATE_CTORS_UNDEF_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <boost/detail/workaround.hpp>

#if BOOST_FLYWEIGHT_LIMIT_NUM_CTOR_ARGS<=5
#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
#include <boost/flyweight/detail/msvc60_template_ctors_undef.hpp>
#else
#include <boost/flyweight/detail/pp_template_ctors_undef.hpp>
#endif
#else
#include <boost/flyweight/detail/dyn_template_ctors_undef.hpp>
#endif

#endif
