// (C) Copyright 2003-2004: Jonathan Turkanis; Reece H. Dunn

#ifndef BOOST_IOFM_TypeTraits_HPP
#define BOOST_IOFM_TypeTraits_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/outfmt/detail/range.hpp>

#  include <complex>     // std::complex
#  include <string>      // std::basic_string
#  include <deque>       // std::deque
#  include <list>        // std::list
#  include <vector>      // std::vector
#  include <set>         // std::set, std::multiset
#  include <map>         // std::map, std::multimap
#  include <utility>     // std::pair

#  if defined(BOOST_IOFM_HASH_CONTAINERS)
#     include <hash_set> // std::hash_set, std::hash_multiset
#     include <hash_map> // std::hash_map, std::hash_multimap
#  endif
#  if defined(BOOST_HAS_SLIST)
#     include <slist>    // std::slist
#  endif

#  include <boost/compressed_pair.hpp>           
#  include <boost/rational.hpp>
#  if !defined(BOOST_IOFM_NO_LIB_INTERVAL)
#     include <boost/numeric/interval/interval.hpp>
#  endif
#  if !defined(BOOST_IOFM_NO_LIB_QUATERNION)
#     include <boost/math/quaternion.hpp>           
#  endif
#  if !defined(BOOST_IOFM_NO_LIB_OCTONION)
#     include <boost/math/octonion.hpp>             
#  endif

#  include <boost/mpl/or.hpp>
#  include <boost/mpl/if.hpp>
#  include <boost/mpl/apply_if.hpp>
#  include <boost/mpl/identity.hpp>

   namespace boost { namespace io
   {
      // classification values

      static const int                 basic_type             =  1;
      static const int                 std_string_type        =  2;
      static const int                 array_type             =  3;
      static const int                 nary_type              =  4;
      static const int                 pair_type              =  5;
      static const int                 nary2value_type        =  6;
      static const int                 nary2base_type         =  7;
      static const int                 nary2int_type          =  8;
      static const int                 seq_container_type     =  9;
      static const int                 assoc_container_type   = 10;
      static const int                 range_type             = 11;
      static const int                 wrapped_type           = 12;
      static const int                 state_type             = 13;

      template< int id >
      struct seq_type
      {
         char                          type[ id ];
      };

      // classification function

      namespace detail
      {
         boost::io::seq_type< boost::io::basic_type > classify( ... );
      }

      // type checker implementation

      template< typename T >
      struct get_typeid
      {
         static const T *              io;
         struct type
         {
            BOOST_STATIC_CONSTANT( int, value = sizeof( boost::io::detail::classify( io )));
         };
         BOOST_STATIC_CONSTANT( int, value = type::value );
      };

      template< typename T, int id >
      struct is_type
      {
         static const T *              io;
         struct type
         {
            BOOST_STATIC_CONSTANT( bool, value = ( sizeof( boost::io::detail::classify( io )) == id ));
         };
         BOOST_STATIC_CONSTANT( bool, value = type::value  );
      };

      template< class FmtObject, int id >
      struct is_formatter
      {
         struct type
         {
            BOOST_STATIC_CONSTANT( bool, value = ( sizeof( FmtObject::formatter_type ) == id ));
         };
         BOOST_STATIC_CONSTANT( bool, value = type::value  );
      };

      // helpers -- simple type classification

      template< typename T >
      struct is_basic: public is_type< T, basic_type >
      {
      };

      template< typename T >
      struct is_std_string: public is_type< T, std_string_type >
      {
      };

      template< typename T >
      struct is_nary: public is_type< T, nary_type >
      {
      };

      template< typename T >
      struct is_array: public is_type< T, array_type >
      {
      };

      template< typename T >
      struct is_seq_container: public is_type< T, seq_container_type >
      {
      };

      template< typename T >
      struct is_assoc_container: public is_type< T, assoc_container_type >
      {
      };

      template< typename T >
      struct is_range: public is_type< T, range_type >
      {
      };

      // helpers -- grouped type classification

      template< typename T >
      struct is_container: public mpl::or_
                           <
                              is_seq_container< T >,
                              is_assoc_container< T >
                           >
      {
      };

      template< typename T >
      struct is_pair: public mpl::or_
                      <
                         is_type< T, pair_type >,
                         is_type< T, nary2value_type >,
                         is_type< T, nary2base_type >,
                         is_type< T, nary2int_type >
                      >
      {
      };
   }}

   // type classification

   // helper macros

#  define BOOST_IO_CLASSIFY_TYPE_1( type, id )\
      template< typename T1 >                 \
      boost::io::seq_type< id > classify( const type< T1 > * );
