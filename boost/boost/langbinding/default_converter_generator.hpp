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

#ifndef BOOST_LANGBINDING_DEFAULT_CONVERTER_GENERATOR
#define BOOST_LANGBINDING_DEFAULT_CONVERTER_GENERATOR

namespace boost { namespace langbinding {

   namespace detail {

      template<class>
      struct select_default_converter;

      template<>
      struct select_default_converter<xxx_to_cpp>
      {
         template<class T>
         struct apply
         {
         };
      };

      template<>
      struct select_default_converter<cpp_to_xxx>
      {
         template<class T>
         struct apply
         {
         };
      };

   }
   
   struct default_converter_generator
   {
      template<class T, class U>
      struct generate_converter
         : detail::select_default_converter<U>::template apply<T>
      {};
   };

}}

#endif

