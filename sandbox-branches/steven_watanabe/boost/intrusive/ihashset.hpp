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
#ifndef BOOST_INTRUSIVE_IHASHSET_HPP
#define BOOST_INTRUSIVE_IHASHSET_HPP

#include <boost/intrusive/detail/config_begin.hpp>
#include <boost/intrusive/detail/ihashtable.hpp>

namespace boost {
namespace intrusive {

//! The class template ihashset is an intrusive container, that mimics most of 
//! the interface of std::unordered_set as described in the C++ standard.
//!
//! ihashset is a pseudo-intrusive container: each object to be stored in the
//! container must contain a proper hook, but the container also needs
//! additional auxiliary memory to work: ihashset needs an pointer to an array
//! of type bucket_type to be passed in the constructor. This bucket array must
//! have at least, the same lifetime as the container. This makes the use of
//! ihashset more complicated than purely intrusive containers.
//!
//! ihashset is implemented as an array of singly linked lists. The singly
//! linked list where a value will be inserted is obtained by the value
//! returned by the remainder of the hash function applied to the value and
//! the length of the auxiliary bucket array provided by the user.
//!
//! The template parameter ValueTraits is called "value traits". It stores
//! information and operations about the type to be stored
//! in ilist and what type of hook has been chosen to include it in the list.
//! The value_traits class is supplied by the appropriate hook as a template subtype 
//! called "value_traits".
//!
//! The template parameter Compare, provides a function object that can compare two 
//!   element values as sort keys to determine their relative order in the ihashset. 
//!
//! If the user specifies ConstantTimeSize as "true", a member of type SizeType
//! will be embedded in the class, that will keep track of the number of stored objects.
//! This will allow constant-time O(1) size() member, instead of default O(N) size.
//!
//! ihashset only provides forward iterators but it provides 4 iterator types:
//! iterator and const_iterator to navigate through the whole container and
//! local_iterator and const_local_iterator to navigate through the values
//! stored in a single bucket. local iterators are more faster and smaller.
//!
//! It's not recommended to use non ConstantTimeSize ihashsets because several
//! key functions, like "empty()", become non-constant time functions. Non
//! ConstantTimeSize ihashsets are mainly provided to support auto-unlink hooks.
//!
//! ihashset, unlike std::unordered_set, does not make automatic rehashings nor
//! offers functions related to a load factor. Rehashing can be explicitly requested
//! and the user must provide a new bucket array that would be used from that moment.
//!
//! Since no automatic rehashing is done, iterator are never invalidated when
//! inserting or erasing elements. Iterators are only invalidated when rehasing.
template< class ValueTraits
        , class Hash             = boost::hash<typename ValueTraits::value_type>
        , class Equal            = std::equal_to<typename ValueTraits::value_type>
        , bool  ConstantTimeSize = false
        , class SizeType         = std::size_t
        >
class ihashset
{
   private:
   typedef detail::ihashtable<ValueTraits, Hash, Equal, ConstantTimeSize, SizeType> table_type;

   //! This class is non-copyable
   ihashset (const ihashset&);

   //!This class is non-assignable
   ihashset &operator =(const ihashset&);

   public:
   typedef typename table_type::value_type            value_type;
   typedef typename table_type::pointer               pointer;
   typedef typename table_type::const_pointer         const_pointer;
   typedef typename table_type::reference             reference;
   typedef typename table_type::const_reference       const_reference;
   typedef typename table_type::size_type             size_type;
   typedef typename table_type::difference_type       difference_type;
   typedef value_type                                 key_type;
   typedef typename table_type::key_equal             key_equal;
   typedef typename table_type::hasher                hasher;
   typedef typename table_type::iterator              iterator;
   typedef typename table_type::const_iterator        const_iterator;
   typedef typename table_type::insert_commit_data    insert_commit_data;
   typedef typename table_type::bucket_type           bucket_type;
   typedef typename table_type::bucket_ptr            bucket_ptr;
   typedef typename table_type::local_iterator        local_iterator;
   typedef typename table_type::const_local_iterator  const_local_iterator;

   private:
   table_type table_;

   public:

   //! <b>Requires</b>: buckets must not be being used by any other resource.
   //!   hasher must be a standard-compatible hash functor and equal must be an
   //!   standard-compatible equality functor.
   //!
   //! <b>Effects</b>: Constructs an empty ihashset, storing a reference
   //!   to the bucket array and copies of the hasher and equal functors.
   //!   
   //! <b>Complexity</b>: Constant. 
   //! 
   //! <b>Throws</b>: Nothing unless the copy constructor of Hash or
   //!   Equal throws. 
   //!
   //! <b>Notes</b>: buckets array must be destroyed after only after
   //!   *this is destroyed. 
   ihashset( bucket_ptr buckets
           , size_type buckets_len
           , const Hash & hasher = Hash()
           , const Equal &equal = Equal()) 
      :  table_(buckets, buckets_len, hasher, equal)
   {}

   //! <b>Requires</b>: buckets must not be being used by any other resource
   //!   and Dereferencing Iterator must yield to an lvalue of type value_type.
   //!   hasher must be a standard-compatible hash functor and equal must be an
   //!   standard-compatible equality functor.
   //! 
   //! <b>Effects</b>: Constructs an empty ihashset and inserts elements from 
   //!   the range [first, last).
   //!   
   //! <b>Complexity</b>: If N is std::distance(b, e): Average case is O(N)
   //!   (with a good has function and with buckets_len >= N),worst case O(N2).
   //! 
   //! <b>Throws</b>: Nothing unless the copy constructor of Hash or
   //!   Equal throws. 
   //!
   //! <b>Notes</b>: buckets array must be destroyed after only after
   //!   *this is destroyed. 
   template<class Iterator>
   ihashset( bucket_ptr buckets
           , size_type buckets_len
           , Iterator b
           , Iterator e
           , const Hash & hasher = Hash()
           , const Equal &equal = Equal()) 
      :  table_(buckets, buckets_len, hasher, equal)
   {  table_.insert_unique(b, e);  }

   //! <b>Effects</b>: Detaches all elements from this. The objects in the ihashset 
   //!   are not deleted (i.e. no destructors are called).
   //! 
   //! <b>Complexity</b>: Linear to the number of elements in the ihashset, if 
   //!   it's a safe-mode or auto-unlink value. Otherwise constant.
   //! 
   //! <b>Throws</b>: Nothing.
   ~ihashset() 
   {}

   //! <b>Effects</b>: Returns an iterator pointing to the beginning of the ihashset.
   //! 
   //! <b>Complexity</b>: Amortized constant time.
   //!   Worst case (empty ihashset): O(this->bucket_count())
   //! 
   //! <b>Throws</b>: Nothing.
   iterator begin()
   { return table_.begin();  }

