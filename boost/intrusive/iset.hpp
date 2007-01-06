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
#ifndef BOOST_INTRUSIVE_ISET_HPP
#define BOOST_INTRUSIVE_ISET_HPP

#include <boost/intrusive/detail/config_begin.hpp>
#include <boost/intrusive/detail/irbtree.hpp>

namespace boost {
namespace intrusive {

//! The class template iset is an intrusive container, that mimics most of 
//! the interface of std::set as described in the C++ standard.
//! 
//! The template parameter ValueTraits is called "value traits". It stores
//! information and operations about the type to be stored
//! in ilist and what type of hook has been chosen to include it in the list.
//! The value_traits class is supplied by the appropriate hook as a template subtype 
//! called "value_traits".
//!
//! The template parameter Compare, provides a function object that can compare two 
//!   element values as sort keys to determine their relative order in the set. 
//!
//! If the user specifies ConstantTimeSize as "true", a member of type SizeType
//! will be embedded in the class, that will keep track of the number of stored objects.
//! This will allow constant-time O(1) size() member, instead of default O(N) size.
template<class ValueTraits
        ,class Compare = std::less<typename ValueTraits::value_type>
        ,bool ConstantTimeSize = false, class SizeType = std::size_t>
class iset
{
   typedef detail::irbtree<ValueTraits, Compare, ConstantTimeSize, SizeType> tree_type;

   //!This class is non-copyable
   iset (const iset&);

   //!This class is non-assignable
   iset operator =(const iset&);

   public:
   typedef typename tree_type::value_type             value_type;
   typedef typename tree_type::pointer                pointer;
   typedef typename tree_type::const_pointer          const_pointer;
   typedef typename tree_type::reference              reference;
   typedef typename tree_type::const_reference        const_reference;
   typedef typename tree_type::size_type              size_type;
   typedef typename tree_type::difference_type        difference_type;
   typedef value_type                                 key_type;
   typedef typename tree_type::value_compare          value_compare;
   typedef value_compare                              key_compare;

   typedef typename tree_type::iterator               iterator;
   typedef typename tree_type::const_iterator         const_iterator;
   typedef typename tree_type::reverse_iterator       reverse_iterator;
   typedef typename tree_type::const_reverse_iterator const_reverse_iterator;
   typedef typename tree_type::insert_commit_data     insert_commit_data;

   private:
   tree_type tree_;

   public:

   //! <b>Effects</b>: Constructs an empty set. 
   //!   
   //! <b>Complexity</b>: Constant. 
   //! 
   //! <b>Throws</b>: Nothing unless the copy constructor of the Compare object throws. 
   iset(Compare cmp = Compare()) 
      :  tree_(cmp)
   {}

   //! <b>Requires</b>: Dereferencing Iterator must yield to an lvalue of type value_type. 
   //!   cmp must be a comparison function that induces a strict weak ordering.
   //! 
   //! <b>Effects</b>: Constructs an empty set and inserts elements from 
   //!   the range [first, last).
   //! 
   //! <b>Complexity</b>: Linear in N if the range [first, last) is already sorted using 
   //!   comp and otherwise N * log N, where N is last ­ first. Only the copy constructor 
   //!   of the Compare object is called. 
   //! 
   //! <b>Throws</b>: Nothing unless the copy constructor of the Compare object throws. 
   template<class Iterator>
   iset(Iterator b, Iterator e, Compare cmp = Compare())
      : tree_(true, b, e, cmp)
   {  insert(b, e);  }

   //! <b>Effects</b>: Detaches all elements from this. The objects in the set 
   //!   are not deleted (i.e. no destructors are called).
   //! 
   //! <b>Complexity</b>: O(log(size()) + size()) if it's a safe-mode or auto-unlink
   //!   value. Otherwise constant.
   //! 
   //! <b>Throws</b>: Nothing.
   ~iset() 
   {}

   //! <b>Effects</b>: Returns an iterator pointing to the beginning of the set.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   iterator begin()
   { return tree_.begin();  }

   //! <b>Effects</b>: Returns a const_iterator pointing to the beginning of the set.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   const_iterator begin() const
   { return tree_.begin();  }

   //! <b>Effects</b>: Returns an iterator pointing to the end of the set.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   iterator end()
   { return tree_.end();  }

   //! <b>Effects</b>: Returns a const_iterator pointing to the end of the set.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   const_iterator end() const
   { return tree_.end();  }

   //! <b>Effects</b>: Returns a reverse_iterator pointing to the beginning of the
   //!    reversed set.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   reverse_iterator rbegin()
   { return tree_.rbegin();  }

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the beginning
   //!    of the reversed set.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   const_reverse_iterator rbegin() const
   { return tree_.rbegin();  }

   //! <b>Effects</b>: Returns a reverse_iterator pointing to the end
   //!    of the reversed set.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   reverse_iterator rend()
   { return tree_.rend();  }

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the end
   //!    of the reversed set.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   const_reverse_iterator rend() const
   { return tree_.rend();  }

   //! <b>Effects</b>: Returns the key_compare object used by the set.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: If key_compare copy-constructor throws.
   key_compare key_comp() const
   { return tree_.value_comp(); }

   //! <b>Effects</b>: Returns the value_compare object used by the set.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: If value_compare copy-constructor throws.
   value_compare value_comp() const
   { return tree_.value_comp(); }

