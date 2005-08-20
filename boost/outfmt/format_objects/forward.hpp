// (C) Copyright 2003: Reece H. Dunn 

#ifndef BOOST__IOFM__FORMAT_OBJECTS__FORWARD__HPP
#define BOOST__IOFM__FORMAT_OBJECTS__FORWARD__HPP
#  include <boost/outfmt/formatter.hpp>

   namespace boost { namespace io
   {
      class                            basic_output;

      template< typename FormatType, class Outputter = boost::io::basic_output >
      class                            wrapped_output;

      template< typename FormatType, class StateObject, bool pre = true, class Outputter = boost::io::basic_output >
      class                            state_output;

      template< typename FormatType, class Outputter1 = boost::io::basic_output, class Outputter2 = boost::io::basic_output >
      class                            pair_output;

      template< typename FormatType, class Outputter = boost::io::basic_output >
      class                            static_nary_output;

      template< typename FormatType, class Outputter = boost::io::basic_output >
      class                            array_output;

      template< typename FormatType, class Outputter = boost::io::basic_output >
      class                            container_output;
   }}
#endif
