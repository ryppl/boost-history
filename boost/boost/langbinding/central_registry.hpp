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

#ifndef BOOST_CENTRAL_LANGBINDING_REGISTRY
#define BOOST_CENTRAL_LANGBINDING_REGISTRY

#include <boost/langbinding/registry.hpp>
#include <boost/langbinding/config.hpp>

namespace boost { namespace langbinding {

   template<class T>
   class BOOST_LANGBINDING_DECL central_registry
   {
   public:
      typedef registry<T>* registry_ptr;
    
      static void register_module(const char* name, registry_ptr r);

      static void insert_converter(
           registry_ptr r
         , const typename registry<T>::type_info_&
         , typename registry<T>::lvalue_from_function
      );

      static void insert_converter(
           registry_ptr r
         , const typename registry<T>::type_info_&
         , typename registry<T>::rvalue_from_stage1
         , typename registry<T>::rvalue_from_stage2
      );

      static void import(
           registry_ptr r
         , const char* module
      );

      static void import(
           registry_ptr r
         , const char* module
         , const typename registry<T>::type_info_& type
      );
   };

}}

#endif