   //! <b>Effects</b>: Returns true is the container is empty.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   bool empty() const
   { return tree_.empty(); }

   //! <b>Effects</b>: Returns the number of elements stored in the set.
   //! 
   //! <b>Complexity</b>: Linear to elements contained in *this if,
   //!   ConstantTimeSize is false. Constant-time otherwise.
   //! 
   //! <b>Throws</b>: Nothing.
   size_type size() const
   { return tree_.size(); }

   //! <b>Effects</b>: Swaps the contents of two sets.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   void swap(iset& other)
   { tree_.swap(other.tree_); }

   //! <b>Requires</b>: val must be an lvalue
   //! 
   //! <b>Effects</b>: Tries to inserts val into the set.
   //!
   //! <b>Returns</b>: If the value
   //!   is not already present inserts it and returns a pair containing the
   //!   iterator to the new value and true. If the value is already present
   //!   returns a pair containing an iterator to the already present value
   //!   and false.
   //! 
   //! <b>Complexity</b>: Average complexity for insert element is at
   //!   most logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   //!   No copy-constructors are called.
   std::pair<iterator, bool> insert(value_type& val)
   {  return tree_.insert_unique(val);  }

   //! <b>Requires</b>: val must be an lvalue
   //! 
   //! <b>Effects</b>: Tries to to insert x into the set, using "hint" 
   //!   as a hint to where it will be inserted.
   //!
   //! <b>Returns</b>: An iterator that points to the position where the 
   //!   new element was inserted into the set.
   //! 
   //! <b>Complexity</b>: Logarithmic in general, but it's amortized
   //!   constant time if t is inserted immediately before hint.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   //!   No copy-constructors are called.
   iterator insert(const_iterator hint, value_type& val)
   {  return tree_.insert_unique(hint, val);  }

   //! <b>Requires</b>: key_value_comp must be a comparison function that induces 
   //!   the same strict weak ordering as value_compare. The difference is that
   //!   key_value_comp compares an arbitrary key with the contained values.
   //! 
   //! <b>Effects</b>: Checks if a value can be inserted in the set, using
   //!   a user provided key instead of the value itself.
   //!
   //! <b>Returns</b>: If an equivalent value is already present
   //!   returns a pair containing an iterator to the already present value
   //!   and false. If the value can be inserted returns true in the returned
   //!   pair boolean and fills "commit_data" that is meant to be used with
   //!   the "insert_commit" function.
   //! 
   //! <b>Complexity</b>: Average complexity is at most logarithmic.
   //!
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Notes</b>: This function is used to improve performance when constructing
   //!   a value_type is expensive: if an equivalent value is already present
   //!   the constructed object must be discarded. Many times, the part of the
   //!   node that is used to impose the order is much cheaper to construct
   //!   than the value_type and this function offers the possibility to use that 
   //!   part to check if the insertion will be successful.
   //!
   //!   If the check is successful, the user can construct the value_type and use
   //!   "insert_commit" to insert the object in constant-time. This gives a total
   //!   logarithmic complexity to the insertion: check(O(log(N)) + commit(O(1)).
   //!
   //!   "commit_data" remains valid for a subsequent "insert_commit" only if no more
   //!   objects are inserted or erased from the set.
   template<class KeyType, class KeyValueCompare>
   std::pair<iterator, bool> insert_check
      (const KeyType &key, const KeyValueCompare &key_value_comp, insert_commit_data &commit_data)
   {  return tree_.insert_unique_check(key, key_value_comp, commit_data); }

   //! <b>Requires</b>: key_value_comp must be a comparison function that induces 
   //!   the same strict weak ordering as value_compare. The difference is that
   //!   key_value_comp compares an arbitrary key with the contained values.
   //! 
   //! <b>Effects</b>: Checks if a value can be inserted in the set, using
   //!   a user provided key instead of the value itself, using "hint" 
   //!   as a hint to where it will be inserted.
   //!
   //! <b>Returns</b>: If an equivalent value is already present
   //!   returns a pair containing an iterator to the already present value
   //!   and false. If the value can be inserted returns true in the returned
   //!   pair boolean and fills "commit_data" that is meant to be used with
   //!   the "insert_commit" function.
   //! 
   //! <b>Complexity</b>: Logarithmic in general, but it's amortized
   //!   constant time if t is inserted immediately before hint.
   //!
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Notes</b>: This function is used to improve performance when constructing
   //!   a value_type is expensive: if the equivalent is already present
   //!   the constructed object must be discarded. Many times, the part of the
   //!   constructing that is used to impose the order is much cheaper to construct
   //!   than the value_type and this function offers the possibility to use that key 
   //!   to check if the insertion will be successful.
   //!
   //!   If the check is successful, the user can construct the value_type and use
   //!   "insert_commit" to insert the object in constant-time. This can give a total
   //!   constant-time complexity to the insertion: check(O(1)) + commit(O(1)).
   //!   
   //!   "commit_data" remains valid for a subsequent "insert_commit" only if no more
   //!   objects are inserted or erase from the set.
   template<class KeyType, class KeyValueCompare>
   std::pair<iterator, bool> insert_check
      (const_iterator hint, const KeyType &key
      ,const KeyValueCompare &key_value_comp, insert_commit_data &commit_data)
   {  return tree_.insert_unique_check(hint, key, key_value_comp, commit_data); }

