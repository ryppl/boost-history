// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIXED_STRING_HPP
#define BOOST_FIXED_STRING_HPP
#  include <boost/fixed_string/detail/basic_string_impl.hpp>
#  include <boost/mpl/or.hpp>
#  include <boost/mpl/equal_to.hpp>
#  include <boost/mpl/int.hpp>
#  include <boost/type_traits/is_same.hpp>

#  include <string.h>
#  include <stdio.h>
#  include <stddef.h>

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

      // string interface for handling variable-capacity strings

      template< typename CharT, class Traits = std::char_traits< CharT > >
      struct fixed_string_base: public detail::basic_string_impl
                                       <
                                          fixed_string_base< CharT, Traits >, CharT, Traits,
                                          detail::string_policy< CharT, std::basic_string< CharT, Traits > >
                                       >
      {
         virtual const CharT *                   c_str()  const = 0;
         virtual const CharT *                   data()   const = 0;
         virtual CharT &                         at( size_t i ) = 0;
         virtual CharT *                         iter_offset(       ptrdiff_t ) = 0;
         virtual const CharT *                   const_iter_offset( ptrdiff_t ) const = 0;

         virtual size_t                          length()   const = 0;
         virtual size_t                          capacity() const = 0;
         virtual size_t                          max_size() const = 0;

         virtual void                            resize(  size_t, char ) = 0;
         virtual void                            reserve( size_t ) = 0;

         virtual CharT *                         assign( const CharT * s, size_t l = size_t( -1 )) = 0;
         virtual CharT *                         append( const CharT * s, size_t l = size_t( -1 )) = 0;
         virtual void                            push_back( CharT ) = 0;
         virtual int                             format( const CharT * fmt, va_list args ) = 0;
      };

      typedef fixed_string_base< char >                    char_string;
      typedef fixed_string_base< wchar_t >                 wchar_string;

      // fixed capacity string implementation (std::basic_string-style interface)

      template< size_t n, typename CharT = char, class StringPolicy = std::char_traits< CharT > >
      class fixed_string: public fixed_string_base< CharT, StringPolicy >
      {
         private:
            CharT                      str[ n ];
            size_t                     len;
         public:
            typedef fixed_string_base< CharT, StringPolicy >         base_type;
            typedef typename base_type::size_type                    size_type;
            typedef typename base_type::difference_type              difference_type;
            typedef typename base_type::reference                    reference;
            typedef typename base_type::const_reference              const_reference;
         public: // validation checks
            struct ok{ typedef char value; };
            // zero-buffer check: n != 0
            struct zero_buffer_error{};
            typedef typename mpl::if_< mpl::equal_to< mpl::int_< n >, mpl::int_< 0 > >, 
                                       zero_buffer_error, ok
                                     >::type::value                  zero_buffer_check;
            // string policy check: StringPolicy::char_type == CharT
            struct string_policy_error{};
            typedef typename mpl::if_< is_same< typename StringPolicy::char_type, CharT >,
                                       ok, string_policy_error
                                     >::type::value                  string_policy_check;
         public: // access
            inline                operator const CharT *() const
            {
               return( str );
            }
            inline const CharT *                 data() const
            {
               return( str );
            }
            inline const CharT *                 c_str() const
            {
               return( str );
            }
            inline reference                     at( size_type i )
            {
               if( i >= n )            throw( std::out_of_range( "" ));
               return( str[ i ]);
            }
            inline CharT *                       iter_offset( difference_type off )
            {
               return( str + off );
            }
            inline const CharT *                 const_iter_offset( difference_type off ) const
            {
               return( str + off );
            }
         public: // size and capacity
            inline size_type                     length() const
            {
               return( len );
            }
            inline size_type                     capacity() const
            {
               return( n );
            }
            inline size_type                     max_size() const
            {
               return( capacity());
            }
            inline void                          resize( size_t sz, CharT c )
            {
               if( sz > len )          StringPolicy::assign( str + len, sz - len, c );
               len = sz;
               str[ len ] = CharT();
            }
            inline void                          reserve( size_type sz )
            {
               if( sz < len )
               {
                  len = sz;
                  str[ len ] = CharT();
               }
            }
         public: // string operations
            inline CharT *                       assign( const CharT * s, size_type l = npos )
            {
               if( l == npos )         l = StringPolicy::length( s ) + 1;
               len = ( l > n ) ? ( n - 1 ) : l;
               CharT *                 ret = StringPolicy::copy( str, s, len );
               str[ len ] = CharT();
               return( ret );
            }
            inline CharT *                       append( const CharT * s, size_type l = npos )
            {
               if( l == npos )         l = StringPolicy::length( s ) + 1;
               l = (( l + len ) > n ) ? ( n - len ) : l;
               CharT *                 ret = StringPolicy::copy( str + len - 1, s, l );
               len += l - 1;
               str[ len ] = CharT();
               return( ret );
            }
            inline void                          push_back( CharT c )
            {
               if( len < ( n - 1 ))
               {
                  str[   len ] = c;
                  str[ ++len ] = CharT();
               }
            }
            inline int                           format( const CharT * fmt, va_list args )
            {
               int                     ret = detail::format_policy< CharT >::format( str, n, fmt, args );
               len = ( ret == -1 ) ? ( n - 1 ) : ret;
               str[ len ] = CharT();
               return( ret );
            }
         public: // construction
            inline           fixed_string(): len( 0 )
            {
               str[ 0 ] = CharT();
            }
            inline           fixed_string( const fixed_string & s, size_type p = 0, size_type l = npos ): len( 0 )
            {
               assign( s.c_str(), p, l );
            }
            inline           fixed_string( const base_type & s, size_type p = 0, size_type l = npos ): len( 0 )
            {
               assign( s.c_str(), p, l );
            }
            inline           fixed_string( const CharT * s, size_type l ): len( 0 )
            {
               assign( s, l );
            }
            inline           fixed_string( const CharT * s ): len( 0 )
            {
               assign( s );
            }
            inline           fixed_string( size_type l, CharT c ): len( 0 )
            {
               assign( l, c );
            }
            template< typename InputIterator >
            inline           fixed_string( InputIterator first, InputIterator last ): len( 0 )
            {
               assign( first, last );
            }
      };
   }
#endif
