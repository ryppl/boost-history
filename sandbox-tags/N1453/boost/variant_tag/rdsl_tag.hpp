/***************************************************************************
                        rdsl.hpp  -  description
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


//
//This source code is based on Typelist.h (part of the loki library):
//

// The Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// The author or Addison-Welsey Longman make no representations about the 
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.
//


///////////////////////////////////////////////////////////////////////


#ifndef _RDSL_HPP_INC_
#define _RDSL_HPP_INC_

#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/pending/ct_if.hpp>
#include <boost/math/common_factor.hpp>
#include <boost/variant_tag/detail.hpp>


#define BOOST_RDSL_1(a1) boost::detail::variant::rdsl<a1,         \
   boost::detail::variant::eol_tag>

#define BOOST_RDSL_2(a1, a2)                                      \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_1(a2) >

#define BOOST_RDSL_3(a1, a2, a3)                                  \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_2(a2, a3) >

#define BOOST_RDSL_4(a1, a2, a3, a4)                              \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_3(a2, a3, a4) >

#define BOOST_RDSL_5(a1, a2, a3, a4, a5)                          \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_4(a2, a3, a4, a5) >

#define BOOST_RDSL_6(a1, a2, a3, a4, a5, a6)                      \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_5(a2, a3, a4, a5, a6) >

#define BOOST_RDSL_7(a1, a2, a3, a4, a5, a6, a7)                  \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_6(a2, a3, a4, a5, a6, a7) >

#define BOOST_RDSL_8(a1, a2, a3, a4, a5, a6, a7, a8)              \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_7(a2, a3, a4, a5, a6, a7, a8) > 

#define BOOST_RDSL_9(a1, a2, a3, a4, a5, a6, a7, a8, a9)          \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_8(a2, a3, a4, a5, a6, a7, a8, a9) >

#define BOOST_RDSL_10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)    \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_9(a2, a3, a4, a5, a6, a7, a8, a9, a10) >

#define BOOST_RDSL_11(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11)                                                           \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_10(a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) >

#define BOOST_RDSL_12(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,    \
   a11, a12)                                                      \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_11(a2, a3, a4, a5, a6, a7, a8, a9, a10,    \
      a11, a12) >

#define BOOST_RDSL_13(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,    \
   a11, a12, a13)                                                 \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_12(a2, a3, a4, a5, a6, a7, a8, a9, a10,    \
      a11, a12, a13) >

#define BOOST_RDSL_14(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,    \
   a11, a12, a13, a14)                                            \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_13(a2, a3, a4, a5, a6, a7, a8, a9, a10,    \
      a11, a12, a13, a14) >


#define BOOST_RDSL_15(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,    \
   a11, a12, a13, a14, a15)                                       \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_14(a2, a3, a4, a5, a6, a7, a8, a9, a10,    \
      a11, a12, a13, a14, a15) >


#define BOOST_RDSL_16(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,    \
   a11, a12, a13, a14, a15, a16)                                  \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_15(a2, a3, a4, a5, a6, a7, a8, a9, a10,    \
      a11, a12, a13, a14, a15, a16) >


#define BOOST_RDSL_17(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,    \
   a11, a12, a13, a14, a15, a16, a17)                             \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_16(a2, a3, a4, a5, a6, a7, a8, a9, a10,    \
      a11, a12, a13, a14, a15, a16, a17) >


#define BOOST_RDSL_18(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,    \
   a11, a12, a13, a14, a15, a16, a17, a18)                        \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_17(a2, a3, a4, a5, a6, a7, a8, a9, a10,    \
      a11, a12, a13, a14, a15, a16, a17, a18) >


#define BOOST_RDSL_19(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,    \
   a11, a12, a13, a14, a15, a16, a17, a18, a19)                   \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_18(a2, a3, a4, a5, a6, a7, a8, a9, a10,    \
      a11, a12, a13, a14, a15, a16, a17, a18, a19) >


#define BOOST_RDSL_20(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,    \
   a11, a12, a13, a14, a15, a16, a17, a18, a19, a20)              \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_19(a2, a3, a4, a5, a6, a7, a8, a9, a10,   \
      a11, a12, a13, a14, a15, a16, a17, a18, a19, a20) >

#define BOOST_RDSL_21(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,    \
   a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21)              \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_20(a2, a3, a4, a5, a6, a7, a8, a9, a10,   \
      a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21) >

#define BOOST_RDSL_22(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,    \
   a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22)          \
   boost::detail::variant::rdsl<a1, BOOST_RDSL_21(a2, a3, a4, a5, a6, a7, a8, a9, a10,   \
      a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22) >

///////////////////////////////////////////////////////////////////////

namespace boost {

namespace detail {
namespace variant {


   template<typename Head, typename Tail>
   struct rdsl
   {
      typedef Head t_head;
      typedef Tail t_tail;
   }; //rdsl


   ///////////////////////////////////////////////////////////////////////


   struct eol_tag 
   { 
      typedef eol_tag t_head;
      typedef eol_tag t_tail;
   };


   template<typename Sequence>
   struct get_head
   {
      typedef typename Sequence::t_head type;
   };  //get_head


   template<typename Sequence>
   struct get_rest
   {
      typedef typename Sequence::t_tail type;
   }; //get_rest


   template<typename Sequence>
   struct is_empty
   {
      typedef is_same<Sequence, eol_tag> t_temp;
      BOOST_STATIC_CONSTANT(int, value = t_temp::value);
   }; //is_empty
 

   ///////////////////////////////////////////////////////////////////////


#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION


   ///////////////////////////////////////////////////////////////////////



   template<typename Tail>
   struct length_impl
   {
      template<typename Head>
      struct inner
      {
         typedef typename Tail::t_head t_head;
         typedef typename Tail::t_tail t_tail;

         typedef typename length_impl<t_tail>::template inner<t_head> t_temp;
         BOOST_STATIC_CONSTANT(int, value = 1 + t_temp::value);

      }; //inner

   }; //length_impl


   template<>
   struct length_impl<eol_tag>
   {
      template<typename Head>
      struct inner
      {
         BOOST_STATIC_CONSTANT(int, value = 1);
      };

   }; //length_impl


   template<typename Sequence>
   struct get_size
   {
      typedef typename Sequence::t_head t_head;
      typedef typename Sequence::t_tail t_tail;

      typedef length_impl<t_tail>::template inner<t_head> t_temp;
      BOOST_STATIC_CONSTANT(int, value = t_temp::value);

   }; //get_size


   ///////////////////////////////////////////////////////////////////////


   template<bool AreTheSame>
   struct first_derived_helper
   {
      template<typename NewHead, typename NewTail, typename WhichType>
      struct action
      {
         BOOST_STATIC_CONSTANT( int, value = 0);
      }; //inner

   }; //index_of_helper


   template<typename Tail>
   struct first_derived_impl
   {

      template<typename Head, typename WhichType>
      struct inner
      {
         typedef is_base_and_derived<WhichType, Head> t_temp_1;
         BOOST_STATIC_CONSTANT(bool, identical = t_temp_1::value);

         typedef typename Tail::t_head t_new_head;
         typedef typename Tail::t_tail t_new_tail;

         typedef typename 
            first_derived_helper<identical>::template action<t_new_head, t_new_tail, WhichType> 
            t_temp_2;

         BOOST_STATIC_CONSTANT( int, value = t_temp_2::value);

      }; //inner

   }; //index_of_impl


   template<>
   struct first_derived_impl<eol_tag>
   {
      template<typename Head, typename WhichType>
      struct inner
      {
         typedef is_base_and_derived<WhichType, Head> t_temp;
         BOOST_STATIC_CONSTANT(int, value = t_temp::value ? 0 : -1);
      };

   }; //index_of_impl


   template<>
   struct first_derived_helper<false>
   {
      template<typename NewHead, typename NewTail, typename WhichType>
      struct action
      {
         typedef first_derived_impl<NewTail>::template inner<NewHead, WhichType> t_temp;
         BOOST_STATIC_CONSTANT( int, temp = t_temp::value);   

         BOOST_STATIC_CONSTANT( int, value = temp >= 0 ? 1+temp : -1 );
      }; //inner

   }; //index_of_helper


   template<typename Sequence, typename WhichType>
   struct first_derived
   {
      typedef typename Sequence::t_head t_head;
      typedef typename Sequence::t_tail t_tail;

      typedef first_derived_impl<t_tail>::template inner<t_head, WhichType> t_temp;
      BOOST_STATIC_CONSTANT(int, value = t_temp::value);

   }; //type_to_tid


   ///////////////////////////////////////////////////////////////////////

   
   template<bool AreTheSame>
   struct index_of_helper
   {
      template<typename NewHead, typename NewTail, typename WhichType>
      struct action
      {
         BOOST_STATIC_CONSTANT( int, value = 0);
      }; //inner

   }; //index_of_helper



   template<typename Tail>
   struct index_of_impl
   {

      template<typename Head, typename WhichType>
      struct inner
      {
         typedef is_same<Head, WhichType> t_temp_1;
         BOOST_STATIC_CONSTANT(bool, identical = t_temp_1::value);

         typedef typename Tail::t_head t_new_head;
         typedef typename Tail::t_tail t_new_tail;

         typedef typename 
            index_of_helper<identical>::template action<t_new_head, t_new_tail, WhichType> 
            t_temp_2;

         BOOST_STATIC_CONSTANT( int, value = t_temp_2::value);

      }; //inner

   }; //index_of_impl


   template<>
   struct index_of_impl<eol_tag>
   {
      template<typename Head, typename WhichType>
      struct inner
      {
         typedef is_same<Head, WhichType> t_temp;
         BOOST_STATIC_CONSTANT(int, value = t_temp::value ? 0 : -1);
      };

   }; //index_of_impl


   template<>
   struct index_of_helper<false>
   {
      template<typename NewHead, typename NewTail, typename WhichType>
      struct action
      {
         typedef index_of_impl<NewTail>::template inner<NewHead, WhichType> t_temp;
         BOOST_STATIC_CONSTANT( int, temp = t_temp::value);   

         BOOST_STATIC_CONSTANT( int, value = temp >= 0 ? 1+temp : -1 );
      }; //inner

   }; //index_of_helper


   template<typename Sequence, typename WhichType>
   struct type_to_tid
   {
      typedef typename Sequence::t_head t_head;
      typedef typename Sequence::t_tail t_tail;

      typedef index_of_impl<t_tail>::template inner<t_head, WhichType> t_temp;
      BOOST_STATIC_CONSTANT(int, value = t_temp::value);

   }; //type_to_tid


   ///////////////////////////////////////////////////////////////////////


   template<int Count>
   struct copy_first_n_impl
   {
      template<typename Sequence>
      struct inner
      {
         typedef typename Sequence::t_head t_head;
         typedef typename Sequence::t_tail t_tail;

         typedef typename copy_first_n_impl<Count-1>::template inner<t_tail>::type t_temp;

         typedef rdsl<t_head, t_temp> type;
      }; //inner

   }; //copy_first_n_impl


   template<>
   struct copy_first_n_impl<1>
   {
      template<typename Sequence>
      struct inner
      {
         typedef rdsl<typename Sequence::t_head, eol_tag> type;
      }; //inner

   }; //copy_first_n_impl<1>


   template<typename Sequence, int Count>
   struct copy_first_n
   {
      typedef copy_first_n_impl<Count>::template inner<Sequence>::type type;
   }; //copy_first_n


   ///////////////////////////////////////////////////////////////////////


   template<typename Tail>
   struct max_size_impl
   {
      template<typename Head>
      struct inner
      {
         typedef typename Tail::t_head t_head;
         typedef typename Tail::t_tail t_tail;

         typedef typename max_size_impl<t_tail>::template inner<t_head> t_temp;
         BOOST_STATIC_CONSTANT(int, n1 = t_temp::value);
         BOOST_STATIC_CONSTANT(int, n2 = sizeof(Head));
      
         BOOST_STATIC_CONSTANT(int, value = n1 > n2 ? n1 : n2);
      }; //inner

   }; //max_size_impl


   template<>
   struct max_size_impl<eol_tag>
   {
      template<typename Head>
      struct inner
      {
         BOOST_STATIC_CONSTANT(int, value = sizeof(Head));
      };

   }; //max_size_impl


   template<typename Sequence>
   struct max_size
   {
      typedef typename Sequence::t_head t_head;
      typedef typename Sequence::t_tail t_tail;

      typedef max_size_impl<t_tail>::template inner<t_head> t_temp;
      BOOST_STATIC_CONSTANT(int, value = t_temp::value);

   }; //max_size


   ///////////////////////////////////////////////////////////////////////


   template<typename Tail>
   struct needed_alignment_impl
   {
      template<typename Head>
      struct inner
      {
         typedef typename Tail::t_head t_head;
         typedef typename Tail::t_tail t_tail;

         BOOST_STATIC_CONSTANT(int, n1 = boost::alignment_of<Head>::value);

         typedef typename needed_alignment_impl<t_tail>::template inner<t_head> t_temp;
         BOOST_STATIC_CONSTANT(int, n2 = t_temp::value);
      
         typedef boost::math::static_lcm<n1, n2> t_temp_2;
         BOOST_STATIC_CONSTANT(int, value = t_temp_2::value);
      }; //inner

   }; //needed_alignment_impl


   template<>
   struct needed_alignment_impl<eol_tag>
   {
      template<typename Head>
      struct inner
      {
         BOOST_STATIC_CONSTANT(int, value = 1);
      };

   }; //max_size_impl


   template<typename Sequence>
   struct needed_alignment
   {
      typedef typename Sequence::t_head t_head;
      typedef typename Sequence::t_tail t_tail;

      typedef max_size_impl<t_tail>::template inner<t_head> t_temp;
      BOOST_STATIC_CONSTANT(int, value = t_temp::value);

   }; //needed_alignment


   ///////////////////////////////////////////////////////////////////////


#else  //BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION


   ///////////////////////////////////////////////////////////////////////


   template<typename Sequence>
   struct get_size;


   template<typename Head, typename Tail>
   struct get_size<rdsl<Head, Tail> >
   {
      BOOST_STATIC_CONSTANT(int, value = 1 + get_size<Tail>::value);

   }; //length

   template<typename Head>
   struct get_size<rdsl<Head, eol_tag> >
   {
      BOOST_STATIC_CONSTANT(int, value = 1);

   }; //length


   ///////////////////////////////////////////////////////////////////////


   template<typename Sequence, typename WhichType>
   struct type_to_tid;

   template<typename Head, typename Tail, typename WhichType>
   struct type_to_tid<rdsl<Head, Tail>, WhichType>
   {
      typedef is_same<Head, WhichType> t_temp_1;
      BOOST_STATIC_CONSTANT(bool, identical = t_temp_1::value);

      typedef type_to_tid<Tail, WhichType> t_temp_2;
      BOOST_STATIC_CONSTANT( int, temp = 
         t_temp_2::value >= 0 ? 1+t_temp_2::value : -1);


      BOOST_STATIC_CONSTANT( int, value = identical ? 0 : temp );

   }; //index_of


   template<typename WhichType>
   struct type_to_tid<eol_tag, WhichType>
   {
      BOOST_STATIC_CONSTANT(int, value = -1);

   }; //type_to_tid


   ///////////////////////////////////////////////////////////////////////


   template<typename Sequence, typename WhichType>
   struct first_derived;

   template<typename Head, typename Tail, typename WhichType>
   struct first_derived<rdsl<Head, Tail>, WhichType>
   {
      typedef is_base_and_derived<WhichType, Head> t_temp_1;
      BOOST_STATIC_CONSTANT(bool, is_here = t_temp_1::value);

      typedef first_derived<Tail, WhichType> t_temp_2;
      BOOST_STATIC_CONSTANT( int, temp = 
         t_temp_2::value >= 0 ? 1+t_temp_2::value : -1);


      BOOST_STATIC_CONSTANT( int, value = (is_here ? 0 : temp) );

   }; //first_derived


   template<typename WhichType>
   struct first_derived<eol_tag, WhichType>
   {
      BOOST_STATIC_CONSTANT(int, value = -1);

   }; //first_derived


   ///////////////////////////////////////////////////////////////////////


   template<typename Sequence>
   struct max_size;


   template<typename Head, typename Tail>
   struct max_size<rdsl<Head, Tail> >
   {

      typedef max_size<Tail> t_temp;
      BOOST_STATIC_CONSTANT(int, n1 = t_temp::value);
      BOOST_STATIC_CONSTANT(int, n2 = sizeof(Head));
   
      BOOST_STATIC_CONSTANT(int, value = n1 > n2 ? n1 : n2);

   }; //max_size


   template<>
   struct max_size<eol_tag>
   {
      BOOST_STATIC_CONSTANT(int, value = 0);

   }; //max_size


   ///////////////////////////////////////////////////////////////////////


   template<typename Sequence>
   struct needed_alignment;


   template<typename Head, typename Tail>
   struct needed_alignment<rdsl<Head, Tail> >
   {

      BOOST_STATIC_CONSTANT(int, n1 = boost::alignment_of<Head>::value);

      typedef needed_alignment<Tail> t_temp;
      BOOST_STATIC_CONSTANT(int, n2 = t_temp::value);
   
      typedef boost::math::static_lcm<n1, n2> t_temp_2;
      BOOST_STATIC_CONSTANT(int, value = t_temp_2::value);

   }; //needed_alignment


   template<>
   struct needed_alignment<eol_tag>
   {
      BOOST_STATIC_CONSTANT(int, value = 1);

   }; //needed_alignment

   ///////////////////////////////////////////////////////////////////////


   template<int Count, typename Sequence>
   struct copy_first_n_impl
   {
      typedef typename Sequence::t_head t_head; 
      typedef typename Sequence::t_tail t_tail;
   
      typedef typename copy_first_n_impl<Count-1, t_tail>::type t_temp;

      typedef rdsl<t_head, t_temp> type;

   }; //copy_first_n_impl


   template<typename Sequence>
   struct copy_first_n_impl<1, Sequence>
   {
      typedef rdsl<typename Sequence::t_head, eol_tag> type;

   }; //copy_first_n_impl<1>



   template<typename Sequence, int Count>
   struct copy_first_n
   {
      typedef typename copy_first_n_impl<Count,Sequence>::type type;
   }; //copy_first_n


   ///////////////////////////////////////////////////////////////////////


#endif //BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION


   ///////////////////////////////////////////////////////////////////////


} //namespace variant
} //namespace detail

} //namespace boost



#endif //_RDSL_HPP_INC_