   //! <b>Requires</b>: value must be an lvalue of type value_type. commit_data
   //!   must have been obtained from a previous call to "insert_check".
   //!   No objects should have been inserted or erased from the set between
   //!   the "insert_check" that filled "commit_data" and the call to "insert_commit".
   //! 
   //! <b>Effects</b>: Inserts the value in the set using the information obtained
   //!   from the "commit_data" that a previous "insert_check" filled.
   //!
   //! <b>Returns</b>: An iterator to the newly inserted object.
   //! 
   //! <b>Complexity</b>: Constant time.
   //!
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Notes</b>: This function has only sense if a "insert_check" has been
   //!   previously executed to fill "commit_data". No value should be inserted or
   //!   erased between the "insert_check" and "insert_commit" calls.
   iterator insert_commit(value_type &val, insert_commit_data &commit_data)
   {  return tree_.insert_unique_commit(val, commit_data); }

   //! <b>Requires</b>: Dereferencing Iterator must yield to an lvalue 
   //!   of type value_type.
   //! 
   //! <b>Effects</b>: Inserts a range into the set.
   //! 
   //! <b>Complexity</b>: Insert range is in general O(N * log(N)), where N is the 
   //!   size of the range. However, it is linear in N if the range is already sorted 
   //!   by value_comp().
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   //!   No copy-constructors are called.
   template<class Iterator>
   void insert(Iterator b, Iterator e)
   {  tree_.insert_unique(b, e);  }

   //! <b>Effects</b>: Erases the element pointed to by pos. 
   //! 
   //! <b>Complexity</b>: Average complexity is constant time. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   iterator erase(iterator i)
   {  return tree_.erase(i);  }

   //! <b>Effects</b>: Erases the range pointed to by b end e. 
   //! 
   //! <b>Complexity</b>: Average complexity for erase range is at most 
   //!   O(log(size()) + N), where N is the number of elements in the range.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   iterator erase(iterator b, iterator e)
   {  return tree_.erase(b, e);  }

   //! <b>Effects</b>: Erases all the elements with the given value.
   //! 
   //! <b>Returns</b>: The number of erased elements.
   //! 
   //! <b>Complexity</b>: O(log(size()) + N.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   size_type erase(const value_type &value)
   {  return tree_.erase(value);  }

   //! <b>Effects</b>: Erases all the elements that compare equal with
   //!   the given key and the given comparison functor.
   //! 
   //! <b>Returns</b>: The number of erased elements.
   //! 
   //! <b>Complexity</b>: O(log(size()) + N.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   template<class KeyType, class KeyValueCompare>
   size_type erase(const KeyType& key, KeyValueCompare comp)
   {  return tree_.erase(key, comp);  }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the element pointed to by pos. 
   //!   Destroyer::operator()(pointer) is called for the removed element.
   //! 
   //! <b>Complexity</b>: Average complexity for erase element is constant time. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators 
   //!    to the erased elements.
   template<class Destroyer>
   iterator erase(iterator i, Destroyer destroyer)
   {  return tree_.erase(i, destroyer);  }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the range pointed to by b end e.
   //!   Destroyer::operator()(pointer) is called for the removed element.
   //! 
   //! <b>Complexity</b>: Average complexity for erase range is at most 
   //!   O(log(size()) + N), where N is the number of elements in the range.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators
   //!    to the erased elements.
   template<class Destroyer>
   iterator erase(iterator b, iterator e, Destroyer destroyer)
   {  return tree_.erase(b, e, destroyer);  }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases all the elements with the given value.
   //!   Destroyer::operator()(pointer) is called for the removed elements.
   //! 
   //! <b>Returns</b>: The number of erased elements.
   //! 
   //! <b>Complexity</b>: O(log(size()) + N.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   template<class Destroyer>
   size_type erase(const value_type &value, Destroyer destroyer)
   {  return tree_.erase(value, destroyer);  }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases all the elements with the given key.
   //!   according to the comparison functor "comp".
   //!   Destroyer::operator()(pointer) is called for the removed elements.
   //!
   //! <b>Returns</b>: The number of erased elements.
   //! 
   //! <b>Complexity</b>: O(log(size()) + N.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators
   //!    to the erased elements.
   template<class KeyType, class KeyValueCompare, class Destroyer>
   size_type erase(const KeyType& key, KeyValueCompare comp, Destroyer destroyer)
   {  return tree_.erase(key, comp, destroyer);  }

   //! <b>Effects</b>: Erases all of the elements. 
   //! 
   //! <b>Complexity</b>: Linear to the number of elements on the container.
   //!   if it's a safe-mode or auto-unlink value_type. Constant time otherwise.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   void clear()
   {  return tree_.clear();  }

   //! <b>Effects</b>: Returns the number of contained elements with the given key
   //! 
   //! <b>Complexity</b>: Logarithmic to the number of elements contained plus lineal
   //!   to number of objects with the given key.
   //! 
   //! <b>Throws</b>: Nothing.
   size_type count(const value_type &value) const
   {  return tree_.find(value) != end();  }

