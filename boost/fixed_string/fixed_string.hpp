// (C) Copyright 2003-2005: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIXED_STRING_HPP
#define BOOST_FIXED_STRING_HPP

#include <boost/fixed_string/detail/basic_string_impl.hpp>

#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>

namespace boost
{

namespace detail
{

/** @brief encapsulate printf-style functionality into a char_traits-like
  * class.
  */

template< typename CharT >
struct format_policy{};

template<>
struct format_policy< char >
{
   static int format( char * s, size_t n, const char * fmt, va_list args )
   {
#  if defined(BOOST_MSVC) || defined(BOOST_INTEL_WIN)
      return ::_vsnprintf( s, n, fmt, args );
#  else
      return ::vsnprintf( s, n, fmt, args );
#  endif
   }
};

#if !defined(BOOST_NO_CWCHAR)

template<>
struct format_policy< wchar_t >
{
   static int format( wchar_t * s, size_t n, const wchar_t * fmt, va_list args )
   {
#  if defined(BOOST_MSVC) || defined(BOOST_INTEL_WIN)
      return ::_vsnwprintf( s, n, fmt, args );
#  else
      return ::vswprintf( s, n, fmt, args );
#  endif
   }
};

#endif

template
<
   typename CharT,
   class CharStringPolicy = std::char_traits< CharT >,
   class FmtPolicy = format_policy< CharT >
>
struct fixed_string_impl
{
   typedef void                                          substring_type;
   typedef CharStringPolicy                              traits_type;
   typedef std::allocator< CharT >                       allocator_type;
   typedef typename allocator_type::pointer              iterator;
   typedef typename allocator_type::const_pointer        const_iterator;

   typedef typename allocator_type::size_type            size_type;
   typedef typename allocator_type::reference            reference;
   typedef typename allocator_type::const_reference      const_reference;
private:
   CharT *   str;
   size_type cap;
   size_type len;
public:
   // iterators

   iterator begin_()
   {
      return str;
   }

   const_iterator begin_() const
   {
      return str;
   }

   iterator end_()
   {
      return str + len;
   }

   const_iterator end_() const
   {
      return str + len;
   }

   // access

   CharT * buffer_()
   {
      return str;
   }

   const CharT * data_() const
   {
      return str;
   }

   const CharT * c_str_() const
   {
      return str;
   }

   reference at_( size_type i )
   {
      return str[ i ];
   }

   const_reference at_( size_type i ) const
   {
      return str[ i ];
   }

   // size and capacity

   size_type length_() const
   {
      return len;
   }

   size_type capacity_() const
   {
      return cap;
   }

   size_type max_size_() const
   {
      return cap;
   }

   void resize_( size_t sz, CharT c )
   {
      if( sz > len )
      {
         if( sz >= cap )
         {
            sz = cap;
         }
         traits_type::assign( str + len, sz - len, c );
      }
      len = sz;
      str[ len ] = CharT();
   }

   void setlength_( size_t sz )
   {
      if( sz >= cap )
      {
         sz = cap;
      }
      len = sz;
      str[ len ] = CharT();
   }

   void reserve_( size_type sz )
   {
      if( sz < len )
      {
         len = sz;
         str[ len ] = CharT();
      }
   }

   // string operations

   void assign_( const CharT * s, size_type l )
   {
      if( l == size_type( -1 ))
      {
         l = traits_type::length( s );
      }

      len = ( l >= cap ) ? cap : l;
      traits_type::copy( str, s, len );
      str[ len ] = CharT();
   }

   void append_( const CharT * s, size_type l = size_type( -1 ))
   {
      if( l == size_type( -1 ))
      {
         l = traits_type::length( s );
      }

      l = (( l + len ) >= cap ) ? ( cap - len ) : l;
      traits_type::copy( str + len, s, l );
      len += l;
      str[ len ] = CharT();
   }

   int format_( const CharT * fmt, va_list args )
   {
      int ret = FmtPolicy::format( str, cap, fmt, args );
      len = ( ret == -1 ) ? cap : ret;
      str[ len ] = CharT();
      return ret;
   }

   void push_back_( CharT c )
   {
      if( len < cap )
      {
         str[   len ] = c;
         str[ ++len ] = CharT();
      }
   }

