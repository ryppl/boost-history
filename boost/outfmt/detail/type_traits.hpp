// (C) Copyright 2003-2004: Jonathan Turkanis; Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_TypeTraits_HPP
#define BOOST_IOFM_TypeTraits_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/config.hpp>               // partial spec.
#  include <boost/outfmt/detail/range.hpp>
#  include <string>

#  include <boost/mpl/or.hpp>
#  include <boost/mpl/if.hpp>
#  include <boost/mpl/apply_if.hpp>
#  include <boost/mpl/equal_to.hpp>
#  include <boost/mpl/identity.hpp>
#  include <boost/mpl/int.hpp>          // used by CLASSIFY_TYPE and the is_type template.
#  include <boost/preprocessor/enum_params.hpp>

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

         // array types:

         template< typename T, int N >
         boost::io::seq_type< boost::io::array_type > classify( const T( * )[ N ] );
      }

      // type checker implementation

      template< typename T >
      struct get_typeid
      {
         struct type
         {
            BOOST_STATIC_CONSTANT( int, value =
                sizeof( boost::io::detail::classify( static_cast<T*>(0) )));
         };
         BOOST_STATIC_CONSTANT( int, value = type::value );
      };

      template< typename T, int id >
      struct is_type : mpl::equal_to< get_typeid<T>, mpl::int_<id> > { };

      template< class FormatObject, int id >
      struct is_formatter
      {
         struct type
         {
#           if defined(__BORLANDC__)
               BOOST_STATIC_CONSTANT( bool, value = ( sizeof( FormatObject::formatter_type ) == id ));
#           else
               BOOST_STATIC_CONSTANT( bool, value = ( sizeof( BOOST_DEDUCED_TYPENAME FormatObject::formatter_type ) == id ));
#           endif
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

#  ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#    define BOOST_IO_CLASSIFY_TYPE(arity, type, id)              \
       namespace boost { namespace io {                          \
       template<BOOST_PP_ENUM_PARAMS(arity, typename T)>         \
       struct get_typeid< type<BOOST_PP_ENUM_PARAMS(arity, T)> > \
           : mpl::int_<id> { };                                  \
       } }                                                       \
       /**/
#  else
#    define BOOST_IO_CLASSIFY_TYPE(arity, type, id)                       \
       namespace boost { namespace io { namespace detail {                \
       template<BOOST_PP_ENUM_PARAMS(arity, typename T)>                  \
       boost::io::seq_type< id >                                          \
       classify( const volatile type<BOOST_PP_ENUM_PARAMS(arity, T)> * ); \
       } } }                                                              \
       /**/
#  endif

   BOOST_IO_CLASSIFY_TYPE( 2, std::basic_string,  boost::io::std_string_type )
   BOOST_IO_CLASSIFY_TYPE( 1, boost::io::range_t, boost::io::range_type )
#endif
