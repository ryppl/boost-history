//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_DETAIL_UTILITIES_HPP
#define BOOST_SHMEM_DETAIL_UTILITIES_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/shmem_fwd.hpp>
#include <boost/get_pointer.hpp>
#include <boost/detail/no_exceptions_support.hpp>
#include <boost/type_traits/has_nothrow_destructor.hpp>
#include <boost/noncopyable.hpp>
#include <boost/call_traits.hpp>
#include <algorithm>
#include <functional>
#include <utility>

namespace boost { namespace shmem { namespace detail {

/*!Overload for smart pointers to avoid ADL problems with get_pointer*/
template<class Ptr>
inline typename Ptr::value_type *get_pointer(const Ptr &ptr)
   {  using boost::get_pointer;  return get_pointer(ptr);   }

/*!Overload for raw pointers to avoid ADL problems with get_pointer*/
template<class T>
inline T *get_pointer(T *ptr)
   {  return ptr; }

/*!To avoid ADL problems with swap*/
template <class T>
inline void swap(T& x, T& y)
{
   using std::swap;
   swap(x, y);
}

/*!A deleter for scoped_ptr that deallocates the memory
   allocated for an object using a STL allocator.*/
template <class Allocator>
struct scoped_deallocator
{
   typedef typename Allocator::pointer pointer;

   Allocator& m_alloc;

   scoped_deallocator(Allocator& a)
         : m_alloc(a) {}

   void operator()(pointer ptr)
      {  if (ptr) m_alloc.deallocate(ptr, 1);  }

};

/*!A deleter for scoped_ptr that deallocates the memory
   allocated for an array of objects using a STL allocator.*/
template <class Allocator>
struct scoped_array_deallocator
{
   typedef typename Allocator::pointer pointer;

   scoped_array_deallocator(Allocator& a, std::size_t length)
         : m_alloc(a), m_length(length) {}

   void operator()(pointer &ptr)
      {  m_alloc.deallocate(ptr, m_length);  }

   private:
   Allocator&  m_alloc;
   std::size_t m_length;
};

/*!A deleter for scoped_ptr that destroys
   an object using a STL allocator.*/
template <class Allocator>
struct scoped_destructor
{
   typedef typename Allocator::pointer pointer;

   Allocator& m_alloc;

   scoped_destructor(Allocator& a)
         : m_alloc(a){}

   void operator()(pointer &ptr)
      {  m_alloc.destroy(ptr);  }
};

/*!Forces a cast from any pointer to char * pointer*/
template<class T>
inline char* char_ptr_cast(T *ptr)
{
   //This is nasty, but we like it a lot!
   return (char*)(ptr);
}

inline char* char_ptr_cast()
{
   //This is nasty, but we like it a lot!
   return (char*)(0);
}

template <class Pair>
struct select1st 
   : public std::unary_function<Pair, typename Pair::first_type> 
{
   const typename Pair::first_type& operator()(const Pair& x) const 
      {  return x.first;   }
};

// identity is an extension: it is not part of the standard.
template <class T>
struct identity 
   : public std::unary_function<T,T> 
{
   const T& operator()(const T& x) const 
      { return x; }
};

//Rounds "orig_size" by excess to round_to bytes
inline std::size_t get_rounded_size(std::size_t orig_size, std::size_t round_to)
{
   return ((orig_size-1)/round_to+1)*round_to;
}
 
template <std::size_t OrigSize, std::size_t RoundTo>
struct ct_rounded_size
{
   enum { value = ((OrigSize-1)/RoundTo+1)*RoundTo };
};

/*!Obtains a generic pointer of the same type that
   can point to other pointed type: Ptr<?> -> Ptr<NewValueType>*/
template <class Ptr, class NewValueType> 
struct pointer_to_other
{
   typedef typename Ptr::template 
      rebind<NewValueType>::type type;
};

/*!Obtains a generic pointer of the same type that
   can point to other pointed type. Raw pointer 
   specialization Ptr<?> -> Ptr<NewValueType>*/
template <class T, class NewValueType> 
struct pointer_to_other<T*, NewValueType>
{
   typedef NewValueType       *type;
};

//Anti-exception node eraser
template<class Cont>
class value_eraser : boost::noncopyable
{
   public:
   value_eraser(Cont & cont, typename Cont::iterator it) 
      : m_map(cont), m_index_it(it), m_erase(true){}
   ~value_eraser()  
   {
      if(boost::has_nothrow_destructor<typename Cont::value_type>::value){
         if(m_erase) m_map.erase(m_index_it);
      }
      else{
         //value_eraser is used in exceptions, so we
         //disable double-exception danger
         BOOST_TRY{  if(m_erase) m_map.erase(m_index_it);  }
         BOOST_CATCH(...){}
         BOOST_CATCH_END
      }
   }
   void release() {  m_erase = false;  }

   private:
   Cont                    &m_map;
   typename Cont::iterator  m_index_it;
   bool                    m_erase;
};

}  //namespace detail {

/*!Trait class to detect if an allocator has
   a templatized construct function. If this is the case
   in node containers we only need just one allocator
   instead of three*/
template <class Allocator>
struct has_convertible_construct
{
   enum {   value = false };
};

/*!Trait classes to detect if an index is a node
   index. This allows more efficient operations
   when deallocating named objects.*/
template <class Index>
struct is_node_index
{
   enum {   value = false };
};

/*!Trait class to detect if an smart pointer has 
   multi-segment addressing capabilities.*/
template <class Ptr>
struct is_multisegment_ptr
{
   enum {   value = false };
};


/*!A Shmem shared pointer deleter that uses the segment manager's 
   destroy_ptr function to destroy the shared resource.*/
template<class SegmentManager>
class shared_ptr_deleter
{
   public:
   typedef typename SegmentManager::void_pointer      pointer;

   private:
   typedef typename detail::pointer_to_other
      <pointer, SegmentManager>::type   segment_manager_pointer;

   segment_manager_pointer mp_deleter;

   public:
   shared_ptr_deleter(const segment_manager_pointer &pdeleter)
      :  mp_deleter(pdeleter)
   {}

   template<class Ptr>
   void operator()(const Ptr &p)
   {  mp_deleter->destroy_ptr(detail::get_pointer(p));   }
};

}}   //namespace boost { namespace shmem { 

#endif   //#ifndef BOOST_SHMEM_DETAIL_UTILITIES_HPP

