// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_DETAIL_InputHelper_HPP
#define BOOST_IOFM_DETAIL_InputHelper_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/outfmt/detail/config.hpp>
#  include <boost/noncopyable.hpp>

   namespace boost { namespace io { namespace detail
   {
      // helper class for handling input-related tasks

      template< class InputStream >
      class input_helper: public boost::noncopyable
      {
         public:
#        if !defined(BOOST_IOFM_NO_BASIC_STREAM)
            typedef typename InputStream::char_type                  char_type;
            typedef typename InputStream::traits_type                traits_type;
            typedef std::basic_string< char_type, traits_type >      string_type;
            typedef std::ios_base                                    iosbase_type;
#        else
            typedef char                                             char_type;
            typedef std::string                                      string_type;
            typedef std::ios                                         iosbase_type;
#        endif
         private:
            InputStream              & is;
         public:
            template< typename T >
            inline input_helper & operator>>( T & t )
            {
               is >> t;
               return( *this );
            }
         public:
            inline bool                          isgood() const
            {
               return( !is.fail());
            }
            inline bool                          isspace( char_type ch ) const
            {
#              if !defined(BOOST_IOFM_NO_BASIC_STREAM)
                  return( std::isspace( ch, is.getloc()));
#              else
                  return( std::isspace( ch ));
#              endif
            }
            inline bool                          isnewline( char_type ch ) const
            {
               return(( ch == is.widen( '\n' )) || ( ch == is.widen( '\r' )));
            }
            inline bool                          eq( char_type c1, char_type c2 ) const
            {
#              if !defined(BOOST_IOFM_NO_BASIC_STREAM)
                  return( traits_type::eq( c1, c2 ));
#              else
                  return( c1 == c2 );
#              endif
            }
         public:
            inline bool                          failed() const
            {
               return( is.fail());
            }
            inline void                          clear
                                                 (
                                                    typename iosbase_type::iostate s =
#                                                   if defined(BOOST_MSVC)
                                                       0 // MS VC has problems with iosbase_type::goodbit
#                                                   else
                                                       iosbase_type::goodbit
#                                                   endif
                                                 )
            {
               is.clear( s );
            }
            inline void                          setstate( typename iosbase_type::iostate s )
            {
               is.setstate( s );
            }
         public:
            inline bool                          getch( char_type & ch )
            {
               is.get( ch );
               return( isgood());
            }
            inline bool                          readch( char_type & ch )
            {
               is.get( ch );
               is.putback( ch );
               return( isgood());
            }
            inline bool                          readfirstch( char_type & ch )
            {
               is >> ch;
               is.putback( ch );
               return( isgood());
            }
            inline void                          putback( char_type ch )
            {
               is.putback( ch );
            }
         public:
            inline char_type                     firstch( char_type ch ) const
            {
               return( ch );
            }
            inline char_type                     firstch( const char_type * s ) const
            {
               while( *s && isspace( *s ))
                  ++s;
               return( *s );
            }
         public:
            inline void                          skipws()
            {
               char_type               ch;
               do { } while( is.get( ch ) && isspace( ch )); // Suppress MW warning.
               is.putback( ch );
            }
         public: // string and character matching
            inline bool                          match( const string_type & s, bool restore = false )
            {
               return( match( s.begin(), s.end(), restore ));
            }
            inline bool                          match( const char_type * s, bool restore = false )
            {
#              if !defined(BOOST_IOFM_NO_BASIC_STREAM)
                  return( match( s, s + traits_type::length( s ), restore ));
#              else
                  return( match( s, s + ::strlen( s ), restore ));
#              endif
            }
            bool                                 match( char_type c )
            {
               char_type               ch;
               if( getch( ch ) && eq( c, ch ))
                  return( true );

               is.putback( ch );
               setstate( iosbase_type::failbit );
               return( false );
            }
         private:
            template< typename ForwardIterator >
            bool                                 match( ForwardIterator first, ForwardIterator last, bool restore = false )
            {
               skipws();

               // remove leading whitespace
               while(( first != last ) && isspace( *first ))
                  ++first;

               // remove trailing whitespace
               while(( first != last ) && isspace( *( last - 1 )))
                  --last;

               // compare
               ForwardIterator         rlast = first;
               --rlast; // [review]: check logic

               while(( first != last ) && match( *first ))
                  ++first;

               if( restore && failed())
               {
                  while( --last != rlast ) // [review]: check logic
                     is.putback( *last );

                  return( false );
               }
               return( true );
            }
         public: // constructors
            inline           input_helper( InputStream & in ): is( in )
            {
            }
      };
   }}}
#endif
