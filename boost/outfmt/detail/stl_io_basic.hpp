// (C) Copyright 2003-2004: Reece H. Dunn

#ifndef BOOST_IOFM_STL_IO_BASIC_OSTREAM_HPP
#define BOOST_IOFM_STL_IO_BASIC_OSTREAM_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

   // std::vector

   template< typename CharT, class TraitsT, typename T, class Allocator >
   inline std::basic_ostream< CharT, TraitsT > & operator<<
   (
      std::basic_ostream< CharT, TraitsT > & os,
      const std::vector< T, Allocator >    & v
   )
   {
      return( os << boost::io::formatob( v, boost::io::containerfmt()));
   }

   template< typename CharT, class TraitsT, typename T, class Allocator >
   inline std::basic_istream< CharT, TraitsT > & operator>>
   (
      std::basic_istream< CharT, TraitsT > & is,
      std::vector< T, Allocator >          & v
   )
   {
      return( is >> boost::io::formatob( v, boost::io::containerfmt()));
   }

   // std::list

   template< typename CharT, class TraitsT, typename T, class Allocator >
   inline std::basic_ostream< CharT, TraitsT > & operator<<
   (
      std::basic_ostream< CharT, TraitsT > & os,
      const std::list< T, Allocator >      & l
   )
   {
      return( os << boost::io::formatob( l, boost::io::containerfmt()));
   }

   template< typename CharT, class TraitsT, typename T, class Allocator >
   inline std::basic_istream< CharT, TraitsT > & operator>>
   (
      std::basic_istream< CharT, TraitsT > & is,
      std::list< T, Allocator >            & l
   )
   {
      return( is >> boost::io::formatob( l, boost::io::containerfmt()));
   }

   // std::deque 

   template< typename CharT, class TraitsT, typename T, class Allocator >
   inline std::basic_ostream< CharT, TraitsT > & operator<<
   (
      std::basic_ostream< CharT, TraitsT > & os,
      const std::deque< T, Allocator >     & dq
   )
   {
      return( os << boost::io::formatob( dq, boost::io::containerfmt()));
   }

   template< typename CharT, class TraitsT, typename T, class Allocator >
   inline std::basic_istream< CharT, TraitsT > & operator>>
   (
      std::basic_istream< CharT, TraitsT > & is,
      std::deque< T, Allocator >           & dq
   )
   {
      return( is >> boost::io::formatob( dq, boost::io::containerfmt()));
   }

   // std::map 

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
         os << boost::io::formatob
         (
            m,
            boost::io::containerfmt( boost::io::pairfmt())
         )
      );
   }

   template
   <
      typename CharT, class TraitsT,
      typename KeyT, typename T, class Cmp, class Allocator
   >
   inline std::basic_istream< CharT, TraitsT > & operator>>
   (
      std::basic_istream< CharT, TraitsT > & is,
      std::map< KeyT, T, Cmp, Allocator >  & m
   )
   {
      return
      (
         is >> boost::io::formatob
         (
            m,
            boost::io::containerfmt( boost::io::pairfmt())
         )
      );
   }

   // std::multimap 

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
         os << boost::io::formatob
         (
            mm,
            boost::io::containerfmt( boost::io::pairfmt())
         )
      );
   }

   template
   <
      typename CharT, class TraitsT,
      typename KeyT, typename T, class Cmp, class Allocator
   >
   inline std::basic_istream< CharT, TraitsT > & operator>>
   (
      std::basic_istream< CharT, TraitsT >     & is,
      std::multimap< KeyT, T, Cmp, Allocator > & mm
   )
   {
      return
      (
         is >> boost::io::formatob
         (
            mm,
            boost::io::containerfmt( boost::io::pairfmt())
         )
      );
   }

   // std::set

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
      return( os << boost::io::formatob( s, boost::io::containerfmt()));
   }

   template
   <
      typename CharT, class TraitsT,
      typename KeyT, class Cmp, class Allocator
   >
   inline std::basic_istream< CharT, TraitsT > & operator>>
   (
      std::basic_istream< CharT, TraitsT > & is,
      std::set< KeyT, Cmp, Allocator >     & s
   )
   {
      return( is >> boost::io::formatob( s, boost::io::containerfmt()));
   }

   // std::multiset

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
      return( os << boost::io::formatob( ms, boost::io::containerfmt()));
   }

   template
   <
      typename CharT, class TraitsT,
      typename KeyT, class Cmp, class Allocator
   >
   inline std::basic_istream< CharT, TraitsT > & operator>>
   (
      std::basic_istream< CharT, TraitsT >  & is,
      std::multiset< KeyT, Cmp, Allocator > & ms
   )
   {
      return( is >> boost::io::formatob( ms, boost::io::containerfmt()));
   }

   // std::pair

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
      return( os << boost::io::formatob( p, boost::io::pairfmt()));
   }

   template
   <
      typename CharT, class TraitsT,
      typename T1, typename T2
   >
   inline std::basic_istream< CharT, TraitsT > & operator>>
   (
      std::basic_istream< CharT, TraitsT > & is,
      std::pair< T1, T2 >                  & p
   )
   {
      return( is >> boost::io::formatob( p, boost::io::pairfmt()));
   }

