
#ifndef _SEQADAPTOR_HPP_INC_
#define _SEQADAPTOR_HPP_INC_


//////////////////////////////////////////////////////////////////////


#include <boost/math/common_factor.hpp>
#include <boost/type_traits.hpp>
#include <boost/config.hpp>
#include <boost/variant_tag/detail.hpp>


//////////////////////////////////////////////////////////////////////


#ifndef BOOST_VARIANT_USE_MPL_SEQ
#ifndef BOOST_VARIANT_USE_RDSL_SEQ

#if BOOST_MSVC > 1200  //VC7
// #define BOOST_VARIANT_USE_RDSL_SEQ
#define BOOST_VARIANT_USE_MPL_SEQ

#else
#define BOOST_VARIANT_USE_RDSL_SEQ

#endif // BOOST_MSVC > 1200 

#endif // !BOOST_VARIANT_USE_RDSL_SEQ
#endif // !BOOST_VARIANT_USE_MPL_SEQ


//////////////////////////////////////////////////////////////////////


//
//Set upper limit of number on length of sequence
//
#ifdef BOOST_VARIANT_USE_MPL_SEQ
#define BOOST_VARIANT_LIMIT_TYPES 20

#else
#define BOOST_VARIANT_LIMIT_TYPES 20

#endif //BOOST_VARIANT_USE_MPL_SEQ


//
//Now, define a macro which instantiates 
//a sequence of BOOST_VARIANT_LIMIT_TYPES types
//
#ifdef BOOST_VARIANT_USE_MPL_SEQ

//#define BOOST_MPL_LIMIT_LIST_SIZE 20
#define BOOST_VARIANT_IMPL_TL mpl::list

#elif defined(BOOST_VARIANT_USE_RDSL_SEQ)
#define BOOST_VARIANT_IMPL_TL BOOST_VARIANT_TYPE_SEQ_20

#endif //BOOST_VARIANT_USE_MPL_SEQ


//////////////////////////////////////////////////////////////////////


#define BOOST_VARIANT_TL_NS boost::detail::variant


//////////////////////////////////////////////////////////////////////

#ifdef BOOST_VARIANT_USE_MPL_SEQ

#include <boost/mpl/list.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/count.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/iter_fold.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/fold_backward.hpp>
#include <boost/mpl/pop_front.hpp>


#include <boost/mpl/distance.hpp>


#define BOOST_VARIANT_TYPE_SEQ_1(a1)\
   boost::mpl::list<a1>

#define BOOST_VARIANT_TYPE_SEQ_2(a1, a2)\
   boost::mpl::list<a1, a2>

#define BOOST_VARIANT_TYPE_SEQ_3(a1, a2, a3)\
   boost::mpl::list<a1, a2, a3>

#define BOOST_VARIANT_TYPE_SEQ_4(a1, a2, a3, a4)\
   boost::mpl::list<a1, a2, a3, a4>

#define BOOST_VARIANT_TYPE_SEQ_5(a1, a2, a3, a4, a5)\
   boost::mpl::list<a1, a2, a3, a4, a5>

#define BOOST_VARIANT_TYPE_SEQ_6(a1, a2, a3, a4, a5, a6)\
   boost::mpl::list<a1, a2, a3, a4, a5, a6>

#define BOOST_VARIANT_TYPE_SEQ_7(a1, a2, a3, a4, a5, a6, a7)\
   boost::mpl::list<a1, a2, a3, a4, a5, a6, a7>

#define BOOST_VARIANT_TYPE_SEQ_8(a1, a2, a3, a4, a5, a6, a7, a8)\
   boost::mpl::list<a1, a2, a3, a4, a5, a6, a7, a8>

#define BOOST_VARIANT_TYPE_SEQ_9(a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   boost::mpl::list<a1, a2, a3, a4, a5, a6, a7, a8, a9>

#define BOOST_VARIANT_TYPE_SEQ_10(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20)               \
   boost::mpl::list<a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20>

#define BOOST_VARIANT_TYPE_SEQ_11(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11)                                                            \
   boost::mpl::list<a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11>

#define BOOST_VARIANT_TYPE_SEQ_12(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11, a12)                                                       \
   boost::mpl::list<a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11, a12>

