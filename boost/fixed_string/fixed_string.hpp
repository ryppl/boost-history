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

         template< typename CharT, class Policy = std::char_traits< CharT > >
         struct fixed_string_iface
         {
            typedef std::basic_string< CharT, Policy >               substring_type;
            typedef Policy                                           traits_type;
            typedef std::allocator< CharT >                          allocator_type;
            typedef typename allocator_type::pointer                 iterator;
            typedef typename allocator_type::const_pointer           const_iterator;

            typedef typename allocator_type::size_type               size_type;
            typedef typename allocator_type::reference               reference;
            typedef typename allocator_type::const_reference         const_reference;

            virtual iterator                     begin_() = 0;
            virtual const_iterator               begin_() const = 0;
            virtual iterator                     end_() = 0;
            virtual const_iterator               end_() const = 0;

            virtual       CharT *                buffer_() = 0;
            virtual const CharT *                data_()  const = 0;
            virtual const CharT *                c_str_() const = 0;
            virtual reference                    at_( size_type ) = 0;
            virtual const_reference              at_( size_type ) const = 0;

            virtual size_type                    length_() const = 0;
            virtual size_type                    capacity_() const = 0;
            virtual size_type                    max_size_() const = 0;

            virtual void                         resize_(  size_type, char ) = 0;
            virtual void                         reserve_( size_type ) = 0;

            virtual void                         assign_( const CharT *, size_type ) = 0;
            virtual void                         append_( const CharT *, size_type ) = 0;
            virtual int                          format_( const CharT *, va_list )   = 0;
            virtual void                         push_back_( CharT ) = 0;
         };
      }

      template< typename CharT, class CharStringPolicy = std::char_traits< CharT > >
      struct fixed_string_base: public detail::basic_string_impl< detail::fixed_string_iface< CharT, CharStringPolicy > >
      {
         public: // extended interface
            inline                operator const CharT *() const
            {
               return( c_str());
            }
            inline CharT *                       buffer()
            {
               return( buffer_());
            }
            inline int                           format( const CharT * fmt, va_list args )
            {
               return( format_( fmt, args ));
            }
      };

      typedef fixed_string_base< char >                    char_string;
      typedef fixed_string_base< wchar_t >                 wchar_string;

      template< size_t n, typename CharT = char, class CharStringPolicy = std::char_traits< CharT > >
      class fixed_string: public fixed_string_base< CharT, CharStringPolicy >
      {
         private:
            CharT                      str[ n ];
            size_t                     len;
         public: // validation checks
            struct ok{ typedef char value; };
            // zero-buffer check: n != 0
            struct zero_buffer_error{};
            typedef typename mpl::if_< mpl::equal_to< mpl::int_< n >, mpl::int_< 0 > >, 
                                       zero_buffer_error, ok
                                     >::type::value                  zero_buffer_check;
            // string policy check: StringPolicy::char_type == CharT
            struct char_string_policy_error{};
            typedef typename mpl::if_< is_same< typename CharStringPolicy::char_type, CharT >,
                                       ok, char_string_policy_error
                                     >::type::value                  char_string_policy_check;
         public:
            typedef fixed_string_base< CharT, CharStringPolicy >     base_type;
            typedef typename base_type::size_type                    size_type;
            typedef typename base_type::iterator                     iterator;
            typedef typename base_type::const_iterator               const_iterator;
            typedef typename base_type::reference                    reference;
            typedef typename base_type::const_reference              const_reference;
         private: // iterators
            inline iterator                      begin_()
            {
               return( str );
            }
            inline const_iterator                begin_() const
            {
               return( str );
            }
            inline iterator                      end_()
            {
               return( str + len );
            }
            inline const_iterator                end_() const
            {
               return( str + len );
            }
         private: // access
            inline CharT *                       buffer_()
            {
               return( str );
            }
            inline const CharT *                 data_() const
            {
               return( str );
            }
            inline const CharT *                 c_str_() const
            {
               return( str );
            }
            inline reference                     at_( size_type i )
            {
               return( str[ i ]);
            }
            inline const_reference               at_( size_type i ) const
            {
               return( str[ i ]);
            }
         private: // size and capacity
            inline size_type                     length_() const
            {
               return( len );
            }
            inline size_type                     capacity_() const
            {
               return( n );
            }
            inline size_type                     max_size_() const
            {
               return( capacity());
            }
            inline void                          resize_( size_t sz, CharT c )
            {
               if( sz > len )          CharStringPolicy::assign( str + len, sz - len, c );
               len = sz;
               str[ len ] = CharT();
            }
            inline void                          reserve_( size_type sz )
            {
               if( sz < len )
               {
                  len = sz;
                  str[ len ] = CharT();
               }
            }
         private: // string operations
            inline void                          assign_( const CharT * s, size_type l )
            {
               if( l == npos )         l = CharStringPolicy::length( s );
               len = ( l > n ) ? ( n - 1 ) : l;
               CharStringPolicy::copy( str, s, len );
               str[ len ] = CharT();
            }
            inline void                          append_( const CharT * s, size_type l = npos )
            {
               if( l == npos )         l = CharStringPolicy::length( s );
               l = (( l + len ) > n ) ? ( n - len ) : l;
               CharStringPolicy::copy( str + len, s, l );
               len += l - 1;
               str[ len ] = CharT();
            }
            inline int                           format_( const CharT * fmt, va_list args )
            {
               int                     ret = detail::format_policy< CharT >::format( str, n, fmt, args );
               len = ( ret == -1 ) ? ( n - 1 ) : ret;
               str[ len ] = CharT();
               return( ret );
            }
            inline void                          push_back_( CharT c )
            {
               if( len < ( n - 1 ))
               {
                  str[   len ] = c;
                  str[ ++len ] = CharT();
               }
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
