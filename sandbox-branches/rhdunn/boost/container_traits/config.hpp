// (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CONTAINER_TRAITS_CONFIG_HPP
#define BOOST_CONTAINER_TRAITS_CONFIG_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp>

#ifdef BOOST_CT_DEDUCED_TYPENAME
#error "macro already defined!"
#endif

#ifdef __BORLANDC__
#define BOOST_CT_DEDUCED_TYPENAME
#else
#define BOOST_CT_DEDUCED_TYPENAME BOOST_DEDUCED_TYPENAME
#endif

#ifdef BOOST_CT_NO_ARRAY_SUPPORT
#error "macro already defined!"
#endif

#if _MSC_VER <= 1200
#define BOOST_CT_NO_ARRAY_SUPPORT
#endif

#ifdef BOOST_CT_NO_ARRAY_SUPPORT
#define BOOST_ARRAY_REF (array)
#define BOOST_CT_NO_STATIC_ASSERT
#else
#define BOOST_ARRAY_REF (&array)
#endif



#endif