   //! <b>Effects</b>: Returns a const_iterator pointing to the beginning
   //!   of the ihashset.
   //!
   //! <b>Complexity</b>: Amortized constant time.
   //!   Worst case (empty ihashset): O(this->bucket_count())
   //! 
   //! <b>Throws</b>: Nothing.
   const_iterator begin() const
   { return table_.begin();  }

   //! <b>Effects</b>: Returns an iterator pointing to the end of the ihashset.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   iterator end()
   { return table_.end();  }

   //! <b>Effects</b>: Returns a const_iterator pointing to the end of the ihashset.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   const_iterator end() const
   { return table_.end();  }

   //! <b>Effects</b>: Returns the hasher object used by the ihashset.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: If hasher copy-constructor throws.
   hasher hash_function() const
   { return table_.hash_function(); }

   //! <b>Effects</b>: Returns the key_equal object used by the ihashset.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: If key_equal copy-constructor throws.
   key_equal key_eq() const
   { return table_.key_eq(); }

   //! <b>Effects</b>: Returns true is the container is empty.
   //! 
   //! <b>Complexity</b>: if ConstantTimeSize is false, average constant time
   //!   (worst case, with empty() == true): O(this->bucket_count()).
   //!   Otherwise constant.
   //! 
   //! <b>Throws</b>: Nothing.
   bool empty() const
   { return table_.empty(); }

   //! <b>Effects</b>: Returns the number of elements stored in the ihashset.
   //! 
   //! <b>Complexity</b>: Linear to elements contained in *this if
   //!   ConstantTimeSize is false. Constant-time otherwise.
   //! 
   //! <b>Throws</b>: Nothing.
   size_type size() const
   { return table_.size(); }

   //! <b>Requires</b>: the hasher and the equality function unqualified swap
   //!   call should not throw.
   //! 
   //! <b>Effects</b>: Swaps the contents of two ihashsets.
   //!   Swaps also the contained equality and hasher functors.
   //! 
   //! <b>Complexity</b>: Constant.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Note</b>: Nothing.
   void swap(ihashset& other)
   { table_.swap(other.table_); }

   //! <b>Requires</b>: val must be an lvalue
   //! 
   //! <b>Effects</b>: Tries to inserts val into the ihashset.
   //!
   //! <b>Returns</b>: If the value
   //!   is not already present inserts it and returns a pair containing the
   //!   iterator to the new value and true. If the value is already present
   //!   returns a pair containing an iterator to the already present value
   //!   and false.
   //! 
   //! <b>Complexity</b>: Average case O(1), worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   //!   No copy-constructors are called.
   std::pair<iterator, bool> insert(value_type& val)
   {  return table_.insert_unique(val);  }

   //! <b>Requires</b>: "hasher" must be a hash function that induces 
   //!   the same hash values as the stored hasher. The difference is that
   //!   "hasher" hashes the given key instead of the value_type.
   //!
   //!   "key_value_equal" must be a equality function that induces 
   //!   the same equality as key_equal. The difference is that
   //!   "key_value_equal" compares an arbitrary key with the contained values.
   //! 
   //! <b>Effects</b>: Checks if a value can be inserted in the ihashset, using
   //!   a user provided key instead of the value itself.
   //!
   //! <b>Returns</b>: If an equivalent value is already present
   //!   returns a pair containing an iterator to the already present value
   //!   and false. If the value can be inserted returns true in the returned
   //!   pair boolean and fills "commit_data" that is meant to be used with
   //!   the "insert_commit" function.
   //! 
   //! <b>Complexity</b>: Average case O(1), worst case O(this->size()).
   //!
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   //! 
   //! <b>Notes</b>: This function is used to improve performance when constructing
   //!   a value_type is expensive: if an equivalent value is already present
   //!   the constructed object must be discarded. Many times, the part of the
   //!   node that is used to impose the hash or the equality is much cheaper to
   //!   construct than the value_type and this function offers the possibility to
   //!   use that the part to check if the insertion will be successful.
   //!
   //!   If the check is successful, the user can construct the value_type and use
   //!   "insert_commit" to insert the object in constant-time.
   //!
   //!   "commit_data" remains valid for a subsequent "insert_commit" only if no more
   //!   objects are inserted or erased from the ihashset.
   template<class KeyType, class KeyHasher, class KeyValueEqual>
   std::pair<iterator, bool> insert_check
      (const KeyType &key, const KeyHasher &hasher, const KeyValueEqual &key_value_equal, insert_commit_data &commit_data)
   {  return table_.insert_unique_check(key, hasher, key_value_equal, commit_data); }

   //! <b>Requires</b>: value must be an lvalue of type value_type. commit_data
   //!   must have been obtained from a previous call to "insert_check".
   //!   No objects should have been inserted or erased from the ihashset between
   //!   the "insert_check" that filled "commit_data" and the call to "insert_commit".
   //! 
   //! <b>Effects</b>: Inserts the value in the ihashset using the information obtained
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
   iterator insert_commit(value_type &val, const insert_commit_data &commit_data)
   {  return table_.insert_unique_commit(val, commit_data); }

   //! <b>Requires</b>: Dereferencing Iterator must yield to an lvalue 
   //!   of type value_type.
   //! 
   //! <b>Effects</b>: Equivalent to this->insert(t) for each element in [b, e).
   //! 
   //! <b>Complexity</b>: Average case O(N), where N is std::distance(b, e).
   //!   Worst case O(N*this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   //!   No copy-constructors are called.
   template<class Iterator>
   void insert(Iterator b, Iterator e)
   {  table_.insert_unique(b, e);  }

   //! <b>Effects</b>: Erases the element pointed to by i. 
   //! 
   //! <b>Complexity</b>: Average case O(1), worst case O(this->size()).
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased element. No destructors are called.
   void erase(iterator i)
   {  table_.erase(i);  }

   //! <b>Effects</b>: Erases the range pointed to by b end e. 
   //! 
   //! <b>Complexity</b>: Average case O(std::distance(b, e)),
   //!   worst case O(this->size()).
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   iterator erase(iterator b, iterator e)
   {  return table_.erase(b, e);  }

   //! <b>Effects</b>: Erases all the elements with the given value.
   //! 
   //! <b>Returns</b>: The number of erased elements.
   //! 
   //! <b>Complexity</b>: Average case O(this->count(value)).
   //!   Worst case O(this->size()).
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   size_type erase(const value_type &value)
   {  return table_.erase(value);  }

