// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_FormatObjects_Basic_HPP
#define BOOST_IOFM_FormatObjects_Basic_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#include <boost/outfmt/detail/select.hpp>

   namespace boost { namespace io
   {
      class basicfmt_t
      {
         public:
            typedef void                                             traits_type;
            typedef const char *                                     format_type;
            typedef seq_type< basic_type >                           formatter_type;
         public:
           template< typename T, class InputStream >
            static inline bool                   read( InputStream & is, T & value )
            {
               is >> value;
               return( is.isgood());
            }
            template< typename T, class OutputStream >
            static inline OutputStream &         write( OutputStream & os, const T & value )
            {
               return( os << value );
            }
         public:
            inline           basicfmt_t()
            {
            }
            inline           basicfmt_t( const basicfmt_t & )
            {
            }
      };

      inline basicfmt_t                          basicfmt()
      {
         basicfmt_t                  ob;
         return( ob );
      }
   }}
#endif
