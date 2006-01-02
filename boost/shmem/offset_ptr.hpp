//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_OFFSET_PTR_HPP
#define BOOST_OFFSET_PTR_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/shmem/shmem_fwd.hpp>
#include <boost/shmem/detail/utilities.hpp>
#include <boost/shmem/detail/cast_tags.hpp>
#include <boost/assert.hpp>
#include <boost/type_traits/has_trivial_constructor.hpp>
#include <boost/type_traits/has_trivial_destructor.hpp>

/*!\file
   Describes a smart pointer that stores the offset between this pointer and
   target pointee, called offset_ptr. This file defines some variations of 
   this pointer. 
*/
namespace boost {

namespace shmem {

/*!A base class for offset_ptr that stores the offset between this and pointee
   and allows storing the null value.*/
class full_offset_ptr
{
 protected:
   /*!Sets internal offset from address*/
   void set_offset(const void *ptr)
   {  
      m_null   = !ptr; 
      if(ptr)
         m_offset = detail::char_ptr_cast(ptr) - 
                    detail::char_ptr_cast(this); 
   }

   /*!Increments internal offset*/
   void inc_offset(std::ptrdiff_t bytes)
      {  m_offset += bytes;   }

   /*!Decrements internal offset*/
   void dec_offset(std::ptrdiff_t bytes)
      {  m_offset -= bytes;   }

   /*!Obtains offset from address*/
   void* get_pointer(void) const
      {  return m_null? 0 : detail::char_ptr_cast(this) + m_offset; }

 private:   // Private attributes
   std::ptrdiff_t m_offset; //Distance between this object and pointed address
   bool           m_null;   //Is it a null pointer?
};

/*!This pointer stores null pointer as offset 1, so it can't point to the
   "this" + 1 byte.*/
class offset_1_null_ptr
{
 protected:
   /*!Sets internal offset from address*/
   void set_offset(const void *ptr)
   {  
      //offset == 1 && ptr != 0 is not legal for this pointer
      if(ptr){
         m_offset = detail::char_ptr_cast(ptr) - 
                    detail::char_ptr_cast(this);
         BOOST_ASSERT(m_offset != 1);
      }
      else{
         m_offset = 1;
      }
   }

   /*!Increments internal offset*/
   void inc_offset(std::ptrdiff_t bytes)
      {  m_offset += bytes;   }

   /*!Decrements internal offset*/
   void dec_offset(std::ptrdiff_t bytes)
      {  m_offset -= bytes;   }

   /*!Obtains offset from address*/
   void* get_pointer(void) const
      {  return m_offset == 1 ? 0 : detail::char_ptr_cast(this) + m_offset; }

 private:   // Private attributes
   std::ptrdiff_t m_offset; //Distance between this object and pointed address
};

/*!
   A smart pointer that can point to a pointee that resides in the same shared
   memory segment. If it resides in user private memory, it can point to a shared
   memory segment pointee or to other user private memory pointee.
*/
template <class PointedType, class RootPtr>
class offset_ptr : public RootPtr
{
   typedef RootPtr                                          base_t;
   typedef boost::shmem::workaround::random_it<PointedType> random_it_t;
   typedef offset_ptr<PointedType, RootPtr>                 self_t;
   typedef typename random_it_t::const_pointer              const_pointer_t;
   typedef typename random_it_t::const_reference            const_reference_t;

   void unspecified_bool_type_func() const {}
   typedef void (self_t::*unspecified_bool_type)() const;

 public:
   typedef typename random_it_t::pointer                    pointer;
   typedef typename random_it_t::reference                  reference;
   typedef typename random_it_t::value_type                 value_type;
   typedef typename random_it_t::difference_type            difference_type;
   typedef typename random_it_t::iterator_category          iterator_category;

 public:   //Public Functions

   /*!Obtains offset_ptr <OtherType> from offset_ptr*/
   template<class OtherType>
   struct rebind
   {   
      typedef offset_ptr<OtherType, RootPtr> type;
   };

   /*!Default Constructor. Never throws */
   offset_ptr(){}

   /*!Constructor from raw pointer (allows "0" pointer conversion). Never throws.*/
   offset_ptr(pointer ptr) {  this->set_offset(ptr); }

   /*!Constructor from other pointer. Never throws.*/
   template <class T>
   offset_ptr(T *ptr) 
   {  pointer p (ptr);  (void)p; this->set_offset(ptr); }

   /*!Constructor from other offset_ptr */
   offset_ptr(const offset_ptr& ptr) 
      {  this->set_offset(ptr.get());   }

   /*!Constructor from other offset_ptr. If pointers of pointee types are 
      convertible, offset_ptrs will be convertibles. Never throws.*/
   template<class T2>
   offset_ptr(const offset_ptr<T2, RootPtr> &ptr) 
      {  this->set_offset(ptr.get());   }