   //! <b>Effects</b>: Returns the number of contained elements with the same key
   //!   compared with the given comparison functor.
   //! 
   //! <b>Complexity</b>: Logarithmic to the number of elements contained plus lineal
   //!   to number of objects with the given key.
   //! 
   //! <b>Throws</b>: Nothing.
   template<class KeyType, class KeyValueCompare>
   size_type count(const KeyType& key, KeyValueCompare comp) const
   {  return tree_.find(key, comp) != end();  }

   //! <b>Effects</b>: Returns an iterator to the first element whose
   //!   key is not less than k or end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   iterator lower_bound(const value_type &value)
   {  return tree_.lower_bound(value);  }

   //! <b>Requires</b>: comp must imply the same element order as
   //!   value_compare. Usually key is the part of the value_type
   //!   that is used in the ordering functor.
   //!
   //! <b>Effects</b>: Returns an iterator to the first element whose
   //!   key according to the comparison functor is not less than k or 
   //!   end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyValueCompare>
   iterator lower_bound(const KeyType& key, KeyValueCompare comp)
   {  return tree_.lower_bound(key, comp);  }

   //! <b>Effects</b>: Returns a const iterator to the first element whose
   //!   key is not less than k or end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   const_iterator lower_bound(const value_type &value) const
   {  return tree_.lower_bound(value);  }

   //! <b>Requires</b>: comp must imply the same element order as
   //!   value_compare. Usually key is the part of the value_type
   //!   that is used in the ordering functor.
   //!
   //! <b>Effects</b>: Returns a const_iterator to the first element whose
   //!   key according to the comparison functor is not less than k or 
   //!   end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyValueCompare>
   const_iterator lower_bound(const KeyType& key, KeyValueCompare comp) const
   {  return tree_.lower_bound(key, comp);  }

   //! <b>Effects</b>: Returns an iterator to the first element whose
   //!   key is greater than k or end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   iterator upper_bound(const value_type &value)
   {  return tree_.upper_bound(value);  }

   //! <b>Requires</b>: comp must imply the same element order as
   //!   value_compare. Usually key is the part of the value_type
   //!   that is used in the ordering functor.
   //!
   //! <b>Effects</b>: Returns an iterator to the first element whose
   //!   key according to the comparison functor is greater than key or 
   //!   end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyValueCompare>
   iterator upper_bound(const KeyType& key, KeyValueCompare comp)
   {  return tree_.upper_bound(key, comp);  }

   //! <b>Effects</b>: Returns an iterator to the first element whose
   //!   key is greater than k or end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   const_iterator upper_bound(const value_type &value) const
   {  return tree_.upper_bound(value);  }

   //! <b>Requires</b>: comp must imply the same element order as
   //!   value_compare. Usually key is the part of the value_type
   //!   that is used in the ordering functor.
   //!
   //! <b>Effects</b>: Returns a const_iterator to the first element whose
   //!   key according to the comparison functor is greater than key or 
   //!   end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyValueCompare>
   const_iterator upper_bound(const KeyType& key, KeyValueCompare comp) const
   {  return tree_.upper_bound(key, comp);  }

   //! <b>Effects</b>: Finds a iterator to the first element whose key is 
   //!   k or end() if that element does not exist.
   //!
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   iterator find(const value_type &value)
   {  return tree_.find(value);  }

   //! <b>Requires</b>: comp must imply the same element order as
   //!   value_compare. Usually key is the part of the value_type
   //!   that is used in the ordering functor.
   //!
   //! <b>Effects</b>: Finds a iterator to the first element whose key is 
   //!   k according to the comparison functor or end() if that element 
   //!   does not exist.
   //!
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyValueCompare>
   iterator find(const KeyType& key, KeyValueCompare comp)
   {  return tree_.find(key, comp);  }

   //! <b>Effects</b>: Finds a const_iterator to the first element whose key is 
   //!   k or end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   const_iterator find(const value_type &value) const
   {  return tree_.find(value);  }

   //! <b>Requires</b>: comp must imply the same element order as
   //!   value_compare. Usually key is the part of the value_type
   //!   that is used in the ordering functor.
   //!
   //! <b>Effects</b>: Finds a const_iterator to the first element whose key is 
   //!   k according to the comparison functor or end() if that element 
   //!   does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyValueCompare>
   const_iterator find(const KeyType& key, KeyValueCompare comp) const
   {  return tree_.find(key, comp);  }

   //! <b>Effects</b>: Finds a range containing all elements whose key is k or
   //!   an empty range that indicates the position where those elements would be
   //!   if they there is no elements with key k.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   std::pair<iterator,iterator> equal_range(const value_type &value)
   {  return tree_.equal_range(value);  }

   //! <b>Requires</b>: comp must imply the same element order as
   //!   value_compare. Usually key is the part of the value_type
   //!   that is used in the ordering functor.
   //!
   //! <b>Effects</b>: Finds a range containing all elements whose key is k 
   //!   according to the comparison functor or an empty range 
   //!   that indicates the position where those elements would be
   //!   if they there is no elements with key k.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyValueCompare>
   std::pair<iterator,iterator> equal_range(const KeyType& key, KeyValueCompare comp)
   {  return tree_.equal_range(key, comp);  }