   //! <b>Requires</b>: "hasher" must be a hash function that induces 
   //!   the same hash values as the stored hasher. The difference is that
   //!   "hasher" hashes the given key instead of the value_type.
   //!
   //!   "key_value_equal" must be a equality function that induces 
   //!   the same equality as key_equal. The difference is that
   //!   "key_value_equal" compares an arbitrary key with the contained values.
   //!
   //! <b>Effects</b>: Erases all the elements that have the same hash and
   //!   compare equal with the given key.
   //! 
   //! <b>Returns</b>: The number of erased elements.
   //! 
   //! <b>Complexity</b>: Average case O(this->count(value)).
   //!   Worst case O(this->size()).
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   template<class KeyType, class KeyHasher, class KeyValueEqual>
   size_type erase(const KeyType& key, const KeyHasher &hasher, const KeyValueEqual &equal)
   {  return table_.erase(key, hasher, equal);  }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the element pointed to by i. 
   //!   Destroyer::operator()(pointer) is called for the removed element.
   //! 
   //! <b>Complexity</b>: Average case O(1), worst case O(this->size()).
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators 
   //!    to the erased elements.
   template<class Destroyer>
   iterator erase(iterator i, Destroyer destroyer)
   {  return table_.erase(i, destroyer);  }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the range pointed to by b end e.
   //!   Destroyer::operator()(pointer) is called for the removed element.
   //! 
   //! <b>Complexity</b>: Average case O(std::distance(b, e)),
   //!   worst case O(this->size()).
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators
   //!    to the erased elements.
   template<class Destroyer>
   iterator erase(iterator b, iterator e, Destroyer destroyer)
   {  return table_.erase(b, e, destroyer);  }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases all the elements with the given value.
   //!   Destroyer::operator()(pointer) is called for the removed elements.
   //! 
   //! <b>Returns</b>: The number of erased elements.
   //! 
   //! <b>Complexity</b>: Average case O(this->count(value)).
   //!   Worst case O(this->size()).
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   template<class Destroyer>
   size_type erase(const value_type &value, Destroyer destroyer)
   {  return table_.erase(value, destroyer);  }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases all the elements with the given key.
   //!   according to the comparison functor "equal".
   //!   Destroyer::operator()(pointer) is called for the removed elements.
   //!
   //! <b>Returns</b>: The number of erased elements.
   //! 
   //! <b>Complexity</b>: Average case O(this->count(value)).
   //!   Worst case O(this->size()).
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators
   //!    to the erased elements.
   template<class KeyType, class KeyHasher, class KeyValueEqual, class Destroyer>
   size_type erase(const KeyType& key, const KeyHasher &hasher, const KeyValueEqual &equal, Destroyer destroyer)
   {  return table_.erase(key, hasher, equal, destroyer);  }

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
   {  return table_.clear();  }

   //! <b>Effects</b>: Returns the number of contained elements with the given key
   //! 
   //! <b>Complexity</b>: Average case O(1), worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   size_type count(const value_type &value) const
   {  return table_.find(value) != end();  }

   //! <b>Requires</b>: "hasher" must be a hash function that induces 
   //!   the same hash values as the stored hasher. The difference is that
   //!   "hasher" hashes the given key instead of the value_type.
   //!
   //!   "key_value_equal" must be a equality function that induces 
   //!   the same equality as key_equal. The difference is that
   //!   "key_value_equal" compares an arbitrary key with the contained values.
   //!
   //! <b>Effects</b>: Returns the number of contained elements with the given key
   //!
   //! <b>Complexity</b>: Average case O(1), worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   template<class KeyType, class KeyHasher, class KeyValueEqual, class Destroyer>
   size_type count(const KeyType& key, const KeyHasher &hasher, const KeyValueEqual &equal) const
   {  return table_.find(key, hasher, equal) != end();  }

   //! <b>Effects</b>: Finds a iterator to the first element whose key is 
   //!   k or end() if that element does not exist.
   //!
   //! <b>Complexity</b>: Average case O(1), worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   iterator find(const value_type &value)
   {  return table_.find(value);  }

   //! <b>Requires</b>: "hasher" must be a hash function that induces 
   //!   the same hash values as the stored hasher. The difference is that
   //!   "hasher" hashes the given key instead of the value_type.
   //!
   //!   "key_value_equal" must be a equality function that induces 
   //!   the same equality as key_equal. The difference is that
   //!   "key_value_equal" compares an arbitrary key with the contained values.
   //!
   //! <b>Effects</b>: Finds a iterator to the first element whose key is 
   //!   key according to the given hasher and equality functor or end() if
   //!   that element does not exist.
   //!
   //! <b>Complexity</b>: Average case O(1), worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyHasher, class KeyValueEqual>
   iterator find(const KeyType& key, const KeyHasher &hasher, const KeyValueEqual &equal)
   {  return table_.find(key, hasher, equal);  }

   //! <b>Effects</b>: Finds a const_iterator to the first element whose key is 
   //!   k or end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Average case O(1), worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   const_iterator find(const value_type &value) const
   {  return table_.find(value);  }

   //! <b>Requires</b>: "hasher" must be a hash function that induces 
   //!   the same hash values as the stored hasher. The difference is that
   //!   "hasher" hashes the given key instead of the value_type.
   //!
   //!   "key_value_equal" must be a equality function that induces 
   //!   the same equality as key_equal. The difference is that
   //!   "key_value_equal" compares an arbitrary key with the contained values.
   //!
   //! <b>Effects</b>: Finds a iterator to the first element whose key is 
   //!   key according to the given hasher and equality functor or end() if
   //!   that element does not exist.
   //! 
   //! <b>Complexity</b>: Average case O(1), worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyHasher, class KeyValueEqual>
   const_iterator find(const KeyType& key, const KeyHasher &hasher, const KeyValueEqual &equal) const
   {  return table_.find(key, equal);  }

   //! <b>Effects</b>: Returns a range containing all elements with values equivalent
   //!   to value. Returns std::make_pair(this->end(), this->end()) if no such 
   //!   elements exist.
   //! 
   //! <b>Complexity</b>: Average case O(this->count(k)). Worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   std::pair<iterator,iterator> equal_range(const value_type &value)
   {  return table_.equal_range(value);  }

   //! <b>Requires</b>: "hasher" must be a hash function that induces 
   //!   the same hash values as the stored hasher. The difference is that
   //!   "hasher" hashes the given key instead of the value_type.
   //!
   //!   "key_value_equal" must be a equality function that induces 
   //!   the same equality as key_equal. The difference is that
   //!   "key_value_equal" compares an arbitrary key with the contained values.
   //!
   //! <b>Effects</b>: Returns a range containing all elements with equivalent
   //!   keys. Returns std::make_pair(this->end(), this->end()) if no such 
   //!   elements exist.
   //! 
   //! <b>Complexity</b>: Average case O(this->count(k)). Worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyHasher, class KeyValueEqual>
   std::pair<iterator,iterator> equal_range(const KeyType& key, const KeyHasher& hasher, KeyValueEqual equal)
   {  return table_.equal_range(key, hasher, equal);  }

