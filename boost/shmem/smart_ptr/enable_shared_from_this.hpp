//////////////////////////////////////////////////////////////////////////////
//
// This file is the adaptation for Shmem of boost/enable_shared_from_this.hpp
//
// (C) Copyright Peter Dimov 2002
// (C) Copyright Ion Gaztañaga 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_ENABLE_SHARED_FROM_THIS_HPP_INCLUDED
#define BOOST_SHMEM_ENABLE_SHARED_FROM_THIS_HPP_INCLUDED

#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/shmem/smart_ptr/weak_ptr.hpp>
#include <boost/shmem/smart_ptr/shared_ptr.hpp>


namespace boost{
namespace shmem{

template<class T, class A, class D>
class enable_shared_from_this
{
   protected:

   enable_shared_from_this()
   {}

   enable_shared_from_this(enable_shared_from_this const &)
   {}

   enable_shared_from_this & operator=(enable_shared_from_this const &)
   {  return *this;  }

   ~enable_shared_from_this()
   {}

   public:

   shared_ptr<T, A, D> shared_from_this()
   {
      shared_ptr<T, A, D> p(_internal_weak_this);
      BOOST_ASSERT(detail::get_pointer(p.get()) == this);
      return p;
   }

   shared_ptr<T const, A, D> shared_from_this() const
   {
      shared_ptr<T const, A, D> p(_internal_weak_this);
      BOOST_ASSERT(detail::get_pointer(p.get()) == this);
      return p;
   }

   typedef T element_type;
   mutable weak_ptr<element_type, A, D> _internal_weak_this;
};

} // namespace shmem
} // namespace boost

#include <boost/shmem/detail/config_end.hpp>

#endif  // #ifndef BOOST_SHMEM_ENABLE_SHARED_FROM_THIS_HPP_INCLUDED

