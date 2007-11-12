/* Copyright 2006 Joaquín M López Muñoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#ifndef BOOST_FLYWEIGHT_DETAIL_PP_TEMPLATE_CTORS_UNDEF_HPP
#define BOOST_FLYWEIGHT_DETAIL_PP_TEMPLATE_CTORS_UNDEF_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#undef BOOST_FLYWEIGHT_CTORS

#if BOOST_FLYWEIGHT_LIMIT_NUM_CTOR_ARGS>=1
#undef BOOST_FLYWEIGHT_CTORS_1
#endif

#if BOOST_FLYWEIGHT_LIMIT_NUM_CTOR_ARGS>=2
#undef BOOST_FLYWEIGHT_CTORS_2
#endif

#if BOOST_FLYWEIGHT_LIMIT_NUM_CTOR_ARGS>=3
#undef BOOST_FLYWEIGHT_CTORS_3
#endif

#if BOOST_FLYWEIGHT_LIMIT_NUM_CTOR_ARGS>=4
#undef BOOST_FLYWEIGHT_CTORS_4
#endif

#if BOOST_FLYWEIGHT_LIMIT_NUM_CTOR_ARGS>=5
#undef BOOST_FLYWEIGHT_CTORS_5
#endif

#endif
