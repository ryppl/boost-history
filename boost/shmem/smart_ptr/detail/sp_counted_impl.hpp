#ifndef BOOST_SHMEM_DETAIL_SP_COUNTED_IMPL_HPP_INCLUDED
#define BOOST_SHMEM_DETAIL_SP_COUNTED_IMPL_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  This file is the adaptation for shared memory memory mapped
//  files of boost/detail/sp_counted_impl.hpp
//
//  Copyright (c) 2001, 2002, 2003 Peter Dimov and Multi Media Ltd.
//  Copyright 2004-2005 Peter Dimov
//  Copyright 2006      Ion Gaztañaga
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/config.hpp>
#include <boost/shmem/smart_ptr/detail/sp_counted_base.hpp>
#include <boost/shmem/smart_ptr/scoped_ptr.hpp>
#include <boost/shmem/detail/utilities.hpp>

namespace boost {

namespace shmem {

namespace detail {

template<class A, class D>
class sp_counted_impl_pd 
   :  public sp_counted_base
   ,  A::template rebind< sp_counted_impl_pd<A, D> >::other
   ,  D  // copy constructor must not throw
   
{
   private:
   typedef sp_counted_impl_pd<A, D>          this_type;
   typedef typename A::template rebind
      <this_type>::other                     this_allocator;
   typedef typename this_allocator::pointer  this_pointer;

   sp_counted_impl_pd( sp_counted_impl_pd const & );
   sp_counted_impl_pd & operator= ( sp_counted_impl_pd const & );

   typedef typename detail::pointer_to_other
            <typename A::pointer, const D>::type   const_deleter_pointer;

   typedef typename detail::pointer_to_other
            <typename A::pointer, const A>::type   const_allocator_pointer;

   public:

   // pre: d(p) must not throw

   sp_counted_impl_pd(const A &a, const D &d )
      :  this_allocator(a), D(d)
   {}

   const_deleter_pointer get_deleter() const
   {  return const_deleter_pointer(&static_cast<const D&>(*this)); }

   const_allocator_pointer get_allocator() const
   {  return const_allocator_pointer(&static_cast<const A&>(*this)); }

   template <class Ptr>
   void dispose(const Ptr &ptr) // nothrow
      {  static_cast<D&>(*this)(ptr); }

   void destroy() // nothrow
   {
      //Self destruction, so get a copy of the allocator
      //(in the future we could move it)
      this_allocator a_copy(*this);
      BOOST_ASSERT(a_copy == *this);
      this_pointer this_ptr (this);
      //Do it now!
      scoped_ptr<this_type, 
                 scoped_deallocator<this_allocator> >(this_ptr, a_copy);
      a_copy.destroy(this_ptr);
   }

   template<class Ptr>
   void release(const Ptr & ptr) // nothrow
   {
      if(this->ref_release()){
         this->dispose(ptr);
         this->weak_release();
      }
   }

   void weak_release() // nothrow
   {
      if(sp_counted_base::weak_release()){
         this->destroy();
      }
   }
};


} // namespace detail

} // namespace shmem

} // namespace boost

#endif  // #ifndef BOOST_SHMEM_DETAIL_SP_COUNTED_IMPL_HPP_INCLUDED