#define BOOST_VARIANT_TYPE_SEQ_13(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11, a12, a13)                                                  \
   boost::mpl::list<a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11, a12, a13>

#define BOOST_VARIANT_TYPE_SEQ_14(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11, a12, a13, a14)                                             \
   boost::mpl::list<a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11, a12, a13, a14>

#define BOOST_VARIANT_TYPE_SEQ_15(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11, a12, a13, a14, a15)                                        \
   boost::mpl::list<a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11, a12, a13, a14, a15>

#define BOOST_VARIANT_TYPE_SEQ_16(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11, a12, a13, a14, a15, a16)                                   \
   boost::mpl::list<a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11, a12, a13, a14, a15, a16>

#define BOOST_VARIANT_TYPE_SEQ_17(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11, a12, a13, a14, a15, a16, a17)                              \
   boost::mpl::list<a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11, a12, a13, a14, a15, a16, a17>

#define BOOST_VARIANT_TYPE_SEQ_18(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11, a12, a13, a14, a15, a16, a17, a18)                         \
   boost::mpl::list<a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11, a12, a13, a14, a15, a16, a17, a18>

#define BOOST_VARIANT_TYPE_SEQ_19(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11, a12, a13, a14, a15, a16, a17, a18, a19)                    \
   boost::mpl::list<a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11, a12, a13, a14, a15, a16, a17, a18, a19>


#define BOOST_VARIANT_TYPE_SEQ_20(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20)               \
   boost::mpl::list<a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20>


#endif //BOOST_VARIANT_USE_MPL_SEQ


//////////////////////////////////////////////////////////////////////



#ifdef BOOST_VARIANT_USE_RDSL_SEQ

#include <boost/variant_tag/rdsl_tag.hpp>

#define BOOST_VARIANT_TYPE_SEQ_1(a1)                                \
   BOOST_RDSL_1(a1)

#define BOOST_VARIANT_TYPE_SEQ_2(a1, a2)                            \
   BOOST_RDSL_2(a1, a2)

#define BOOST_VARIANT_TYPE_SEQ_3(a1, a2, a3)                        \
   BOOST_RDSL_3(a1, a2, a3)

#define BOOST_VARIANT_TYPE_SEQ_4(a1, a2, a3, a4)                    \
   BOOST_RDSL_4(a1, a2, a3, a4)

#define BOOST_VARIANT_TYPE_SEQ_5(a1, a2, a3, a4, a5)                \
   BOOST_RDSL_5(a1, a2, a3, a4, a5)

#define BOOST_VARIANT_TYPE_SEQ_6(a1, a2, a3, a4, a5, a6)            \
   BOOST_RDSL_6(a1, a2, a3, a4, a5, a6)

#define BOOST_VARIANT_TYPE_SEQ_7(a1, a2, a3, a4, a5, a6, a7)        \
   BOOST_RDSL_7(a1, a2, a3, a4, a5, a6, a7)

#define BOOST_VARIANT_TYPE_SEQ_8(a1, a2, a3, a4, a5, a6, a7, a8)    \
   BOOST_RDSL_8(a1, a2, a3, a4, a5, a6, a7, a8)

#define BOOST_VARIANT_TYPE_SEQ_9(a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   BOOST_RDSL_9(a1, a2, a3, a4, a5, a6, a7, a8, a9)

#define BOOST_VARIANT_TYPE_SEQ_10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   BOOST_RDSL_10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)

#define BOOST_VARIANT_TYPE_SEQ_11(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11)                                                            \
   BOOST_RDSL_11(a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11)

#define BOOST_VARIANT_TYPE_SEQ_12(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11, a12)                                                       \
   BOOST_RDSL_12(a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11, a12)

#define BOOST_VARIANT_TYPE_SEQ_13(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11, a12, a13)                                                  \
   BOOST_RDSL_13(a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11, a12, a13)

#define BOOST_VARIANT_TYPE_SEQ_14(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11, a12, a13, a14)                                             \
   BOOST_RDSL_14(a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11, a12, a13, a14)

#define BOOST_VARIANT_TYPE_SEQ_15(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11, a12, a13, a14, a15)                                        \
   BOOST_RDSL_15(a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11, a12, a13, a14, a15)