   //! <b>Effects</b>: Returns a range containing all elements with values equivalent
   //!   to value. Returns std::make_pair(this->end(), this->end()) if no such 
   //!   elements exist.
   //! 
   //! <b>Complexity</b>: Average case O(this->count(k)). Worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   std::pair<const_iterator, const_iterator>
      equal_range(const value_type &value) const
   {  return table_.equal_range(value);  }

   //! <b>Requires</b>: "hasher" must be a hash function that induces 
   //!   the same hash values as the stored hasher. The difference is that
   //!   "hasher" hashes the given key instead of the value_type.
   //!
   //!   "key_value_equal" must be a equality function that induces 
   //!   the same equality as key_equal. The difference is that
   //!   "key_value_equal" compares an arbitrary key with the contained values.
   //!
   //! <b>Effects</b>: Returns a range containing all elements with equivalent
   //!   keys. Returns std::make_pair(this->end(), this->end()) if no such 
   //!   elements exist.
   //! 
   //! <b>Complexity</b>: Average case O(this->count(k)). Worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyHasher, class KeyValueEqual>
   std::pair<const_iterator, const_iterator>
      equal_range(const KeyType& key, const KeyHasher &hasher, const KeyValueEqual &equal) const
   {  return table_.equal_range(key, equal);  }

   //! <b>Requires</b>: val must be an lvalue and shall be in a ihashset of
   //!   appropriate type. Otherwise the behavior is undefined.
   //! 
   //! <b>Effects</b>: Returns: a valid iterator i belonging to the ihashset
   //!   that points to the value
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: If the hash function throws.
   iterator current(value_type& v)
   {  return table_.current(v);  }

   //! <b>Requires</b>: val must be an lvalue and shall be in a ihashset of
   //!   appropriate type. Otherwise the behavior is undefined.
   //! 
   //! <b>Effects</b>: Returns: a valid const_iterator i belonging to the
   //!   ihashset that points to the value
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: If the hash function throws.
   const_iterator current(const value_type& v)
   {  return table_.current(v);  }

   //! <b>Requires</b>: val must be an lvalue and shall be in a ihashset of
   //!   appropriate type. Otherwise the behavior is undefined.
   //! 
   //! <b>Effects</b>: Returns: a valid local_iterator i belonging to the ihashset
   //!   that points to the value
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   static local_iterator current_local(value_type& v)
   {  return table_type::current_local(v);  }

   //! <b>Requires</b>: val must be an lvalue and shall be in a ihashset of
   //!   appropriate type. Otherwise the behavior is undefined.
   //! 
   //! <b>Effects</b>: Returns: a valid const_local_iterator i belonging to
   //!   the ihashset that points to the value
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   static const_local_iterator current_local(const value_type& v)
   {  return table_type::current_local(v);  }

   //! <b>Effects</b>: Returns the number of buckets passed in the constructor
   //!   or the last rehash function.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   size_type bucket_count() const
   {  return table_.bucket_count();   }

   //! <b>Requires</b>: n is in the range [0, this->bucket_count()).
   //!
   //! <b>Effects</b>: Returns the number of elements in the nth bucket.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   size_type bucket_size(size_type n)
   {  return table_.bucket_size(n);   }

   //! <b>Effects</b>: Returns the index of the bucket in which elements
   //!   with keys equivalent to k would be found, if any such element existed.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: If the hash functor throws.
   //!
   //! <b>Note</b>: the return value is in the range [0, this->bucket_count()).
   size_type bucket(const key_type& k)
   {  return table_.bucket(k);   }

   //! <b>Requires</b>: "hasher" must be a hash function that induces 
   //!   the same hash values as the stored hasher. The difference is that
   //!   "hasher" hashes the given key instead of the value_type.
   //!
   //! <b>Effects</b>: Returns the index of the bucket in which elements
   //!   with keys equivalent to k would be found, if any such element existed.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: If the hash functor throws.
   //!
   //! <b>Note</b>: the return value is in the range [0, this->bucket_count()).
   template<class KeyType, class KeyHasher>
   size_type bucket(const KeyType& k, const KeyHasher &hasher)
   {  return table_.bucket(k, hasher);   }

   //! <b>Effects</b>: Returns the bucket array pointer passed in the constructor
   //!   or the last rehash function.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   bucket_ptr bucket_pointer() const
   {  return table_.bucket_pointer();   }

   //! <b>Requires</b>: n is in the range [0, this->bucket_count()).
   //!
   //! <b>Effects</b>: Returns a local_iterator pointing to the beginning
   //!   of the sequence stored in the bucket n.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>:  [b.begin(n), b.end(n)) is a valid range
   //!   containing all of the elements in the nth bucket. 
   local_iterator begin(size_type n)
   {  return table_.begin(n);   }

   //! <b>Requires</b>: n is in the range [0, this->bucket_count()).
   //!
   //! <b>Effects</b>: Returns a const_local_iterator pointing to the beginning
   //!   of the sequence stored in the bucket n.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>:  [b.begin(n), b.end(n)) is a valid range
   //!   containing all of the elements in the nth bucket. 
   const_local_iterator begin(size_type n) const
   {  return table_.begin(n);   }

   //! <b>Requires</b>: n is in the range [0, this->bucket_count()).
   //!
   //! <b>Effects</b>: Returns a local_iterator pointing to the end
   //!   of the sequence stored in the bucket n.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>:  [b.begin(n), b.end(n)) is a valid range
   //!   containing all of the elements in the nth bucket. 
   local_iterator end(size_type n)
   {  return table_.end(n);   }

   //! <b>Requires</b>: n is in the range [0, this->bucket_count()).
   //!
   //! <b>Effects</b>: Returns a const_local_iterator pointing to the end
   //!   of the sequence stored in the bucket n.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>:  [b.begin(n), b.end(n)) is a valid range
   //!   containing all of the elements in the nth bucket. 
   const_local_iterator end(size_type n) const
   {  return table_.end(n);   }

   //! <b>Requires</b>: new_buckets must be a pointer to a new bucket array
   //!   or the same as this->bucket_pointer(). n is the length of the
   //!   the array pointed by new_buckets. If new_buckets == this->bucket_pointer()
   //!   n can be bigger or smaller than this->bucket_count().
   //!
   //! <b>Effects</b>: Updates the internal reference with the new bucket erases
   //!   the values from the old bucket and inserts then in the new one. 
   //! 
   //! <b>Complexity</b>: Average case linear in this->size(), worst case quadratic.
   //! 
   //! <b>Throws</b>: If the hasher functor throws.
   void rehash(bucket_ptr new_buckets, size_type new_size)
   {  table_.rehash(new_buckets, new_size); }