   //! <b>Effects</b>: Finds a range containing all elements whose key is k or
   //!   an empty range that indicates the position where those elements would be
   //!   if they there is no elements with key k.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   std::pair<const_iterator, const_iterator>
      equal_range(const value_type &value) const
   {  return tree_.equal_range(value);  }

   //! <b>Requires</b>: comp must imply the same element order as
   //!   value_compare. Usually key is the part of the value_type
   //!   that is used in the ordering functor.
   //!
   //! <b>Effects</b>: Finds a range containing all elements whose key is k 
   //!   according to the comparison functor or an empty range 
   //!   that indicates the position where those elements would be
   //!   if they there is no elements with key k.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyValueCompare>
   std::pair<const_iterator, const_iterator>
      equal_range(const KeyType& key, KeyValueCompare comp) const
   {  return tree_.equal_range(key, comp);  }

   //! <b>Requires</b>: val must be an lvalue and shall be in a set of
   //!   appropriate type. Otherwise the behavior is undefined.
   //! 
   //! <b>Effects</b>: Returns: a valid iterator i belonging to the set
   //!   that points to the value
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   static iterator current(value_type& v)
   {  return tree_type::current(v);  }

   //! <b>Requires</b>: val must be an lvalue and shall be in a set of
   //!   appropriate type. Otherwise the behavior is undefined.
   //! 
   //! <b>Effects</b>: Returns: a valid const_iterator i belonging to the
   //!   set that points to the value
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   static const_iterator current(const value_type& v)
   {  return tree_type::current(v);  }
/*
   //! <b>Requires</b>: v shall not be in a set of the appropriate type.
   //! 
   //! <b>Effects</b>: init_node post-constructs the node data in x used by multisets of 
   //! the appropriate type. For the accessors multiset_derived_node and multiset_member_node 
   //! init_node has no effect, since the constructors of multiset_node_d and multiset_node_m 
   //! have already initialized the node data. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant time.
   //! 
   //! <b>Note</b>: This function is meant to be used mainly with the member value_traits, 
   //! where no implicit node initialization during construction occurs.
   static void init_node(reference v)
   {  return tree_type::init_node(v);  }

   //! <b>Effects</b>: removes x from a set of the appropriate type. It has no effect,
   //! if x is not in such a set. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant time.
   //! 
   //! <b>Note</b>: This static function is only usable with the "safe mode"
   //! hook and non-constant time size lists. Otherwise, the user must use
   //! the non-static "erase(value_type &)" member. If the user calls
   //! this function with a non "safe mode" or constant time size list
   //! a compilation error will be issued.
   template<class T>
   static void remove_node(T& v)
   {  tree_type::remove_node(v); }
*/
};

//! The class template imultiset is an intrusive container, that mimics most of 
//! the interface of std::multiset as described in the C++ standard.
//! 
//! The template parameter ValueTraits is called "value traits". It stores
//! information and operations about the type to be stored
//! in ilist and what type of hook has been chosen to include it in the list.
//! The value_traits class is supplied by the appropriate hook as a template subtype 
//! called "value_traits".
//!
//! The template parameter Compare, provides a function object that can compare two 
//!   element values as sort keys to determine their relative order in the set. 
//!
//! If the user specifies ConstantTimeSize as "true", a member of type SizeType
//! will be embedded in the class, that will keep track of the number of stored objects.
//! This will allow constant-time O(1) size() member, instead of default O(N) size.
template<class ValueTraits
        ,class Compare = std::less<typename ValueTraits::value_type>
        ,bool ConstantTimeSize = false, class SizeType = std::size_t>
class imultiset
{
   typedef detail::irbtree<ValueTraits, Compare, ConstantTimeSize, SizeType> tree_type;

   //noncopyable
   imultiset (const imultiset&);
   imultiset operator =(const imultiset&);

   public:
   typedef typename tree_type::value_type             value_type;
   typedef typename tree_type::pointer                pointer;
   typedef typename tree_type::const_pointer          const_pointer;
   typedef typename tree_type::reference              reference;
   typedef typename tree_type::const_reference        const_reference;
   typedef typename tree_type::size_type              size_type;
   typedef typename tree_type::difference_type        difference_type;
   typedef value_type                                 key_type;
   typedef typename tree_type::value_compare          value_compare;
   typedef value_compare                              key_compare;
   
   typedef typename tree_type::iterator               iterator;
   typedef typename tree_type::const_iterator         const_iterator;
   typedef typename tree_type::reverse_iterator       reverse_iterator;
   typedef typename tree_type::const_reverse_iterator const_reverse_iterator;

   private:
   tree_type tree_;

   public:
   //! <b>Effects</b>: Constructs an empty multiset. 
   //!   
   //! <b>Complexity</b>: Constant. 
   //! 
   //! <b>Throws</b>: Nothing unless the copy constructor of the Compare object throws. 
   imultiset(Compare cmp = Compare()) 
      :  tree_(cmp)
   {}

   //! <b>Requires</b>: Dereferencing Iterator must yield to an lvalue of type value_type. 
   //!   cmp must be a comparison function that induces a strict weak ordering.
   //! 
   //! <b>Effects</b>: Constructs an empty multiset and inserts elements from 
   //!   the range [first, last).
   //! 
   //! <b>Complexity</b>: Linear in N if the range [first, last) is already sorted using 
   //!   comp and otherwise N * log N, where N is last ­ first. Only the copy constructor 
   //!   of the Compare object is called. 
   //! 
   //! <b>Throws</b>: Nothing unless the copy constructor of the Compare object throws. 
   template<class Iterator>
   imultiset(Iterator b, Iterator e, Compare cmp = Compare())
      : tree_(false, b, e, cmp)
   {}

