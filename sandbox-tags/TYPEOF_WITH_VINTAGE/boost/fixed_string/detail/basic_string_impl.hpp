// (C) Copyright 2003-2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** This code is based on boost-sandbox/boost/wave/util/flex_string.hpp by
  * Andrei Alexandrescu, modified by Hartmut Kasier
  *
  * The main changes are:
  * +  made a few modifications to the StoragePolicy interface
  * +  uses an ErrorPolicy class to handle out_of_range and length_error
  *    exceptions
  * +  uses the traits_type functions instead of the POD functions
  *    -  pod_move( const Pod * b, const Pod * e, Pod * d )
  *         --> traits_type::move( d, b, ( e - b ))
  *    -  pod_copy( const Pod * b, const Pod * e, Pod * d )
  *         --> traits_type::copy( d, b, ( e - b ))
  *    -  pod_fill( Pod * b, Pod * e, T c )
  *         --> traits_type::assign( b, ( e - b ), c )
  * +  added exception handling for resize and reserve
  * +  uses an MPL-based approach to check InputIterator on
  *    replace( iterator i1, iterator i2, InputIterator j1, InputIterator j2 )
  * +  uses a substring_type typedef for the result of substr to allow substring
  *    implementation supression and/or alternative implementations that return
  *    a different type
  * +  uses optimized versions of find and rfind for searching for single
  *    characters
  * +  uses boost::reverse_iterator< Iterator >: there are various problems when
  *    using std::reverse_iterator< Iterator > on non-conformant
  *    compilers/standard libraries
  * +  proper implementation of reverse< InputIterator >( ... ) for non-integral
  *    types
  * +  implementations of swap and getline are in the std namespace to allow std
  *    qualified calls to work
  * +  operator+ is not implemented to allow for non-constructable policy types
  *    (e.g. my fixed_string implementation)
  */

