// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIXED_STRING_HPP
#define BOOST_FIXED_STRING_HPP
#  include <boost/fixed_string/detail/basic_string_impl.hpp>
#  include <boost/mpl/and.hpp>
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

            virtual void                         resize_(  size_type, CharT ) = 0;
            virtual void                         reserve_( size_type ) = 0;

            virtual void                         assign_( const CharT *, size_type ) = 0;
            virtual void                         append_( const CharT *, size_type ) = 0;
            virtual int                          format_( const CharT *, va_list )   = 0;
            virtual void                         push_back_( CharT ) = 0;

            virtual void                         swap_( fixed_string_iface< CharT, Policy > & ) = 0;
         };
      }

      template< typename CharT, class CharStringPolicy = std::char_traits< CharT > >
      struct fixed_string_base: public detail::basic_string_impl< detail::fixed_string_iface< CharT, CharStringPolicy > >
      {
         public:
            typedef detail::basic_string_impl< detail::fixed_string_iface< CharT, CharStringPolicy > >
                                                                     impl_type;
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
            BOOST_STATIC_CONSTANT( size_t, storage_c  = n + 1 );
            BOOST_STATIC_CONSTANT( size_t, capacity_c = n );
         public:
            typedef fixed_string_base< CharT, CharStringPolicy >::impl_type base_type;
            typedef fixed_string< n, CharT, CharStringPolicy >       this_type;
         private:
            CharT                      str[ storage_c ];
            size_t                     len;
         public: // validation checks
            struct ok{ typedef char value; };
            // string policy check: StringPolicy::char_type == CharT
            struct char_string_policy_error{};
            typedef typename mpl::if_< is_same< typename CharStringPolicy::char_type, CharT >,
                                       ok, char_string_policy_error
                                     >::type::value                  char_string_policy_check;
         public:
            typedef typename base_type::traits_type                  triats_type;
            typedef typename base_type::value_type                   value_type;
            typedef typename base_type::char_type                    char_type;
            typedef typename base_type::allocator_type               allocator_type;
            typedef typename base_type::size_type                    size_type;
            typedef typename base_type::difference_type              difference_type;
            typedef typename base_type::reference                    reference;
            typedef typename base_type::const_reference              const_reference;
            typedef typename base_type::reference                    pointer;
            typedef typename base_type::const_reference              const_pointer;
            typedef typename base_type::iterator                     iterator;
            typedef typename base_type::const_iterator               const_iterator;
            typedef typename base_type::reverse_iterator             reverse_iterator;
            typedef typename base_type::const_reverse_iterator       const_reverse_iterator;
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
               return( capacity_c );
            }
            inline size_type                     max_size_() const
            {
               return( capacity_());
            }
            inline void                          resize_( size_t sz, CharT c )
            {
               if( sz > len )
               {
                  if( sz >= capacity_c )
                     sz = capacity_c;
                  triats_type::assign( str + len, sz - len, c );
               }
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
               if( l == npos )         l = triats_type::length( s );
               len = ( l >= capacity_c ) ? capacity_c : l;
               triats_type::copy( str, s, len );
               str[ len ] = CharT();
            }
            inline void                          append_( const CharT * s, size_type l = npos )
            {
               if( l == npos )         l = triats_type::length( s );
               l = (( l + len ) >= capacity_c ) ? ( capacity_c - len ) : l;
               triats_type::copy( str + len, s, l );
               len += l;
               str[ len ] = CharT();
            }
            inline int                           format_( const CharT * fmt, va_list args )
            {
               int                     ret = detail::format_policy< CharT >::format( str, capacity_c, fmt, args );
               len = ( ret == -1 ) ? capacity_c : ret;
               str[ len ] = CharT();
               return( ret );
            }
            inline void                          push_back_( CharT c )
            {
               if( len < capacity_c )
               {
                  str[   len ] = c;
                  str[ ++len ] = CharT();
               }
            }
            inline void                          swap_( boost::detail::fixed_string_iface< CharT, CharStringPolicy > & b )
            {
               this_type               a( b.data_(), b.length_());
               b.assign_( data(), length());
               *this = a;
            }
         public:
            inline this_type &    operator=( const this_type & s )
            {
               this_type::assign_( s.str, s.len );
               return( *this );
            }
            inline this_type &    operator=( const base_type & s )
            {
               this_type::assign_( s.c_str(), s.length());
               return( *this );
            }
            inline this_type &    operator=( const value_type * s )
            {
               this_type::assign_( s, triats_type::length( s ));
               return( *this );
            }
            inline this_type &    operator=( value_type c )
            {
               len = 0;
               this_type::push_back_( c );
               return( *this );
            }
         public: // construction
            inline           fixed_string(): len( 0 )
            {
               str[ 0 ] = CharT();
            }
            inline           fixed_string( const this_type & s ): len( 0 )
            {
               this_type::assign_( s.str, s.len );
            }
            inline           fixed_string( const base_type & s, size_type p = 0, size_type l = npos ): len( 0 )
            {
               assign( s, p, l );
            }
            inline           fixed_string( const CharT * s, size_type l ): len( 0 )
            {
               this_type::assign_( s, l );
            }
            inline           fixed_string( const CharT * s ): len( 0 )
            {
               this_type::assign_( s, traits_type::length( s ));
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
      
      // lhs + rhs

      template< size_t n1, size_t n2, class C, class CSP >
      inline fixed_string< ( n1 + n2 ), C, CSP > 
                                  operator+
                                  ( 
                                     const fixed_string< n1, C, CSP > & lhs,
                                     const fixed_string< n2, C, CSP > & rhs
                                  )
      {
         return( fixed_string< ( n1 + n2 ), C, CSP >( lhs ).append( rhs ));
      }

      template< size_t n, class C, class CSP >
      inline fixed_string< n, C, CSP > 
                                  operator+
                                  ( 
                                     const fixed_string< n, C, CSP > & lhs,
                                     const typename fixed_string< n, C, CSP >::value_type * rhs
                                  )
      {
         return( fixed_string< n, C, CSP >( lhs ).append( rhs ));
      }

      template< size_t n, class C, class CSP >
      inline fixed_string< n, C, CSP > 
                                  operator+
                                  ( 
                                     const typename fixed_string< n, C, CSP >::value_type * lhs,
                                     const fixed_string< n, C, CSP > & rhs
                                  )
      {
         return( fixed_string< n, C, CSP >( lhs ).append( rhs ));
      }

#     if !defined(BOOST_MSVC) || ( BOOST_MSVC > 1200 )
         template< size_t n, size_t m, class C, class CSP >
         inline fixed_string< ( n + m ), C, CSP > 
                                  operator+
                                  ( 
                                     const fixed_string< n, C, CSP > & lhs,
                                     const typename fixed_string< n, C, CSP >::value_type( & rhs )[ m ]
                                  )
         {
            return( fixed_string< ( n + m ), C, CSP >( lhs ).append( rhs ));
         }

         template< size_t n, size_t m, class C, class CSP >
         inline fixed_string< ( n + m ), C, CSP > 
                                  operator+
                                  ( 
                                     const typename fixed_string< n, C, CSP >::value_type( & lhs )[ m ],
                                     const fixed_string< n, C, CSP > & rhs
                                  )
         {
            return( fixed_string< ( n + m ), C, CSP >( lhs ).append( rhs ));
         }
#     endif

      template< size_t n, class C, class CSP >
      inline fixed_string< ( n + 1 ), C, CSP > 
                                  operator+
                                  ( 
                                     const fixed_string< n, C, CSP > & lhs,
                                     typename fixed_string< n, C, CSP >::value_type rhs
                                  )
      {
         fixed_string< ( n + 1 ), C, CSP > res( lhs );
         res.push_back( rhs );
         return( res );
      }

      template< size_t n, class C, class CSP >
      inline fixed_string< ( n + 1 ), C, CSP > 
                                  operator+
                                  ( 
                                     typename fixed_string< n, C, CSP >::value_type lhs,
                                     const fixed_string< n, C, CSP > & rhs
                                  )
      {
         fixed_string< ( n + 1 ), C, CSP > res;
         res.push_back( lhs );
         res.append(    rhs );
         return( res );
      }
   }
#endif
