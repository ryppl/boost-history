// Copyright (c) 2003 Daniel Wallin

// Permission is hereby granted, free of charge, to any person or organization 
// obtaining a copy of the software covered by this license (the "Software") 
// to use, reproduce, display, distribute, execute, and transmit the Software, 
// and to prepare derivative works of the Software, and to permit others
// to do so, all subject to the following:

// The copyright notice in the Software and this entire statement, including 
// the above license grant, this restriction and the following disclaimer, 
// must be included in all copies of the Software, in whole or in part, and 
// all derivative works of the Software, unless such copies or derivative 
// works are solely in the form of machine-executable object code generated by 
// a source language processor.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT 
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE 
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, 
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.

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

