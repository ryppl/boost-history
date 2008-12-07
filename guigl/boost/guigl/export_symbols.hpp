/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
    Copyright 2007 Jeremy Pack
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#ifndef BOOST_EXPORT_SYMBOLS

#if __GNUC__ >= 4
#  define BOOST_EXPORT_SYMBOLS __attribute__((visibility("default")))
#  define BOOST_IMPORT_SYMBOLS __attribute__((visibility("hidden")))
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(MSC_VER)
#  define BOOST_EXPORT_SYMBOLS __declspec(dllexport)
#  define BOOST_IMPORT_SYMBOLS __declspec(dllimport)
#elif
#  define BOOST_EXPORT_SYMBOLS
#  define BOOST_IMPORT_SYMBOLS
#endif

#endif