#define BOOST_VARIANT_TYPE_SEQ_16(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11, a12, a13, a14, a15, a16)                                   \
   BOOST_RDSL_16(a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11, a12, a13, a14, a15, a16)

#define BOOST_VARIANT_TYPE_SEQ_17(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11, a12, a13, a14, a15, a16, a17)                              \
   BOOST_RDSL_17(a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11, a12, a13, a14, a15, a16, a17)

#define BOOST_VARIANT_TYPE_SEQ_18(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11, a12, a13, a14, a15, a16, a17, a18)                         \
   BOOST_RDSL_18(a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11, a12, a13, a14, a15, a16, a17, a18)

#define BOOST_VARIANT_TYPE_SEQ_19(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11, a12, a13, a14, a15, a16, a17, a18, a19)                    \
   BOOST_RDSL_19(a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11, a12, a13, a14, a15, a16, a17, a18, a19)


#define BOOST_VARIANT_TYPE_SEQ_20(a1, a2, a3, a4, a5, a6, a7, a8, a9,   \
   a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20)               \
   BOOST_RDSL_20(a1, a2, a3, a4, a5, a6, a7, a8, a9,                 \
      a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20)

#endif  //BOOST_VARIANT_USE_RDSL_SEQ


//////////////////////////////////////////////////////////////////////


namespace boost { namespace detail {

namespace variant {


#ifdef BOOST_VARIANT_USE_MPL_SEQ





#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

   template<bool MoreThanOne>
   struct type_to_tid_impl;

   struct type_to_tid_impl<false>
   {
      template<typename Sequence, typename WhichType>
      struct inner
      {
         BOOST_STATIC_CONSTANT(int, value = -1);
      }; //inner

   }; //type_to_tid_impl


   template<bool MoreThanOne>
   struct type_to_tid_impl
   {
      template<typename Sequence, typename WhichType>
      struct inner
      {
         typedef typename mpl::find<Sequence, WhichType>::type t_iter;
         typedef typename mpl::begin<Sequence>::type t_begin;

         BOOST_STATIC_CONSTANT( int, value =
            (mpl::distance<t_begin, t_iter>::type::value) );
      }; //inner

   }; //type_to_tid_impl


   template<typename Sequence, typename WhichType>
   struct type_to_tid
   {
      BOOST_STATIC_CONSTANT( int, how_many =
         (boost::mpl::count<Sequence, WhichType>::type::value) );


      typedef type_to_tid_impl<how_many >= 1 >::inner<Sequence, WhichType> t_temp;

      BOOST_STATIC_CONSTANT(int, value = t_temp::value);

   }; //type_to_tid

#else
   template<bool MoreThanOne, typename Sequence, typename WhichType>
   struct type_to_tid_impl;


   template<typename Sequence, typename WhichType>
   struct type_to_tid_impl<false, Sequence, WhichType>
   {
      BOOST_STATIC_CONSTANT(int, value = -1);

   }; //type_to_tid_impl


   template<bool MoreThanOne, typename Sequence, typename WhichType>
   struct type_to_tid_impl
   {
      typedef typename mpl::find<Sequence, WhichType>::type t_iter;
      typedef typename mpl::begin<Sequence>::type t_begin;

      BOOST_STATIC_CONSTANT( int, value =
         (mpl::distance<t_begin, t_iter>::value) );

   }; //type_to_tid_impl


   template<typename Sequence, typename WhichType>
   struct type_to_tid
   {
      BOOST_STATIC_CONSTANT( int, how_many =
         (boost::mpl::count<Sequence, WhichType>::value) );


      typedef type_to_tid_impl<how_many >= 0, Sequence, WhichType> t_temp;

      BOOST_STATIC_CONSTANT(int, value = t_temp::value);

   }; //type_to_tid


#endif //BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION


   template<typename Sequence>
   struct get_head
   {
	   typedef typename boost::mpl::begin<Sequence>::type iter;
	   typedef typename iter::type type;
   };  //get_head


   template<typename Sequence>
   struct get_rest
   {
      typedef typename boost::mpl::pop_front<Sequence>::type type;
   }; //get_rest


   template<typename Sequence>
   struct get_size
   {
      BOOST_STATIC_CONSTANT(int, value = boost::mpl::size<Sequence>::value);
   }; //get_size

