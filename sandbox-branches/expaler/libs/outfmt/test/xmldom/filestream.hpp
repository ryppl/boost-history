// (C) Copyright 2004: Reece H. Dunn

#ifndef BOOST_XMLDOM_LITE_FileStream_HPP
#define BOOST_XMLDOM_LITE_FileStream_HPP
#  include <boost/noncopyable.hpp>
#  include <fstream> // std::ifstream
#  include <stack>   // std::stack
#  include <cctype>  // std::isspace
#  include <string>  // std::string

   // filestream -- implement a character stream from a file
   // *  provide a putback buffer independant of the std::ifstream implementation
   // *  provide line count and filename information (for error tracking)
   // *  provide lookahead/scanning functions to aid lexer implementation

   namespace xmldom { namespace detail
   {
      class filestream: public std::ifstream,
                        public boost::noncopyable
      {
         private:
            bool                    isgood;
            std::stack< char >      pbbuf;
         protected:
            long                    line;
            std::string             filename;
            char                    cch;
         public:
                   char                          getch();
            inline void                          putback( char );
         public:
            bool                                 match( const char *, long len, bool = false );
         public:
            inline bool                          eof() const;
            inline bool                          open( const std::string & );
         public:
            inline           filestream();
      };

      void filestream::putback( char ch )
      {
         pbbuf.push( ch );
      }

      bool filestream::eof() const
      {
         return( !isgood );
      }

      bool filestream::open( const std::string & fn )
      {
          static_cast< std::ifstream * >( this ) -> open( fn.c_str() );
         filename = fn;
         line     = 1;
         return(getch() != 0);
      }

      filestream::filestream():
         isgood( true ),
         line(   0 ),
         cch(    0 )
      {
      }
   }}
#endif
