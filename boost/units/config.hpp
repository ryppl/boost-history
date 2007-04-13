// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_CONFIG_HPP
#define BOOST_UNITS_CONFIG_HPP

#include <boost/version.hpp>
#include <boost/config.hpp>

#if (BOOST_VERSION >= 103400)
    #define BOOST_UNITS_HAS_BOOST_TYPEOF    1
#else
    #define BOOST_UNITS_HAS_BOOST_TYPEOF    0
#endif

// uncomment this to test without Boost Typeof
//#undef BOOST_UNITS_HAS_BOOST_TYPEOF
//#define BOOST_UNITS_HAS_BOOST_TYPEOF      0   

#if (BOOST_UNITS_HAS_BOOST_TYPEOF)
    #include <boost/typeof/typeof.hpp>   
    #include <boost/typeof/std/complex.hpp>   
    #define BOOST_UNITS_HAS_TYPEOF          1
#else    
    #if (__GNUC__ && __cplusplus && __GNUC__ >= 3)
        #define BOOST_UNITS_HAS_TYPEOF          1
        #define BOOST_UNITS_HAS_GNU_TYPEOF      1
    #elif defined(__MWERKS__)
        #define BOOST_UNITS_HAS_TYPEOF          1
        #define BOOST_UNITS_HAS_MWERKS_TYPEOF   1
    #else
        #define BOOST_UNITS_HAS_TYPEOF          0
    #endif
#endif

// uncomment this to test without typeof support at all
//#undef BOOST_UNITS_HAS_TYPEOF
//#define BOOST_UNITS_HAS_TYPEOF          0

#ifndef BOOST_UNITS_NO_COMPILER_CHECK

    #ifdef BOOST_NO_MEMBER_TEMPLATES
        #error Boost.Units requires member template
    #endif

    #ifdef BOOST_NO_MEMBER_TEMPLATE_KEYWORD
        #error Boost.Units requires member template keyword
    #endif

    #ifdef BOOST_NO_INCLASS_MEMBER_INITIALIZATION
        #error Boost.Units requires in class member initialization
    #endif

    #ifdef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
        #error Boost.Units requires function template partial ordering
    #endif

    #ifdef BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS
        #error Boost.Units requires explicit function template arguments
    #endif

    #ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        #error Boost.Units requires partial specialization
    #endif

#endif

#ifdef BOOST_UNITS_REQUIRE_LAYOUT_COMPATIBILITY
    #define BOOST_UNITS_CHECK_LAYOUT_COMPATIBILITY(a, b) BOOST_STATIC_ASSERT((sizeof(a) == sizeof(b)))
#else
    #define BOOST_UNITS_CHECK_LAYOUT_COMPATIBILITY(a, b) ((void)0)
#endif

#endif
