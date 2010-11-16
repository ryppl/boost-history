/*
 *
 * Copyright (c) 2010
 * John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#ifndef BOOST_REGEX_ALLOCATOR_SUPPORT
#define BOOST_REGEX_ALLOCATOR_SUPPORT

#include <memory>
#include <new>

namespace boost{ namespace re_detail{

template <class Allocator>
struct allocator_deleter : public Allocator
{
   allocator_deleter(const Allocator& a) : Allocator(a){}

   template <class T>
   void operator()(T* p)
   {
      this->destroy(p);
      this->deallocate(p, 1);
   }
};

template <class T, class Allocator>
shared_ptr<T> create_shared_ptr(const Allocator& a1)
{
   typedef typename Allocator::template rebind<T> bound_type;
   typedef typename bound_type::other             alloc_type;

   alloc_type a(a1);
   T* p = a.allocate(1);
   try{
      new (p) T();
   }
   catch(...)
   {
      a.destroy(p);
      throw;
   }
   return shared_ptr<T>(p, allocator_deleter<alloc_type>(a), a1);
}

template <class T, class Arg1, class Allocator>
shared_ptr<T> create_shared_ptr(const Arg1& arg1, const Allocator& a1)
{
   typedef typename Allocator::template rebind<T> bound_type;
   typedef typename bound_type::other             alloc_type;

   alloc_type a(a1);
   T* p = a.allocate(1);
   try{
      new (p) T(arg1);
   }
   catch(...)
   {
      a.destroy(p);
      throw;
   }
   return shared_ptr<T>(p, allocator_deleter<alloc_type>(a), a1);
}

template <class T, class Arg1, class Arg2, class Allocator>
shared_ptr<T> create_shared_ptr(const Arg1& arg1, const Arg2 arg2, const Allocator& a1)
{
   typedef typename Allocator::template rebind<T> bound_type;
   typedef typename bound_type::other             alloc_type;

   alloc_type a(a1);
   T* p = a.allocate(1);
   try{
      new (p) T(arg1, arg2);
   }
   catch(...)
   {
      a.destroy(p);
      throw;
   }
   return shared_ptr<T>(p, allocator_deleter<alloc_type>(a), a1);
}

}} // namespaces

#endif // BOOST_REGEX_ALLOCATOR_SUPPORT
