// (C) Copyright 2003-2004: Reece H. Dunn
// Use, modification, and distribution are subject to the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// NOTE: This is presently under construction: not all of the functions are properly implemented.
// part of this code is based on http://moderncppdesign.com/code/flex_string.zip by Andrei Alexandrescu

#ifndef BOOST_BASIC_STRING_IMPL_HPP
#define BOOST_BASIC_STRING_IMPL_HPP
#  include <boost/config.hpp>
#  include <boost/mpl/if.hpp>
#  include <boost/mpl/bool.hpp>
#  include <algorithm>
#  include <iostream>

   namespace boost{ namespace detail
   {
      inline size_t min_( size_t a, size_t b )
      {
         return(( a < b ) ? a : b );
      }

      struct noerror_on_out_of_range
      {
         static inline void                      out_of_range()
         {
         }
      };

      struct exception_on_out_of_range
      {
         static inline void                      out_of_range()
         {
            throw( std::out_of_range( "" ));
         }
      };
      
      /*
         begin() | end() :: rbegin() | rend()
         length() | capacity() | max_size()
         at( size_t i )
         resize(  size_type n, value_type c )
         reserve( size_type n )

         assign( const char_type * s, size_type n )
         append( const char_type * s, size_type n )
         push_back( char_type c )
         copy( char_type * s, size_type n, size_type pos = 0 ) const
      */

      template
      <
         class StringPolicy, typename CharT = char,
         class CharPolicy = std::char_traits< CharT >,
         class ErrorPolicy = noerror_on_out_of_range 
      >
      class basic_string_impl
      {
         public: // types
            typedef StringPolicy                                     string_type;
            typedef std::basic_string< CharT, CharPolicy >           substring_type;
            typedef typename CharPolicy                              traits_type;
            typedef typename traits_type::char_type                  value_type;
            typedef typename traits_type::char_type                  char_type;
            typedef std::allocator< value_type >                     allocator_type;
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
#           if !defined(BOOST_NO_STD_ITERATOR)
               typedef std::reverse_iterator< iterator >             reverse_iterator;
               typedef std::reverse_iterator< const_iterator >       const_reverse_iterator;
#           else
               typedef std::reverse_iterator< iterator, value_type > reverse_iterator;
               typedef std::reverse_iterator< const_iterator, const value_type >
                                                                     const_reverse_iterator;
#           endif
         public:
            BOOST_STATIC_CONSTANT( size_type, npos = -1 );
         private: // implementation helpers
            inline string_type &                 get_impl()
            {
               return( *static_cast< string_type * >( this ));
            }
            inline const string_type &           get_impl() const
            {
               return( *static_cast< const string_type * >( this ));
            }
         public: // 21.3.2: iterators
            inline const_iterator                begin() const
            {
               return( get_impl().begin());
            }
            inline iterator                      begin()
            {
               return( get_impl().begin());
            }
            inline const_iterator                end() const
            {
               return( get_impl().end()); 
            }
            inline iterator                      end()
            {
               return( get_impl().end());
            }
            inline const_reverse_iterator        rbegin() const
            {
               return( get_impl().rbegin());
            }
            inline reverse_iterator              rbegin()
            {
               return( get_impl().rbegin());
            }
            inline const_reverse_iterator        rend() const
            {
               return( get_impl().rend());
            }
            inline reverse_iterator              rend()
            {
               return( get_impl().rend());
            }
         public: // 21.3.3: capacity
            inline size_type                     size() const
            {
               return( get_impl().length());
            }
            inline size_type                     length() const
            {
               return( get_impl().length());
            }
            inline size_type                     max_size() const
            {
               return( get_impl().max_size());
            }
            inline void                          resize( size_type n, char_type c )
            {
               get_impl().resize( n, c );
            }
            inline void                          resize( size_type n )
            {
               resize( n, value_type());
            }
            inline size_type                     capacity() const
            {
               return( get_impl().capacity());
            }
            inline void                          reserve( size_type n = 0 )
            {
               get_impl().reserve( n );
            }
            inline void                          clear()
            {
               erase();
            }
            inline bool                          empty() const
            {
               return( get_impl().length() == 0 );
            }
         public: // 21.3.4: element access
            inline const_reference operator[]( size_type i ) const
            {
               return( get_impl().at( i ));
            }
            inline reference       operator[]( size_type i )
            {
               return( get_impl().at( i ));
            }
            inline const_reference               at( size_type i ) const
            {
               if( i > length())       ErrorPolicy::out_of_range();
               return( get_impl().at( i ));
            }
            inline reference                     at( size_type i )
            {
               return( get_impl().at( i ));
            }
         public: // 21.3.5: modifiers
            inline string_type &                 assign( const string_type & s )
            {
               if( &s != this )        assign( s.data(), s.length());
               return( get_impl());
            }
            inline string_type &                 assign( const string_type & s, size_type pos, size_type n )
            {
               if( pos > s.size())     ErrorPolicy::out_of_range();
               return( assign( s.data() + pos, min_( n, s.size() - pos )));
            }
            inline string_type &                 assign( const char_type * s, size_type n )
            {
               return( get_impl().assign( s, n ));
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
            inline size_type &                   assign( InputIterator first, InputIterator last )
            {
               clear();
               while( first != last )  get_impl().push_back( *first++ );
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
               get_impl().push_back( c );
               return( get_impl());
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
               return( get_impl().append( s, n ));
            }
            inline string_type &                 append( const char_type * s )
            {
               return( append( s, traits_type::length( s )));
            }
            inline string_type &                 append( size_type n, char_type c )
            {
               return( get_impl()); // todo == resize( size() + n, c );
            }
            template< typename InputIterator >
            inline size_type &                   append( InputIterator first, InputIterator last )
            {
               while( first != last )  get_impl().push_back( *first++ );
            }
            inline void                          push_back( const char_type c )
            {
               get_impl().push_back( c );
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
               return( get_impl());
            }
         public:
            inline string_type &                 insert( size_type pos1, const string_type & s )
            {
               return( insert( pos1, str, 0, npos ));
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
               const size_type         pos = position - begin();
               erase( pos, 1 );
               return( begin() + pos );
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
               return( replace( pos1, n1, static_cast< const char_type * >( s.begin() + pos2 ), min_( n2, s.length() - pos2 )));
            }
            inline string_type &                 replace( size_type pos, size_type n1, const char_type * s, size_type n2 )
            {
               return( replace( pos, n1, s, traits_type::length( s )));
            }
            inline string_type &                 replace( size_type pos, size_type n1, const char_type * s )
            {
               return( get_impl()); // todo
            }
            inline string_type &                 replace( size_type pos, size_type n1, size_type n2, char_type c )
            {
               return( get_impl()); // todo
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
            template< typename InputIterator >
            inline string_type &                 replace( iterator i1, iterator i2, InputIterator j1, InputIterator j2 )
            {
               struct ok{ typedef char value; };
               struct non_integral_type_error{};
               typedef typename mpl::if_< mpl::bool_< std::numeric_limits<InputIterator>::is_specialized >,
                                          ok, non_integral_type_error
                                        >::type::value               non_integral_type_check;

               return( replace( i1, i2, static_cast< size_type >( n ), static_cast< value_type >( c )));
            }
         public:
            inline size_type                     copy( char_type * s, size_type n, size_type pos = 0 ) const
            {
               return( get_impl().copy( s, n, pos ));
            }
            inline void                          swap( string_type & s )
            {
               get_impl().swap( s );
            }
         public: // 21.3.6: string operations
            inline const char_type *             c_str() const
            {
               return( get_impl().c_str());
            }
            inline const char_type *             data() const
            {
               return( get_impl().data());
            }
            inline allocator_type                get_allocator() const
            {
               return( get_impl().get_allocator());
            }
         public:
            inline size_type                     find( const string_type & s, size_type pos = 0 ) const
            {
               return( find( s.data(), pos, s.length()));
            }
            inline size_type                     find( const char_type * s, size_type pos, size_type n ) const
            {
               return( npos ); // todo
            }
            inline size_type                     find( const char_type * s, size_type pos = 0 ) const
            {
               return( find( s, pos, traits_type::length( s )));
            }
            inline size_type                     find( char_type c, size_type pos = 0 ) const
            {
               return( npos ); // todo
            }
         public:
            inline size_type                     rfind( const string_type & s, size_type pos = npos ) const
            {
               return( rfind( s.c_str(), pos, s.length()));
            }
            inline size_type                     rfind( const char_type * s, size_type pos, size_type n ) const
            {
               return( npos ); // todo
            }
            inline size_type                     rfind( const char_type * s, size_type pos = npos ) const
            {
               return( rfind( s, pos, traits_type::length( s )));
            }
            inline size_type                     rfind( char_type c, size_type pos = npos ) const
            {
               return( npos ); // todo
            }
         public:
            inline size_type                     find_first_of( const string_type & s, size_type pos = 0 ) const
            {
               return( find_first_of( s.c_str(), pos, s.length()));
            }
            inline size_type                     find_first_of( const char_type * s, size_type pos, size_type n ) const
            {
               return( npos ); // todo
            }
            inline size_type                     find_first_of( const char_type * s, size_type pos = 0 ) const
            {
               return( find_first_of( s, pos, traits_type::length( s )));
            }
            inline size_type                     find_first_of( char_type c, size_type pos = 0 ) const
            {
               return( npos ); // todo
            }
         public:
            inline size_type                     find_last_of( const string_type & s, size_type pos = 0 ) const
            {
               return( find_last_of( s.c_str(), pos, s.length()));
            }
            inline size_type                     find_last_of( const char_type * s, size_type pos, size_type n ) const
            {
               return( npos ); // todo
            }
            inline size_type                     find_last_of( const char_type * s, size_type pos = 0 ) const
            {
               return( find_last_of( s, pos, traits_type::length( s )));
            }
            inline size_type                     find_last_of( char_type c, size_type pos = 0 ) const
            {
               return( npos ); // todo
            }
         public:
            inline size_type                     find_first_not_of( const string_type & s, size_type pos = 0 ) const
            {
               return( find_first_not_of( s.data(), pos, s.length()));
            }
            inline size_type                     find_first_not_of( const char_type * s, size_type pos, size_type n ) const
            {
               return( npos ); // todo
            }
            inline size_type                     find_first_not_of( const char_type * s, size_type pos = 0 ) const
            {
               return( find_first_not_of( s, pos, traits_type::length( s )));
            }
            inline size_type                     find_first_not_of( char_type c, size_type pos = 0 ) const
            {
               return( npos ); // todo
            }
         public:
            inline size_type                     find_last_not_of( const string_type & s, size_type pos = 0 ) const
            {
               return( find_last_not_of( s.c_str(), pos, s.length()));
            }
            inline size_type                     find_last_not_of( const char_type * s, size_type pos, size_type n ) const
            {
               return( npos ); // todo
            }
            inline size_type                     find_last_not_of( const char_type * s, size_type pos = 0 ) const
            {
               return( find_last_not_of( s, pos, traits_type::length( s )));
            }
            inline size_type                     find_last_not_of( char_type c, size_type pos = 0 ) const
            {
               return( npos ); // todo
            }
         public:
            inline substring_type                substr( size_type pos = 0, size_type n = npos ) const
            {
               if( pos > s.size())     ErrorPolicy::out_of_range();
               return( substring_type( data() + pos, min_( n, size() - pos )));
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
               return(( res != 0 ) ? res : int( n1 - n2 ));
            }
         public:
            inline explicit  basic_string_impl()
            {
            }
      };

      template< typename CharT, class StreamPolicy, class StringPolicy, class ErrorPolicy >
      inline std::basic_ostream< CharT, StreamPolicy > & 
                                  operator<<
                                  (
                                     std::basic_ostream< CharT, StreamPolicy > & os,
                                     const basic_string_impl< StringPolicy, CharT, StreamPolicy, ErrorPolicy > & str
                                  )
      {
         return( os << str.c_str());
      }
   }}
#endif
