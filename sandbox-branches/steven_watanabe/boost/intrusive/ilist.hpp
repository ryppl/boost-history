/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Olaf Krzikalla 2004-2007.
// (C) Copyright Ion Gaztañaga  2006.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_ILIST_HPP
#define BOOST_INTRUSIVE_ILIST_HPP

#include <boost/intrusive/detail/config_begin.hpp>
#include <boost/utility.hpp>
#include <boost/assert.hpp>
#include <boost/intrusive/ilist_hook.hpp>
#include <boost/intrusive/list_algorithms.hpp>
#include <boost/intrusive/detail/pointer_type.hpp>
#include <boost/intrusive/detail/pointer_to_other.hpp>
#include <boost/intrusive/value_traits_type.hpp>
#include <boost/get_pointer.hpp>
#include <boost/static_assert.hpp>
#include <iterator>
#include <stdexcept>
#include <functional>
#include <cstddef>
#include <boost/type_traits/is_convertible.hpp>

namespace boost {
namespace intrusive {

//! The class template ilist is an intrusive container, that mimics most of the 
//! interface of std::list as described in the C++ standard.
//!
//! The template parameter ValueTraits is called "value traits". It stores
//! information and operations about the type to be stored
//! in ilist and what type of hook has been chosen to include it in the list.
//! The value_traits class is supplied by the appropriate hook as a template subtype 
//! called "value_traits".
//!
//! If the user specifies ConstantTimeSize as "true", a member of type SizeType
//! will be embedded in the class, that will keep track of the number of stored objects.
//! This will allow constant-time O(1) size() member, instead of default O(N) size.
template< class ValueTraits
        , bool  ConstantTimeSize = false
        , class SizeType         = std::size_t>
class ilist
   :  private detail::size_holder<ConstantTimeSize, SizeType>
{
   private:

   typedef ilist<ValueTraits, ConstantTimeSize, SizeType>   this_type; 
   typedef typename ValueTraits::node_traits                node_traits;
   typedef detail::size_holder<ConstantTimeSize, SizeType>  size_traits;

   //! This class is non-copyable
   ilist (const ilist&);

   //! This class is non-asignable
   ilist &operator =(const ilist&);

   //Public typedefs
   public:

   typedef typename ValueTraits::value_type        value_type;
   typedef typename detail::pointer_to_other
      <typename node_traits::void_pointer
      ,value_type>::type                           pointer;
   typedef typename detail::pointer_to_other
      <pointer, const value_type>::type            const_pointer;
   typedef value_type&                             reference;
   typedef const value_type&                       const_reference;
   typedef typename size_traits::size_type         size_type;
   typedef typename std::iterator_traits
      <pointer>::difference_type                   difference_type;

   class iterator;
   class const_iterator;
   friend class iterator;
   friend class const_iterator;

   private:

   typedef typename node_traits::node              node;
   typedef typename detail::pointer_to_other
      <pointer, node>::type                        node_ptr;
   typedef typename detail::pointer_to_other
      <pointer, const node>::type                  const_node_ptr;
   typedef list_algorithms<node_traits>    sequence_algorithms;
   enum { safemode_or_autounlink  = 
            (int)ValueTraits::type == (int)auto_unlink_value_traits   ||
            (int)ValueTraits::type == (int)safe_mode_value_traits     };

   //Constant-time size is incompatible with auto-unlink hooks!
   BOOST_STATIC_ASSERT(!(ConstantTimeSize && ((int)ValueTraits::type == (int)auto_unlink_value_traits)));

   //This functor compares a stored value
   //and the one passed as an argument
   class equal_to_value
   {
      const value_type &t_;

      public:
      equal_to_value(const value_type &t)
         :  t_(t)
      {}

      bool operator()(const value_type &t)const
      {  return t_ == t;   }
   };

   //Const cast emulation for smart pointers
   static node_ptr uncast(const_node_ptr ptr)
   {
      using boost::get_pointer;
      return node_ptr(const_cast<node*>(get_pointer(ptr)));
   }

   //This is the root node of the circular list
   node root;

   public:

   //!The bidirectional iterator of the container
   class iterator
      :  public detail::list_iterator<value_type, iterator, node_traits>
   {
      private:
      // gcc warns about an ambiguity between iterator::value_type and
      // ilist<ValueTraits>::value_type, thus I introduce a unique name:
      typedef typename ilist<ValueTraits, ConstantTimeSize, SizeType>::value_type  private_vt;
      typedef typename ilist<ValueTraits, ConstantTimeSize, SizeType>::pointer     private_pointer;
      typedef typename ilist<ValueTraits, ConstantTimeSize, SizeType>::reference   private_reference;
      typedef detail::list_iterator<private_vt, iterator, node_traits>   inherited;

      public:
      iterator()
      {}

      private_pointer     operator->() const
      { return  ValueTraits::to_value_ptr(this->list_node()); }

      private_reference   operator*() const
      { return *ValueTraits::to_value_ptr(this->list_node()); }

      private:
      explicit iterator(node_ptr node)
         :  inherited (node)
      {}
      friend class ilist<ValueTraits, ConstantTimeSize, SizeType>;
      friend class detail::list_iterator<private_vt, iterator, node_traits>;
   };

   //!The bidirectional const_iterator of the container
   class const_iterator
      :  public detail::list_iterator<const value_type, const_iterator, node_traits>
   {
      private:
      // gcc warns about an ambiguity between const_iterator::value_type and
      // ilist<ValueTraits>::value_type, thus I introduce a unique name:
      typedef const typename ilist<ValueTraits, ConstantTimeSize, SizeType>::value_type  private_vt;
      typedef typename ilist<ValueTraits, ConstantTimeSize, SizeType>::const_pointer     private_pointer;
      typedef typename ilist<ValueTraits, ConstantTimeSize, SizeType>::const_reference   private_reference;
      typedef detail::list_iterator<private_vt, const_iterator, node_traits>   inherited;
      
      public: 
      const_iterator()
      {}

      const_iterator(const typename ilist::iterator& it)
         :  inherited (it.list_node())
      {}

      const_iterator & operator=(const typename ilist::iterator& it)
      {  return inherited::operator=(it.list_node());  }

      private_pointer   operator->() const
      { return  ValueTraits::to_value_ptr(this->list_node()); }

      private_reference operator*() const
      { return *ValueTraits::to_value_ptr(this->list_node()); }

      private:
      explicit const_iterator(const_node_ptr node)
         :  inherited (uncast(node))
      {}
      friend class ilist<ValueTraits, ConstantTimeSize, SizeType>;
      friend class detail::list_iterator<private_vt, const_iterator, node_traits>;
   };

   //!The bidirectional reverse iterator of the container
   typedef std::reverse_iterator<iterator> reverse_iterator;

   //!The bidirectional const_reverse iterator of the container
   typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

   public:

   //! <b>Effects</b>: constructs an empty list. 
   //! 
   //! <b>Complexity</b>: Constant 
   //! 
   //! <b>Throws</b>: Nothing unless the value_traits::node_traits::node
   //!   constructor throws.
   //!
   //! <b>Notes</b>: This won't throw if ilist_base_hook<>::value_traits or
   //!   ilist_member_hook::::value_traits are used as value traits.
   ilist()
   {  
      size_traits::set_size(size_type(0));
      sequence_algorithms::init(node_ptr(&root));  
   }

   //! <b>Requires</b>: Dereferencing Iterator must yield to an lvalue of type value_type.
   //! 
   //! <b>Effects</b>: Constructs a list equal to the range [first,last).
   //! 
   //! <b>Complexity</b>: Linear in first-last. No copy constructors are called.  
   //! 
   //! <b>Throws</b>: Nothing unless the value_traits::node_traits::node
   //!   constructor throws.
   //!
   //! <b>Notes</b>: This won't throw if ilist_base_hook<>::value_traits or
   //!   ilist_member_hook::::value_traits are used as value traits.
   template<class Iterator>
   ilist(Iterator b, Iterator e)
   {
      sequence_algorithms::init(node_ptr(&root));
      this->insert(this->end(), b, e);
   }

   //! <b>Effects</b>: If it's not a safe-mode or an auto-unlink value_type 
   //!   is false, the destructor does nothing
   //!   (ie. no code is generated). Otherwise it detaches all elements from this. 
   //!   In this case the objects in the list are not deleted (i.e. no destructors 
   //!   are called), but the hooks according to the ValueTraits template parameter
   //!   are set to their default value.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements in the list, if 
   //!   it's a safe-mode or auto-unlink value . Otherwise constant. 
   ~ilist() 
   {
      if(safemode_or_autounlink){
         this->clear(); 
      }
   }

   //! <b>Requires</b>: v must be an lvalue.
   //! 
   //! <b>Effects</b>: Inserts the value in the back of the list.
   //!   No copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   void push_back(value_type& v) 
   {
      node_ptr to_insert = ValueTraits::to_node_ptr(v);
      if(safemode_or_autounlink)
         BOOST_ASSERT(sequence_algorithms::unique(to_insert));
      sequence_algorithms::link_before(node_ptr(&root), to_insert);
      size_traits::increment();
   }

   //! <b>Requires</b>: v must be an lvalue.
   //! 
   //! <b>Effects</b>: Inserts the value in the front of the list.
   //!   No copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   void push_front(value_type& v) 
   {
      node_ptr to_insert = ValueTraits::to_node_ptr(v);
      if(safemode_or_autounlink)
         BOOST_ASSERT(sequence_algorithms::unique(to_insert));
      sequence_algorithms::link_before(node_traits::get_next(const_node_ptr(&root)), to_insert); 
      size_traits::increment();
   }

   //! <b>Effects</b>: Erases the last element of the list.
   //!   No destructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the erased element.
   void pop_back() 
   {
      node_ptr to_erase = node_traits::get_previous(const_node_ptr(&root));
      sequence_algorithms::unlink(to_erase);
      size_traits::decrement();
      if(safemode_or_autounlink)
         sequence_algorithms::init(to_erase);
   }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the last element of the list.
   //!   No destructors are called.
   //!   Destroyer::operator()(pointer) is called for the removed element.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Invalidates the iterators to the erased element.
   template<class Destroyer>
   void pop_back(Destroyer destroyer)
   {
      node_ptr to_erase = node_traits::get_previous(const_node_ptr(&root));
      sequence_algorithms::unlink(to_erase);
      size_traits::decrement();
      if(safemode_or_autounlink)
         sequence_algorithms::init(to_erase);
      destroyer(ValueTraits::to_value_ptr(to_erase));
   }

   //! <b>Effects</b>: Erases the first element of the list.
   //!   No destructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the erased element.
   void pop_front() 
   { 
      node_ptr to_erase = node_traits::get_next(const_node_ptr(&root));
      sequence_algorithms::unlink(to_erase);
      size_traits::decrement();
      if(safemode_or_autounlink)
         sequence_algorithms::init(to_erase);
   }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the first element of the list.
   //!   No destructors are called.
   //!   Destroyer::operator()(pointer) is called for the removed element.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Invalidates the iterators to the erased element.
   template<class Destroyer>
   void pop_front(Destroyer destroyer)
   { 
      node_ptr to_erase = node_traits::get_next(const_node_ptr(&root));
      sequence_algorithms::unlink(to_erase);
      size_traits::decrement();
      if(safemode_or_autounlink)
         sequence_algorithms::init(to_erase);
      destroyer(ValueTraits::to_value_ptr(to_erase));
   }

   //! <b>Effects</b>: Returns a reference to the first element of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   reference front() 
   { return *ValueTraits::to_value_ptr(node_traits::get_next(const_node_ptr(&root))); }

   //! <b>Effects</b>: Returns a const_reference to the first element of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_reference front() const 
   { return *ValueTraits::to_value_ptr(uncast(node_traits::get_next(const_node_ptr(&root)))); }

   //! <b>Effects</b>: Returns a reference to the last element of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   reference back() 
   { return *ValueTraits::to_value_ptr(node_traits::get_previous(const_node_ptr(&root))); }

   //! <b>Effects</b>: Returns a const_reference to the last element of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_reference back() const 
   { return *ValueTraits::to_value_ptr(uncast(node_traits::get_previous(const_node_ptr(&root)))); }

   //! <b>Effects</b>: Returns an iterator to the first element contained in the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   iterator begin() 
   { return iterator(node_traits::get_next(const_node_ptr(&root))); }

   //! <b>Effects</b>: Returns a const_iterator to the first element contained in the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_iterator begin() const 
   { return const_iterator(node_traits::get_next(const_node_ptr(&root))); }

   //! <b>Effects</b>: Returns an iterator to the end of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   iterator end() 
   { return iterator(node_ptr(&root)); }

   //! <b>Effects</b>: Returns a const_iterator to the end of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_iterator end() const 
   { return const_iterator(const_node_ptr(&root)); }

   //! <b>Effects</b>: Returns a reverse_iterator pointing to the beginning 
   //! of the reversed list. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   reverse_iterator rbegin()
   { return reverse_iterator(end()); }

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the beginning 
   //! of the reversed list. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_reverse_iterator rbegin() const 
   { return const_reverse_iterator(end()); }

   //! <b>Effects</b>: Returns a reverse_iterator pointing to the end
   //! of the reversed list. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   reverse_iterator rend()	
   { return reverse_iterator(begin()); }

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the end
   //! of the reversed list. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_reverse_iterator rend() const	
   { return const_reverse_iterator(begin()); }

   //! <b>Effects</b>: Returns the number of the elements contained in the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements contained in the list.
   //!   if ConstantTimeSize is false. Constant time otherwise.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   size_type size() const
   {
      if(ConstantTimeSize)
         return size_traits::get_size();
      else
         return sequence_algorithms::count(const_node_ptr(&root)) - 1; 
   }

   //! <b>Effects</b>: Returns true if the list contains no elements.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   bool empty() const
   { return sequence_algorithms::unique(const_node_ptr(&root)); }

   //! <b>Effects</b>: Swaps the elements of x and *this.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   void swap(ilist& other)
   {
      sequence_algorithms::swap_nodes(node_ptr(&root), node_ptr(&other.root)); 
      if(ConstantTimeSize){
         size_type backup = size_traits::get_size();
         size_traits::set_size(other.get_size());
         other.set_size(backup);
      }
   }

   //! <b>Effects</b>: Erases the element pointed by i of the list.
   //!   No destructors are called.
   //!
   //! <b>Returns</b>: the first element remaining beyond the removed element,
   //!   or end() if no such element exists.
   //!
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the
   //!   erased element.
   iterator erase(iterator i)
   {
      iterator erase = i;
      ++i;
      node_ptr to_erase = erase.list_node();
      sequence_algorithms::unlink(to_erase);
      size_traits::decrement();
      if(safemode_or_autounlink)
         sequence_algorithms::init(to_erase);
      return i;
   }

   //! <b>Requires</b>: first and last must be valid iterator to elements in *this.
   //!
   //! <b>Effects</b>: Erases the element range pointed by b and e
   //! No destructors are called.
   //!
   //! <b>Returns</b>: the first element remaining beyond the removed elements,
   //!   or end() if no such element exists.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements erased if it's a safe-mode
   //!   or auto-unlink value. Constant time otherwise.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the 
   //!   erased elements.
   iterator erase(iterator b, iterator e)
   {
      if(safemode_or_autounlink || ConstantTimeSize){
         while(b != e){
            b = this->erase(b);
         }
         return b;
      }
      else{
         sequence_algorithms::unlink(b.list_node(), e.list_node());
         return e;
      }
   }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the element pointed by i of the list.
   //!   No destructors are called.
   //!   Destroyer::operator()(pointer) is called for the removed element.
   //!
   //! <b>Returns</b>: the first element remaining beyond the removed element,
   //!   or end() if no such element exists.
   //!
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Invalidates the iterators to the erased element.
   template <class Destroyer>
   iterator erase(iterator i, Destroyer destroyer)
   {
      iterator erase = i;
      ++i;
      node_ptr to_erase = erase.list_node();
      sequence_algorithms::unlink(to_erase);
      size_traits::decrement();
      if(safemode_or_autounlink)
         sequence_algorithms::init(to_erase);
      destroyer(ValueTraits::to_value_ptr(to_erase));
      return i;
   }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the element range pointed by b and e
   //!   No destructors are called.
   //!   Destroyer::operator()(pointer) is called for the removed elements.
   //!
   //! <b>Returns</b>: the first element remaining beyond the removed elements,
   //!   or end() if no such element exists.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements erased.
   //! 
   //! <b>Note</b>: Invalidates the iterators to the erased elements.
   template <class Destroyer>
   iterator erase(iterator b, iterator e, Destroyer destroyer)
   {
      while(b != e){
         b = this->erase(b, destroyer);
      }
      return b;
   }

   //! <b>Effects</b>: Erases the element range pointed by b and e
   //!   No destructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements of the list.
   //!   if it's a safe-mode or auto-unlink value_type. Constant time otherwise.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the erased elements.
   void clear()
   {
      if(safemode_or_autounlink){
         this->erase(this->begin(), this->end()); 
      }
      else{
         sequence_algorithms::init(node_ptr(&root));
         size_traits::set_size(size_type(0));
      }
   }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the element range pointed by b and e
   //!   No destructors are called.
   //!   Destroyer::operator()(pointer) is called for the removed elements.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements of the list.
   //! 
   //! <b>Note</b>: Invalidates the iterators to the erased elements.
   template <class Destroyer>
   void clear(Destroyer destroyer)
   {  this->erase(this->begin(), this->end(), destroyer);   }

   //! <b>Requires</b>: v must be an lvalue and p must point to an element contained
   //!   by the list.
   //!
   //! <b>Effects</b>: Inserts the value before the position pointed by p.
   //!
   //! <b>Returns</b>: An iterator to the inserted element.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant time. No copy constructors are called.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   iterator insert(iterator p, value_type& v)
   {
      node_ptr to_insert = ValueTraits::to_node_ptr(v);
      if(safemode_or_autounlink)
         BOOST_ASSERT(sequence_algorithms::unique(to_insert));
      sequence_algorithms::link_before(p.list_node(), to_insert);
      size_traits::increment();
      return iterator(to_insert);
   }

   //! <b>Requires</b>: Dereferencing Iterator must yield to 
   //!   an lvalue of type value_type and p must point to an element contained
   //!   by the list.
   //! 
   //! <b>Effects</b>: Inserts the range pointed by b and e before the position p.
   //!   No copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements inserted.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   template<class Iterator>
   void insert(iterator p, Iterator b, Iterator e)
   {
      for (; b != e; ++b)
         this->insert(p, *b);
   }

   //! <b>Requires</b>: Dereferencing Iterator must yield to 
   //!   an lvalue of type value_type.
   //! 
   //! <b>Effects</b>: Clears the list and inserts the range pointed by b and e.
   //!   No destructors or copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements inserted plus
   //!   linear to the elements contained in the list if it's a safe-mode
   //!   or auto-unlink value.
   //!   Linear to the number of elements inserted in the list otherwise.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!   to the erased elements.
   template<class Iterator>
   void assign(Iterator b, Iterator e)
   {
      this->clear();
      this->insert(this->end(), b, e);
   }

   //! <b>Requires</b>: p must point to an element contained
   //!   by the list.
   //!
   //! <b>Effects</b>: Transfers all the elements of list x to this list, before the
   //!   the element pointed by p. No destructors or copy constructors are called.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Iterators of values obtained from list x now point to elements of
   //!    this list. Iterators of this list and all the references are not invalidated.
   void splice(iterator p, ilist& x)
   {
      if(!x.empty()){
         sequence_algorithms::transfer
            (p.list_node(), x.begin().list_node(), x.end().list_node());
         size_traits::set_size(size_traits::get_size() + x.get_size());
         x.set_size(size_type(0));
      }
   }

   //! <b>Requires</b>: p must point to an element contained
   //!   by this list. new_ele must point to an element contained in list x.
   //! 
   //! <b>Effects</b>: Transfers the value pointed by new_ele, from list x to this list, 
   //!   before the the element pointed by p. No destructors or copy constructors are called.
   //!   If p == new_ele or p == ++new_ele, this function is a null operation. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Iterators of values obtained from list x now point to elements of this
   //!   list. Iterators of this list and all the references are not invalidated.
   void splice(iterator p, ilist&x, iterator new_ele)
   {
      sequence_algorithms::transfer(p.list_node(), new_ele.list_node());
      x.decrement();
      size_traits::increment();
   }

   //! <b>Requires</b>: p must point to an element contained
   //!   by this list. start and end must point to elements contained in list x.
   //! 
   //! <b>Effects</b>: Transfers the range pointed by start and end from list x to this list, 
   //!   before the the element pointed by p. No destructors or copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements transferred
   //!   if ConstantTimeSize is true. Constant-time otherwise.
   //! 
   //! <b>Note</b>: Iterators of values obtained from list x now point to elements of this
   //!   list. Iterators of this list and all the references are not invalidated.
   void splice(iterator p, ilist&x, iterator start, iterator end)
   {
      if(start != end){
         if(ConstantTimeSize){
            size_type increment = std::distance(start, end);
            sequence_algorithms::transfer(p.list_node(), start.list_node(), end.list_node());
            size_traits::set_size(size_traits::get_size() + increment);
            x.set_size(x.get_size() - increment);
         }
         else{
            sequence_algorithms::transfer(p.list_node(), start.list_node(), end.list_node());
         }
      }
   }

   //! <b>Requires</b>: p must point to an element contained
   //!   by this list. start and end must point to elements contained in list x.
   //!   n == std::distance(start, end)
   //! 
   //! <b>Effects</b>: Transfers the range pointed by start and end from list x to this list, 
   //!   before the the element pointed by p. No destructors or copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Iterators of values obtained from list x now point to elements of this
   //!   list. Iterators of this list and all the references are not invalidated.
   void splice(iterator p, ilist&x, iterator start, iterator end, difference_type n)
   {
      if(n){
         if(ConstantTimeSize){
            BOOST_ASSERT(n == std::distance(start, end));
            sequence_algorithms::transfer(p.list_node(), start.list_node(), end.list_node());
            size_traits::set_size(size_traits::get_size() + n);
            x.set_size(x.get_size() - n);
         }
         else{
            sequence_algorithms::transfer(p.list_node(), start.list_node(), end.list_node());
         }
      }
   }

   //! <b>Effects</b>: This function sorts the list *this according to std::less<value_type>. 
   //!   The sort is stable, that is, the relative order of equivalent elements is preserved.
   //! 
   //! <b>Throws</b>: Nothing unless the value_traits::node_traits::node
   //!   constructor throws. 
   //!
   //! <b>Notes</b>: This won't throw if ilist_base_hook<>::value_traits or
   //!   ilist_member_hook::::value_traits are used as value traits.
   //!   Iterators and references are not invalidated.
   //! 
   //! <b>Complexity</b>: The number of comparisons is approximately N log N, where N
   //!   is the list's size.
   void sort() 
   {  sort(std::less<value_type>());  }

   //! <b>Requires</b>: p must be a comparison function that induces a strict weak ordering
   //! 
   //! <b>Effects</b>: This function sorts the list *this according to p. The sort is 
   //!   stable, that is, the relative order of equivalent elements is preserved.
   //! 
   //! <b>Throws</b>: Nothing unless the value_traits::node_traits::node
   //!   constructor throws. 
   //!
   //! <b>Notes</b>: This won't throw if ilist_base_hook<>::value_traits or
   //!   ilist_member_hook::::value_traits are used as value traits.
   //!   Iterators and references are not invalidated.
   //! 
   //! <b>Complexity</b>: The number of comparisons is approximately N log N, where N
   //!   is the list's size.
   template<class Predicate>
   void sort(Predicate p)
   {
      if(node_traits::get_next(const_node_ptr(&root)) 
         != node_traits::get_previous(const_node_ptr(&root))){
         ilist carry;
         ilist counter[64];
         int fill = 0;
         while(!this->empty()){
            carry.splice(carry.begin(), *this, this->begin());
            int i = 0;
            while(i < fill && !counter[i].empty()) {
               carry.merge(counter[i++], p);
            }
            carry.swap(counter[i]);
            if(i == fill)
               ++fill;
         }
         for (int i = 1; i < fill; ++i)
            counter[i].merge(counter[i-1], p);
         this->swap(counter[fill-1]);
      }
   }

   //! <b>Effects</b>: This function removes all of x's elements and inserts them
   //!   in order into *this according to std::less<value_type>. The merge is stable; 
   //!   that is, if an element from *this is equivalent to one from x, then the element 
   //!   from *this will precede the one from x. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: This function is linear time: it performs at most
   //!   size() + x.size() - 1 comparisons.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated
   void merge(ilist& x)
   { merge(x, std::less<value_type>()); }

   //! <b>Requires</b>: p must be a comparison function that induces a strict weak
   //!   ordering and both *this and x must be sorted according to that ordering
   //!   The lists x and *this must be distinct. 
   //! 
   //! <b>Effects</b>: This function removes all of x's elements and inserts them
   //!   in order into *this. The merge is stable; that is, if an element from *this is 
   //!   equivalent to one from x, then the element from *this will precede the one from x. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: This function is linear time: it performs at most
   //!   size() + x.size() - 1 comparisons.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated.
   template<class Predicate>
   void merge(ilist& x, Predicate p)
   {
      iterator e = this->end();
      iterator bx = x.begin();
      iterator ex = x.end();

      for (iterator b = this->begin(); b != e; ++b) {
         size_type n(0);
         iterator ix(bx);
         while(ix != ex && p(*ix, *b)){
            ++ix; ++n;
         }
         this->splice(b, x, bx, ix, n);
         bx = ix;
      }
      //Now transfer the rest at the end of the container
      this->splice(e, x);
   }

   //! <b>Effects</b>: Reverses the order of elements in the list. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: This function is linear time.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated
   void reverse()
   {  sequence_algorithms::reverse(node_ptr(&root));   }

   //! <b>Effects</b>: Removes all the elements that compare equal to val.
   //!   No destructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear time. It performs exactly size() comparisons for equality.
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   void remove(const value_type& v)
   {  remove_if(equal_to_value(v));  }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Removes all the elements that compare equal to val.
   //!   Destroyer::operator()(pointer) is called for every removed element.
   //!
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear time. It performs exactly size() comparisons for equality.
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   template<class Destroyer>
   void remove(const value_type& v, Destroyer destroyer)
   {  remove_if(equal_to_value(v), destroyer);  }

   //! <b>Effects</b>: Removes all the elements for which a specified
   //!   predicate is satisfied. No destructors are called.
   //! 
   //! <b>Throws</b>: If pred throws.
   //! 
   //! <b>Complexity</b>: Linear time. It performs exactly size() calls to the predicate.
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   template<class Pred>
   void remove_if(Pred pred)
   {  remove_if(pred, detail::null_destroyer());   }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Removes all the elements for which a specified
   //!   predicate is satisfied.
   //!   Destroyer::operator()(pointer) is called for every removed element.
   //!
   //! <b>Throws</b>: If pred throws.
   //! 
   //! <b>Complexity</b>: Linear time. It performs exactly size() comparisons for equality.
   //!
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   template<class Pred, class Destroyer>
   void remove_if(Pred pred, Destroyer destroyer)
   {
      iterator first = begin();
      iterator last = end();
      while(first != last) {
         iterator next = first;
         ++next;
         if(pred(*first)){
            pointer p = first.operator->();
            this->erase(first);
            destroyer(p);
         }
         first = next;
      }
   }

   //! <b>Effects</b>: Removes adjacent duplicate elements or adjacent 
   //!   elements that are equal from the list. No destructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear time (size()-1 comparisons calls to pred()).
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   void unique()
   {  unique(std::equal_to<value_type>(), detail::null_destroyer());  }

   //! <b>Effects</b>: Removes adjacent duplicate elements or adjacent 
   //!   elements that satisfy some binary predicate from the list.
   //!   No destructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear time (size()-1 comparisons equality comparisons).
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   template<class BinaryPredicate>
   void unique(BinaryPredicate pred)
   {  unique(pred, detail::null_destroyer());  }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Removes adjacent duplicate elements or adjacent 
   //!   elements that satisfy some binary predicate from the list.
   //!   Destroyer::operator()(pointer) is called for every removed element.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear time (size()-1) comparisons equality comparisons.
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   template<class BinaryPredicate, class Destroyer>
   void unique(BinaryPredicate pred, Destroyer destroyer)
   {
		if(!this->empty()){
			iterator first = begin();
			iterator after = first;
         ++after;
			while(after != this->end()){
            if(pred(*first, *after)){
               pointer p = after.operator->();
               after = erase(after);
               destroyer(p);
            }
				else{
					first = after++;
            }
         }
      }
   }

   //! <b>Requires</b>: v must be a reference to a value inserted in a list.
   //! 
   //! <b>Effects</b>: This function returns a const_iterator pointing to the element
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant time.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated.
   static iterator current(value_type& v)
   { 
      BOOST_ASSERT(!sequence_algorithms::unique(ValueTraits::to_node_ptr(v)));
      return iterator(ValueTraits::to_node_ptr(v)); 
   }

   //! <b>Requires</b>: v must be a const reference to a value inserted in a list.
   //! 
   //! <b>Effects</b>: This function returns an iterator pointing to the element.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant time.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated.
   static const_iterator current(const value_type& v) 
   { 
      BOOST_ASSERT(!sequence_algorithms::unique(ValueTraits::to_node_ptr(const_cast<value_type&> (v))));
      return const_iterator(ValueTraits::to_node_ptr(const_cast<value_type&> (v))); 
   }
/*
   //! <b>Requires</b>: v shall not be in a list of the appropriate type.
   //! 
   //! <b>Effects</b>: init_node post-constructs the node data in x used by lists of 
   //! the appropriate type. For the accessors ilist_derived_node and ilist_member_node 
   //! init_node has no effect, since the constructors of ilist_base_hook and ilist_member_hook 
   //! have already initialized the node data. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant time.
   //! 
   //! <b>Note</b>: This function is meant to be used mainly with the member value_traits, 
   //! where no implicit node initialization during construction occurs.
   static void init_node(reference v)
   { sequence_algorithms::init(node_ptr(&*ValueTraits::to_node_ptr(v))); }

   static void swap_nodes(reference v1, reference v2)
   { sequence_algorithms::swap_nodes(ValueTraits::to_node_ptr(v1), ValueTraits::to_node_ptr(v2)); }
*/
   //! <b>Effects</b>: removes x from a list of the appropriate type. It has no effect,
   //!   if x is not in such a list. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant time.
   //! 
   //! <b>Note</b>: This static function is only usable with the "safe mode"
   //!   hook and non-constant time size lists. Otherwise, the user must use
   //!   the non-static "erase(value_type &)" member. If the user calls
   //!   this function with a non "safe mode" or constant time size list
   //!   a compilation error will be issued.
   template<class T>
   static void remove_node(T& v)
   {
      //This function is only usable for safe mode hooks and non-constant
      //time lists. 
      //BOOST_STATIC_ASSERT((safemode_or_autounlink));
      BOOST_STATIC_ASSERT((!ConstantTimeSize));
      BOOST_STATIC_ASSERT((boost::is_convertible<T, value_type>::value));
      node_ptr to_remove(ValueTraits::to_node_ptr(v));
      sequence_algorithms::unlink(to_remove);
      if(safemode_or_autounlink)
         sequence_algorithms::init(to_remove);
   }

};

} //namespace intrusive 
} //namespace boost 

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_ILIST_HPP
