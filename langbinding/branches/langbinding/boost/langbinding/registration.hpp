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

#include <boost/langbinding/registry.hpp>

#ifndef BOOST_LANGBINDING_REGISTRATION_IMPLEMENTATION
#define BOOST_LANGBINDING_REGISTRATION_IMPLEMENTATION

namespace boost { namespace langbinding { namespace detail {

   template<class T>
   struct registration
   {
      registration()
         : lvalue_converters(0)
         , rvalue_converters(0)
      {}

      typename registry<T>::type_info_ type;
      lvalue_chain<T>* lvalue_converters;
      rvalue_chain<T>* rvalue_converters;
   };

   template<class T>
   struct lvalue_chain
   {
      typename registry<T>::lvalue_from_function convert;
      lvalue_chain* next;
   };

   template<class T>
   struct rvalue_chain
   {
      typename registry<T>::rvalue_from_stage1 convertible;
      typename registry<T>::rvalue_from_stage2 convert;
      rvalue_chain* next;
   };

}}}

#endif

