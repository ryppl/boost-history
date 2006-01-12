//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Peter Dimov 2001, 2002
// (C) Copyright Ion Gaztañaga 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_INTRUSIVE_PTR_HPP_INCLUDED
#define BOOST_SHMEM_INTRUSIVE_PTR_HPP_INCLUDED


#include <boost/config.hpp>

#ifdef BOOST_MSVC  // moved here to work around VC++ compiler crash
# pragma warning(push)
# pragma warning(disable:4284) // odd return type for operator->
#endif

#include <boost/assert.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/shmem/detail/utilities.hpp>
#include <boost/shmem/detail/generic_cast.hpp>

#include <functional>           // for std::less
#include <iosfwd>               // for std::basic_ostream


namespace boost {
namespace shmem {

//
//  basic_intrusive_ptr
//
//  A smart pointer that uses intrusive reference counting.
//
//  Relies on unqualified calls to
//  
//      void intrusive_ptr_add_ref(T * p);
//      void intrusive_ptr_release(T * p);
//
//          (p != 0)
//
//  The object is responsible for destroying itself.
//

template<class T, class VoidPointer>
class basic_intrusive_ptr
{
   private:

   typedef VoidPointer VP;
   typedef basic_intrusive_ptr this_type;

public:
   typedef typename detail::pointer_to_other<VP, T>::type pointer;

   typedef T element_type;

   basic_intrusive_ptr(): p_(0)
   {}

   basic_intrusive_ptr(const pointer &p, bool add_ref = true): p_(p)
   {
      if(p_ != 0 && add_ref) intrusive_ptr_add_ref(p_);
   }

   template<class U> basic_intrusive_ptr
      (basic_intrusive_ptr<U, VP> const & rhs)
      :  p_(rhs.get())
   {
      if(p_ != 0) intrusive_ptr_add_ref(p_);
   }

   basic_intrusive_ptr(basic_intrusive_ptr const & rhs)
      :  p_(rhs.p_)
   {
      if(p_ != 0) intrusive_ptr_add_ref(p_);
   }

   ~basic_intrusive_ptr()
   {
      if(p_ != 0) intrusive_ptr_release(p_);
   }

   template<class U> basic_intrusive_ptr & operator=
      (basic_intrusive_ptr<U, VP> const & rhs)
   {
      this_type(rhs).swap(*this);
      return *this;
   }

   basic_intrusive_ptr & operator=(basic_intrusive_ptr const & rhs)
   {
      this_type(rhs).swap(*this);
      return *this;
   }

   basic_intrusive_ptr & operator=(pointer rhs)
   {
      this_type(rhs).swap(*this);
      return *this;
   }

   pointer &get()
      {  return p_;  }

   const pointer &get() const
      {  return p_;  }

   T & operator*() const
      {  return *p_; }

   const pointer &operator->() const
      {  return p_;  }

   pointer &operator->()
      {  return p_;  }

   typedef pointer this_type::*unspecified_bool_type;

   operator unspecified_bool_type () const
      {  return p_ == 0? 0: &this_type::p_;  }

   // operator! is a Borland-specific workaround
   bool operator! () const
      {  return p_ == 0;   }

   void swap(basic_intrusive_ptr & rhs)
   {
      using std::swap;
      swap(p_, rhs.p_);
   }

private:

   pointer p_;
};

template<class T, class U, class VP>
inline bool operator==(basic_intrusive_ptr<T, VP> const & a, 
                       basic_intrusive_ptr<U, VP> const & b)
   {  return a.get() == b.get(); }

template<class T, class U, class VP>
inline bool operator!=(basic_intrusive_ptr<T, VP> const & a,
                       basic_intrusive_ptr<U, VP> const & b)
   {  return a.get() != b.get(); }

template<class T, class VP>
inline bool operator==(basic_intrusive_ptr<T, VP> const & a,
                       const typename basic_intrusive_ptr<T, VP>::pointer &b)
   {  return a.get() == b; }

template<class T, class VP>
inline bool operator!=(basic_intrusive_ptr<T, VP> const & a,
                       const typename basic_intrusive_ptr<T, VP>::pointer &b)
   {  return a.get() != b; }

template<class T, class VP>
inline bool operator==(const typename basic_intrusive_ptr<T, VP>::pointer &a,
                       basic_intrusive_ptr<T, VP> const & b)
   {  return a == b.get(); }

template<class T, class VP>
inline bool operator!=(const typename basic_intrusive_ptr<T, VP>::pointer &a,
                       basic_intrusive_ptr<T, VP> const & b)
   {  return a != b.get(); }

template<class T, class VP>
inline bool operator<
   (basic_intrusive_ptr<T, VP> const & a, 
    basic_intrusive_ptr<T, VP> const & b)
{  
   return std::less<typename basic_intrusive_ptr<T, VP>::pointer>()
      (a.get(), b.get());   
}

template<class T, class VP>
void swap(basic_intrusive_ptr<T, VP> & lhs,
          basic_intrusive_ptr<T, VP> & rhs)
   {  lhs.swap(rhs); }

// mem_fn support
template<class T, class VP>
typename basic_intrusive_ptr<T, VP>::pointer
   get_pointer(basic_intrusive_ptr<T, VP> const & p)
   {  return p.get();   }

template<class T, class U, class VP>
basic_intrusive_ptr<T, VP> static_pointer_cast
   (basic_intrusive_ptr<U, VP> const & p)
   {  return do_static_cast<U>(p.get());  }

template<class T, class U, class VP>
basic_intrusive_ptr<T, VP> const_pointer_cast
   (basic_intrusive_ptr<U, VP> const & p)
   {  return do_const_cast<U>(p.get());   }

template<class T, class U, class VP>
basic_intrusive_ptr<T, VP> dynamic_pointer_cast
   (basic_intrusive_ptr<U, VP> const & p)
   {  return do_dynamic_cast<U>(p.get()); }

template<class T, class U, class VP>
basic_intrusive_ptr<T, VP>reinterpret_pointer_cast
   (basic_intrusive_ptr<U, VP> const & p)
   {  return do_reinterpret_cast<U>(p.get());   }

// operator<<
template<class E, class T, class Y, class VP>
std::basic_ostream<E, T> & operator<< 
   (std::basic_ostream<E, T> & os, basic_intrusive_ptr<Y, VP> const & p)
   {  os << p.get(); return os;  }

} // namespace shmem
} // namespace boost

#ifdef BOOST_MSVC
# pragma warning(pop)
#endif    

#endif  // #ifndef BOOST_SHMEM_INTRUSIVE_PTR_HPP_INCLUDED
