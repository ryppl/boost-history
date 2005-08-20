// (C) Copyright 2003: Reece H. Dunn

#ifndef BOOST__IOFM__STL_IO__BASIC_OSTREAM__HPP
#define BOOST__IOFM__STL_IO__BASIC_OSTREAM__HPP
   // std::vector outputter

   template< typename CharT, class TraitsT, typename T, class Allocator >
   inline std::basic_ostream< CharT, TraitsT > & operator<<
   (
      std::basic_ostream< CharT, TraitsT > & os,
      const std::vector< T, Allocator >    & v
   )
   {
      return( os << boost::io::formatobout( v, boost::io::containerfmt()));
   }

   // std::list outputter

   template< typename CharT, class TraitsT, typename T, class Allocator >
   inline std::basic_ostream< CharT, TraitsT > & operator<<
   (
      std::basic_ostream< CharT, TraitsT > & os,
      const std::list< T, Allocator >      & l
   )
   {
      return( os << boost::io::formatobout( l, boost::io::containerfmt()));
   }

   // std::deque outputter

   template< typename CharT, class TraitsT, typename T, class Allocator >
   inline std::basic_ostream< CharT, TraitsT > & operator<<
   (
      std::basic_ostream< CharT, TraitsT > & os,
      const std::deque< T, Allocator >     & dq
   )
   {
      return( os << boost::io::formatobout( dq, boost::io::containerfmt()));
   }

   // std::map outputter

   template
   <
      typename CharT, class TraitsT,
      typename KeyT, typename T, class Cmp, class Allocator
   >
   inline std::basic_ostream< CharT, TraitsT > & operator<<
   (
      std::basic_ostream< CharT, TraitsT >      & os,
      const std::map< KeyT, T, Cmp, Allocator > & m
   )
   {
      return
      (
         os << boost::io::formatobout
         (
            m,
            boost::io::containerfmtout( boost::io::pairfmt())
         )
      );
   }

   // std::multimap outputter

   template
   <
      typename CharT, class TraitsT,
      typename KeyT, typename T, class Cmp, class Allocator
   >
   inline std::basic_ostream< CharT, TraitsT > & operator<<
   (
      std::basic_ostream< CharT, TraitsT >           & os,
      const std::multimap< KeyT, T, Cmp, Allocator > & mm
   )
   {
      return
      (
         os << boost::io::formatobout
         (
            mm,
            boost::io::containerfmtout( boost::io::pairfmt())
         )
      );
   }

   // std::set outputter

   template
   <
      typename CharT, class TraitsT,
      typename KeyT, class Cmp, class Allocator
   >
   inline std::basic_ostream< CharT, TraitsT > & operator<<
   (
      std::basic_ostream< CharT, TraitsT >   & os,
      const std::set< KeyT, Cmp, Allocator > & s
   )
   {
      return( os << boost::io::formatobout( s, boost::io::containerfmt()));
   }

   // std::multiset outputter

   template
   <
      typename CharT, class TraitsT,
      typename KeyT, class Cmp, class Allocator
   >
   inline std::basic_ostream< CharT, TraitsT > & operator<<
   (
      std::basic_ostream< CharT, TraitsT >        & os,
      const std::multiset< KeyT, Cmp, Allocator > & ms
   )
   {
      return( os << boost::io::formatobout( ms, boost::io::containerfmt()));
   }

   // std::pair outputter

   template
   <
      typename CharT, class TraitsT,
      typename T1, typename T2
   >
   inline std::basic_ostream< CharT, TraitsT > & operator<<
   (
      std::basic_ostream< CharT, TraitsT > & os,
      const std::pair< T1, T2 >            & p
   )
   {
      return( os << boost::io::formatobout( p, boost::io::pairfmt()));
   }

#  if defined(BOOST_IOFM__HASH_CONTAINERS)
      // std::hash_map outputter

      template< typename CharT, class TraitsT, BOOST_IOFM__HASH_MAP_T >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT >            & os,
         const std::hash_map< BOOST_IOFM__HASH_MAP_ARG > & m
      )
      {
         return
         (
            os << boost::io::formatobout
            (
               m,
               boost::io::containerfmtout( boost::io::pairfmt())
            )
         );
      }

      // std::hash_multimap outputter

      template< typename CharT, class TraitsT, BOOST_IOFM__HASH_MAP_T >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT >                 & os,
         const std::hash_multimap< BOOST_IOFM__HASH_MAP_ARG > & mm
      )
      {
         return
         (
            os << boost::io::formatobout
            (
               mm,
               boost::io::containerfmtout( boost::io::pairfmt())
            )
         );
      }

      // std::hash_set outputter

      template< typename CharT, class TraitsT, BOOST_IOFM__HASH_SET_T >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT >            & os,
         const std::hash_set< BOOST_IOFM__HASH_SET_ARG > & s
      )
      {
         return( os << boost::io::formatobout( s, boost::io::containerfmt()));
      }

      // std::hash_multiset outputter

      template< typename CharT, class TraitsT, BOOST_IOFM__HASH_SET_T >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT >                 & os,
         const std::hash_multiset< BOOST_IOFM__HASH_SET_ARG > & ms
      )
      {
         return( os << boost::io::formatobout( ms, boost::io::containerfmt()));
      }
#  endif

#  if defined(BOOST_HAS_SLIST)
      // std::slist outputter

      template< typename CharT, class TraitsT, typename T, class Allocator >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT > & os,
         const std::slist< T, Allocator >     & sl
      )
      {
         return( os << boost::io::formatobout( sl, boost::io::containerfmt()));
      }
#  endif
#endif