   /*!Emulates static_cast operator. Never throws.  */
   template<class Y>
   offset_ptr(const offset_ptr<Y, RootPtr> & r, detail::static_cast_tag)
      {  this->set_offset(static_cast<PointedType*>(r.get()));   }

   /*!Emulates const_cast operator. Never throws.*/
   template<class Y>
   offset_ptr(const offset_ptr<Y, RootPtr> & r, detail::const_cast_tag)
      {  this->set_offset(const_cast<PointedType*>(r.get()));   }

   /*!Emulates dynamic_cast operator. Never throws.*/
   template<class Y>
   offset_ptr(const offset_ptr<Y, RootPtr> & r, detail::dynamic_cast_tag)
      {  this->set_offset(dynamic_cast<PointedType*>(r.get()));   }

   /*!Emulates reinterpret_cast operator. Never throws.*/
   template<class Y>
   offset_ptr(const offset_ptr<Y, RootPtr> & r, detail::reinterpret_cast_tag)
      {  this->set_offset(reinterpret_cast<PointedType*>(r.get()));   }

   /*!Obtains raw pointer from offset. Never throws.*/
   pointer get()const
      {  return (pointer)this->get_pointer();   }

   /*!Pointer-like -> operator. It can return 0 pointer. Never throws.*/
   pointer operator->() const           
      {  return this->get(); }

   /*!Dereferencing operator, if it is a null offset_ptr behavior 
         is undefined. Never throws.*/
   reference operator* () const           
      {  return *(this->get());   }

   /*!Indexing operator. Never throws.*/
   reference operator[](std::ptrdiff_t idx) const   
      {  return this->get()[idx];  }

   /*!Assignment from pointer (saves extra conversion). Never throws.*/
   offset_ptr& operator= (pointer from)
      {  pointer p(from);  (void)p; this->set_offset(from); return *this;  }

   /*!Assignment from other offset_ptr. Never throws.*/
   offset_ptr& operator= (const offset_ptr & pt)
      {  this->set_offset(pt.get());  return *this;  }

   /*!Assignment from related offset_ptr. If pointers of pointee types 
         are assignable, offset_ptrs will be assignable. Never throws.*/
   template <class T2>
   offset_ptr& operator= (const offset_ptr<T2, RootPtr> & pt)
      {  this->set_offset(pt.get());  return *this;  }
 
   /*!offset_ptr + std::ptrdiff_t. Never throws.*/
   offset_ptr operator+ (std::ptrdiff_t offset) const   
      {  return offset_ptr(this->get()+offset);   }

  /*!std::ptrdiff_t + offset_ptr. Never throws.*/
   friend offset_ptr operator+ (std::ptrdiff_t offset, const offset_ptr &ptr)   
      {  return offset_ptr(offset + ptr.get());   }

   /*!offset_ptr - std::ptrdiff_t. Never throws.*/
   offset_ptr operator- (std::ptrdiff_t offset) const   
      {  return offset_ptr(this->get()-offset);   }

   /*!offset_ptr += std::ptrdiff_t. Never throws.*/
   offset_ptr &operator+= (std::ptrdiff_t offset)
      {  this->inc_offset(offset * sizeof (PointedType));   return *this;  }

   /*!offset_ptr -= std::ptrdiff_t. Never throws.*/
   offset_ptr &operator-= (std::ptrdiff_t offset)
      {  this->dec_offset(offset * sizeof (PointedType));   return *this;  }

   /*!++offset_ptr. Never throws.*/
   offset_ptr& operator++ (void) 
      {  this->inc_offset(sizeof (PointedType));   return *this;  }

   /*!offset_ptr++. Never throws.*/
   offset_ptr operator++ (int)
      {  offset_ptr temp(*this); ++*this; return temp; }

   /*!--offset_ptr. Never throws.*/
   offset_ptr& operator-- (void) 
      {  this->dec_offset(sizeof (PointedType));   return *this;  }

   /*!offset_ptr--. Never throws.*/
   offset_ptr operator-- (int)
      {  offset_ptr temp(*this); --*this; return temp; }

   /*!safe bool conversion operator. Never throws.*/
   operator unspecified_bool_type() const  
      {  return this->get()? &self_t::unspecified_bool_type_func : 0;   }

