// (C) Copyright 2003-2004: Reece H. Dunn

#ifndef BOOST_INDENT_HPP
#define BOOST_INDENT_HPP
#  include <boost/noncopyable.hpp>
#  include <boost/outfmt/detail/config.hpp>

   namespace boost { namespace detail
   {
      template< class OutputFileType >
      class indentor: public OutputFileType,
                      public boost::noncopyable
      {
         public:
#           if !defined(BOOST_IOFM_NO_BASIC_STREAM)
               typedef std::ios_base                                 iosbase_type;
               typedef typename OutputFileType::char_type            fill_type;
#           else
               typedef std::ios                                      iosbase_type;
               typedef char                                          fill_type;
#           endif
         private: // indentation
            struct indent_info
            {
               long                    level;
               fill_type               fill;
               char                    length;

               inline        indent_info():
                  level(  0 ),
                  fill(   fill_type( ' ' )),
                  length( 3 )
               {
               }
            }                          m_indent;
         public:
                   indentor &                    indent( bool = false );
            inline indentor &                    beginIndent() throw();
            inline indentor &                    endIndent()   throw();
            inline void                          set_indent( char = 3, fill_type = fill_type( ' ' )) throw();
         public:
            template< typename T >
            inline indentor &     operator<<( const T & ob )
            {
               *static_cast< OutputFileType * >( this ) << ob;
               return( *this );
            }
         public:
            class block
            {
               private:
                  indentor *           i;
               public:
                  inline     block( indentor & in ) throw(): i( &in )
                  {
                     i -> beginIndent();
                  }
                  inline    ~block() throw()
                  {
                     i -> endIndent();
                  }
            };
         public:
            inline           indentor() throw();
            inline           indentor( const char * ) throw();
            inline           indentor( const char *, iosbase_type::openmode ) throw();
      };

      template< class OutputFileType >
      indentor< OutputFileType > & indentor< OutputFileType >::indent( bool nl )
      {
         if( nl )                      *static_cast< OutputFileType * >( this ) << '\n';
         if( m_indent.level == 0 )     return( *this );

#        if !defined(BOOST_IOFM_NO_BASIC_STREAM)
            fill_type                  f = fill();
            std::streamsize            w = width();

            fill(  m_indent.fill );
            width( m_indent.level * m_indent.length );

            *static_cast< OutputFileType * >( this ) << m_indent.fill;

            fill(  f );
            width( w );
#        else
            fill_type                  buf[ 300 ];
            std::memset( buf, m_indent.fill, 300 );
            *( buf + 299 ) = '\0';

            unsigned long              len = m_indent.length * m_indent.level;
            while( len > 0 )
            {
               if( len > 300 )            len -= 299;
               else
               {
                  *( buf + len ) = '\0';
                  len = 0;
               }
               *static_cast< OutputFileType * >( this ) << buf;
            }
#        endif

         return( *this );
      }

      template< class OutputFileType >
      indentor< OutputFileType > & indentor< OutputFileType >::beginIndent() throw()
      {
         ++m_indent.level;
         return( *this );
      }

      template< class OutputFileType >
      indentor< OutputFileType > & indentor< OutputFileType >::endIndent() throw()
      {
         if( m_indent.level > 0 )      --m_indent.level;
         return( *this );
      }

      template< class OutputFileType >
      void indentor< OutputFileType >::set_indent( char l, fill_type f ) throw()
      {
         m_indent.length = l;
         m_indent.fill   = f;
      }

      template< class OutputFileType >
      indentor< OutputFileType >::indentor() throw()
      {
      }

      template< class OutputFileType >
      indentor< OutputFileType >::indentor( const char * fn ) throw(): OutputFileType( fn )
      {
      }

      template< class OutputFileType >
      indentor< OutputFileType >::indentor( const char * fn, iosbase_type::openmode om ) throw(): OutputFileType( fn, om )
      {
      }
   }}
#endif
