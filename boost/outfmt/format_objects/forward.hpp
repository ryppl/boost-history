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
      class                            basic_object;

      template< typename FormatType, class FmtObject = boost::io::basic_object >
      class                            wrapped_object;

      template< typename FormatType, class StateObject, bool pre = true, class FmtObject = boost::io::basic_object >
      class                            state_object;

      template< typename FormatType, class FmtObject1 = boost::io::basic_object, class FmtObject2 = boost::io::basic_object >
      class                            pair_object;

      template< typename FormatType, class FmtObject = boost::io::basic_object >
      class                            static_nary_object;

      template< typename FormatType, class FmtObject = boost::io::basic_object >
      class                            range_object;

      template< typename FormatType, class FmtObject = boost::io::basic_object >
      class                            array_object;

      template< typename FormatType, class FmtObject = boost::io::basic_object >
      class                            container_object;
   }}
#endif
