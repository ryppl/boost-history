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

#ifndef BOOST_LANGBINDING_CENTRAL_REGISTRY_IMPLEMENTATION
#define BOOST_LANGBINDING_CENTRAL_REGISTRY_IMPLEMENTATION

#include <vector>
#include <map>
#include <string>
#include <utility>
#include <iostream>
#include <set>

#include <boost/shared_ptr.hpp>

#include <boost/langbinding/central_registry.hpp>
#include <boost/langbinding/registry.ipp>

namespace boost { namespace langbinding {

   namespace
   {
      template<class T>
      struct implementation
      {
         typedef typename central_registry<T>::registry_ptr registry_ptr;

         struct entry
         {
            registry_ptr registry_;
         };

         typedef boost::shared_ptr<entry> entry_ptr;

         struct cmp_entry
         {
            bool operator()(
                 const entry_ptr& lhs
               , const entry_ptr& rhs) const
            {
               return lhs->registry_ < rhs->registry_;
            }
         };

         typedef std::vector<entry_ptr> registry_t;
         typedef std::map<std::string, entry_ptr> name_to_entry_t;

         static registry_t& entries()
         {
            static registry_t x;
            return x;
         }

         static name_to_entry_t& name_to_entry()
         {
            static name_to_entry_t x;
            return x;
         }
      };

      struct null_deleter
      {
         void operator()(const void*) const {};
      };
   }

   template<class T>
   BOOST_LANGBINDING_DECL
   void central_registry<T>::register_module(const char* name, registry_ptr r)
   {
      std::cout << "central registry: registering module \""
                << name << "\"..\n";

      typename implementation<T>::entry tmp;
      tmp.registry_ = r;
      typename implementation<T>::entry_ptr ptr(&tmp, null_deleter());

      typedef typename implementation<T>::registry_t::iterator iter_type;

      iter_type iter = std::lower_bound(
           implementation<T>::entries().begin()
         , implementation<T>::entries().end()
         , ptr
         , typename implementation<T>::cmp_entry()
      );

      if (iter == implementation<T>::entries().end() ||
          (*iter)->registry_ != ptr->registry_)
      {
         ptr = typename implementation<T>::entry_ptr(
            new typename implementation<T>::entry);
         ptr->registry_ = r;

         implementation<T>::entries().insert(
            iter, ptr);
      }
      else
         ptr = *iter;

      std::string id(name);
      implementation<T>::name_to_entry()[id] = ptr;
   }

   template<class T>
   BOOST_LANGBINDING_DECL
   void central_registry<T>::insert_converter(
           registry_ptr r
         , const typename registry<T>::type_info_& type
         , typename registry<T>::lvalue_from_function convert
      )
   {
      typename implementation<T>::entry tmp;
      tmp.registry_ = r;
      typename implementation<T>::entry_ptr ptr(&tmp, null_deleter());

      typedef typename implementation<T>::registry_t::iterator iter_type;

      iter_type iter = std::lower_bound(
           implementation<T>::entries().begin()
         , implementation<T>::entries().end()
         , ptr
         , typename implementation<T>::cmp_entry()
      );

      if (iter == implementation<T>::entries().end() ||
          (*iter)->registry_ != ptr->registry_)
      {
         ptr = typename implementation<T>::entry_ptr(
            new typename implementation<T>::entry);
         ptr->registry_ = r;

         implementation<T>::entries().insert(
            iter, ptr);
      }
      else
         ptr = *iter;

      // .. add converter
   }

   template<class T>
   BOOST_LANGBINDING_DECL
   void central_registry<T>::insert_converter(
           registry_ptr r
         , const typename registry<T>::type_info_& type
         , typename registry<T>::rvalue_from_stage1 convertible
         , typename registry<T>::rvalue_from_stage2 convert
      )
   {
      typename implementation<T>::entry tmp;
      tmp.registry_ = r;
      typename implementation<T>::entry_ptr ptr(&tmp, null_deleter());

      typedef typename implementation<T>::registry_t::iterator iter_type;

      iter_type iter = std::lower_bound(
           implementation<T>::entries().begin()
         , implementation<T>::entries().end()
         , ptr
         , typename implementation<T>::cmp_entry()
      );

      if (iter == implementation<T>::entries().end() ||
          (*iter)->registry_ != ptr->registry_)
      {
         ptr = typename implementation<T>::entry_ptr(
            new typename implementation<T>::entry);
         ptr->registry_ = r;

         implementation<T>::entries().insert(
            iter, ptr);
      }
      else
         ptr = *iter;

      // .. add converter
   }

   template<class T>
   BOOST_LANGBINDING_DECL
   void central_registry<T>::import(
           registry_ptr r
         , const char* module)
   {
      typename implementation<T>::name_to_entry_t::const_iterator iter
         = implementation<T>::name_to_entry().find(module);

      assert(iter != implementation<T>::name_to_entry().end());
      typename implementation<T>::entry_ptr e = iter->second;

      e->registry_->export_converters(*r);
   }

   template<class T>
   BOOST_LANGBINDING_DECL
   void central_registry<T>::import(
           registry_ptr r
         , const char* module
         , const typename registry<T>::type_info_& type)
   {
      typename implementation<T>::name_to_entry_t::const_iterator iter
         = implementation<T>::name_to_entry().find(module);

      assert(iter != implementation<T>::name_to_entry().end());
      typename implementation<T>::entry_ptr e = iter->second;

      e->registry_->export_converters(type, *r);
   }

}}

#endif