   //! <b>Effects</b>: Returns the nearest new bucket count optimized for
   //!   the container that is bigger than n. This suggestion can be used
   //!   to create bucket arrays with a size that will usually improve
   //!   container's performance.
   //! 
   //! <b>Complexity</b>: Amortized constant time.
   //! 
   //! <b>Throws</b>: Nothing.
   static size_type suggested_upper_bucket_count(size_type n)
   {  return table_type::suggested_upper_bucket_count(n);  }

   //! <b>Effects</b>: Returns the nearest new bucket count optimized for
   //!   the container that is smaller than n. This suggestion can be used
   //!   to create bucket arrays with a size that will usually improve
   //!   container's performance.
   //! 
   //! <b>Complexity</b>: Amortized constant time.
   //! 
   //! <b>Throws</b>: Nothing.
   static size_type suggested_lower_bucket_count(size_type n)
   {  return table_type::suggested_lower_bucket_count(n);  }
/*
   //! <b>Requires</b>: v shall not be in a ihashset of the appropriate type.
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
   {  return table_type::init_node(v);  }

   //! <b>Effects</b>: removes x from a ihashset of the appropriate type. It has no effect,
   //! if x is not in such a ihashset. 
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
   {  table_type::remove_node(v); }
*/
};

//! The class template ihashmultiset is an intrusive container, that mimics most of 
//! the interface of std::unordered_multiset as described in the C++ standard.
//!
//! ihashmultiset is a pseudo-intrusive container: each object to be stored in the
//! container must contain a proper hook, but the container also needs
//! additional auxiliary memory to work: ihashmultiset needs an pointer to an array
//! of type bucket_type to be passed in the constructor. This bucket array must
//! have at least, the same lifetime as the container. This makes the use of
//! ihashmultiset more complicated than purely intrusive containers.
//!
//! ihashmultiset is implemented as an array of singly linked lists. The singly
//! linked list where a value will be inserted is obtained by the value
//! returned by the remainder of the hash function applied to the value and
//! the length of the auxiliary bucket array provided by the user.
//!
//! The template parameter ValueTraits is called "value traits". It stores
//! information and operations about the type to be stored
//! in ilist and what type of hook has been chosen to include it in the list.
//! The value_traits class is supplied by the appropriate hook as a template subtype 
//! called "value_traits".
//!
//! The template parameter Compare, provides a function object that can compare two 
//!   element values as sort keys to determine their relative order in the ihashmultiset. 
//!
//! If the user specifies ConstantTimeSize as "true", a member of type SizeType
//! will be embedded in the class, that will keep track of the number of stored objects.
//! This will allow constant-time O(1) size() member, instead of default O(N) size.
//!
//! ihashmultiset only provides forward iterators but it provides 4 iterator types:
//! iterator and const_iterator to navigate through the whole container and
//! local_iterator and const_local_iterator to navigate through the values
//! stored in a single bucket. local iterators are more faster and smaller.
//!
//! It's not recommended to use non ConstantTimeSize ihashmultisets because several
//! key functions, like "empty()", become non-constant time functions. Non
//! ConstantTimeSize ihashmultisets are mainly provided to support auto-unlink hooks.
//!
//! ihashmultiset, unlike std::unordered_set, does not make automatic rehashings nor
//! offers functions related to a load factor. Rehashing can be explicitly requested
//! and the user must provide a new bucket array that would be used from that moment.
//!
//! Since no automatic rehashing is done, iterator are never invalidated when
//! inserting or erasing elements. Iterators are only invalidated when rehasing.
template< class ValueTraits
        , class Hash             = boost::hash<typename ValueTraits::value_type>
        , class Equal            = std::equal_to<typename ValueTraits::value_type>
        , bool  ConstantTimeSize = false
        , class SizeType         = std::size_t
        >
class ihashmultiset
{
   private:
   typedef detail::ihashtable<ValueTraits, Hash, Equal, ConstantTimeSize, SizeType> table_type;

   //! This class is non-copyable
   ihashmultiset (const ihashmultiset&);

   //!This class is non-assignable
   ihashmultiset &operator =(const ihashmultiset&);

   public:
   typedef typename table_type::value_type            value_type;
   typedef typename table_type::pointer               pointer;
   typedef typename table_type::const_pointer         const_pointer;
   typedef typename table_type::reference             reference;
   typedef typename table_type::const_reference       const_reference;
   typedef typename table_type::size_type             size_type;
   typedef typename table_type::difference_type       difference_type;
   typedef value_type                                 key_type;
   typedef typename table_type::key_equal             key_equal;
   typedef typename table_type::hasher                hasher;
   typedef typename table_type::iterator              iterator;
   typedef typename table_type::const_iterator        const_iterator;
   typedef typename table_type::insert_commit_data    insert_commit_data;
   typedef typename table_type::bucket_type           bucket_type;
   typedef typename table_type::bucket_ptr            bucket_ptr;
   typedef typename table_type::local_iterator        local_iterator;
   typedef typename table_type::const_local_iterator  const_local_iterator;

   private:
   table_type table_;

   public:

   //! <b>Requires</b>: buckets must not be being used by any other resource.
   //!   hasher must be a standard-compatible hash functor and equal must be an
   //!   standard-compatible equality functor.
   //!
   //! <b>Effects</b>: Constructs an empty ihashmultiset, storing a reference
   //!   to the bucket array and copies of the hasher and equal functors.
   //!   
   //! <b>Complexity</b>: Constant. 
   //! 
   //! <b>Throws</b>: Nothing unless the copy constructor of Hash or
   //!   Equal throws. 
   //!
   //! <b>Notes</b>: buckets array must be destroyed after only after
   //!   *this is destroyed. 
   ihashmultiset( bucket_ptr buckets
           , size_type buckets_len
           , const Hash & hasher = Hash()
           , const Equal &equal = Equal()) 
      :  table_(buckets, buckets_len, hasher, equal)
   {}

   //! <b>Requires</b>: buckets must not be being used by any other resource
   //!   and Dereferencing Iterator must yield to an lvalue of type value_type.
   //!   hasher must be a standard-compatible hash functor and equal must be an
   //!   standard-compatible equality functor.
   //! 
   //! <b>Effects</b>: Constructs an empty ihashmultiset and inserts elements from 
   //!   the range [first, last).
   //!   
   //! <b>Complexity</b>: If N is std::distance(b, e): Average case is O(N)
   //!   (with a good has function and with buckets_len >= N),worst case O(N2).
   //! 
   //! <b>Throws</b>: Nothing unless the copy constructor of Hash or
   //!   Equal throws. 
   //!
   //! <b>Notes</b>: buckets array must be destroyed after only after
   //!   *this is destroyed.
   template<class Iterator>
   ihashmultiset( bucket_ptr buckets
           , size_type buckets_len
           , Iterator b
           , Iterator e
           , const Hash & hasher = Hash()
           , const Equal &equal = Equal()) 
      :  table_(buckets, buckets_len, hasher, equal)
   {  table_.insert_equal(b, e);  }

