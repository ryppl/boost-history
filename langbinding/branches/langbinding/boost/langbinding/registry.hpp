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

#ifndef BOOST_LANGBINDING_REGISTRY
#define BOOST_LANGBINDING_REGISTRY

#define BOOST_LANGBINDING_DECL

#include <memory>

namespace boost { namespace langbinding {

   struct rvalue_data_base
   {};

   template<class T>
   class registry
   {
   public:
      struct lvalue_chain;
      struct rvalue_chain;
      struct registration;

      typedef typename T::type_info type_info_;
      typedef typename T::argument_type argument_type;

      typedef void*(*lvalue_from_function)(const argument_type&, int&);
      typedef void*(*rvalue_from_stage1)(const argument_type&, int&);
      typedef void(*rvalue_from_stage2)(const argument_type&, rvalue_data_base*);

      BOOST_LANGBINDING_DECL
      const registration& lookup(const type_info_&);

      BOOST_LANGBINDING_DECL
      const registration* query(const type_info_&);

      BOOST_LANGBINDING_DECL
      void insert(const type_info_&, lvalue_from_function);

      BOOST_LANGBINDING_DECL
      void insert(const type_info_& x, 
         rvalue_from_stage1 convertible, rvalue_from_stage2 convert);

      BOOST_LANGBINDING_DECL
      void export_converters(const type_info_&, registry&);

      BOOST_LANGBINDING_DECL
      void export_converters(registry&);

      BOOST_LANGBINDING_DECL
      static registry* instance();

      registry(const registry&);
      registry& operator=(const registry&);

   private:
      registry();

      struct implementation;
      std::auto_ptr<implementation> m_pimpl;
   };

}}

#endif

