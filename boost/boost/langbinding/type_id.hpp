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

#ifndef BOOST_LANGBINDING_TYPE_ID_HPP
#define BOOST_LANGBINDING_TYPE_ID_HPP

#include <typeinfo>

namespace boost { namespace langbinding {

   #ifndef BOOST_LANGBINDING_TYPE_INFO
   #  define BOOST_LANGBINDING_TYPE_INFO const std::type_info*
   #endif

   typedef BOOST_LANGBINDING_TYPE_INFO type_info;

   template<class T>
   struct type_id_traits {};

   template<>
   struct type_id_traits<const std::type_info*>
   {
      template<class T>
      static const std::type_info* static_type_id(T*)
      {
         return &typeid(T);
      }

     template<class T>
     static const std::type_info* dynamic_type_id(T* p)
     {
        return &typeid(*p);
     }
   };
   
   template<class T>
   type_info static_type_id(const volatile T* = 0)
   {
      return type_id_traits<type_info>::static_type_id((T*)0);
   }

   template<class T>
   type_info dynamic_type_id(const volatile T* p)
   {
      return type_id_traits<type_info>::dynamic_type_id(p);
   }

}}

#endif