#  define BOOST_IO_CLASSIFY_TYPE_2( type, id )\
      template< typename T1, typename T2 >    \
      boost::io::seq_type< id > classify( const type< T1, T2 > * );
#  define BOOST_IO_CLASSIFY_TYPE_3( type, id )\
      template< typename T1, typename T2, typename T3 >\
      boost::io::seq_type< id > classify( const type< T1, T2, T3 > * );
#  define BOOST_IO_CLASSIFY_TYPE_4( type, id )\
      template< typename T1, typename T2, typename T3, typename T4 >\
      boost::io::seq_type< id > classify( const type< T1, T2, T3, T4 > * );
#  define BOOST_IO_CLASSIFY_TYPE_5( type, id )\
      template< typename T1, typename T2, typename T3, typename T4, typename T5 >\
      boost::io::seq_type< id > classify( const type< T1, T2, T3, T4, T5 > * );

   namespace boost { namespace io { namespace detail
   {
      // array types

      template< typename T, int N >
      boost::io::seq_type< boost::io::array_type > classify( const T( * )[ N ] );

      // standard containers and types

      BOOST_IO_CLASSIFY_TYPE_2( std::basic_string, boost::io::std_string_type );
      BOOST_IO_CLASSIFY_TYPE_1( std::complex,      boost::io::nary2value_type );
      BOOST_IO_CLASSIFY_TYPE_2( std::pair,         boost::io::pair_type );
      BOOST_IO_CLASSIFY_TYPE_2( std::deque,        boost::io::seq_container_type );
      BOOST_IO_CLASSIFY_TYPE_2( std::list,         boost::io::seq_container_type );
      BOOST_IO_CLASSIFY_TYPE_2( std::vector,       boost::io::seq_container_type );
      BOOST_IO_CLASSIFY_TYPE_3( std::set,          boost::io::seq_container_type );
      BOOST_IO_CLASSIFY_TYPE_3( std::multiset,     boost::io::seq_container_type );
      BOOST_IO_CLASSIFY_TYPE_4( std::map,          boost::io::assoc_container_type );
      BOOST_IO_CLASSIFY_TYPE_4( std::multimap,     boost::io::assoc_container_type );

      // standard extension container types

#     if defined(BOOST_IOFM_HASH_CONTAINERS)
#        if defined(BOOST_DINKUMWARE_STDLIB)
            BOOST_IO_CLASSIFY_TYPE_3( std::hash_set,      boost::io::seq_container_type );
            BOOST_IO_CLASSIFY_TYPE_3( std::hash_multiset, boost::io::seq_container_type );
            BOOST_IO_CLASSIFY_TYPE_4( std::hash_map,      boost::io::assoc_container_type );
            BOOST_IO_CLASSIFY_TYPE_4( std::hash_multimap, boost::io::assoc_container_type );
#        else // SGI containers
            BOOST_IO_CLASSIFY_TYPE_4( std::hash_set,      boost::io::seq_container_type );
            BOOST_IO_CLASSIFY_TYPE_4( std::hash_multiset, boost::io::seq_container_type );
            BOOST_IO_CLASSIFY_TYPE_5( std::hash_map,      boost::io::assoc_container_type );
            BOOST_IO_CLASSIFY_TYPE_5( std::hash_multimap, boost::io::assoc_container_type );
#        endif
#     endif

#     if defined(BOOST_HAS_SLIST)
         BOOST_IO_CLASSIFY_TYPE_2( std::slist, boost::io::seq_container_type );
#     endif

      // Boost types

      BOOST_IO_CLASSIFY_TYPE_2( boost::compressed_pair, boost::io::pair_type );
      BOOST_IO_CLASSIFY_TYPE_1( boost::rational,        boost::io::nary2int_type );

#     if !defined(BOOST_IOFM_NO_LIB_INTERVAL)
         BOOST_IO_CLASSIFY_TYPE_2( boost::numeric::interval, boost::io::nary2base_type );
#     endif

#     if !defined(BOOST_IOFM_NO_LIB_QUATERNION)
         BOOST_IO_CLASSIFY_TYPE_1( boost::math::quaternion, boost::io::pair_type );
#     endif

#     if !defined(BOOST_IOFM_NO_LIB_OCTONION)
         BOOST_IO_CLASSIFY_TYPE_1( boost::math::octonion, boost::io::pair_type );
#     endif

      BOOST_IO_CLASSIFY_TYPE_1( boost::io::range_t, boost::io::range_type );
   }}}
#endif
