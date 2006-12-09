/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga  2006.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_DETAIL_UTILITIES_HPP
#define BOOST_INTRUSIVE_DETAIL_UTILITIES_HPP

#include <boost/intrusive/detail/config_begin.hpp>
#include <boost/intrusive/detail/pointer_type.hpp>
#include <boost/intrusive/detail/pointer_to_other.hpp>
#include <boost/get_pointer.hpp>
#include <cstddef>

namespace boost {
namespace intrusive {
namespace detail {

class null_destroyer
{
   public:
   template <class Pointer>
   void operator()(Pointer)
   {}
};

template <class A>
class allocator_destroyer
{
   private:
   A & a_;

   public:
   allocator_destroyer(A &a)
      :  a_(a)
   {}

   void operator()(typename A::pointer p)
   {  
      a_.destroy(p);
      a_.deallocate(p, 1);
   }
};

class null_cloner
{
   public:
   template<class Pointer>
   Pointer operator()(Pointer p)
   {  return p; }
};


template<bool ConstantSize, class SizeType>
struct size_holder
{
   enum {   constant_time_size = ConstantSize  };
   typedef SizeType  size_type;

   SizeType get_size() const
   {  return size_;  }

   void set_size(SizeType size)
   {  size_ = size; }

   void decrement()
   {  --size_; }

   void increment()
   {  ++size_; }

   SizeType size_;
};

template<class SizeType>
struct size_holder<false, SizeType>
{
   enum {   constant_time_size = false  };
   typedef SizeType  size_type;

   size_type get_size() const
   {  return 0;  }

   void set_size(size_type size)
   {}

   void decrement()
   {}

   void increment()
   {}
};

template<class T, class NodeDType, int Tag>
struct derivation_value_traits
{
   public:
   typedef typename NodeDType::node_traits            node_traits;
   typedef T                                          value_type;

   enum { type = NodeDType::type };

   private:
   typedef typename detail::pointer_to_other
      <typename node_traits::void_pointer, T>::type   pointer;
   typedef NodeDType                                  derivation_hook;
   typedef typename node_traits::node                 node;
   typedef typename detail::pointer_to_other
      <typename node_traits::void_pointer
      ,node>::type                                    node_ptr;
   typedef typename detail::pointer_to_other
      <node_ptr, const node>::type                    const_node_ptr;
   typedef typename detail::pointer_to_other
      <pointer, const T>::type                        const_pointer;

   public:  
   static node_ptr to_node_ptr(value_type& v)
   { return static_cast<derivation_hook &>(v).to_node_ptr(); }

   static const_node_ptr to_node_ptr(const value_type& v)
   { return static_cast<const derivation_hook &>(v).to_node_ptr(); }

   static pointer to_value_ptr(node_ptr n) 
   { 
      using boost::get_pointer;
      return static_cast<T*>(get_pointer(derivation_hook::to_hook(n))); 
   }

   static const_pointer to_value_ptr(const_node_ptr n)
   { 
      using boost::get_pointer;
      return static_cast<const T*>(get_pointer(derivation_hook::to_hook(n))); 
   }
};


template<class T, class NodeMType, NodeMType T::* P>
struct member_value_traits
{
   public:
   typedef typename NodeMType::node_traits            node_traits;
   typedef T                                          value_type;

   enum { type = NodeMType::type };

   private:
   typedef typename detail::pointer_to_other
      <typename node_traits::void_pointer, T>::type   pointer;
   typedef typename detail::pointer_to_other
      <pointer, const T>::type                        const_pointer;
   typedef NodeMType                                  member_hook;
   typedef typename node_traits::node                 node;
   typedef typename detail::pointer_to_other
      <typename node_traits::void_pointer
      ,node>::type                                    node_ptr;
   typedef typename detail::pointer_to_other
      <typename node_traits::void_pointer
      ,const node>::type                              const_node_ptr;

   public:
   static node_ptr to_node_ptr(value_type& v)
   {
      NodeMType* result = &(v.*P);
      return result->to_node_ptr();
   }

   static const_node_ptr to_node_ptr(const value_type& v)
   {
      const NodeMType* result = &(v.*P);
      return result->to_node_ptr();
   }

   //Now let's be nasty. The distance between the
   //start of the value_type and the internal node is
   //constant. That's why a pointer to member is
   //a compile-time value. Now apply apply it
   //with a dummy value, get the offset in bytes
   //and go backwards from n to the value subtracting
   //the needed bytes.
   static pointer to_value_ptr(node_ptr n)
   {
      using boost::get_pointer;
      return pointer(reinterpret_cast<value_type*>
         ((char*)get_pointer(n) - value_to_node_offset()));
   }

   static const_pointer to_value_ptr(const_node_ptr n)
   {
      using boost::get_pointer;
      return const_pointer(reinterpret_cast<const value_type*>
         ((const char*)get_pointer(n) - value_to_node_offset()));
   }
   private:
   static std::size_t value_to_node_offset()
   {
      using boost::get_pointer;
      const node *np = get_pointer((((const value_type *)0)->*P).to_node_ptr());
      return ((const char*)np - (const char*)(const value_type *)0);
   }
};

} //namespace detail 
} //namespace intrusive 
} //namespace boost 

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_DETAIL_UTILITIES_HPP
