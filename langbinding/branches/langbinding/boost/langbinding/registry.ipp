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

#ifndef BOOST_LANGBINDING_REGISTRY_IMPLEMENTATION
#define BOOST_LANGBINDING_REGISTRY_IMPLEMENTATION

#include <boost/langbinding/registry.hpp>
#include <boost/langbinding/registration.hpp>
#include <boost/langbinding/central_registry.hpp>

#include <boost/langbinding/rvalue_data.hpp>

#include <memory>
#include <map>
#include <iostream>

namespace boost { namespace langbinding {

   namespace detail {
   
      template<class T>
      struct BOOST_LANGBINDING_DECL registry_impl
      {
         typedef std::map<
              typename registry<T>::type_info_
            , registration<T>
         > registry_t;

         registry_t entries;
      };

   }
   
   template<class T>
   registry_base<T>::registry_base()
   {}

   template<class T>
   registry_base<T>::~registry_base() 
   {}
   
   template<class T>
   const detail::registration<T>*
   registry_base<T>::lookup(const type_info_& x)
   {
      std::cout << "lookup for \"" << x << "\"\n";

      const detail::registration<T>& c = m_pimpl->entries[x];
      return &c;
   }

   template<class T>
   const detail::registration<T>* 
      registry_base<T>::query(const type_info_& x)
   {
      std::cout << "query for \"" << x << "\"\n";

      typename detail::registry_impl<T>::registry_t::const_iterator iter
         = m_pimpl->entries.find(x);

      return iter == m_pimpl->entries.end() ? 0 : &iter->second;
   }

   template<class T>
   void registry_base<T>::insert(const type_info_& x, 
      typename registry_base<T>::lvalue_from_function convert)
   {
      std::cout << "lvalue insert for \"" << x << "\"\n";

      detail::registration<T>& r 
         = const_cast<detail::registration<T>&>(*lookup(x));

      typedef detail::lvalue_chain<T> chain_t;

      chain_t* chain = new chain_t;
      chain->convert = convert;
      chain->next = r.lvalue_converters;
      r.lvalue_converters = chain;

      central_registry<T>::insert_converter(this, x, convert);
   }

   template<class T>
   void registry_base<T>::insert(const type_info_& x,
      typename registry_base<T>::rvalue_from_stage1 convertible,
      typename registry_base<T>::rvalue_from_stage2 convert)
   {
      std::cout << "rvalue insert for \"" << x << "\"\n";

      detail::registration<T>& r 
         = const_cast<detail::registration<T>&>(*lookup(x));

      typedef detail::rvalue_chain<T> chain_t;

      chain_t* chain = new chain_t;
      chain->convertible = convertible;
      chain->convert = convert;
      chain->next = r.rvalue_converters;
      r.rvalue_converters = chain;

      central_registry<T>::insert_converter(this, x, convertible, convert);
   }

   template<class T>
   void registry_base<T>::export_converters(registry_base& to)
   {
      for (typename detail::registry_impl<T>::registry_t::const_iterator iter
            = m_pimpl->entries.begin()
            ; iter != m_pimpl->entries.end()
            ; ++iter)
      {
         const type_info_& type = iter->first;

         for (detail::lvalue_chain<T>* c = iter->second.lvalue_converters; 
               c != 0; c = c->next)
         {
            to.insert(type, c->convert);
         }

         for (detail::rvalue_chain<T>* c2 = iter->second.rvalue_converters; 
               c2 != 0; c2 = c2->next)
         {
            to.insert(type, c2->convertible, c2->convert);
         }
      }
   }

   template<class T>
   void registry_base<T>::export_converters(const type_info_& x, registry_base& to)
   {
      const detail::registration<T>* r = query(x);
      if (!r) return;

      for (detail::lvalue_chain<T>* c = r->lvalue_converters; 
            c != 0; c = c->next)
      {
         to.insert(x, c->convert);
      }

      for (detail::rvalue_chain<T>* c2 = r->rvalue_converters;
            c2 != 0; c2 = c2->next)
      {
         to.insert(x, c2->convertible, c2->convert);
      }
   }

}}

#endif

