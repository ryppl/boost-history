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

#  if defined(BOOST_IO_NO_DEPRECATED_MODIFIER) && defined(BOOST_DINKUMWARE_STDLIB)
#     define _DEPRECATED_TEMP _DEPRECATED
#     define _DEPRECATED
#  endif
#  if defined(BOOST_IOFM_HASH_CONTAINERS)
#     include <hash_set> // std::hash_set, std::hash_multiset
#     include <hash_map> // std::hash_map, std::hash_multimap
#  endif
#  if defined(BOOST_IO_NO_DEPRECATED_MODIFIER) && defined(BOOST_DINKUMWARE_STDLIB)
#     define _DEPRECATED _DEPRECATED_TEMP
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
      static const int                 seq_container_type     =  9; // list, ...
      static const int                 assoc_container_type   = 10; // map, ...
      static const int                 set_container_type     = 11; // set, ...
      static const int                 range_type             = 12;
      static const int                 wrapped_type           = 13;
      static const int                 state_type             = 14;

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
            BOOST_STATIC_CONSTANT( bool, value = ( sizeof( BOOST_DEDUCED_TYPENAME FmtObject::formatter_type ) == id ));
         };
         BOOST_STATIC_CONSTANT( bool, value = type::value  );
      };

      // helpers -- simple type classification
      
#     define BOOST_IOFM_IS_TYPE( name, type )  \
         template< typename T >                \
         struct name: public is_type< T, type >\
         {                                     \
         };

      BOOST_IOFM_IS_TYPE( is_basic,           basic_type )
      BOOST_IOFM_IS_TYPE( is_std_string,      std_string_type )
      BOOST_IOFM_IS_TYPE( is_nary,            nary_type )
      BOOST_IOFM_IS_TYPE( is_array,           array_type )
      BOOST_IOFM_IS_TYPE( is_seq_container,   seq_container_type )
      BOOST_IOFM_IS_TYPE( is_set_container,   set_container_type )
      BOOST_IOFM_IS_TYPE( is_assoc_container, assoc_container_type )
      BOOST_IOFM_IS_TYPE( is_range,           range_type )

#     undef BOOST_IOFM_IS_TYPE

      // helpers -- grouped type classification

      template< typename T >
      struct is_container: public mpl::or_
                           <
                              is_seq_container< T >,
                              is_set_container< T >,
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
      BOOST_IO_CLASSIFY_TYPE_3( std::set,          boost::io::set_container_type );
      BOOST_IO_CLASSIFY_TYPE_3( std::multiset,     boost::io::set_container_type );
      BOOST_IO_CLASSIFY_TYPE_4( std::map,          boost::io::assoc_container_type );
      BOOST_IO_CLASSIFY_TYPE_4( std::multimap,     boost::io::assoc_container_type );

      // standard extension container types

#     if defined(BOOST_IOFM_HASH_CONTAINERS)
#        if defined(BOOST_DINKUMWARE_STDLIB)
            BOOST_IO_CLASSIFY_TYPE_3( BOOST_IOFM_STDEXT::hash_set,      boost::io::set_container_type );
            BOOST_IO_CLASSIFY_TYPE_3( BOOST_IOFM_STDEXT::hash_multiset, boost::io::set_container_type );
            BOOST_IO_CLASSIFY_TYPE_4( BOOST_IOFM_STDEXT::hash_map,      boost::io::assoc_container_type );
            BOOST_IO_CLASSIFY_TYPE_4( BOOST_IOFM_STDEXT::hash_multimap, boost::io::assoc_container_type );
#        else // SGI containers
            BOOST_IO_CLASSIFY_TYPE_4( BOOST_IOFM_STDEXT::hash_set,      boost::io::set_container_type );
            BOOST_IO_CLASSIFY_TYPE_4( BOOST_IOFM_STDEXT::hash_multiset, boost::io::set_container_type );
            BOOST_IO_CLASSIFY_TYPE_5( BOOST_IOFM_STDEXT::hash_map,      boost::io::assoc_container_type );
            BOOST_IO_CLASSIFY_TYPE_5( BOOST_IOFM_STDEXT::hash_multimap, boost::io::assoc_container_type );
#        endif
#     endif

#     if defined(BOOST_HAS_SLIST) // hack: slist does not support push_back
         BOOST_IO_CLASSIFY_TYPE_2( BOOST_IOFM_STDEXT::slist, boost::io::set_container_type );
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
