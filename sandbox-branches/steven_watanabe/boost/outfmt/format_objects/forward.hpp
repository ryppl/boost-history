// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_FormatObjects_Forward_HPP
#define BOOST_IOFM_FormatObjects_Forward_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/outfmt/formatter.hpp>

   namespace boost { namespace io
   {
      class                            basicfmt_t;

      template< typename DelimeterType, class FormatObject = boost::io::basicfmt_t >
      class                            wrappedfmt_t;

      template< typename DelimeterType, class StateObject, bool pre = true, class FormatObject = boost::io::basicfmt_t >
      class                            statefmt_t;

      template< typename DelimeterType, class FormatObject1 = boost::io::basicfmt_t, class FormatObject2 = boost::io::basicfmt_t >
      class                            pairfmt_t;

      template< typename DelimeterType, class FormatObject = boost::io::basicfmt_t >
      class                            naryfmt_t;

      template< typename DelimeterType, class FormatObject = boost::io::basicfmt_t >
      class                            rangefmt_t;

      template< typename DelimeterType, class FormatObject = boost::io::basicfmt_t >
      class                            arrayfmt_t;

      template< typename DelimeterType, class FormatObject = boost::io::basicfmt_t >
      class                            containerfmt_t;
   }}
#endif
