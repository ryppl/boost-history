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
#include <boost/langbinding/central_registry.hpp>
#include <boost/langbinding/registration.ipp>

#include <memory>
#include <map>
#include <iostream>

namespace boost { namespace langbinding {

   template<class T>
   struct registry<T>::implementation
   {
      typedef std::map<
           typename registry<T>::type_info_
         , typename registry<T>::registration
      > registry_t;

      registry_t entries;
   };

   template<class T>
   registry<T>::registry()
      : m_pimpl(new implementation)
   {}

   template<class T>
   BOOST_LANGBINDING_DECL
   const typename registry<T>::registration& 
      registry<T>::lookup(const type_info_& x)
   {
      std::cout << "lookup for \"" << x << "\"\n";

      return m_pimpl->entries[x];
   }

   template<class T>
   BOOST_LANGBINDING_DECL
   const typename registry<T>::registration* 
      registry<T>::query(const type_info_& x)
   {
      std::cout << "query for \"" << x << "\"\n";

      typename implementation::registry_t::const_iterator iter
         = m_pimpl->entries.find(x);

      return iter == m_pimpl->entries.end() ? 0 : &iter->second;
   }

   template<class T>
   BOOST_LANGBINDING_DECL
   void registry<T>::insert(const type_info_& x, 
      typename registry<T>::lvalue_from_function convert)
   {
      std::cout << "lvalue insert for \"" << x << "\"\n";

      registration& r = const_cast<registration&>(lookup(x));

      typedef typename registry<T>::lvalue_chain chain_t;

      chain_t* chain = new chain_t;
      chain->convert = convert;
      chain->next = r.lvalue_converters;
      r.lvalue_converters = chain;

      central_registry<T>::insert_converter(this, x, convert);
   }

   template<class T>
   BOOST_LANGBINDING_DECL
   void registry<T>::insert(const type_info_& x,
      typename registry<T>::rvalue_from_stage1 convertible,
      typename registry<T>::rvalue_from_stage2 convert)
   {
      std::cout << "rvalue insert for \"" << x << "\"\n";

      registration& r = const_cast<registration&>(lookup(x));

      typedef typename registry<T>::rvalue_chain chain_t;

      chain_t* chain = new chain_t;
      chain->convertible = convertible;
      chain->convert = convert;
      chain->next = r.rvalue_converters;
      r.rvalue_converters = chain;

      central_registry<T>::insert_converter(this, x, convertible, convert);
   }

   template<class T>
   BOOST_LANGBINDING_DECL
   void registry<T>::export_converters(registry& to)
   {
      for (typename implementation::registry_t::const_iterator iter
            = m_pimpl->entries.begin()
            ; iter != m_pimpl->entries.end()
            ; ++iter)
      {
         const type_info_& type = iter->second.type;

         for (lvalue_chain* c = iter->second.lvalue_converters; 
               c != 0; c = c->next)
         {
            to.insert(type, c->convert);
         }

         for (rvalue_chain* c = iter->second.rvalue_converters; 
               c != 0; c = c->next)
         {
            to.insert(type, c->convertible, c->convert);
         }
      }
   }

   template<class T>
   BOOST_LANGBINDING_DECL
   void registry<T>::export_converters(const type_info_& type, registry& to)
   {
      const registration* r = query(type);
      if (!r) return;

      for (lvalue_chain* c = r->lvalue_converters; 
            c != 0; c = c->next)
      {
         to.insert(type, c->convert);
      }

      for (rvalue_chain* c = r->rvalue_converters;
            c != 0; c = c->next)
      {
         to.insert(type, c->convertible, c->convert);
      }
   }

   template<class T>
   BOOST_LANGBINDING_DECL
   registry<T>* registry<T>::instance()
   {
      static std::auto_ptr<registry<T> > p(new registry);
      return p.get();
   }

}}

#endif