   //! <b>Effects</b>: Detaches all elements from this. The objects in the set 
   //!   are not deleted (i.e. no destructors are called).
   //! 
   //! <b>Complexity</b>: O(log(size()) + size()) if it's a safe-mode or
   //!   auto-unlink value. Otherwise constant.
   //! 
   //! <b>Throws</b>: Nothing.
   ~imultiset() 
   {}

   //! <b>Effects</b>: Returns an iterator pointing to the beginning of the multiset.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   iterator begin()
   { return tree_.begin();  }

   //! <b>Effects</b>: Returns a const_iterator pointing to the beginning of the multiset.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   const_iterator begin() const
   { return tree_.begin();  }

   //! <b>Effects</b>: Returns an iterator pointing to the end of the multiset.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   iterator end()
   { return tree_.end();  }

   //! <b>Effects</b>: Returns a const_iterator pointing to the end of the multiset.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   const_iterator end() const
   { return tree_.end();  }

   //! <b>Effects</b>: Returns a reverse_iterator pointing to the beginning of the
   //!    reversed multiset.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   reverse_iterator rbegin()
   { return tree_.rbegin();  }

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the beginning
   //!    of the reversed multiset.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   const_reverse_iterator rbegin() const
   { return tree_.rbegin();  }

   //! <b>Effects</b>: Returns a reverse_iterator pointing to the end
   //!    of the reversed multiset.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   reverse_iterator rend()
   { return tree_.rend();  }

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the end
   //!    of the reversed multiset.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   const_reverse_iterator rend() const
   { return tree_.rend();  }

   //! <b>Effects</b>: Returns the key_compare object used by the multiset.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: If key_compare copy-constructor throws.
   key_compare key_comp() const
   { return tree_.value_comp(); }

   //! <b>Effects</b>: Returns the value_compare object used by the multiset.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: If value_compare copy-constructor throws.
   value_compare value_comp() const
   { return tree_.value_comp(); }

   //! <b>Effects</b>: Returns true is the container is empty.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   bool empty() const
   { return tree_.empty(); }

   //! <b>Effects</b>: Returns the number of elements stored in the multiset.
   //! 
   //! <b>Complexity</b>: Linear to elements contained in *this if,
   //!   ConstantTimeSize is false. Constant-time otherwise.
   //! 
   //! <b>Throws</b>: Nothing.
   size_type size() const
   { return tree_.size(); }

   //! <b>Effects</b>: Swaps the contents of two multisets.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   void swap(imultiset& other)
   { tree_.swap(other.tree_); }

   //! <b>Requires</b>: val must be an lvalue
   //! 
   //! <b>Effects</b>: Inserts val into the multiset.
   //! 
   //! <b>Complexity</b>: Average complexity for insert element is at
   //!   most logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   //!   No copy-constructors are called.
   iterator insert(value_type& val)
   {  return tree_.insert_equal_upper_bound(val);  }

   //! <b>Requires</b>: val must be an lvalue
   //! 
   //! <b>Effects</b>: Inserts x into the multiset, using pos as a hint to
   //!   where it will be inserted.
   //! 
   //! <b>Complexity</b>: Logarithmic in general, but it is amortized
   //!   constant time if t is inserted immediately before hint.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   //!   No copy-constructors are called.
   iterator insert(const_iterator hint, value_type& val)
   {  return tree_.insert_equal(hint, val);  }

   //! <b>Requires</b>: Dereferencing Iterator must yield to an lvalue 
   //!   of type value_type.
   //! 
   //! <b>Effects</b>: Inserts a range into the multiset.
   //! 
   //! <b>Complexity</b>: Insert range is in general O(N * log(N)), where N is the 
   //!   size of the range. However, it is linear in N if the range is already sorted 
   //!   by value_comp().
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   //!   No copy-constructors are called.
   template<class Iterator>
   void insert(Iterator b, Iterator e)
   {  tree_.insert_equal(b, e);  }

   //! <b>Effects</b>: Erases the element pointed to by pos. 
   //! 
   //! <b>Complexity</b>: Average complexity is constant time. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   iterator erase(iterator i)
   {  return tree_.erase(i);  }

   //! <b>Effects</b>: Erases the range pointed to by b end e. 
   //! 
   //! <b>Complexity</b>: Average complexity for erase range is at most 
   //!   O(log(size()) + N), where N is the number of elements in the range.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   iterator erase(iterator b, iterator e)
   {  return tree_.erase(b, e);  }

   //! <b>Effects</b>: Erases all the elements with the given value.
   //! 
   //! <b>Returns</b>: The number of erased elements.
   //! 
   //! <b>Complexity</b>: O(log(size()) + N.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   size_type erase(const value_type &value)
   {  return tree_.erase(value);  }

