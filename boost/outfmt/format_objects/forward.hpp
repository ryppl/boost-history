// (C) Copyright 2003: Reece H. Dunn 

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_FormatObjects_Forward_HPP
#define BOOST_IOFM_FormatObjects_Forward_HPP
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