   void swap_( fixed_string_impl & fsi )
   {
      size_type ml = (std::min)( len, fsi.len );
      size_type i  = 0;

      for( ; i < ml; ++i )
      {
         std::swap( str[ i ], fsi.str[ i ]);
      }

      if( len > fsi.len ) // copy remainder of this into fsi
      {
         ml = (std::min)( len, fsi.cap );
         for( ; i < ml; ++i )
         {
            fsi.str[ i ] = str[ i ];
         }

         len     = fsi.len;
         fsi.len = ml;
      }
      else // copy remainder of fsi into this
      {
         ml = (std::min)( fsi.len, cap );
         for( ; i < ml; ++i )
         {
            str[ i ] = fsi.str[ i ];

            fsi.len = len;
            len     = ml;
         }

         fsi.str[ fsi.len ] = CharT();
         str[ len ]         = CharT();
      }
   }

   // constructors

   fixed_string_impl( const CharT * b, size_t c, size_t l = 0 ):
      str( const_cast< CharT * >( b )), cap( c ), len( l )
   {
   }

   virtual ~fixed_string_impl()
   {
   }
};

}

template
<
   typename CharT,
   class CharStringPolicy = std::char_traits< CharT >,
   class FmtPolicy = detail::format_policy< CharT >
>
struct fixed_string_base: public
   detail::basic_string_impl
   <
      detail::fixed_string_impl< CharT, CharStringPolicy, FmtPolicy >
   >
{
   typedef detail::basic_string_impl
           <
              detail::fixed_string_impl< CharT, CharStringPolicy, FmtPolicy >
           >
           base_type;
   typedef fixed_string_base< CharT, CharStringPolicy, FmtPolicy >
           this_type;

   BOOST_STATIC_CONSTANT( size_t, npos = base_type::npos );

   typedef typename base_type::traits_type                  traits_type;
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
   typedef typename base_type::substring_type               substring_type;
   typedef typename base_type::string_type                  string_type;

   operator const CharT *() const
   {
      return c_str();
   }

   CharT * buffer()
   {
      return buffer_();
   }

   void setlength( size_type sz = npos )
   {
      // recalculate string length after external modification
      if( sz == npos )
      {
         sz = ::strlen( c_str());
      }
      recalc_( sz );
   }

   int format( const CharT * fmt, va_list args )
   {
      return format_( fmt, args );
   }

   class length_proxy
   {
      int                 len_;
      fixed_string_base & str_;
   public:
      length_proxy( fixed_string_base & source ):
         str_( source ), len_( source.capacity())
      {
      }

      ~length_proxy()
      {
         str_.setlength( len_ );
      }

      operator int *()
      {
         return &len_;
      }
   };

   length_proxy newlength()
   {
      return length_proxy( *this );
   }

   // assignment

   this_type & operator=( const this_type & s )
   {
      base_type::operator=( s );
      return *this;
   }

   this_type & operator=( const string_type & s )
   {
      base_type::operator=( s );
      return *this;
   }

   this_type & operator=( const value_type * s )
   {
      base_type::operator=( s );
      return *this;
   }

   this_type & operator=( value_type c )
   {
      base_type::operator=( c );
      return *this;
   }

private: // construction
   fixed_string_base();
public:
   fixed_string_base( const fixed_string_base & b ): base_type( b.data(), b.capacity())
   {
      resize( b.length());
   }

   fixed_string_base( CharT * b, size_t c ): base_type( b, c )
   {
   }
};

typedef fixed_string_base< char >    char_string;
typedef fixed_string_base< wchar_t > wchar_string;

template
<
   size_t n, typename CharT = char,
   class CharStringPolicy = std::char_traits< CharT >,
   class FmtPolicy        = detail::format_policy< CharT >
>
struct fixed_string: public
   fixed_string_base< CharT, CharStringPolicy, FmtPolicy >
{
   typedef fixed_string_base< CharT, CharStringPolicy, FmtPolicy > base_type;
   typedef fixed_string< n,   CharT, CharStringPolicy, FmtPolicy > this_type;

   BOOST_STATIC_CONSTANT( size_t, npos      = base_type::npos );
   BOOST_STATIC_CONSTANT( size_t, ncapacity = n );
   BOOST_STATIC_CONSTANT( size_t, nstorage  = n + 1 );

   typedef typename base_type::traits_type                  traits_type;
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
   typedef typename base_type::substring_type               substring_type;
   typedef typename base_type::string_type                  string_type;
private:
   CharT buf[ nstorage ];
public:
   this_type substr( size_type pos, size_type _n ) const
   {
      return this_type( *this, pos, _n );
   }

   // assignment

   this_type & operator=( const this_type & s )
   {
      base_type::operator=( s );
      return *this;
   }

   this_type & operator=( const string_type & s )
   {
      base_type::operator=( s );
      return *this;
   }

   this_type & operator=( const value_type * s )
   {
      base_type::operator=( s );
      return *this;
   }

   this_type & operator=( value_type c )
   {
      base_type::operator=( c );
      return *this;
   }

   // construction

   fixed_string(): base_type( buf, ncapacity )
   {
      buf[ 0 ] = CharT();
   }

   fixed_string( const this_type & s ): base_type( buf, ncapacity )
   {
      assign( s );
   }

   fixed_string( const string_type & s, size_type p = 0, size_type l = npos ): base_type( buf, ncapacity )
   {
      assign( s, p, l );
   }

   fixed_string( const CharT * s, size_type l ): base_type( buf, ncapacity )
   {
      assign( s, l );
   }

   fixed_string( const CharT * s ): base_type( buf, ncapacity )
   {
      assign( s, traits_type::length( s ));
   }

   fixed_string( size_type l, CharT c ): base_type( buf, ncapacity )
   {
      assign( l, c );
   }

   template< typename InputIterator >
   fixed_string( InputIterator first, InputIterator last ):
      base_type( buf, ncapacity )
   {
      assign( first, last );
   }
};

// lhs + rhs

template< size_t n1, size_t n2, class C, class CSP, class FP >
inline fixed_string< ( n1 + n2 ), C, CSP, FP >
operator+
(
   const fixed_string< n1, C, CSP, FP > & lhs,
   const fixed_string< n2, C, CSP, FP > & rhs
)
{
   return fixed_string< ( n1 + n2 ), C, CSP, FP >( lhs ).append( rhs );
}

template< size_t n, class C, class CSP, class FP >
inline fixed_string< n, C, CSP, FP > 
operator+
(
   const fixed_string< n, C, CSP, FP > & lhs,
   const typename fixed_string< n, C, CSP, FP >::value_type * rhs
)
{
   return fixed_string< n, C, CSP, FP >( lhs ).append( rhs );
}

template< size_t n, class C, class CSP, class FP >
inline fixed_string< n, C, CSP, FP > 
operator+
(
   const typename fixed_string< n, C, CSP, FP >::value_type * lhs,
   const fixed_string< n, C, CSP, FP > & rhs
)
{
   return fixed_string< n, C, CSP, FP >( lhs ).append( rhs );
}

#if !defined(BOOST_MSVC) || ( BOOST_MSVC > 1200 )

template< size_t n, size_t m, class C, class CSP, class FP >
inline fixed_string< ( n + m ), C, CSP, FP > 
operator+
(
   const fixed_string< n, C, CSP, FP > & lhs,
   const typename fixed_string< n, C, CSP, FP >::value_type( & rhs )[ m ]
)
{
   return fixed_string< ( n + m ), C, CSP, FP >( lhs ).append( rhs );
}

template< size_t n, size_t m, class C, class CSP, class FP >
inline fixed_string< ( n + m ), C, CSP, FP > 
operator+
(
   const typename fixed_string< n, C, CSP, FP >::value_type( & lhs )[ m ],
   const fixed_string< n, C, CSP, FP > & rhs
)
{
   return fixed_string< ( n + m ), C, CSP, FP >( lhs ).append( rhs );
}

#endif

template< size_t n, class C, class CSP, class FP >
inline fixed_string< ( n + 1 ), C, CSP, FP > 
operator+
(
   const fixed_string< n, C, CSP, FP > & lhs,
   typename fixed_string< n, C, CSP, FP >::value_type rhs
)
{
   fixed_string< ( n + 1 ), C, CSP, FP > res( lhs );
   res.push_back( rhs );
   return res;
}

template< size_t n, class C, class CSP, class FP >
inline fixed_string< ( n + 1 ), C, CSP, FP > 
operator+
(
   typename fixed_string< n, C, CSP, FP >::value_type lhs,
   const fixed_string< n, C, CSP, FP > & rhs
)
{
   fixed_string< ( n + 1 ), C, CSP, FP > res;
   res.push_back( lhs );
   res.append(    rhs );
   return res;
}

}

#endif
