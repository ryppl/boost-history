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

#include <memory>
#include <boost/langbinding/config.hpp>

// language descriptor concept
//
// typedef /* impl */ type_info
// typedef /* impl */ param_type
// typedef /* impl */ args_type
//
// param_type extract_param(args_type, index_type)

namespace boost { namespace langbinding {

   namespace detail {

      template<class T>
      struct registry_impl;

      template<class T>
      struct lvalue_chain;

      template<class T>
      struct rvalue_chain;

      template<class T>
      struct registration;
   }
  
   template<class T> 
   struct rvalue_data_base;

   template<class T>
   class BOOST_LANGBINDING_DECL registry_base
   {
   public:
      typedef typename T::type_info type_info_;
      typedef typename T::param_type param_type;

      typedef void*(*lvalue_from_function)(const param_type&, int&);
      typedef void*(*rvalue_from_stage1)(const param_type&, int&);
      typedef void(*rvalue_from_stage2)(const param_type&, rvalue_data_base<T>*);

      const detail::registration<T>* lookup(const type_info_&);

      const detail::registration<T>* query(const type_info_&);

      void insert(const type_info_&, lvalue_from_function);

      void insert(const type_info_& x, 
         rvalue_from_stage1 convertible, rvalue_from_stage2 convert);

      void export_converters(const type_info_&, registry_base&);

      void export_converters(registry_base&);
 
      registry_base(const registry_base&);
      registry_base& operator=(const registry_base&);

      virtual ~registry_base();
      
   protected:
      registry_base();

   private:
      std::auto_ptr<detail::registry_impl<T> > m_pimpl;
   };

   template<class T>
   class registry : public registry_base<T>
   {
   public:
      static registry* instance();
      ~registry();

   private:
      registry();
   };

}}

#endif

