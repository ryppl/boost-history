// (C) Copyright 2003-2004: Reece H. Dunn

#ifndef BOOST_CHAR_STRING_HPP
#define BOOST_CHAR_STRING_HPP
#  include <boost/config.hpp>
#  include <boost/mpl/if.hpp>
#  include <boost/mpl/equal_to.hpp>
#  include <boost/mpl/int.hpp>
#  include <iostream>
#  include <string.h>
#  include <stdio.h>
#  include <stddef.h>

#  if defined(BOOST_MSVC) && (BOOST_MSVC <= 1200) // MS VC6 has problems with 'const char( & s )[ m ]' syntax
#     define BOOST_STRING_NOCHARARRAY
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

      template< size_t n, bool null_terminate = true, typename CharT = char, class StringPolicy = std::char_traits< CharT > >
      class char_string
      {
         private:
            CharT                      str[ n ];
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
         public:
            inline const_reference operator[]( size_type i ) const
            {
               if( i < 0 || i >= n )             throw( std::out_of_range( "char_string< n >" ));
               return( str[ i ]);
            }
            inline reference operator[]( size_type i )
            {
               if( i < 0 || i >= n )             throw( std::out_of_range( "char_string< n >" ));
               return( str[ i ]);
            }
         public: // size and capacity
            inline size_type                     size() const
            {
               return( StringPolicy::length( str ));
            }
            inline size_type                     length() const
            {
               return( size());
            }
            inline bool                          empty() const
            {
               return( size() == 0 );
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
            inline CharT *                       copy( const CharT * s, size_type l = size_type( -1 ))
            {
               if( l == size_type( -1 )) l = StringPolicy::length( s ) + 1;
               CharT *                 ret = StringPolicy::copy( str, s, ( l > n ) ? n : l );
               if( null_terminate && l >= n )
                  str[ n - 1 ] = '\0';
               return( ret );
            }
            inline CharT *                       copy( const CharT * s, size_type off, size_type l )
            {
               if( l == size_type( -1 )) l = StringPolicy::length( s ) + 1;
               return( copy( s + off, l - off ));
            }
            template< size_t m >
            inline CharT *                       copy( const char_string< m > & s, size_type off = 0, size_type l = size_type( -1 ))
            {
               return( copy( s.c_str(), off, ( l >= m ) ? m : l ));
            }
#           if !defined(BOOST_MSVC) || (BOOST_MSVC > 1200)
            template< size_t m >
            inline CharT *                       copy( const CharT( & s )[ m ], size_type off = 0, size_type l = size_type( -1 ))
            {
               return( copy( static_cast< const CharT * >( s ), off, ( l >= m ) ? m : l ));
            }
#           endif
         public: // comparison
            inline int                           comp( const CharT * s, size_type l = size_type( -1 )) const
            {
               if( l == size_type( -1 )) l = StringPolicy::length( s ) + 1;
               return( StringPolicy::compare( str, s, ( l > n ) ? n : l ));
            }
            inline int                           comp( const char_string< n > & s ) const
            {
               return( StringPolicy::compare( str, s, n ));
            }
#           if !defined(BOOST_MSVC) || (BOOST_MSVC > 1200)
            template< size_t m >
            inline int                           comp( const CharT( & s )[ m ] ) const
            {
               return( comp( static_cast< const CharT * >( s ), m ));
            }
#           endif
         public: // formatting
            inline int                           format( const CharT * fmt, va_list args )
            {
               int                     ret = detail::format_policy< CharT >::format( str, n, fmt, args );
               if( null_terminate && ret >= n )
                  str[ n - 1 ] = '\0';
               return( ret );
            }
         public: // construction
            inline           char_string()
            {
               str[ 0 ] = '\0';
            }
            template< size_t m >
            inline           char_string( const char_string< m > & s, size_type p, size_type l = size_type( -1 ))
            {
               copy( s.c_str(), p, l );
            }
      };

      template< size_t n, bool nt, typename CharT, class StringPolicy >
      inline std::basic_ostream< CharT, StringPolicy > & 
                                  operator<<
                                  (
                                     std::basic_ostream< CharT, StringPolicy >       & os,
                                     const char_string< n, nt, CharT, StringPolicy > & str
                                  )
      {
         return( os << str.c_str());
      }

      template< size_t n, bool nt, typename CharT, class StringPolicy >
      bool operator==( const char_string< n, nt, CharT, StringPolicy > & a, const char_string< n, nt, CharT, StringPolicy > & b )
      {
         return( a.comp( b ) == 0 );
      }

#     if !defined(BOOST_MSVC) || (BOOST_MSVC > 1200)
      template< size_t n, size_t m, bool nt, typename CharT, class StringPolicy >
      bool operator==( const char_string< n, nt, CharT, StringPolicy > & a, const CharT( & b )[ m ] )
      {
         return( a.comp( b ) == 0 );
      }

      template< size_t n, size_t m, bool nt, typename CharT, class StringPolicy >
      bool operator==( const CharT( & b )[ m ], const char_string< n, nt, CharT, StringPolicy > & a )
      {
         return( a.comp( b ) == 0 );
      }
#     endif
   }
#endif