   //! <b>Effects</b>: Erases all the elements that compare equal with
   //!   the given key and the given comparison functor.
   //! 
   //! <b>Returns</b>: The number of erased elements.
   //! 
   //! <b>Complexity</b>: O(log(size()) + N.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   template<class KeyType, class KeyValueCompare>
   size_type erase(const KeyType& key, KeyValueCompare comp)
   {  return tree_.erase(key, comp);  }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the element pointed to by pos. 
   //!   Destroyer::operator()(pointer) is called for the removed element.
   //! 
   //! <b>Complexity</b>: Average complexity for erase element is constant time. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators 
   //!    to the erased elements.
   template<class Destroyer>
   iterator erase(iterator i, Destroyer destroyer)
   {  return tree_.erase(i, destroyer);  }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the range pointed to by b end e.
   //!   Destroyer::operator()(pointer) is called for the removed element.
   //! 
   //! <b>Complexity</b>: Average complexity for erase range is at most 
   //!   O(log(size()) + N), where N is the number of elements in the range.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators
   //!    to the erased elements.
   template<class Destroyer>
   iterator erase(iterator b, iterator e, Destroyer destroyer)
   {  return tree_.erase(b, e, destroyer);  }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases all the elements with the given value.
   //!   Destroyer::operator()(pointer) is called for the removed elements.
   //! 
   //! <b>Returns</b>: The number of erased elements.
   //! 
   //! <b>Complexity</b>: O(log(size()) + N.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   template<class Destroyer>
   size_type erase(const value_type &value, Destroyer destroyer)
   {  return tree_.erase(value, destroyer);  }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases all the elements with the given key.
   //!   according to the comparison functor "comp".
   //!   Destroyer::operator()(pointer) is called for the removed elements.
   //!
   //! <b>Returns</b>: The number of erased elements.
   //! 
   //! <b>Complexity</b>: O(log(size()) + N.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators
   //!    to the erased elements.
   template<class KeyType, class KeyValueCompare, class Destroyer>
   size_type erase(const KeyType& key, KeyValueCompare comp, Destroyer destroyer)
   {  return tree_.erase(key, comp, destroyer);  }

   //! <b>Effects</b>: Erases all of the elements. 
   //! 
   //! <b>Complexity</b>: Linear to the number of elements on the container.
   //!   if it's a safe-mode or auto-unlink value_type. Constant time otherwise.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   void clear()
   {  return tree_.clear();  }

   //! <b>Effects</b>: Returns the number of contained elements with the given key
   //! 
   //! <b>Complexity</b>: Logarithmic to the number of elements contained plus lineal
   //!   to number of objects with the given key.
   //! 
   //! <b>Throws</b>: Nothing.
   template<class KeyType>
   size_type count(const KeyType& key) const
   {  return tree_.count(key);  }

   //! <b>Effects</b>: Returns an iterator to the first element whose
   //!   key is not less than k or end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   iterator lower_bound(const value_type &value)
   {  return tree_.lower_bound(value);  }

   //! <b>Requires</b>: comp must imply the same element order as
   //!   value_compare. Usually key is the part of the value_type
   //!   that is used in the ordering functor.
   //!
   //! <b>Effects</b>: Returns an iterator to the first element whose
   //!   key according to the comparison functor is not less than k or 
   //!   end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyValueCompare>
   iterator lower_bound(const KeyType& key, KeyValueCompare comp)
   {  return tree_.lower_bound(key, comp);  }

   //! <b>Effects</b>: Returns a const iterator to the first element whose
   //!   key is not less than k or end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   const_iterator lower_bound(const value_type &value) const
   {  return tree_.lower_bound(value);  }

   //! <b>Requires</b>: comp must imply the same element order as
   //!   value_compare. Usually key is the part of the value_type
   //!   that is used in the ordering functor.
   //!
   //! <b>Effects</b>: Returns a const_iterator to the first element whose
   //!   key according to the comparison functor is not less than k or 
   //!   end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyValueCompare>
   const_iterator lower_bound(const KeyType& key, KeyValueCompare comp) const
   {  return tree_.lower_bound(key, comp);  }

   //! <b>Effects</b>: Returns an iterator to the first element whose
   //!   key is greater than k or end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   iterator upper_bound(const value_type &value)
   {  return tree_.upper_bound(value);  }

   //! <b>Requires</b>: comp must imply the same element order as
   //!   value_compare. Usually key is the part of the value_type
   //!   that is used in the ordering functor.
   //!
   //! <b>Effects</b>: Returns an iterator to the first element whose
   //!   key according to the comparison functor is greater than key or 
   //!   end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyValueCompare>
   iterator upper_bound(const KeyType& key, KeyValueCompare comp)
   {  return tree_.upper_bound(key, comp);  }

   //! <b>Effects</b>: Returns an iterator to the first element whose
   //!   key is greater than k or end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   const_iterator upper_bound(const value_type &value) const
   {  return tree_.upper_bound(value);  }

   //! <b>Requires</b>: comp must imply the same element order as
   //!   value_compare. Usually key is the part of the value_type
   //!   that is used in the ordering functor.
   //!
   //! <b>Effects</b>: Returns a const_iterator to the first element whose
   //!   key according to the comparison functor is greater than key or 
   //!   end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyValueCompare>
   const_iterator upper_bound(const KeyType& key, KeyValueCompare comp) const
   {  return tree_.upper_bound(key, comp);  }