   /*!Not operator. Not needed in theory, but improves portability. 
      Never throws.*/
   bool operator! () const
      {  return this->get() == 0;   }
};

/*!offset_ptr<T1, R1> == offset_ptr<T2, R2>. Never throws.*/
template<class T1, class R1, class T2, class R2>
inline bool operator== (const offset_ptr<T1, R1> &pt1, 
                        const offset_ptr<T2, R2> &pt2)
   {  return pt1.get() == pt2.get();  }

/*!offset_ptr<T1, R1> != offset_ptr<T2, R2>. Never throws.*/
template<class T1, class R1, class T2, class R2>
inline bool operator!= (const offset_ptr<T1, R1> &pt1, 
                        const offset_ptr<T2, R2> &pt2)
   {  return pt1.get() != pt2.get();  }

/*!offset_ptr<T1, R1> < offset_ptr<T2, R2>. Never throws.*/
template<class T1, class R1, class T2, class R2>
inline bool operator< (const offset_ptr<T1, R1> &pt1, 
                       const offset_ptr<T2, R2> &pt2)
   {  return pt1.get() < pt2.get();  }

/*!offset_ptr<T1, R1> <= offset_ptr<T2, R2>. Never throws.*/
template<class T1, class R1, class T2, class R2>
inline bool operator<= (const offset_ptr<T1, R1> &pt1, 
                        const offset_ptr<T2, R2> &pt2)
   {  return pt1.get() <= pt2.get();  }

/*!offset_ptr<T1, R1> > offset_ptr<T2, R2>. Never throws.*/
template<class T1, class R1, class T2, class R2>
inline bool operator> (const offset_ptr<T1, R1> &pt1, 
                       const offset_ptr<T2, R2> &pt2)
   {  return pt1.get() > pt2.get();  }

/*!offset_ptr<T1, R1> >= offset_ptr<T2, R2>. Never throws.*/
template<class T1, class R1, class T2, class R2>
inline bool operator>= (const offset_ptr<T1, R1> &pt1, 
                        const offset_ptr<T2, R2> &pt2)
   {  return pt1.get() >= pt2.get();  }

/*!operator<< */
template<class E, class T, class Y, class R> 
inline std::basic_ostream<E, T> & operator<< 
   (std::basic_ostream<E, T> & os, offset_ptr<Y, R> const & p)
{  return os << p.get();   }

/*!operator>> */
template<class E, class T, class Y, class R> 
inline std::basic_istream<E, T> & operator>> 
   (std::basic_istream<E, T> & os, offset_ptr<Y, R> & p)
{  Y * tmp; return os >> tmp; p = tmp;   }

/*!std::ptrdiff_t + offset_ptr  */
template<class T, class R>
inline offset_ptr<T,R> operator+(std::ptrdiff_t diff, offset_ptr<T,R>& right)
   {  return right + diff;  }

/*!offset_ptr - offset_ptr  */
template<class T, class R, class T2, class R2>
inline std::ptrdiff_t operator- (const offset_ptr<T, R> &pt, const offset_ptr<T2, R2> &pt2)
   {  return pt.get()- pt2.get();   }

/*!swap specialization */
template<class T, class R>
inline void swap (boost::shmem::offset_ptr<T, R> &pt, 
                  boost::shmem::offset_ptr<T, R> &pt2)
{  
   typename offset_ptr<T, R>::value_type *ptr = pt.get();
   pt = pt2;
   pt2 = ptr;
}

}  //namespace shmem {

/*!get_pointer() enables boost::mem_fn to recognize offset_ptr. 
   Never throws.*/
template<class T, class R>
inline T * get_pointer(boost::shmem::offset_ptr<T, R> const & p)
   {  return p.get();   }

/*!Simulation of static_cast between pointers. Never throws.*/
template<class T, class U, class R> 
inline boost::shmem::offset_ptr<T, R> static_pointer_cast(boost::shmem::offset_ptr<U, R> const & r)
   {  return boost::shmem::offset_ptr<T, R>(r, boost::shmem::detail::static_cast_tag());  }

/*!Simulation of const_cast between pointers. Never throws.*/
template<class T, class U, class R> 
inline boost::shmem::offset_ptr<T, R> const_pointer_cast(boost::shmem::offset_ptr<U, R> const & r)
   {  return boost::shmem::offset_ptr<T, R>(r, boost::shmem::detail::const_cast_tag());  }

/*!Simulation of dynamic_cast between pointers. Never throws.*/
template<class T, class U, class R> 
inline boost::shmem::offset_ptr<T, R> dynamic_pointer_cast(boost::shmem::offset_ptr<U, R> const & r)
   {  return boost::shmem::offset_ptr<T, R>(r, boost::shmem::detail::dynamic_cast_tag());  }

/*!Simulation of reinterpret_cast between pointers. Never throws.*/
template<class T, class U, class R> 
inline boost::shmem::offset_ptr<T, R> reinterpret_pointer_cast(boost::shmem::offset_ptr<U, R> const & r)
   {  return boost::shmem::offset_ptr<T, R>(r, boost::shmem::detail::reinterpret_cast_tag());  }

/*!has_trivial_constructor<> == true_type specialization for optimizations*/
template <class PointedType, class RootPtr>
struct has_trivial_constructor
   < boost::shmem::offset_ptr<PointedType, RootPtr> > 
   : public true_type{};

/*!has_trivial_destructor<> == true_type specialization for optimizations*/
template <class PointedType, class RootPtr>
struct has_trivial_destructor
   < boost::shmem::offset_ptr<PointedType, RootPtr> > 
   : public true_type{};

}  //namespace boost {

#include <boost/shmem/detail/config_end.hpp>

#endif //#ifndef BOOST_OFFSET_PTR_HPP

