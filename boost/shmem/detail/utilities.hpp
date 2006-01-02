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
#include <algorithm>
#include <functional>
#include <utility>

namespace boost { namespace shmem { namespace detail {

/*!This class liberates shared memory pointer in destructor and it 
   useful to avoid memory leaks when exceptions are thrown*/

template <class shmem_alloc_type>
class scoped_shm_ptr
{
private:
   scoped_shm_ptr(scoped_shm_ptr&);
   scoped_shm_ptr();
   void *               mp_ptr;
   shmem_alloc_type    &m_shmem_alloc;   

public:

   /*!Constructor, takes the pointer to liberate and a reference to 
      the shared memory allocator*/
   scoped_shm_ptr(void * p, shmem_alloc_type &shmalloc)
      : mp_ptr(p), m_shmem_alloc(shmalloc){}

   /*!Destructor, liberates shared memory pointer if it's not null*/
   ~scoped_shm_ptr()
      {  if(mp_ptr) m_shmem_alloc.deallocate(mp_ptr);  }

   /*!Reassigns the pointer to liberate in destructor*/
   void reset(void * p = 0)
      {  if(mp_ptr) m_shmem_alloc.deallocate(mp_ptr);  mp_ptr = p;  }

   /*!Returns the pointer to liberate in destructor*/
   void * get() const
      {  return mp_ptr;  }

   /*!Sets the pointer to liberate to 0 and return previous pointer*/
   void *release()
      {  void *temp = mp_ptr; mp_ptr = 0; return temp;  }
};

template <class Allocator>
struct scoped_deallocator
{
   typedef typename Allocator::pointer pointer;

   Allocator& alloc_;
   pointer ptr_;

   scoped_deallocator(Allocator& a, pointer p)
         : alloc_(a), ptr_(p) {}

   ~scoped_deallocator() 
      {  if (ptr_) alloc_.deallocate(ptr_, 1);  }

   void release()
      {  ptr_ = 0;  }
};

template <class Allocator>
struct scoped_array_deallocator
{
   typedef typename Allocator::pointer pointer;

   Allocator& alloc_;
   pointer ptr_;
   std::size_t length_;

   scoped_array_deallocator(Allocator& a, pointer p, std::size_t length)
         : alloc_(a), ptr_(p), length_(length) {}

  ~scoped_array_deallocator() 
      {  if (ptr_) alloc_.deallocate(ptr_, length_);  }

   void release()
      {  ptr_ = 0;  }
};

template <class Allocator>
struct scoped_destructor
{
   typedef typename Allocator::pointer pointer;

   Allocator& alloc_;
   pointer ptr_;

   scoped_destructor(Allocator& a, pointer p)
         : alloc_(a), ptr_(p) {}

   ~scoped_destructor() 
      {  if (ptr_) alloc_.destroy(ptr_);  }

   void release()
      {  ptr_ = 0;  }
};

template <class T>
void swap_function(T& x, T& y)
{
   using std::swap;
   swap(x, y);
}

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

// identity is an extensions: it is not part of the standard.
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


}}   //namespace boost { namespace shmem { 

#endif   //#ifndef BOOST_SHMEM_DETAIL_UTILITIES_HPP