#ifndef BOOST_BASIC_STRING_IMPL_HPP
#define BOOST_BASIC_STRING_IMPL_HPP
#  include <boost/config.hpp>
#  include <boost/mpl/if.hpp>
#  include <boost/iterator/reverse_iterator.hpp>
#  include <iostream>
#  include <stdexcept>
#  include <functional> // std::less_equal
#  include <algorithm>  // std::copy
#  include <locale>     // std::isspace
#  include <limits>     // std::numeric_limits
#  include <stddef.h>   // size_t

   namespace boost{ namespace detail
   {
      inline size_t min_( size_t a, size_t b )
      {
         return(( a < b ) ? a : b );
      }

      struct noerror_string_policy
      {
         static inline void                      out_of_range()
         {
         }
         static inline void                      length_error()
         {
         }
      };

      struct exception_string_policy
      {
         static inline void                      out_of_range()
         {
            throw( std::out_of_range( "" ));
         }
         static inline void                      length_error()
         {
            throw( std::length_error( "" ));
         }
      };

      template< class Base, class ErrorPolicy = noerror_string_policy >
      class basic_string_impl: public Base
      {
         public: // types
            typedef typename Base::substring_type                    substring_type;
            typedef basic_string_impl< Base, ErrorPolicy >           string_type;
            typedef typename Base::traits_type                       traits_type;
            typedef typename traits_type::char_type                  value_type;
            typedef typename traits_type::char_type                  char_type;
            typedef typename Base::allocator_type                    allocator_type;
         public:
            typedef typename allocator_type::size_type               size_type;
            typedef typename allocator_type::difference_type         difference_type;
            typedef typename allocator_type::reference               reference;
            typedef typename allocator_type::const_reference         const_reference;
            typedef typename allocator_type::pointer                 pointer;
            typedef typename allocator_type::const_pointer           const_pointer;
         public:
            typedef typename Base::iterator                          iterator;
            typedef typename Base::const_iterator                    const_iterator;
            typedef boost::reverse_iterator< iterator >              reverse_iterator;
            typedef boost::reverse_iterator< const_iterator >        const_reverse_iterator;
         public:
            BOOST_STATIC_CONSTANT( size_type, npos = -1 );
         private: // implementation helpers
            inline char_type *                   offset( size_type pos )
            {
               return( &*( begin() + pos ));
            }
            inline const char_type *             offset( size_type pos ) const
            {
               return( &*( begin() + pos ));
            }
         public: // 21.3.2: iterators
            inline const_iterator                begin() const
            {
               return( begin_());
            }
            inline iterator                      begin()
            {
               return( begin_());
            }
            inline const_iterator                end() const
            {
               return( end_());
            }
            inline iterator                      end()
            {
               return( end_());
            }
            inline const_reverse_iterator        rbegin() const
            {
               return( const_reverse_iterator( end()));
            }
            inline reverse_iterator              rbegin()
            {
               return( reverse_iterator( end()));
            }
            inline const_reverse_iterator        rend() const
            {
               return( const_reverse_iterator( begin()));
            }
            inline reverse_iterator              rend()
            {
               return( reverse_iterator( begin()));
            }
         public: // 21.3.3: capacity
            inline size_type                     size() const
            {
               return( length_());
            }
            inline size_type                     length() const
            {
               return( length_());
            }
            inline size_type                     max_size() const
            {
               return( max_size_());
            }
            inline void                          resize( size_type n, char_type c )
            {
               if( n > max_size())     ErrorPolicy::length_error();
               resize_( n, c );
            }
            inline void                          resize( size_type n )
            {
               resize( n, value_type());
            }
            inline size_type                     capacity() const
            {
               return( capacity_());
            }
            inline void                          reserve( size_type n = 0 )
            {
               if( n > max_size())     ErrorPolicy::length_error();
               reserve_( n );
            }
            inline void                          clear()
            {
               resize_( 0, value_type()); // [review]
            }
            inline bool                          empty() const
            {
               return( length_() == 0 );
            }
         public: // 21.3.4: element access
            inline const_reference operator[]( size_type i ) const
            {
               if( i > capacity())     ErrorPolicy::out_of_range(); // sanity check
               return( at_( i ));
            }
            inline reference       operator[]( size_type i )
            {
               if( i > capacity())     ErrorPolicy::out_of_range(); // sanity check
               return( at_( i ));
            }
            inline const_reference               at( size_type i ) const
            {
               if( i > length())       ErrorPolicy::out_of_range();
               return( at_( i ));
            }
            inline reference                     at( size_type i )
            {
               if( i > length())       ErrorPolicy::out_of_range();
               return( at_( i ));
            }
         public: // 21.3.5: modifiers
            inline string_type &                 assign( const string_type & s )
            {
               if( &s != this )        assign( s.data(), s.length());
               return( *this );
            }
            inline string_type &                 assign( const string_type & s, size_type pos, size_type n )
            {
               if( pos > s.size())     ErrorPolicy::out_of_range();
               return( assign( s.data() + pos, min_( n, s.size() - pos )));
            }
            inline string_type &                 assign( const char_type * s, size_type n )
            {
               assign_( s, n );
               return( *this );
            }
            inline string_type &                 assign( const char_type * s )
            {
               return( assign( s, traits_type::length( s )));
            }
            inline string_type &                 assign( size_type n, char_type c )
            {
               return( replace( begin(), end(), n, c ));
            }
            template< typename InputIterator >
            inline string_type &                 assign( InputIterator first, InputIterator last )
            {
               clear();
               while( first != last )  push_back_( *first++ );
               return( *this );
            }
         public:
            inline string_type &  operator=( const string_type & s )
            {
               return( assign( s ));
            }
            inline string_type &  operator=( const char_type * s )
            {
               return( assign( s ));
            }
            inline string_type &  operator=( char_type c )
            {
               clear();
               push_back_( c );
               return( *this );
            }
         public:
            inline string_type &                 append( const string_type & s )
            {
               return( append( s, 0, npos ));
            }
            inline string_type &                 append( const string_type & s, size_type pos, size_type n )
            {
               const size_type         sz = s.size();
               if( pos > sz )          ErrorPolicy::out_of_range();
               return( append( s.c_str() + pos, min_( n, sz - pos )));
            }
            inline string_type &                 append( const char_type * s, size_type n )
            {
               append_( s, n );
               return( *this );
            }
            inline string_type &                 append( const char_type * s )
            {
               return( append( s, traits_type::length( s )));
            }
            inline string_type &                 append( size_type n, char_type c )
            {
               resize_( size() + n, c );
               return( *this );
            }
            template< typename InputIterator >
            inline string_type &                 append( InputIterator first, InputIterator last )
            {
               while( first != last )  push_back_( *first++ );
               return( *this );
            }
            inline void                          push_back( const char_type c )
            {
               push_back_( c );
            }
         public:
            inline string_type &  operator+=( const string_type & s )
            {
               return( append( s ));
            }
            inline string_type &  operator+=( const char_type * s )
            {
               return( append( s ));
            }
            inline string_type &  operator+=( char_type c )
            {
               push_back( c );
               return( *this );
            }
         public:
            inline string_type &                 insert( size_type pos1, const string_type & s )
            {
               return( insert( pos1, s, 0, npos ));
            }
            inline string_type &                 insert( size_type pos1, const string_type & s, size_type pos2, size_type n )
            {
               return( replace( pos1, 0, s, pos2, n ));
            }
            inline string_type &                 insert( size_type pos, const char_type * s, size_type n )
            {
               return( replace( pos, 0, s, n ));
            }
            inline string_type &                 insert( size_type pos, const char_type * s )
            {
               return( insert( pos, s, traits_type::length( s )));
            }
            inline string_type &                 insert( size_type pos, size_type n, value_type c )
            {
               return( replace( pos, 0, n, c ));
            }
            inline iterator                      insert( iterator p, value_type c = value_type())
            {
               const size_type         pos = p - begin();
               insert( pos, &c, 1 );
               return( begin() + pos );
            }
            inline void                          insert( iterator p, size_type n, char_type c )
            {
               insert( p - begin(), n, c );
            }
            template< typename InputIterator >
            inline void                          insert( iterator p, InputIterator first, InputIterator last )
            {
               replace( p, p, first, last );
            }
         public:
            inline string_type &                 erase( size_type pos = 0, size_type n = npos )
            {
               return( replace( pos, min_( n, size() - pos ), 0, value_type()));
            }
            inline iterator                      erase( iterator pos )
            {
               const size_type         p = pos - begin();
               erase( p, 1 );
               return( begin() + p );
            }
            inline iterator                      erase( iterator first, iterator last )
            {
               const size_type         pos = first - begin();
               erase( pos, last - first );
               return( begin() + pos );
            }
         public:
            inline string_type &                 replace( size_type pos1, size_type n1, const string_type & s )
            {
               return( replace( pos1, n1, s, 0, npos ));
            }
            inline string_type &                 replace( size_type pos1, size_type n1, const string_type & s, size_type pos2, size_type n2 )
            {
               if( pos1 > length())    ErrorPolicy::out_of_range();
               return( replace( pos1, n1, s.offset( pos2 ), min_( n2, s.length() - pos2 )));
            }
            inline string_type &                 replace( size_type d, size_type n1, const char_type * s1, size_type n2 )
            {
               if( d > size())         ErrorPolicy::out_of_range();
               if( d + n1 > size())    n1 = size() - d;
               static const std::less_equal< const value_type * > le = std::less_equal< const value_type * >();

               const int               delta   = int( n2 - n1 );
               const bool              aliased = le( &*begin(), s1 ) && le( s1, &*end());
               if( delta > 0 )
               {
                  if( capacity() < size() + delta ) // realloc the string
                  {
                     if( aliased )
                     {
                        const size_type off = s1 - &*begin();
                        reserve( size() + delta );
                        s1 = offset( off );
                     }
                     else reserve( size() + delta );
                  }

                  const value_type *   s2   = s1 + n2;
                  value_type *         d1   = offset( d );
                  value_type *         d2   = d1 + n1;
                  const int            len_ = int( &*end() - d2 );
                  if( delta <= len_ )
                  {
                     value_type *      end_ = &*end();
                     append_( end_ - delta, delta );
                     traits_type::move( d2 + delta, d2, ( len_ - delta ));
                     if( le( d2, s1 ))
                     {
                        if( aliased )  traits_type::copy( d1, s1 + delta, ( s2 - s1 ));
                        else           traits_type::copy( d1, s1, ( s2 - s1 ));
                     }
                     else
                     {
                        if( le( d2, s2 ))
                        {
                           // assert( aliased );
                           traits_type::move( d1, s1, ( d2 - s1 ));
                           traits_type::move( d1 + ( d2 - s1 ), d2 + delta, ( s2 - d2 ));
                        }
                        else traits_type::move( d1, s1, ( s2 - s1 ));
                     }
                  }
                  else
                  {
                     const size_type   sz = delta - len_;
                     append_( s2 - sz, sz );
                     append_( d2, len_ );
                     traits_type::move( d1, s1, (( s2 - delta + len_ ) - s1 ));
                  }
               }
               else
               {
                  traits_type::move( offset( d ), s1, n2 );
                  traits_type::move( offset( d + n1 + delta ), offset( d + n1 ), ( &*end() - offset( d + n1 )));
                  resize( size() + delta );
               }
               return( *this );
            }
            inline string_type &                 replace( size_type d, size_type n1, const char_type * s )
            {
               return( replace( d, n1, s, traits_type::length( s )));
            }
            inline string_type &                 replace( size_type pos, size_type n1, size_type n2, char_type c )
            {
               if( pos + n1 > size())  n1    = size() - pos;
               const size_type         size_ = size();
               if( pos + n2 > size_ )
               {
                  resize( pos + n2, c );
                  append_( offset( pos + n1 ), size_ - pos - n1 );
                  traits_type::assign( offset( pos ), size_ - pos, c );
               }
               else
               {
                  if( n2 > n1 )
                  {
                     const size_type   delta = n2 - n1;
                     append_( offset( size_ - delta ), delta );
                     traits_type::move( offset( pos + n2 ), offset( pos + n1 ), ( size_ - delta - pos - n1 ));
                  }
                  else
                  {
                     traits_type::move( offset( pos + n2 ), offset( pos + n1 ), size() - pos - n1 );
                     resize( size_ - ( n1 - n2 ));
                  }
                  traits_type::assign( offset( pos ), n2, c );
               }
               return( *this );
            }
            inline string_type &                 replace( iterator i1, iterator i2, const string_type & s )
            {
               return( replace( i1, i2, s.c_str(), s.length()));
            }
            inline string_type &                 replace( iterator i1, iterator i2, const char_type * s, size_type n )
            {
               return( replace( i1 - begin(), i2 - i1, s, n ));
            }
            inline string_type &                 replace( iterator i1, iterator i2, const char_type * s )
            {
               return( replace( i1, i2, s, traits_type::length( s )));
            }
            inline string_type &                 replace( iterator i1, iterator i2, size_type n, char_type c )
            {
               return( replace( i1 - begin(), i2 - i1, n, c ));
            }
         private:
            template< typename IntegralType >
            inline string_type &                 replace( iterator i1, iterator i2, IntegralType j1, IntegralType j2, mpl::true_ )
            {
               return( replace( i1, i2, static_cast< size_type >( n ), static_cast< value_type >( c )));
            }
            template< typename InputIterator >
            inline string_type &                 replace( iterator i1, iterator i2, InputIterator j1, InputIterator j2, mpl::false_ )
            {
               // replace( i1, i2, string_type( j1, j2 ));
               replace( i1, i2, std::distance( j1, j2 ), char_type());
               std::copy( j1, j2, i1 );
               return( *this );
            }
         public:
            template< typename InputIterator >
            inline string_type &                 replace( iterator i1, iterator i2, InputIterator j1, InputIterator j2 )
            {
               return( replace(
                  i1, i2, j1, j2,
                  mpl::if_ // need Borland workaround for this
                  < 
                     mpl::bool_< std::numeric_limits< InputIterator >::is_specialized >,
                     mpl::true_, mpl::false_
                  >::type()
               ));
            }
         public:
            inline size_type                     copy( char_type * s, size_type n, size_type pos = 0 ) const
            {
               if( pos > size())       ErrorPolicy::out_of_range();
               n = min_( n, size() - pos );
               traits_type::copy( s, offset( pos ), n );
               s[ n ] = char_type();
               return( n );
            }
            inline void                          swap( Base & s )
            {
               swap_( s );
            }
         public: // 21.3.6: string operations
            inline const char_type *             data() const
            {
               return( data_());
            }
            inline const char_type *             c_str() const
            {
               return( c_str_());
            }
            inline allocator_type                get_allocator() const
            {
               allocator_type alloc;
               return( alloc );
            }
         public:
            inline size_type                     find( const string_type & s, size_type pos = 0 ) const
            {
               return( find( s.data(), pos, s.length()));
            }
            inline size_type                     find( const char_type * s, size_type pos, size_type n ) const
            {
               const size_type         sz = size();
               for( ; pos <= sz; ++pos )
               {
                  if
                  (
                     traits_type::eq( *offset( pos ), *s ) &&
                     traits_type::compare( offset( pos ), s, n ) == 0
                  )
                     return( pos );
               }
               return( npos );
            }
            inline size_type                     find( const char_type * s, size_type pos = 0 ) const
            {
               return( find( s, pos, traits_type::length( s )));
            }
            inline size_type                     find( char_type c, size_type pos = 0 ) const
            {
               const_iterator          last( end());
               for( const_iterator first( begin() + pos ); first != last; ++first, ++pos )
               {
                  if( traits_type::eq( *first, c ))
                     return( pos );
               }
               return( npos );
            }
         public:
            inline size_type                     rfind( const string_type & s, size_type pos = npos ) const
            {
               return( rfind( s.c_str(), pos, s.length()));
            }
            inline size_type                     rfind( const char_type * s, size_type pos, size_type n ) const
            {
               if( n > length())       return( npos );
               pos = min_( pos, length() - n );
               if( n == 0 )            return( pos );
               
               for( const_iterator i( begin() + pos );; --i )
               {
                  if( traits_type::eq( *i, *s ) && traits_type::compare( &*i, s, n ) == 0 )
                     return( i - begin());
                  if( i == begin())    break;
               }
               return( npos );
            }
            inline size_type                     rfind( const char_type * s, size_type pos = npos ) const
            {
               return( rfind( s, pos, traits_type::length( s )));
            }
            inline size_type                     rfind( char_type c, size_type pos = npos ) const
            {
               if( pos > size())       pos = size();

               const_iterator          last( begin());
               for
               (
                  const_iterator first( begin() + pos );
                  pos != 0 && first != last;
                  --first, --pos
               )
               {
                  if( traits_type::eq( *first, c ))
                     return( pos );
               }
               return( npos );
            }
         public:
            inline size_type                     find_first_of( const string_type & s, size_type pos = 0 ) const
            {
               return( find_first_of( s.c_str(), pos, s.length()));
            }
            inline size_type                     find_first_of( const char_type * s, size_type pos, size_type n ) const
            {
               if( pos > length() || n == 0 )
                  return( npos );
                  
               const_iterator          last( end());
               for( const_iterator first( begin() + pos ); first != last; ++first )
               {
                  if( traits_type::find( s, n, *first ) != 0 )
                     return( first - begin());
               }
               return( npos );
            }
            inline size_type                     find_first_of( const char_type * s, size_type pos = 0 ) const
            {
               return( find_first_of( s, pos, traits_type::length( s )));
            }
            inline size_type                     find_first_of( char_type c, size_type pos = 0 ) const
            {
               return( find( c, pos ));
            }
         public:
            inline size_type                     find_last_of( const string_type & s, size_type pos = npos ) const
            {
               return( find_last_of( s.c_str(), pos, s.length()));
            }
            inline size_type                     find_last_of( const char_type * s, size_type pos, size_type n ) const
            {
               if( !empty() && n > 0 )
               {
                  pos = min_( pos, length() - 1 );
                  for( const_iterator i( begin() + pos );; --i )
                  {
                     if( traits_type::find( s, n, *i ) != 0 )
                        return( i - begin());
                     if( i == begin()) break;
                  }
               }
               return( npos );
            }
            inline size_type                     find_last_of( const char_type * s, size_type pos = npos ) const
            {
               return( find_last_of( s, pos, traits_type::length( s )));
            }
            inline size_type                     find_last_of( char_type c, size_type pos = npos ) const
            {
               return( rfind( c, pos ));
            }
         public:
            inline size_type                     find_first_not_of( const string_type & s, size_type pos = 0 ) const
            {
               return( find_first_not_of( s.data(), pos, s.length()));
            }
            inline size_type                     find_first_not_of( const char_type * s, size_type pos, size_type n ) const
            {
               if( pos < length())
               {
                  const_iterator       last( end());
                  for( const_iterator first( begin() + pos ); first != last; ++first )
                  {
                     if( traits_type::find( s, n, *first ) == 0 )
                        return( first - begin());
                  }
               }
               return( npos );
            }
            inline size_type                     find_first_not_of( const char_type * s, size_type pos = 0 ) const
            {
               return( find_first_not_of( s, pos, traits_type::length( s )));
            }
            inline size_type                     find_first_not_of( char_type c, size_type pos = 0 ) const
            {
               return( find_first_not_of( &c, pos, 1 )); // optimize?
            }
         public:
            inline size_type                     find_last_not_of( const string_type & s, size_type pos = npos ) const
            {
               return( find_last_not_of( s.c_str(), pos, s.length()));
            }
            inline size_type                     find_last_not_of( const char_type * s, size_type pos, size_type n ) const
            {
               if( !empty())
               {
                  pos = min_( pos, size() - 1 );
                  for( const_iterator i( begin() + pos );; --i )
                  {
                     if( traits_type::find( s, n, *i ) == 0 )
                        return( i - begin());
                     if( i == begin()) break;
                  }
               }
               return( npos );
            }
            inline size_type                     find_last_not_of( const char_type * s, size_type pos = npos ) const
            {
               return( find_last_not_of( s, pos, traits_type::length( s )));
            }
            inline size_type                     find_last_not_of( char_type c, size_type pos = npos ) const
            {
               return( find_last_not_of( &c, pos, 1 )); // optimize?
            }
         public:
            inline substring_type                substr( size_type pos = 0, size_type n = npos ) const
            {
               if( pos > size())       ErrorPolicy::out_of_range();
               return( substr_( pos, npos ));
            }
         public:
            inline int                           compare( const string_type & s ) const
            {
               return( compare( 0, size(), s.data(), s.length()));
            }
            inline int                           compare( size_type pos1, size_type n1, const string_type & s ) const
            {
               return( compare( pos1, n1, s.data(), s.length()));
            }
            inline int                           compare( size_type pos1, size_type n1, const string_type & s, size_type pos2, size_type n2 ) const
            {
               if( pos2 > s.size())    ErrorPolicy::out_of_range();
               return( compare( pos1, n1, s.data() + pos2, min_( n2, s.size() - pos2 )));
            }
            inline int                           compare( const char_type * s ) const
            {
               return( compare( 0, size(), s, traits_type::length( s )));
            }
            inline int                           compare( size_type pos1, size_type n1, const char_type * s, size_type n2 = npos ) const
            {
               if( pos1 > size())      ErrorPolicy::out_of_range();
               n1 = min_( size() - pos1, n1 );
               const int               res = traits_type::compare( data() + pos1, s, min_( n1, n2 ));
               return(( res != 0 || n2 == npos ) ? res : int( n1 - n2 ));
            }
         public: // construction
            inline           basic_string_impl(): Base()
            {
            }
            inline           basic_string_impl( const string_type & s, size_type p = 0, size_type l = npos ): Base( s, p, l )
            {
            }
            inline           basic_string_impl( const value_type * s, size_type l ): Base( s, l )
            {
            }
            inline           basic_string_impl( const value_type * s ): Base( s )
            {
            }
            inline           basic_string_impl( size_type l, value_type c ): Base( l, c )
            {
            }
            template< typename InputIterator >
            inline           basic_string_impl( InputIterator first, InputIterator last ): Base( first, last )
            {
            }
      };

      // lhs == rhs

      template< class Base, class EP >
      inline bool operator==( const basic_string_impl< Base, EP > & lhs, const basic_string_impl< Base, EP > & rhs )
      {
         return( lhs.compare( rhs ) == 0 );
      }

      template< class Base, class EP >
      inline bool operator==( const typename basic_string_impl< Base, EP >::value_type * lhs, const basic_string_impl< Base, EP > & rhs )
      {
         return( rhs == lhs );
      }

      template< class Base, class EP >
      inline bool operator==( const basic_string_impl< Base, EP > & lhs, const typename basic_string_impl< Base, EP >::value_type * rhs )
      {
         return( lhs.compare( rhs ) == 0 );
      }

      // lhs != rhs

      template< class Base, class EP >
      inline bool operator!=( const basic_string_impl< Base, EP > & lhs, const basic_string_impl< Base, EP > & rhs )
      {
         return( !( lhs == rhs ));
      }

      template< class Base, class EP >
      inline bool operator!=( const typename basic_string_impl< Base, EP >::value_type * lhs, const basic_string_impl< Base, EP > & rhs )
      {
         return( !( lhs == rhs ));
      }

      template< class Base, class EP >
      inline bool operator!=( const basic_string_impl< Base, EP > & lhs, const typename basic_string_impl< Base, EP >::value_type * rhs )
      {
         return( !( lhs == rhs ));
      }
      
      // lhs < rhs

      template< class Base, class EP >
      inline bool operator<( const basic_string_impl< Base, EP > & lhs, const basic_string_impl< Base, EP > & rhs )
      {
         return( lhs.compare( rhs ) < 0 );
      }

      template< class Base, class EP >
      inline bool operator<( const typename basic_string_impl< Base, EP >::value_type * lhs, const basic_string_impl< Base, EP > & rhs )
      {
         return( rhs.compare( lhs ) > 0 );
      }

      template< class Base, class EP >
      inline bool operator<( const basic_string_impl< Base, EP > & lhs, const typename basic_string_impl< Base, EP >::value_type * rhs )
      {
         return( lhs.compare( rhs ) < 0 );
      }

      // lhs > rhs

      template< class Base, class EP >
      inline bool operator>( const basic_string_impl< Base, EP > & lhs, const basic_string_impl< Base, EP > & rhs )
      {
         return( rhs < lhs );
      }

      template< class Base, class EP >
      inline bool operator>( const typename basic_string_impl< Base, EP >::value_type * lhs, const basic_string_impl< Base, EP > & rhs )
      {
         return( rhs < lhs );
      }

      template< class Base, class EP >
      inline bool operator>( const basic_string_impl< Base, EP > & lhs, const typename basic_string_impl< Base, EP >::value_type * rhs )
      {
         return( rhs < lhs );
      }

      // lhs <= rhs

      template< class Base, class EP >
      inline bool operator<=( const basic_string_impl< Base, EP > & lhs, const basic_string_impl< Base, EP > & rhs )
      {
         return( !( rhs < lhs ));
      }

      template< class Base, class EP >
      inline bool operator<=( const typename basic_string_impl< Base, EP >::value_type * lhs, const basic_string_impl< Base, EP > & rhs )
      {
         return( !( rhs < lhs ));
      }

      template< class Base, class EP >
      inline bool operator<=( const basic_string_impl< Base, EP > & lhs, const typename basic_string_impl< Base, EP >::value_type * rhs )
      {
         return( !( rhs < lhs ));
      }

      // lhs >= rhs

      template< class Base, class EP >
      inline bool operator>=( const basic_string_impl< Base, EP > & lhs, const basic_string_impl< Base, EP > & rhs )
      {
         return( !( lhs < rhs ));
      }

      template< class Base, class EP >
      inline bool operator>=( const typename basic_string_impl< Base, EP >::value_type * lhs, const basic_string_impl< Base, EP > & rhs )
      {
         return( !( lhs < rhs ));
      }

      template< class Base, class EP >
      inline bool operator>=( const basic_string_impl< Base, EP > & lhs, const typename basic_string_impl< Base, EP >::value_type * rhs )
      {
         return( !( lhs < rhs ));
      }

      // inserters and extractors

      // VC6 bug: VC6 has problems matching std::basic_*stream< typename basic_string_impl< S, C, SP, EP >::value_type, ... >
      //    so std::cout << str; will not work as intended
      //    workaround: use the template parameters directly

      template< class Base, class EP, typename CharT, class Traits >
      inline std::basic_istream< CharT, Traits > &
                                  operator>>
                                  (
                                     std::basic_istream< CharT, Traits > & is,
                                     basic_string_impl< Base, EP > & str
                                  )
      {
         typedef typename basic_string_impl< Base, EP >::traits_type traits_type;

         str.erase();
         typename basic_string_impl< Base, EP >::value_type
                                       ch;
         is >> ch;
         while( !std::isspace( ch, is.getloc()))
         {
            str.push_back( ch );
            if( !is.get( ch ))         break;
         }
         return( is );
      }

      template< class Base, class EP, typename CharT, class Traits >
      inline std::basic_ostream< CharT, Traits > &
                                  operator<<
                                  (
                                     std::basic_ostream< CharT, Traits > & os,
                                     const basic_string_impl< Base, EP > & str
                                  )
      {
         return( os << str.c_str());
      }
   }}

   namespace std
   {
      // getline

      template< class Base, class EP, typename CharT, class Traits >
      inline std::basic_istream< CharT, Traits > &
                                                 getline
                                                 (
                                                    std::basic_istream< CharT, Traits > & is,
                                                    boost::detail::basic_string_impl< Base, EP > & str,
                                                    typename boost::detail::basic_string_impl< Base, EP >::value_type delim
                                                 )
      {
         typedef typename boost::detail::basic_string_impl< Base, EP >::traits_type traits_type;

         str.erase();
         typename boost::detail::basic_string_impl< Base, EP >::value_type
                                       ch;
         is >> ch;
         while( !traits_type::eq( ch, delim ))
         {
            str.push_back( ch );
            if( !is.get( ch ))         break;
         }
         return( is );
      }

      template< class Base, class EP, typename CharT, class Traits >
      inline std::basic_istream< CharT, Traits > &
                                                 getline
                                                 (
                                                    std::basic_istream< CharT, Traits > & is,
                                                    boost::detail::basic_string_impl< Base, EP > & str
                                                 )
      {
         return( getline( is, str, is.widen( '\n' )));
      }

      // swap

      template< class Base, class EP >
      inline void                                swap
                                                 (
                                                    boost::detail::basic_string_impl< Base, EP > & lhs,
                                                    boost::detail::basic_string_impl< Base, EP > & rhs
                                                 )
      {
         lhs.swap( rhs );
      }
   }
#endif
