// Copyright (c) 2003 Daniel Wallin

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef BOOST_LANGBINDING_POLICY
#define BOOST_LANGBINDING_POLICY

#include <boost/mpl/int.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/bind.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>

namespace boost { namespace langbinding {

   struct cpp_to_xxx {};
   struct xxx_to_cpp {};

   struct converter_generator_tag_base {};
   
   template<int N>
   struct converter_generator_tag 
      : converter_generator_tag_base
   {
      typedef mpl::int_<N> index;
   };

   template<class>
   struct cons_iterator;
   
   struct nil_type
   {
      typedef cons_iterator<nil_type> begin;
      typedef cons_iterator<nil_type> end;
   };

   template<class Cons>
   struct cons_iterator
   {
      typedef typename Cons::head_type type;
      typedef cons_iterator<typename Cons::tail_type> next;
   };

   template<>
   struct cons_iterator<nil_type>
   {
      typedef nil_type type;
      typedef nil_type next;
   };
   
   template<class H, class T = nil_type>
   struct cons
   {
      typedef cons<H, T> self_type;
      
      typedef cons_iterator<self_type> begin;
      typedef cons_iterator<nil_type> end;

      typedef H head_type;
      typedef T tail_type;

      template<class H2>
      cons<H2, self_type> operator,(const cons<H2, nil_type>& rhs) const
      {
         return cons<H2, self_type>();
      }

      H head;
      T tail;
   };

   struct default_converter_generator;

   template<class T, int N>
   struct find_conversion_policy
   {
      typedef typename mpl::find_if<
            T
          , is_base_and_derived<
               converter_generator_tag<N>
             , mpl::_
             >
       >::type iter;

       typedef typename mpl::if_<
          is_same<cons_iterator<nil_type>, iter>
           , default_converter_generator
           , typename iter::type
       >::type type;
   };
}}

#endif

