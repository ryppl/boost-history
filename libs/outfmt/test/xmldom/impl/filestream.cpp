// (C) Copyright 2004: Reece H. Dunn

#ifndef BOOST_XMLDOM_LITE_FileStream_CPP
#define BOOST_XMLDOM_LITE_FileStream_CPP
#  include "../filestream.hpp"

   char xmldom::detail::filestream::getch()
   {
      cch = 0;
      if( pbbuf.empty() && isgood )
      {
         isgood = !get(cch).fail(); // Converting to bool causes VC7.1 warning.
         if( cch == '\n' )             ++line;
      }
      else
      {
         cch = pbbuf.top();
         pbbuf.pop();
      }
      return( cch );
   }

   bool xmldom::detail::filestream::match( const char * s, long len, bool lookahead )
   {
      bool                             found = false;
      if( cch == s[ 0 ])
      {
         std::stack< char >            pb;
         pb.push( cch );
         --len;

         for( int i = 1; i <= len; ++i )
         {
            if( getch() == s[ i ])
            {
               pb.push( cch );
               if( i == len )
               {
                  if( !lookahead )     return( true );
                  found = true;
               }
            }
         }
         while( !pb.empty())
         {
            putback( pb.top());
            pb.pop();
         }
      }
      return( found );
   }
#endif