   //! <b>Effects</b>: Detaches all elements from this. The objects in the ihashmultiset 
   //!   are not deleted (i.e. no destructors are called).
   //! 
   //! <b>Complexity</b>: Linear to the number of elements in the ihashmultiset, if 
   //!   it's a safe-mode or auto-unlink value. Otherwise constant.
   //! 
   //! <b>Throws</b>: Nothing.
   ~ihashmultiset() 
   {}

   //! <b>Effects</b>: Returns an iterator pointing to the beginning of the ihashmultiset.
   //! 
   //! <b>Complexity</b>: Amortized constant time.
   //!   Worst case (empty ihashmultiset): O(this->bucket_count())
   //! 
   //! <b>Throws</b>: Nothing.
   iterator begin()
   { return table_.begin();  }

   //! <b>Effects</b>: Returns a const_iterator pointing to the beginning
   //!   of the ihashmultiset.
   //!
   //! <b>Complexity</b>: Amortized constant time.
   //!   Worst case (empty ihashmultiset): O(this->bucket_count())
   //! 
   //! <b>Throws</b>: Nothing.
   const_iterator begin() const
   { return table_.begin();  }

   //! <b>Effects</b>: Returns an iterator pointing to the end of the ihashmultiset.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   iterator end()
   { return table_.end();  }

   //! <b>Effects</b>: Returns a const_iterator pointing to the end of the ihashmultiset.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   const_iterator end() const
   { return table_.end();  }

   //! <b>Effects</b>: Returns the hasher object used by the ihashset.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: If hasher copy-constructor throws.
   hasher hash_function() const
   { return table_.hash_function(); }

   //! <b>Effects</b>: Returns the key_equal object used by the ihashmultiset.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: If key_equal copy-constructor throws.
   key_equal key_eq() const
   { return table_.key_eq(); }

   //! <b>Effects</b>: Returns true is the container is empty.
   //! 
   //! <b>Complexity</b>: if ConstantTimeSize is false, average constant time
   //!   (worst case, with empty() == true): O(this->bucket_count()).
   //!   Otherwise constant.
   //! 
   //! <b>Throws</b>: Nothing.
   bool empty() const
   { return table_.empty(); }

   //! <b>Effects</b>: Returns the number of elements stored in the ihashmultiset.
   //! 
   //! <b>Complexity</b>: Linear to elements contained in *this if
   //!   ConstantTimeSize is false. Constant-time otherwise.
   //! 
   //! <b>Throws</b>: Nothing.
   size_type size() const
   { return table_.size(); }

   //! <b>Requires</b>: the hasher and the equality function unqualified swap
   //!   call should not throw.
   //! 
   //! <b>Effects</b>: Swaps the contents of two ihashmultisets.
   //!   Swaps also the contained equality and hasher functors.
   //! 
   //! <b>Complexity</b>: Constant.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Note</b>: Nothing.
   void swap(ihashmultiset& other)
   { table_.swap(other.table_); }

   //! <b>Requires</b>: val must be an lvalue
   //! 
   //! <b>Effects</b>: Inserts val into the ihashmultiset.
   //!
   //! <b>Returns</b>: An iterator to the new inserted value.
   //! 
   //! <b>Complexity</b>: Average case O(1), worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   //!   No copy-constructors are called.
   iterator insert(value_type& val)
   {  return table_.insert_equal(val);  }

   //! <b>Requires</b>: Dereferencing Iterator must yield to an lvalue 
   //!   of type value_type.
   //! 
   //! <b>Effects</b>: Equivalent to this->insert(t) for each element in [b, e).
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
   {  table_.insert_equal(b, e);  }

   //! <b>Effects</b>: Erases the element pointed to by i. 
   //! 
   //! <b>Complexity</b>: Average case O(1), worst case O(this->size()).
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased element. No destructors are called.
   void erase(iterator i)
   {  table_.erase(i);  }

   //! <b>Effects</b>: Erases the range pointed to by b end e. 
   //! 
   //! <b>Complexity</b>: Average case O(std::distance(b, e)),
   //!   worst case O(this->size()).
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   iterator erase(iterator b, iterator e)
   {  return table_.erase(b, e);  }

   //! <b>Effects</b>: Erases all the elements with the given value.
   //! 
   //! <b>Returns</b>: The number of erased elements.
   //! 
   //! <b>Complexity</b>: Average case O(this->count(value)).
   //!   Worst case O(this->size()).
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   size_type erase(const value_type &value)
   {  return table_.erase(value);  }

   //! <b>Requires</b>: "hasher" must be a hash function that induces 
   //!   the same hash values as the stored hasher. The difference is that
   //!   "hasher" hashes the given key instead of the value_type.
   //!
   //!   "key_value_equal" must be a equality function that induces 
   //!   the same equality as key_equal. The difference is that
   //!   "key_value_equal" compares an arbitrary key with the contained values.
   //!
   //! <b>Effects</b>: Erases all the elements that have the same hash and
   //!   compare equal with the given key.
   //! 
   //! <b>Returns</b>: The number of erased elements.
   //! 
   //! <b>Complexity</b>: Average case O(this->count(value)).
   //!   Worst case O(this->size()).
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   template<class KeyType, class KeyHasher, class KeyValueEqual>
   size_type erase(const KeyType& key, const KeyHasher &hasher, const KeyValueEqual &equal)
   {  return table_.erase(key, hasher, equal);  }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the element pointed to by i. 
   //!   Destroyer::operator()(pointer) is called for the removed element.
   //! 
   //! <b>Complexity</b>: Average case O(1), worst case O(this->size()).
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators 
   //!    to the erased elements.
   template<class Destroyer>
   iterator erase(iterator i, Destroyer destroyer)
   {  return table_.erase(i, destroyer);  }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the range pointed to by b end e.
   //!   Destroyer::operator()(pointer) is called for the removed element.
   //! 
   //! <b>Complexity</b>: Average case O(std::distance(b, e)),
   //!   worst case O(this->size()).
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators
   //!    to the erased elements.
   template<class Destroyer>
   iterator erase(iterator b, iterator e, Destroyer destroyer)
   {  return table_.erase(b, e, destroyer);  }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases all the elements with the given value.
   //!   Destroyer::operator()(pointer) is called for the removed elements.
   //! 
   //! <b>Returns</b>: The number of erased elements.
   //! 
   //! <b>Complexity</b>: Average case O(this->count(value)).
   //!   Worst case O(this->size()).
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!    to the erased elements. No destructors are called.
   template<class Destroyer>
   size_type erase(const value_type &value, Destroyer destroyer)
   {  return table_.erase(value, destroyer);  }

