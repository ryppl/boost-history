// (C) Copyright 2003: Reece H. Dunn

#ifndef BOOST__IOFM__DETAIL__INPUT_HELPER__HPP
#define BOOST__IOFM__DETAIL__INPUT_HELPER__HPP
   namespace boost { namespace io { namespace detail
   {
      // helper class for handling input-related tasks

      template< class InputStream >
      class input_helper
      {
         public:
            typedef typename InputStream::char_type                  char_type;
            typedef typename InputStream::traits_type                traits_type;
         private:
            InputStream              & is;
         public:
            inline bool                          getch( char_type & ch )
            {
               return( static_cast< bool >( is.get( ch )));
            }
            inline bool                          isspace( char_type ch ) const
            {
               return( std::isspace( ch, is.getloc()));
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
            inline bool                          match( char_type * s )
            {
               return( match( s, s + traits_type::length( s )));
            }
            bool                                 match( char_type c )
            {
               char_type               ch;
               if( getch( ch ) && traits_type::eq( c, ch ))
                  return( true );

               is.putback( ch );
               is.setstate( std::ios_base::failbit );
               return( false );
            }
         private:
            bool                                 match( char_type * first, char_type * last )
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
