// (C) Copyright 2003: Reece H. Dunn

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_DETAIL_InputHelper_HPP
#define BOOST_IOFM_DETAIL_InputHelper_HPP
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
#        else
            typedef char                                             char_type;
            typedef std::string                                      string_type;
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
               return( !!is );
            }
            inline bool                          getch( char_type & ch )
            {
               is.get( ch );
               return( isgood());
            }
            inline bool                          isspace( char_type ch ) const
            {
#              if !defined(BOOST_IOFM_NO_BASIC_STREAM)
                  return( std::isspace( ch, is.getloc()));
#              else
                  return( std::isspace( ch ));
#              endif
            }
         public:
            inline void                          skipws()
            {
               char_type               ch;
               while( getch( ch ) && isspace( ch ))
                  ;
               is.putback( ch );
            }
         public: // string and character matching
            inline bool                          match( const string_type & s )
            {
               return( match( s.begin(), s.end()));
            }
            inline bool                          match( const char_type * s )
            {
#              if !defined(BOOST_IOFM_NO_BASIC_STREAM)
                  return( match( s, s + traits_type::length( s )));
#              else
                  return( match( s, s + ::strlen( s )));
#              endif
            }
            bool                                 match( char_type c )
            {
               char_type               ch;
#              if !defined(BOOST_IOFM_NO_BASIC_STREAM)
                  if( getch( ch ) && traits_type::eq( c, ch ))
                     return( true );
#              else
                  if( getch( ch ) && ( c == ch ))
                     return( true );
#              endif

               is.putback( ch );
#              if !defined(BOOST_IOFM_NO_BASIC_STREAM)
                  is.setstate( std::ios_base::failbit );
#              else
                  is.setstate( std::ios::failbit );
#              endif
               return( false );
            }
         private:
            template< typename ForwardIterator >
            bool                                 match( ForwardIterator first, ForwardIterator last )
            {
               skipws();

               // remove leading whitespace
               while(( first != last ) && isspace( *first ))
                  ++first;

               // remove trailing whitespace
               while(( first != last ) && isspace( *( last - 1 )))
                  --last;

               // compare
               while(( first != last ) && match( *first ))
                  ++first;

               return( first == last ); // did we get a match?
            }
         public: // constructors
            inline           input_helper( InputStream & in ): is( in )
            {
            }
      };
   }}}
#endif