   //! <b>Requires</b>: Destroyer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases all the elements with the given key.
   //!   according to the comparison functor "equal".
   //!   Destroyer::operator()(pointer) is called for the removed elements.
   //!
   //! <b>Returns</b>: The number of erased elements.
   //! 
   //! <b>Complexity</b>: Average case O(this->count(value)).
   //!   Worst case O(this->size()).
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>: Invalidates the iterators
   //!    to the erased elements.
   template<class KeyType, class KeyHasher, class KeyValueEqual, class Destroyer>
   size_type erase(const KeyType& key, const KeyHasher &hasher, const KeyValueEqual &equal, Destroyer destroyer)
   {  return table_.erase(key, hasher, equal, destroyer);  }

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
   {  return table_.clear();  }

   //! <b>Effects</b>: Returns the number of contained elements with the given key
   //! 
   //! <b>Complexity</b>: Average case O(1), worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   size_type count(const value_type &value) const
   {  return table_.count(value);  }

   //! <b>Requires</b>: "hasher" must be a hash function that induces 
   //!   the same hash values as the stored hasher. The difference is that
   //!   "hasher" hashes the given key instead of the value_type.
   //!
   //!   "key_value_equal" must be a equality function that induces 
   //!   the same equality as key_equal. The difference is that
   //!   "key_value_equal" compares an arbitrary key with the contained values.
   //!
   //! <b>Effects</b>: Returns the number of contained elements with the given key
   //!
   //! <b>Complexity</b>: Average case O(1), worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   template<class KeyType, class KeyHasher, class KeyValueEqual, class Destroyer>
   size_type count(const KeyType& key, const KeyHasher &hasher, const KeyValueEqual &equal) const
   {  return table_.count(key, hasher, equal);  }

   //! <b>Effects</b>: Finds a iterator to the first element whose key is 
   //!   k or end() if that element does not exist.
   //!
   //! <b>Complexity</b>: Average case O(1), worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   iterator find(const value_type &value)
   {  return table_.find(value);  }

   //! <b>Requires</b>: "hasher" must be a hash function that induces 
   //!   the same hash values as the stored hasher. The difference is that
   //!   "hasher" hashes the given key instead of the value_type.
   //!
   //!   "key_value_equal" must be a equality function that induces 
   //!   the same equality as key_equal. The difference is that
   //!   "key_value_equal" compares an arbitrary key with the contained values.
   //!
   //! <b>Effects</b>: Finds a iterator to the first element whose key is 
   //!   key according to the given hasher and equality functor or end() if
   //!   that element does not exist.
   //!
   //! <b>Complexity</b>: Average case O(1), worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyHasher, class KeyValueEqual>
   iterator find(const KeyType& key, const KeyHasher &hasher, const KeyValueEqual &equal)
   {  return table_.find(key, hasher, equal);  }

   //! <b>Effects</b>: Finds a const_iterator to the first element whose key is 
   //!   k or end() if that element does not exist.
   //! 
   //! <b>Complexity</b>: Average case O(1), worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   const_iterator find(const value_type &value) const
   {  return table_.find(value);  }

   //! <b>Requires</b>: "hasher" must be a hash function that induces 
   //!   the same hash values as the stored hasher. The difference is that
   //!   "hasher" hashes the given key instead of the value_type.
   //!
   //!   "key_value_equal" must be a equality function that induces 
   //!   the same equality as key_equal. The difference is that
   //!   "key_value_equal" compares an arbitrary key with the contained values.
   //!
   //! <b>Effects</b>: Finds a iterator to the first element whose key is 
   //!   key according to the given hasher and equality functor or end() if
   //!   that element does not exist.
   //! 
   //! <b>Complexity</b>: Average case O(1), worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyHasher, class KeyValueEqual>
   const_iterator find(const KeyType& key, const KeyHasher &hasher, const KeyValueEqual &equal) const
   {  return table_.find(key, equal);  }

   //! <b>Effects</b>: Returns a range containing all elements with values equivalent
   //!   to value. Returns std::make_pair(this->end(), this->end()) if no such 
   //!   elements exist.
   //! 
   //! <b>Complexity</b>: Average case O(this->count(k)). Worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   std::pair<iterator,iterator> equal_range(const value_type &value)
   {  return table_.equal_range(value);  }

   //! <b>Requires</b>: "hasher" must be a hash function that induces 
   //!   the same hash values as the stored hasher. The difference is that
   //!   "hasher" hashes the given key instead of the value_type.
   //!
   //!   "key_value_equal" must be a equality function that induces 
   //!   the same equality as key_equal. The difference is that
   //!   "key_value_equal" compares an arbitrary key with the contained values.
   //!
   //! <b>Effects</b>: Returns a range containing all elements with equivalent
   //!   keys. Returns std::make_pair(this->end(), this->end()) if no such 
   //!   elements exist.
   //! 
   //! <b>Complexity</b>: Average case O(this->count(k)). Worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyHasher, class KeyValueEqual>
   std::pair<iterator,iterator> equal_range(const KeyType& key, const KeyHasher& hasher, KeyValueEqual equal)
   {  return table_.equal_range(key, hasher, equal);  }

   //! <b>Effects</b>: Returns a range containing all elements with values equivalent
   //!   to value. Returns std::make_pair(this->end(), this->end()) if no such 
   //!   elements exist.
   //! 
   //! <b>Complexity</b>: Average case O(this->count(k)). Worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   std::pair<const_iterator, const_iterator>
      equal_range(const value_type &value) const
   {  return table_.equal_range(value);  }

   //! <b>Requires</b>: "hasher" must be a hash function that induces 
   //!   the same hash values as the stored hasher. The difference is that
   //!   "hasher" hashes the given key instead of the value_type.
   //!
   //!   "key_value_equal" must be a equality function that induces 
   //!   the same equality as key_equal. The difference is that
   //!   "key_value_equal" compares an arbitrary key with the contained values.
   //!
   //! <b>Effects</b>: Returns a range containing all elements with equivalent
   //!   keys. Returns std::make_pair(this->end(), this->end()) if no such 
   //!   elements exist.
   //! 
   //! <b>Complexity</b>: Average case O(this->count(k)). Worst case O(this->size()).
   //! 
   //! <b>Throws</b>: If the hasher or the equality functor throws.
   //!
   //! <b>Note</b>: This function is used when constructing a value_type
   //!   is expensive and the value_type can be compared with a cheaper
   //!   key type. Usually this key is part of the value_type.
   template<class KeyType, class KeyHasher, class KeyValueEqual>
   std::pair<const_iterator, const_iterator>
      equal_range(const KeyType& key, const KeyHasher &hasher, const KeyValueEqual &equal) const
   {  return table_.equal_range(key, equal);  }

