/***************************************************************************
                        detail.hpp  -  description
                       ---------------------------
    begin                : Fri Jun 21 2002
    copyright            : (C) 2002 by Itay Maman
    email                : itay_maman@yahoo.com
 ***************************************************************************/


//
// Permission to use, copy, modify, and distribute this software for any
// purpose is hereby granted without fee, provided that this copyright and
// permissions notice appear in all copies and derivatives.
//
// This software is provided "as is" without express or implied warranty.
//
// For more information, see http://www.boost.org
//


///////////////////////////////////////////////////////////////////////


#ifndef _VARIANT_DETAIL_HPP_INC_
#define _VARIANT_DETAIL_HPP_INC_

#include <boost/config.hpp>
#include <boost/type_traits.hpp>
#include <boost/pending/ct_if.hpp>


///////////////////////////////////////////////////////////////////////


namespace boost { namespace detail {

namespace variant {


   struct unused_tag
   {
      typedef unused_tag type;
   };

   struct plain_never_used : unused_tag { };

   template<int Index>
   struct nu : unused_tag
   {
      BOOST_STATIC_CONSTANT(int, the_index = Index );
   };

   template<int n>
   struct int_to_type
   {
      BOOST_STATIC_CONSTANT(int, value = n );
   };


   struct tag_1 
   {
      char d_;
   };


   struct tag_2
   {
      tag_1 d_[2];
   };

   struct neutral_tag
   {
      typedef neutral_tag type;

   }; //neutral_tag


   template<typename T>
   struct is_neutral
   {
      BOOST_STATIC_CONSTANT(bool, value = (is_same<T, neutral_tag>::value) ); 

   }; //is_neutral


   ///////////////////////////////////////////////////////////////////////


#ifdef BOOST_VARIANT_NO_ASSIGN_HELPER


#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION


   template<bool Trg2IsNeutral>
   struct choose_conversion_impl
   {

      template<typename Src, typename Trg1, typename Trg2>
      struct inner
      {
         static tag_1 decide(const Trg1& );
         static tag_2 decide(const Trg2& );

         BOOST_STATIC_CONSTANT( int, sz = sizeof(decide(*static_cast<Src*>(0))) );
         typedef typename boost::ct_if
         <
            sz == sizeof(tag_1), 
            Trg1, 
            Trg2
         >::type type;

      }; //inner

   }; //choose_conversion_impl
 

   template<>
   struct choose_conversion_impl<true>
   {

      template<typename Src, typename Trg1, typename Trg2>
      struct inner
      {
         typedef typename ct_if
         <
            is_convertible<Src, Trg1>::value,
            Trg1,
            Trg2
         >::type type;
      }; //inner

   }; //choose_conversion_impl
 

   template<typename Src, typename Trg1, typename Trg2>
   struct choose_conversion
   {
      BOOST_STATIC_CONSTANT(bool, trg2_is_neutral = is_neutral<Trg2>::value);

      typedef typename 
         choose_conversion_impl<trg2_is_neutral>::template inner<Src, Trg1, Trg2>::type 
         type;

   }; //choose_conversion


#else  //BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION


   template<bool Trg2IsNeutral, typename Src, typename Trg1, typename Trg2>
   struct choose_conversion_impl;


   template<typename Src, typename Trg1, typename Trg2>
   struct choose_conversion_impl<true, Src, Trg1, Trg2>
   {
      typedef typename ct_if
      <
         is_convertible<Src, Trg1>::value,
         Trg1,
         Trg2
      >::type type;

   }; //choose_conversion_impl


   template<bool Trg2IsNeutral, typename Src, typename Trg1, typename Trg2>
   struct choose_conversion_impl
   {

      static tag_1 decide(Trg1 );
      static tag_2 decide(Trg2 );

      typedef typename boost::ct_if
      <
         sizeof( decide(Src()) ) == sizeof(tag_1), 
         Trg1, 
         Trg2
      >::type type;

   }; //choose_conversion_impl
 

   template<typename Src, typename Trg1, typename Trg2>
   struct choose_conversion
   {
      typedef typename choose_conversion_impl
      <
         is_neutral<Trg2>::value, 
         Src, 
         Trg1, 
         Trg2
      >::type type;

   }; //choose_conversion


///////////////////////////////////////////////////////////////////////


#endif //BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif //BOOST_VARIANT_NO_ASSIGN_HELPER


///////////////////////////////////////////////////////////////////////


} //namespace variant
} //namespace detail
} //namespace boost



#endif //_VARIANT_DETAIL_HPP_INC_


