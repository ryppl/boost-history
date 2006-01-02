///////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_ALLOCATOR_HPP
#define BOOST_SHMEM_ALLOCATOR_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/shmem/shmem_fwd.hpp>
#include <boost/utility/addressof.hpp>
#include <boost/get_pointer.hpp>
#include <boost/assert.hpp>
#include <boost/shmem/detail/utilities.hpp>
#include <boost/shmem/exceptions.hpp>
#include <memory>
#include <algorithm>
#include <stdexcept>

/*!\file
   Describes an allocator that allocates portions of fixed size
   memory buffer (shared memory, mapped file...)
*/

namespace boost {

namespace shmem {

/*!allocator specialization for void*/
template<class SegmentManager>
class  allocator<void, SegmentManager>
{
 public:
   typedef void                                       value_type;
   typedef typename SegmentManager::void_pointer      pointer;
   typedef typename detail::
      pointer_to_other<pointer, const void>::type     const_pointer;

   /*!Obtains an allocator of other type*/
   template<class T2>
   struct rebind
   {   
     public:
      typedef allocator<T2, SegmentManager>      other;
   };
};

/*!An STL compatible allocator that uses a segment manager as 
   memory source. The internal pointer type will of the same type (raw, smart) as
   "typename SegmentManager::void_pointer" type. This allows
   placing the allocator in shared memory, memory mapped-files, etc...*/
template<class T, class SegmentManager>
class allocator 
{
 private:
   /*!Self type*/
   typedef allocator<T, SegmentManager>                      self_t;

   /*!Segment manager*/
   typedef SegmentManager                                    segment_manager;

   /*!Pointer to void */
   typedef typename segment_manager::void_pointer            aux_pointer_t;

   /*!Typedef to const void pointer */
   typedef typename 
      detail::pointer_to_other<aux_pointer_t, const void>::type   cvoid_ptr;

   /*!Pointer to the allocator*/
   typedef typename detail::pointer_to_other
      <cvoid_ptr, segment_manager>::type                          alloc_ptr_t;

   /*!Not assignable from related allocator*/
   template<class T2, class AllocAlgo2>
   allocator& operator=(const allocator<T2, AllocAlgo2>&);

   /*!Not assignable from other allocator*/
   allocator& operator=(const allocator&);

   /*!Pointer to the allocator*/
   alloc_ptr_t mp_mngr;

 public:
   typedef T                                             value_type;
   typedef typename detail::pointer_to_other
      <cvoid_ptr, T>::type                               pointer;
   typedef typename detail::
      pointer_to_other<pointer, const T>::type           const_pointer;
   typedef value_type &                                  reference;
   typedef const value_type &                            const_reference;
   typedef typename std::allocator<T>::size_type         size_type;
   typedef typename std::allocator<T>::difference_type   difference_type;

   /*!Obtains an allocator of other type*/
   template<class T2>
   struct rebind
   {   
      typedef allocator<T2, SegmentManager>         other;
   };

   /*!Returns the segment manager. Never throws*/
   segment_manager* get_segment_manager()const
      {  using boost::get_pointer;  return  get_pointer(mp_mngr);   }

   /*!Returns address of mutable object. Never throws*/
   pointer address(reference value) const
      {  return pointer(boost::addressof(value));  }

   /*!Returns address of non mutable object. Never throws*/
   const_pointer address(const_reference value) const
      {  return const_pointer(boost::addressof(value));  }

   /*!Constructor from the segment manager. Never throws*/
   allocator(segment_manager *segment_mngr) 
      : mp_mngr(segment_mngr) { }

   /*!Constructor from other allocator. Never throws*/
   allocator(const allocator &other) 
      : mp_mngr(other.get_segment_manager()){ }

   /*!Constructor from related allocator. Never throws*/
   template<class T2>
   allocator(const allocator<T2, SegmentManager> &other) 
      : mp_mngr(other.get_segment_manager()){}

   /*!Allocates memory for an array of count elements. 
      Throws boost::shmem::bad_alloc if there is no enough memory*/
   pointer allocate(size_type count, cvoid_ptr hint = 0)
      {  return pointer((value_type*)mp_mngr->allocate(count*sizeof(value_type)));  }

   /*!Deallocates memory previously allocated. Never throws*/
   void deallocate(pointer ptr, size_type)
      {  using boost::get_pointer; mp_mngr->deallocate(get_pointer(ptr));  }

   /*!Construct object, calling constructor. 
      Throws if T(const Convertible &) throws*/
   template<class Convertible>
   void construct(pointer ptr, const Convertible &value)
      {  using boost::get_pointer;  new(get_pointer(ptr)) value_type(value);  }

   /*!Destroys object. Throws if object's destructor throws*/
   void destroy(pointer ptr)
      {  BOOST_ASSERT(ptr != 0); (*ptr).~value_type();  }

   /*!Returns the number of elements that could be allocated. Never throws*/
   size_type max_size() const
      {  return mp_mngr->get_size();   }

   /*!Swap segment manager. Does not throw. If each allocator is placed in
      different memory segments, the result is undefined.*/
   friend void swap(self_t &alloc1, self_t &alloc2)
   {
      detail::swap_function(alloc1.mp_mngr, alloc2.mp_mngr);
   }
};

/*!Equality test for same type of allocator*/
template<class T, class SegmentManager> inline
bool operator==(const allocator<T , SegmentManager>  &alloc1, 
                const allocator<T, SegmentManager>  &alloc2)
   {  return alloc1.get_segment_manager() == alloc2.get_segment_manager(); }

/*!Inequality test for same type of allocator*/
template<class T, class SegmentManager> inline
bool operator!=(const allocator<T, SegmentManager>  &alloc1, 
                const allocator<T, SegmentManager>  &alloc2)
   {  return alloc1.get_segment_manager() != alloc2.get_segment_manager(); }

/*!This specialization indicates that the construct function allows
   convertible types to construct the value type. This allows
   storing less allocator instances in containers.*/
template<class T, class SegmentManager>
struct has_convertible_construct
   <boost::shmem::allocator<T, SegmentManager> >
{
   enum {   value = true };
};

}  //namespace shmem {

}  //namespace boost {

#include <boost/shmem/detail/config_end.hpp>

#endif   //BOOST_SHMEM_ALLOCATOR_HPP

