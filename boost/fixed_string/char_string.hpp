// (C) Copyright 2003-2004: Reece H. Dunn
// Use, modification, and distribution are subject to the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CHAR_STRING_HPP
#define BOOST_CHAR_STRING_HPP
#  include <boost/config.hpp>
#  include <boost/mpl/if.hpp>
#  include <boost/mpl/equal_to.hpp>
#  include <boost/mpl/int.hpp>
#  include <boost/type_traits/is_same.hpp>
#  include <iostream>
#  include <string.h>
#  include <stdio.h>
#  include <stddef.h>

#  if defined(BOOST_MSVC) && (BOOST_MSVC <= 1200) // MS VC6 has problems with 'const char( & s )[ m ]' syntax
#     define BOOST_STRING_NOSTD_REVITERATOR
#  endif

   namespace boost
   {
      namespace detail
      {
         template< typename CharT > class format_policy{};

         template<> class format_policy< char >
         {
            public:
               static inline int              format( char * s, size_t n, const char * fmt, va_list args )
               {
#                 if defined(BOOST_MSVC)
                     return( ::_vsnprintf( s, n, fmt, args ));
#                 else
                     return( ::vsnprintf( s, n, fmt, args ));
#                 endif
               }
         };

         template<> class format_policy< wchar_t >
         {
            public:
               static inline int              format( wchar_t * s, size_t n, const wchar_t * fmt, va_list args )
               {
#                 if defined(BOOST_MSVC)
                     return( ::_vsnwprintf( s, n, fmt, args ));
#                 else
                     return( ::vsnwprintf( s, n, fmt, args ));
#                 endif
               }
         };
      }

      // char and wchar_t string interfaces

      struct char_string
      {
         virtual const char *                    c_str()  const = 0;
         virtual size_t                          length() const = 0;
         virtual char *                          assign(  const char * s, size_t l = size_t( -1 )) = 0;
         virtual char *                          append(  const char * s, size_t l = size_t( -1 )) = 0;
         virtual int                             compare( const char * s, size_t l = size_t( -1 )) = 0;
         virtual int                             format(  const char * fmt, va_list args ) = 0;
      };
      
      struct wchar_string
      {
         virtual const wchar_t *                 c_str() const = 0;
         virtual size_t                          length() const = 0;
         virtual wchar_t *                       assign(  const wchar_t * s, size_t l = size_t( -1 )) = 0;
         virtual wchar_t *                       append(  const wchar_t * s, size_t l = size_t( -1 )) = 0;
         virtual int                             compare( const wchar_t * s, size_t l = size_t( -1 )) = 0;
         virtual int                             format(  const wchar_t * fmt, va_list args ) = 0;
      };
      
      // fixed capacity string implementation (std::basic_string-style interface)

      template< size_t n, typename CharT = char, class StringPolicy = std::char_traits< CharT > >
      class fixed_string: public mpl::if_< is_same< CharT, char >, char_string, wchar_string >::type
      {
         private:
            CharT                      str[ n ];
            size_t                     len;
         public:
            struct buffer_ok{ typedef char value; };
            struct zero_buffer_error{};
            typedef typename mpl::if_< mpl::equal_to< mpl::int_< n >, mpl::int_< 0 > >, 
                                       zero_buffer_error, buffer_ok 
                                     >::type::value                  zero_buffer_check;
         public: // types
            typedef StringPolicy                                     policy_type;
            typedef typename StringPolicy::char_type                 value_type;
         private:
            typedef std::allocator< value_type >                     allocator_type; // for type extraction only
         public:
            typedef typename allocator_type::size_type               size_type;
            typedef typename allocator_type::difference_type         difference_type;
            typedef typename allocator_type::reference               reference;
            typedef typename allocator_type::const_reference         const_reference;
            typedef typename allocator_type::pointer                 pointer;
            typedef typename allocator_type::const_pointer           const_pointer;
         public:
            typedef typename allocator_type::pointer                 iterator;
            typedef typename allocator_type::const_pointer           const_iterator;
#           if !defined(BOOST_STRING_NOSTD_REVITERATOR)
               typedef std::reverse_iterator< iterator >             reverse_iterator;
               typedef std::reverse_iterator< const_iterator >       const_reverse_iterator;
#           endif
         public: // iterators
            inline iterator                      begin()
            {
               return( str );
            }
            inline iterator                      end()
            {
               return( str + n );
            }
            inline const_iterator                begin() const
            {
               return( str );
            }
            inline const_iterator                end() const
            {
               return( str + n );
            }
#           if !defined(BOOST_STRING_NOSTD_REVITERATOR)
               inline reverse_iterator           rbegin()
               {
                  return( reverse_iterator( str + n - 1 ));
               }
               inline reverse_iterator           rend()
               {
                  return( reverse_iterator( str - 1 ));
               }
               inline const_reverse_iterator     rbegin() const
               {
                  return( const_reverse_iterator( str + n - 1 ));
               }
               inline const_reverse_iterator     rend() const
               {
                  return( const_reverse_iterator( str - 1 ));
               }
#           endif
         public: // access
            inline                operator const CharT *() const
            {
               return( str );
            }
            inline const CharT *                 c_str() const
            {
               return( str );
            }
            inline const CharT *                 data() const
            {
               return( str );
            }
         public:
            inline const_reference               at( size_type i ) const
            {
               if( i < 0 || i >= n )   throw( std::out_of_range( "boost::char_string" ));
               return( str[ i ]);
            }
            inline reference                     at( size_type i )
            {
               if( i < 0 || i >= n )   throw( std::out_of_range( "boost::char_string" ));
               return( str[ i ]);
            }
         public:
            inline const_reference operator[]( size_type i ) const
            {
               return( at( i ));
            }
            inline reference operator[]( size_type i )
            {
               return( at( i ));
            }
         public: // size and capacity
            inline size_type                     size() const
            {
               return( len );
            }
            inline size_type                     length() const
            {
               return( len );
            }
            inline bool                          empty() const
            {
               return( len == 0 );
            }
            inline size_type                     capacity() const
            {
               return( n );
            }
            inline size_type                     max_size() const
            {
               return( capacity());
            }
         public: // copy
            inline CharT *                       assign( const CharT * s, size_type l = size_type( -1 ))
            {
               if( l == size_type( -1 )) l = StringPolicy::length( s ) + 1;
               len = ( l > n ) ? ( n - 1 ) : l;
               CharT *                 ret = StringPolicy::copy( str, s, len );
               str[ len ] = '\0';
               return( ret );
            }
            inline CharT *                       assign( const CharT * s, size_type off, size_type l )
            {
               if( l == size_type( -1 )) l = StringPolicy::length( s ) + 1;
               return( assign( s + off, l - off ));
            }
            template< size_t m >
            inline CharT *                       assign( const fixed_string< m > & s, size_type off = 0, size_type l = size_type( -1 ))
            {
               return( assign( s.c_str(), off, ( l >= m ) ? m : l ));
            }
            //                                   assign( size_type n, CharT ch );
            //                                   assign< Iterator >( Iterator first, Iterator last );
         public:
            template< size_t m >
            inline fixed_string & operator=( const fixed_string< m > & s )
            {
               assign( s );
               return( *this );
            }
            inline fixed_string & operator=( const CharT * s )
            {
               assign( s );
               return( *this );
            }
            inline fixed_string & operator=( CharT ch )
            {
               len = ( n > 1 ) ? 1 : 0;
               if( n > 1 )             str[ 0 ] = ch;
               str[ len ] = '\0';
               return( *this );
            }
         public: // concatenation
            inline CharT *                       append( const CharT * s, size_type l = size_type( -1 ))
            {
               if( l == size_type( -1 )) l = StringPolicy::length( s ) + 1;
               l = (( l + len ) > n ) ? ( n - len ) : l;
               CharT *                 ret = StringPolicy::copy( str + len - 1, s, l );
               len += l;
               str[ len ] = '\0';
               return( ret );
            }
            inline CharT *                       append( const CharT * s, size_type off, size_type l )
            {
               if( l == size_type( -1 )) l = StringPolicy::length( s ) + 1;
               return( append( s + off, l - off ));
            }
            template< size_t m >
            inline CharT *                       append( const fixed_string< m > & s, size_type off = 0, size_type l = size_type( -1 ))
            {
               return( append( s.c_str(), off, ( l >= m ) ? m : l ));
            }
         public:
            template< size_t m >
            inline fixed_string & operator+=( const fixed_string< m > & s )
            {
               append( s );
               return( *this );
            }
            inline fixed_string & operator+=( const CharT * s )
            {
               append( s );
               return( *this );
            }
         public: // comparison
            inline int                           compare( const CharT * s, size_type l = size_type( -1 )) const
            {
               if( l == size_type( -1 )) l = StringPolicy::length( s ) + 1;
               return( StringPolicy::compare( str, s, ( l > n ) ? n : l ));
            }
            template< size_t m >
            inline int                           compare( const fixed_string< m > & s ) const
            {
               return( StringPolicy::compare( str, s, ( n > m ) ? m : n ));
            }
         public: // formatting
            inline int                           format( const CharT * fmt, va_list args )
            {
               int                     ret = detail::format_policy< CharT >::format( str, n, fmt, args );
               len = ( ret == -1 ) ? ( n - 1 ) : ret;
               str[ len ] = '\0';
               return( ret );
            }
         public: // construction
            inline           fixed_string(): len( 0 )
            {
               str[ 0 ] = '\0';
            }
            template< size_t m >
            inline           fixed_string( const fixed_string< m > & s, size_type p, size_type l = size_type( -1 )): len( 0 )
            {
               assign( s.c_str(), p, l );
            }
            inline           fixed_string( const CharT * s, size_type l = size_type( -1 ))
            {
               assign( s, l );
            }
      };

      template< size_t n,typename CharT, class StringPolicy >
      inline std::basic_ostream< CharT, StringPolicy > & 
                                  operator<<
                                  (
                                     std::basic_ostream< CharT, StringPolicy >    & os,
                                     const fixed_string< n, CharT, StringPolicy > & str
                                  )
      {
         return( os << str.c_str());
      }

      template< size_t n, typename CharT, class StringPolicy >
      bool operator==( const fixed_string< n, CharT, StringPolicy > & a, const fixed_string< n, CharT, StringPolicy > & b )
      {
         return( a.compare( b ) == 0 );
      }

      template< size_t n, typename CharT, class StringPolicy >
      bool operator==( const fixed_string< n, CharT, StringPolicy > & a, const CharT * b )
      {
         return( a.compare( b ) == 0 );
      }

      template< size_t n, typename CharT, class StringPolicy >
      bool operator==( const CharT * b, const fixed_string< n, CharT, StringPolicy > & a )
      {
         return( a.compare( b ) == 0 );
      }
   }
#endif