   struct max_size_impl
   {
      template<typename Prev, typename CurrIter>
      struct apply
      {
         typedef typename CurrIter::type t_curr;
         BOOST_STATIC_CONSTANT(int, prev = Prev::value);
         BOOST_STATIC_CONSTANT(int, curr = sizeof(t_curr));

         BOOST_STATIC_CONSTANT(int, value = prev >  curr ? prev : curr);

         typedef int_to_type<value> type;
      };
   }; //max_size_impl


   template<typename Sequence>
   struct max_size
   {
      typedef typename boost::mpl::iter_fold
      <
         Sequence,
         int_to_type<0>,
         max_size_impl
      >::type type;

      BOOST_STATIC_CONSTANT(int, value = type::value);

   }; //max_size


   struct needed_alignment_impl
   {
      template<typename Prev, typename CurrIter>
      struct apply
      {
         typedef typename CurrIter::type t_curr;
         BOOST_STATIC_CONSTANT(int, n_curr = boost::alignment_of<t_curr>::value);
         BOOST_STATIC_CONSTANT(int, n_prev = Prev::value);

         BOOST_STATIC_CONSTANT( int, value = (math::static_lcm<n_curr, n_prev>::value));

         typedef int_to_type<value> type;
      };
   }; //needed_alignment_impl


   template<typename Sequence>
   struct needed_alignment
   {
      typedef typename boost::mpl::iter_fold
      <
         Sequence,
         int_to_type<1>,
         needed_alignment_impl
      >::type type;

      BOOST_STATIC_CONSTANT(int, value = type::value);
   }; //needed_alignment



   template<typename WhichType>
   struct erase_all_derived_impl
   {
      template<typename Prev, typename Curr>
      struct apply
      {
         typedef typename Curr t_curr;

         typedef ct_if
         <
            is_base_and_derived<WhichType, t_curr>::value,
            Prev, 
            typename boost::mpl::push_front<Prev, t_curr>::type 
         >::type type;

      };
   }; //erase_all_derived_impl


   struct binary_nothing
   {
      template<typename Prev, typename CurrIter>
      struct apply
      {
         typedef Prev type;
      };
   }; //binary_nothing


   template<typename Sequence, typename WhichType>
   struct erase_all_derived
   {
      typedef typename boost::mpl::fold_backward
      <
         Sequence, 
         boost::mpl::list<>,
//         binary_nothing,
         erase_all_derived_impl<WhichType> 
      >::type type;
   };


   template<typename SrcType>
   struct constructible_from_impl
   {

      template<typename Prev, typename CurrIter>
      struct apply
      {
         typedef Prev t_last;
         typedef typename CurrIter::type t_curr;

         typedef typename
            choose_conversion<SrcType, t_curr, t_last>::type type;

      }; //apply

   }; //constructible_from_impl


   template<typename Sequence, typename SrcType>
   struct constructible_from
   {

      typedef typename mpl::iter_fold
      <
         Sequence,
         neutral_tag,
         constructible_from_impl<SrcType>
      >::type type;


   }; //constructible_from


   template<typename Sequence>
   struct is_empty
   {
      BOOST_STATIC_CONSTANT(int, value = mpl::empty<Sequence>::value);
   }; //is_empty


   //////////////////////////////////////////////////////////////////////


#endif //BOOST_VARIANT_USE_MPL_SEQ


}  //namespace variant
}  //namespace detail
}  //namespace boost





//////////////////////////////////////////////////////////////////////


#define BOOST_VARIANT_PARAMS_DEF             \
      BOOST_PP_ENUM_PARAMS_WITH_DEFAULTS(    \
      BOOST_VARIANT_LIMIT_TYPES,             \
      typename AT,                           \
      detail::variant::ignore_type_)

#define BOOST_VARIANT_PARAMS                 \
      BOOST_PP_ENUM_PARAMS(                  \
      BOOST_VARIANT_LIMIT_TYPES,             \
      typename AT)

#define BOOST_VARIANT_PARAMS_USAGE           \
      BOOST_PP_ENUM_PARAMS(                  \
      BOOST_VARIANT_LIMIT_TYPES,             \
      AT)


#endif //_SEQADAPTOR_HPP_INC_