   //! <b>Effects</b>: Finds a iterator to the first element whose key is 
   //!   k or end() if that element does not exist.
   //!
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   iterator find(const value_type &value)
   {  return tree_.find(value);  }

   //! <b>Requires</b>: comp must imply the same element order as
   //!   value_compare. Usually key is the part of the value_type
   //!   that is used in the ordering functor.
   //!
   //! <b>Effects</b>: Finds a iterator to the first element whose key is 
   //!   k according to the comparison functor or end() if that element 
   //!   does not exist.
   //!
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyValueCompare>
   iterator find(const KeyType& key, KeyValueCompare comp)
   {  return tree_.find(key, comp);  }

   //! <b>Effects</b>: Finds a const_iterator to the first element whose key is 
   //!   k or end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   const_iterator find(const value_type &value) const
   {  return tree_.find(value);  }

   //! <b>Requires</b>: comp must imply the same element order as
   //!   value_compare. Usually key is the part of the value_type
   //!   that is used in the ordering functor.
   //!
   //! <b>Effects</b>: Finds a const_iterator to the first element whose key is 
   //!   k according to the comparison functor or end() if that element 
   //!   does not exist.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyValueCompare>
   const_iterator find(const KeyType& key, KeyValueCompare comp) const
   {  return tree_.find(key, comp);  }

   //! <b>Effects</b>: Finds a range containing all elements whose key is k or
   //!   an empty range that indicates the position where those elements would be
   //!   if they there is no elements with key k.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   std::pair<iterator,iterator> equal_range(const value_type &value)
   {  return tree_.equal_range(value);  }

   //! <b>Requires</b>: comp must imply the same element order as
   //!   value_compare. Usually key is the part of the value_type
   //!   that is used in the ordering functor.
   //!
   //! <b>Effects</b>: Finds a range containing all elements whose key is k 
   //!   according to the comparison functor or an empty range 
   //!   that indicates the position where those elements would be
   //!   if they there is no elements with key k.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyValueCompare>
   std::pair<iterator,iterator> equal_range(const KeyType& key, KeyValueCompare comp)
   {  return tree_.equal_range(key, comp);  }

   //! <b>Effects</b>: Finds a range containing all elements whose key is k or
   //!   an empty range that indicates the position where those elements would be
   //!   if they there is no elements with key k.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   std::pair<const_iterator, const_iterator>
      equal_range(const value_type &value) const
   {  return tree_.equal_range(value);  }

   //! <b>Requires</b>: comp must imply the same element order as
   //!   value_compare. Usually key is the part of the value_type
   //!   that is used in the ordering functor.
   //!
   //! <b>Effects</b>: Finds a range containing all elements whose key is k 
   //!   according to the comparison functor or an empty range 
   //!   that indicates the position where those elements would be
   //!   if they there is no elements with key k.
   //! 
   //! <b>Complexity</b>: Logarithmic.
   //! 
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyValueCompare>
   std::pair<const_iterator, const_iterator>
      equal_range(const KeyType& key, KeyValueCompare comp) const
   {  return tree_.equal_range(key, comp);  }

   //! <b>Requires</b>: val must be an lvalue and shall be in a set of
   //!   appropriate type. Otherwise the behavior is undefined.
   //! 
   //! <b>Effects</b>: Returns: a valid iterator i belonging to the set
   //!   that points to the value
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   static iterator current(value_type& v)
   {  return tree_type::current(v);  }

   //! <b>Requires</b>: val must be an lvalue and shall be in a set of
   //!   appropriate type. Otherwise the behavior is undefined.
   //! 
   //! <b>Effects</b>: Returns: a valid const_iterator i belonging to the
   //!   set that points to the value
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   static const_iterator current(const value_type& v)
   {  return tree_type::current(v);  }
/*
   //! <b>Requires</b>: v shall not be in a multiset of the appropriate type.
   //! 
   //! <b>Effects</b>: init_node post-constructs the node data in x used by multisets of 
   //! the appropriate type. For the accessors multiset_derived_node and multiset_member_node 
   //! init_node has no effect, since the constructors of multiset_node_d and multiset_node_m 
   //! have already initialized the node data. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant time.
   //! 
   //! <b>Note</b>: This function is meant to be used mainly with the member value_traits, 
   //! where no implicit node initialization during construction occurs.
   static void init_node(reference v)
   {  return tree_type::init_node(v);  }

   //! <b>Effects</b>: removes x from a multiset of the appropriate type. It has no effect,
   //! if x is not in such a multiset. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant time.
   //! 
   //! <b>Note</b>: This static function is only usable with the "safe mode"
   //! hook and non-constant time size lists. Otherwise, the user must use
   //! the non-static "erase(value_type &)" member. If the user calls
   //! this function with a non "safe mode" or constant time size list
   //! a compilation error will be issued.
   template<class T>
   static void remove_node(T& v)
   {  tree_type::remove_node(v); }

   std::pair<iterator, bool> insert_check
      (const value_type &value, insert_commit_data &commit_data)
   {  return tree_.insert_unique_check(value, commit_data); }

   std::pair<iterator, bool> insert_check
      (iterator hint, const value_type &value, insert_commit_data &commit_data)
   {  return tree_.insert_unique_check(hint, value, commit_data); }

*/
};

} //namespace intrusive 
} //namespace boost 

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_ISET_HPP
