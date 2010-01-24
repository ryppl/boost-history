/*=============================================================================
   Copyright (c) 2006 Eric Niebler
   Copyright (c) 2010 Christopher Schmidt

   Use, modification and distribution is subject to the Boost Software
   License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
   http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

//TODO: ctor, refactoring in detail files, make_tree in gen etc.

#ifndef BOOST_FUSION_CONTAINER_EXT_TREE_HPP
#define BOOST_FUSION_CONTAINER_EXT_TREE_HPP

#include <boost/fusion/sequence/intrinsic/ext_/segments.hpp>
#include <boost/fusion/container/list/cons.hpp>
#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/container/vector/vector10.hpp>
#else
#   include <boost/fusion/container/vector/vector.hpp>
#endif
#include <boost/fusion/view/single_view.hpp>
#include <boost/fusion/view/ext_/segmented_iterator.hpp>
#include <boost/fusion/support/ext_/is_segmented.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#ifdef BOOST_NO_RVALUE_REFERENCES
#   include <boost/call_traits.hpp>
#endif
#include <boost/type_traits/is_const.hpp>

namespace boost { namespace fusion
{
   struct tree_tag;

   namespace detail
   {
       template<typename T, bool IsConst>
       struct reference : add_reference<T> {};

       template<typename T>
       struct reference<T, true> : reference<typename add_const<T>::type, false> {};

       template<typename T>
       struct reference<T &, true> : reference<T, false> {};
   }

   template<typename Data, typename Left = nil, typename Right = nil>
   struct tree
     : sequence_base<tree<Data, Left, Right> >
   {
       typedef Data data_type;
       typedef Left left_type;
       typedef Right right_type;
       typedef typename
           mpl::if_<traits::is_sequence<Data>, Data, single_view<Data> >::type
       data_view;
#ifdef BOOST_NO_VARIADIC_TEMPLATES
       typedef vector3<Left, data_view, Right> segments_type;
#else
       typedef vector<Left, data_view, Right> segments_type;
#endif

       typedef tree_tag fusion_tag;
       typedef forward_traversal_tag category;
       typedef mpl::false_ is_view;

       tree()
       {}

#ifdef BOOST_NO_RVALUE_REFERENCES
       explicit
       tree(typename call_traits<Data>::type data,
           typename call_traits<Left>::type left = Left(),
           typename call_traits<Right>::type right = Right())
         : segments(left, data, right)
       {}
#else
       template<class OtherData, class OtherLeft=Left, class OtherRight=Right>
       explicit
       tree(OtherData&& data,
           OtherLeft&& left = Left(),
           OtherRight&& right = Right())
         : segments(std::forward<OtherLeft>(left),
                     std::forward<OtherData>(data),
                     std::forward<OtherRight>(right))
       {}
#endif

       segments_type segments;
   };

   template<typename Data>
   tree<typename traits::deduce<Data>::type>
   make_tree(BOOST_FUSION_R_ELSE_CLREF(Data) data)
   {
       return tree<typename traits::deduce<Data>::type>(
               BOOST_FUSION_FORWARD(Data,data));
   }

   template<typename Data, typename Left, typename Right>
   tree<
       typename traits::deduce<Data>::type
     , typename traits::deduce<Left>::type
     , typename traits::deduce<Right>::type
   >
   make_tree(BOOST_FUSION_R_ELSE_CLREF(Data) data,
               BOOST_FUSION_R_ELSE_CLREF(Left) left,
               BOOST_FUSION_R_ELSE_CLREF(Right) right)
   {
       return tree<
           typename traits::deduce<Data>::type
         , typename traits::deduce<Left>::type
         , typename traits::deduce<Right>::type
       >(BOOST_FUSION_FORWARD(Data,data),
               BOOST_FUSION_FORWARD(Left,left),
               BOOST_FUSION_FORWARD(Right,right));
   }

   namespace extension
   {
       template<typename>
       struct empty_impl;

       template<>
       struct empty_impl<tree_tag>
       {
           template<typename SeqRef>
           struct apply
             : mpl::false_
           {};
       };

       template<typename>
       struct is_segmented_impl;

       template<>
       struct is_segmented_impl<tree_tag>
       {
           template<typename SeqRef>
           struct apply
             : mpl::true_
           {};
       };

       template<typename>
       struct segments_impl;

       template<>
       struct segments_impl<tree_tag>
       {
           template<typename SeqRef>
           struct apply
           {
               typedef typename detail::remove_reference<SeqRef>::type seq;

               typedef typename
                   detail::forward_as<SeqRef,typename seq::segments_type>::type
               type;

               static type
               call(SeqRef seq)
               {
                   return seq.segments;
               }
           };
       };

       template<typename>
       struct begin_impl;

       template<>
       struct begin_impl<tree_tag>
       {
           template<typename SeqRef>
           struct apply
             : segmented_begin<SeqRef>
           {};
       };

       template<typename>
       struct end_impl;

       template<>
       struct end_impl<tree_tag>
       {
           template<typename SeqRef>
           struct apply
             : segmented_end<SeqRef>
           {};
       };
   }
}}

#endif