#  if defined(BOOST_IOFM_HASH_CONTAINERS)
      // std::hash_map

      template< typename CharT, class TraitsT, BOOST_IOFM_HASH_MAP_T >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT >           & os,
         const std::hash_map< BOOST_IOFM_HASH_MAP_ARG > & m
      )
      {
         return
         (
            os << boost::io::formatob
            (
               m,
               boost::io::containerfmt( boost::io::pairfmt())
            )
         );
      }

      template< typename CharT, class TraitsT, BOOST_IOFM_HASH_MAP_T >
      inline std::basic_istream< CharT, TraitsT > & operator>>
      (
         std::basic_istream< CharT, TraitsT >     & is,
         std::hash_map< BOOST_IOFM_HASH_MAP_ARG > & m
      )
      {
         return
         (
            is >> boost::io::formatob
            (
               m,
               boost::io::containerfmt( boost::io::pairfmt())
            )
         );
      }

      // std::hash_multimap

      template< typename CharT, class TraitsT, BOOST_IOFM_HASH_MAP_T >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT >                & os,
         const std::hash_multimap< BOOST_IOFM_HASH_MAP_ARG > & mm
      )
      {
         return
         (
            os << boost::io::formatob
            (
               mm,
               boost::io::containerfmt( boost::io::pairfmt())
            )
         );
      }

      template< typename CharT, class TraitsT, BOOST_IOFM_HASH_MAP_T >
      inline std::basic_istream< CharT, TraitsT > & operator>>
      (
         std::basic_istream< CharT, TraitsT >          & is,
         std::hash_multimap< BOOST_IOFM_HASH_MAP_ARG > & mm
      )
      {
         return
         (
            is >> boost::io::formatob
            (
               mm,
               boost::io::containerfmt( boost::io::pairfmt())
            )
         );
      }

      // std::hash_set

      template< typename CharT, class TraitsT, BOOST_IOFM_HASH_SET_T >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT >           & os,
         const std::hash_set< BOOST_IOFM_HASH_SET_ARG > & s
      )
      {
         return( os << boost::io::formatob( s, boost::io::containerfmt()));
      }

      template< typename CharT, class TraitsT, BOOST_IOFM_HASH_SET_T >
      inline std::basic_istream< CharT, TraitsT > & operator>>
      (
         std::basic_istream< CharT, TraitsT >     & is,
         std::hash_set< BOOST_IOFM_HASH_SET_ARG > & s
      )
      {
         return( is >> boost::io::formatob( s, boost::io::containerfmt()));
      }

      // std::hash_multiset

      template< typename CharT, class TraitsT, BOOST_IOFM_HASH_SET_T >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT >                & os,
         const std::hash_multiset< BOOST_IOFM_HASH_SET_ARG > & ms
      )
      {
         return( os << boost::io::formatob( ms, boost::io::containerfmt()));
      }

      template< typename CharT, class TraitsT, BOOST_IOFM_HASH_SET_T >
      inline std::basic_istream< CharT, TraitsT > & operator>>
      (
         std::basic_istream< CharT, TraitsT >          & is,
         std::hash_multiset< BOOST_IOFM_HASH_SET_ARG > & ms
      )
      {
         return( is >> boost::io::formatob( ms, boost::io::containerfmt()));
      }
#  endif

#  if defined(BOOST_HAS_SLIST) // std::slist
      template< typename CharT, class TraitsT, typename T, class Allocator >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT > & os,
         const std::slist< T, Allocator >     & sl
      )
      {
         return( os << boost::io::formatob( sl, boost::io::containerfmt()));
      }

      template< typename CharT, class TraitsT, typename T, class Allocator >
      inline std::basic_istream< CharT, TraitsT > & operator>>
      (
         std::basic_istream< CharT, TraitsT > & is,
         std::slist< T, Allocator >           & sl
      )
      {
         return( is >> boost::io::formatob( sl, boost::io::containerfmt()));
      }
#  endif
#endif