   //! <b>Requires</b>: val must be an lvalue and shall be in a ihashmultiset of
   //!   appropriate type. Otherwise the behavior is undefined.
   //! 
   //! <b>Effects</b>: Returns: a valid iterator i belonging to the ihashmultiset
   //!   that points to the value
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: If the hash function throws.
   iterator current(value_type& v)
   {  return table_.current(v);  }

   //! <b>Requires</b>: val must be an lvalue and shall be in a ihashmultiset of
   //!   appropriate type. Otherwise the behavior is undefined.
   //! 
   //! <b>Effects</b>: Returns: a valid const_iterator i belonging to the
   //!   ihashmultiset that points to the value
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: If the hash function throws.
   const_iterator current(const value_type& v)
   {  return table_.current(v);  }

   //! <b>Requires</b>: val must be an lvalue and shall be in a ihashmultiset of
   //!   appropriate type. Otherwise the behavior is undefined.
   //! 
   //! <b>Effects</b>: Returns: a valid local_iterator i belonging to the ihashmultiset
   //!   that points to the value
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   static local_iterator current_local(value_type& v)
   {  return table_type::current_local(v);  }

   //! <b>Requires</b>: val must be an lvalue and shall be in a ihashmultiset of
   //!   appropriate type. Otherwise the behavior is undefined.
   //! 
   //! <b>Effects</b>: Returns: a valid const_local_iterator i belonging to
   //!   the ihashmultiset that points to the value
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   static const_local_iterator current_local(const value_type& v)
   {  return table_type::current_local(v);  }

   //! <b>Effects</b>: Returns the number of buckets passed in the constructor
   //!   or the last rehash function.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   size_type bucket_count() const
   {  return table_.bucket_count();   }

   //! <b>Requires</b>: n is in the range [0, this->bucket_count()).
   //!
   //! <b>Effects</b>: Returns the number of elements in the nth bucket.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   size_type bucket_size(size_type n)
   {  return table_.bucket_size(n);   }

   //! <b>Effects</b>: Returns the index of the bucket in which elements
   //!   with keys equivalent to k would be found, if any such element existed.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: If the hash functor throws.
   //!
   //! <b>Note</b>: the return value is in the range [0, this->bucket_count()).
   size_type bucket(const key_type& k)
   {  return table_.bucket(k);   }

   //! <b>Requires</b>: "hasher" must be a hash function that induces 
   //!   the same hash values as the stored hasher. The difference is that
   //!   "hasher" hashes the given key instead of the value_type.
   //!
   //! <b>Effects</b>: Returns the index of the bucket in which elements
   //!   with keys equivalent to k would be found, if any such element existed.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: If the hash functor throws.
   //!
   //! <b>Note</b>: the return value is in the range [0, this->bucket_count()).
   template<class KeyType, class KeyHasher>
   size_type bucket(const KeyType& k, const KeyHasher &hasher)
   {  return table_.bucket(k, hasher);   }

   //! <b>Effects</b>: Returns the bucket array pointer passed in the constructor
   //!   or the last rehash function.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   bucket_ptr bucket_pointer() const
   {  return table_.bucket_pointer();   }

   //! <b>Requires</b>: n is in the range [0, this->bucket_count()).
   //!
   //! <b>Effects</b>: Returns a local_iterator pointing to the beginning
   //!   of the sequence stored in the bucket n.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>:  [b.begin(n), b.end(n)) is a valid range
   //!   containing all of the elements in the nth bucket. 
   local_iterator begin(size_type n)
   {  return table_.begin(n);   }

   //! <b>Requires</b>: n is in the range [0, this->bucket_count()).
   //!
   //! <b>Effects</b>: Returns a const_local_iterator pointing to the beginning
   //!   of the sequence stored in the bucket n.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>:  [b.begin(n), b.end(n)) is a valid range
   //!   containing all of the elements in the nth bucket. 
   const_local_iterator begin(size_type n) const
   {  return table_.begin(n);   }

   //! <b>Requires</b>: n is in the range [0, this->bucket_count()).
   //!
   //! <b>Effects</b>: Returns a local_iterator pointing to the end
   //!   of the sequence stored in the bucket n.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>:  [b.begin(n), b.end(n)) is a valid range
   //!   containing all of the elements in the nth bucket. 
   local_iterator end(size_type n)
   {  return table_.end(n);   }

   //! <b>Requires</b>: n is in the range [0, this->bucket_count()).
   //!
   //! <b>Effects</b>: Returns a const_local_iterator pointing to the end
   //!   of the sequence stored in the bucket n.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Note</b>:  [b.begin(n), b.end(n)) is a valid range
   //!   containing all of the elements in the nth bucket. 
   const_local_iterator end(size_type n) const
   {  return table_.end(n);   }

   //! <b>Requires</b>: new_buckets must be a pointer to a new bucket array
   //!   or the same as this->bucket_pointer(). n is the length of the
   //!   the array pointed by new_buckets. If new_buckets == this->bucket_pointer()
   //!   n can be bigger or smaller than this->bucket_count().
   //!
   //! <b>Effects</b>: Updates the internal reference with the new bucket erases
   //!   the values from the old bucket and inserts then in the new one. 
   //! 
   //! <b>Complexity</b>: Average case linear in this->size(), worst case quadratic.
   //! 
   //! <b>Throws</b>: If the hasher functor throws.
   void rehash(bucket_ptr new_buckets, size_type new_size)
   {  table_.rehash(new_buckets, new_size); }

   //! <b>Effects</b>: Returns the nearest new bucket count optimized for
   //!   the container that is bigger than n. This suggestion can be used
   //!   to create bucket arrays with a size that will usually improve
   //!   container's performance.
   //! 
   //! <b>Complexity</b>: Amortized constant time.
   //! 
   //! <b>Throws</b>: Nothing.
   static size_type suggested_upper_bucket_count(size_type n)
   {  return table_type::suggested_upper_bucket_count(n);  }

   //! <b>Effects</b>: Returns the nearest new bucket count optimized for
   //!   the container that is smaller than n. This suggestion can be used
   //!   to create bucket arrays with a size that will usually improve
   //!   container's performance.
   //! 
   //! <b>Complexity</b>: Amortized constant time.
   //! 
   //! <b>Throws</b>: Nothing.
   static size_type suggested_lower_bucket_count(size_type n)
   {  return table_type::suggested_lower_bucket_count(n);  }
/*
   //! <b>Requires</b>: v shall not be in a ihashmultiset of the appropriate type.
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
   {  return table_type::init_node(v);  }

   //! <b>Effects</b>: removes x from a ihashmultiset of the appropriate type. It has no effect,
   //! if x is not in such a ihashmultiset. 
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
   {  table_type::remove_node(v); }
*/
};

} //namespace intrusive 
} //namespace boost 

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_IHASHSET_HPP
