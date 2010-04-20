// Boost.Tokenmap
//
// Copyright (c) 2009 Slawomir Lisznianski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file tokenamp.hpp
/// \brief Includes the basic tokenmap container

#ifndef BOOST_TOKENMAP_TOKENMAP_HPP
#define BOOST_TOKENMAP_TOKENMAP_HPP

#if defined(_MSC_VER) && (_MSC_VER>=1200)
#pragma once
#endif

#include <boost/config.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <utility>
#include <vector>
#include <stdexcept>
#include <memory>
#include <stdint.h>

namespace boost {

/// \brief Boost.Tokemap library namespace
/**
All the entities in the library are defined in this namespace.
*/
namespace tokenmaps {

/// \brief The node type stores an entry in the tokenmap container.
/** 
In addition to storing an element, the node type provides contextual
information allowing us to link prev->next nodes required, for 
example, during the iteration.
*/
template <typename value__, typename key__>
struct node {
  typedef node<value__, key__>               this_type;
  typedef uint64_t                           hash_type;
  typedef key__                              key_type;
  typedef value__                            mapped_type;
  typedef std::pair<key_type, mapped_type*>  value_type;

  key_type next, prev;
  value_type keyval;
};

template<typename node__>
struct forward_traversal_policy {
  static typename node__::key_type next(node__ * node) {
    return node->next;
  }
};

template<typename node__>
struct backward_traversal_policy {
  static typename node__::key_type next(node__ * node) {
    return node->prev;
  }
};

template <typename value__, typename key__>
class tokenmap;

template <typename node__, typename traversal_mode__>
class node_iterator :
  public boost::iterator_facade<node_iterator<node__, traversal_mode__>, 
                                typename node__::value_type,
                                boost::forward_traversal_tag>
{
  typedef node__                          node_type;
  typedef typename node__::mapped_type    mapped_type;
  typedef typename node__::value_type     value_type;
  typedef typename node__::key_type       key_type;
  typedef tokenmap<mapped_type, key_type> map_type;

  map_type * map_;
  node_type * cur_;

  friend class boost::iterator_core_access;  

public:
  node_iterator(map_type * map, node_type * cur);

  void increment();

  bool equal(node_iterator const & other) const;

  typename node__::value_type & dereference() const;
};

/// \brief The tokenmap class is the entry point to the library.
/** 
The tokenmap is a (perfect) hashmap derivative. Unlike other dictionaries, 
tokenmap generates the keys internally. For this reason, the tokenmap is
not useful if you need to map an externally provided key with the stored 
value. 
Lookup and deletion operations are constant time. Insertion is amortized 
constant time.  Elements are stored on the heap, so returned pointers are 
never invalidated, except when the element itself is erased.
*/
template <typename value__, typename key__ = uint32_t>
class tokenmap
{
public:
  typedef node<value__, key__>            node_type;
  typedef typename node_type::hash_type   hash_type;
  typedef typename node_type::key_type    key_type;
  typedef typename node_type::mapped_type mapped_type;
  typedef typename node_type::value_type  value_type;

  typedef node_iterator<node_type, forward_traversal_policy<node_type> >  iterator;
  typedef node_iterator<node_type, backward_traversal_policy<node_type> > reverse_iterator;

  /** creates tokenmap with a capacity of at least specified value.

    @param seed a value passed to the pseudo-random number generator.
  */
  explicit tokenmap(hash_type seed);

  /** erases all stored elements.
  */
  ~tokenmap();

  /** adds element to the container by value.
    
    @param value value to be copied and stored in the tokenmap.

    @returns a pair of unique integral key, identifying element's location
             within the tokenmap, and a pointer to the stored element.
  */
  value_type insert(mapped_type const & value);

  /** Adds element to the container without copying (transfer of ownership).
    
    @param value A ptr to be inserted into the tokenmap.

    @returns A unique integral key identifying element's location
             within the tokenmap.
  */
  value_type insert(std::auto_ptr<mapped_type> value);

  /** Finds element identified by `key'.
    
    @returns If found, iterator pointing at the element, otherwise end-iterator.
  */
  iterator find(key_type key) const;

  /** Finds element identified by `key'.
    
    @returns If found, pointer to the element, otherwise NULL.
  */
  mapped_type * at(key_type key) const;

  /** Removes the element identified by `key' from the container
    and returns ownership of it.
    
    @returns A valid auto pointer if the element was found, otherwise empty.
  */
  std::auto_ptr<mapped_type> pop(key_type key);

  /** Checks if the element identified by `key' exists within tokenmap.
    
    @returns True if the element was found, otherwise false.
  */
  bool exists(key_type key) const;

  /** Rertuns number of stored elements.

    @returns Count of stored elements.
  */
  key_type size() const;

  /** Informs if container stores at least one element.

    @returns True if container is empty, false otherwise.
  */
  bool empty() const;

  /** Iterators.
  */
  iterator begin();
  iterator end();

  reverse_iterator rbegin();
  reverse_iterator rend();

private:
  friend class node_iterator<node_type, forward_traversal_policy<node_type> >;
  friend class node_iterator<node_type, backward_traversal_policy<node_type> >;

  typedef std::vector<node_type> storage_type;

  hash_type imperfect_hash(hash_type key) const;
  key_type find_free_slot();
  key_type generate_key(key_type ix);
  void use_slot(node_type & slot, key_type key, mapped_type * vptr);

  key_type size_, free_head_, used_head_, used_tail_;
  hash_type seed_;
  storage_type store_;
};

} // namespace tokenmaps
} // namespace boost

template <typename value__, typename key__>
inline boost::tokenmaps::tokenmap<value__, key__>::tokenmap(hash_type seed) :
  size_(0), used_head_(0), used_tail_(0), seed_(seed)
{ }

template <typename value__, typename key__>
inline boost::tokenmaps::tokenmap<value__, key__>::~tokenmap()
{
  for (iterator it = begin(); it != end(); ++it) {
    delete it->second; 
  }
}

template <typename value__, typename key__>
inline typename boost::tokenmaps::tokenmap<value__, key__>::value_type
  boost::tokenmaps::tokenmap<value__, key__>::insert(typename boost::tokenmaps::tokenmap<value__, key__>::mapped_type const & value)
{
  std::auto_ptr<mapped_type> tv(new mapped_type(value));
  return insert(tv);
}

template <typename value__, typename key__>
inline typename boost::tokenmaps::tokenmap<value__, key__>::value_type
  boost::tokenmaps::tokenmap<value__, key__>::insert(std::auto_ptr<typename boost::tokenmaps::tokenmap<value__, key__>::mapped_type> value)
{
  // iterator position and index of free slot 
  key_type free_slot_ix = find_free_slot();
  node_type & slot = store_[free_slot_ix];
  use_slot(slot, generate_key(free_slot_ix), value.release());
  return slot.keyval;
}

template <typename value__, typename key__>
inline typename boost::tokenmaps::tokenmap<value__, key__>::iterator
  boost::tokenmaps::tokenmap<value__, key__>::find(typename boost::tokenmaps::tokenmap<value__, key__>::key_type key) const
{
  node_type * node_p = 0;
  if (size_ > 0) {
    // create mask of portion of the key used for indexing
    key_type ix = store_.size() - 1;
    // mask out random portion of the key
    ix &= key;
    // fetch the stored element
    node_p = &store_[ix];
    if (node_p->keyval.first != key) {
      node_p = 0;
    }
  }
  return iterator(this, node_p);
}

template <typename value__, typename key__>
inline typename boost::tokenmaps::tokenmap<value__, key__>::mapped_type *
  boost::tokenmaps::tokenmap<value__, key__>::at(typename boost::tokenmaps::tokenmap<value__, key__>::key_type key) const
{
  if (size_ > 0) {
    key_type ix = store_.size() - 1;
    ix &= key;
    node_type const & node = store_[ix];

    if (node.keyval.first == key) {
      return node.keyval.second;
    }
  }
  return 0;
}

template <typename value__, typename key__>
inline std::auto_ptr<typename boost::tokenmaps::tokenmap<value__, key__>::mapped_type>
  boost::tokenmaps::tokenmap<value__, key__>::pop(typename boost::tokenmaps::tokenmap<value__, key__>::key_type key)
{
  std::auto_ptr<mapped_type> ptr;

  // create mask of portion of the key used for indexing
  key_type ixmask = store_.size() - 1;

  // mask out random portion of the key
  key_type ix = key & ixmask;

  // fetch the stored element
  node_type & cur_node = store_[ix];

  if (cur_node.keyval.first == key)
  {
    ptr.reset(cur_node.keyval.second);

    // Pop the slot from the used-slots list
    if (used_head_ == key) {
      used_head_ = cur_node.next;
    }

    if (used_tail_ == key) {
      used_tail_ = cur_node.prev;
    }

    node_type & prev_node = store_[cur_node.prev & ixmask];
    if (cur_node.next == key) { // last?
      prev_node.next = cur_node.prev;
    }
    else {
      prev_node.next = cur_node.next;
    }

    node_type & next_node = store_[cur_node.next & ixmask];
    if (cur_node.prev == key) { // first?
      next_node.prev = cur_node.next;
    }
    else {
      next_node.prev = cur_node.prev;
    }

    // Push the slot onto free-slots list
    if (size_ < store_.capacity()) {
      cur_node.next = free_head_;
    }
    else {
      cur_node.next = ix;
    }
    free_head_ = ix;

#ifndef NDEBUG
    // reset the node to ease debugging
    cur_node.keyval.first = ix;
    cur_node.keyval.second = 0;
    cur_node.prev = 0;
#endif

    // update free-list and element-count
    --size_;
  }

  return ptr;
}

template <typename value__, typename key__>
inline bool
  boost::tokenmaps::tokenmap<value__, key__>::exists(typename boost::tokenmaps::tokenmap<value__, key__>::key_type key) const
{
  return at(key) != 0;
}

template <typename value__, typename key__>
inline typename boost::tokenmaps::tokenmap<value__, key__>::key_type
  boost::tokenmaps::tokenmap<value__, key__>::size() const
{
  return size_;
}

template <typename value__, typename key__>
inline bool
  boost::tokenmaps::tokenmap<value__, key__>::empty() const
{
  return size_ == 0;
}

template <typename value__, typename key__>
inline typename boost::tokenmaps::tokenmap<value__, key__>::iterator
  boost::tokenmaps::tokenmap<value__, key__>::begin()
{
  node_type * node_p = 0;
  if (size_ > 0) {
    key_type ix = store_.size() - 1;
    ix &= used_head_;
    node_p = &store_[ix];
  }
  return iterator(this, node_p);
}

template <typename value__, typename key__>
inline typename boost::tokenmaps::tokenmap<value__, key__>::iterator
  boost::tokenmaps::tokenmap<value__, key__>::end()
{
  return iterator(this, 0);
}

template <typename value__, typename key__>
inline typename boost::tokenmaps::tokenmap<value__, key__>::reverse_iterator
  boost::tokenmaps::tokenmap<value__, key__>::rbegin()
{
  node_type * node_p = 0;
  if (size_ > 0) {
    key_type ix = store_.size() - 1;
    ix &= used_tail_;
    node_p = &store_[ix];
  }
  return reverse_iterator(this, node_p);
}

template <typename value__, typename key__>
inline typename boost::tokenmaps::tokenmap<value__, key__>::reverse_iterator
  boost::tokenmaps::tokenmap<value__, key__>::rend()
{
  return reverse_iterator(this, 0);
}

template <typename value__, typename key__>
inline typename boost::tokenmaps::tokenmap<value__, key__>::hash_type
  boost::tokenmaps::tokenmap<value__, key__>::imperfect_hash(hash_type key) const
{
  // retry if hashed value == 0 (reserved value)
  while(true)
  {
    key = (~key) + (key << 21); // key = (key << 21) - key - 1;
    key = key ^ (key >> 24);
    key = (key + (key << 3)) + (key << 8); // key * 265
    key = key ^ (key >> 14);
    key = (key + (key << 2)) + (key << 4); // key * 21
    key = key ^ (key >> 28);
    key = key + (key << 31);

    if (key != 0)
      break;
  }
  return key;
}

template <typename value__, typename key__>
inline typename boost::tokenmaps::tokenmap<value__, key__>::key_type
  boost::tokenmaps::tokenmap<value__, key__>::find_free_slot()
{
  // are we full?
  if (!(size_ < store_.capacity())) {

    // Set the beginning of the free-nodes chain
    free_head_ = 0;

    if (store_.empty()) {
      // First time around, we are zero-length
      store_.resize(1);
      node_type & cnode = store_[0];
      cnode.prev = cnode.next = 0;
    }
    else {
      // resize to fill up the new capacity
      storage_type old_store;
      store_.swap(old_store);

      store_.resize(old_store.size() << 1);

      // link `free nodes'
      for (key_type ix = 0; ix < store_.capacity(); ++ix) {
        node_type & cnode = store_[ix];
        cnode.prev = ix-1;
        cnode.next = ix+1;
      }

      // Fix tails such that:
      // -- first node's 'prev' points at self, and
      // -- last node's 'next' points at self
      // Later we use this property to designate one-past-last
      // iterator position.
      store_[0].prev = 0;
      store_[store_.size()-1].next = store_.size()-1;

      // create mask from portion of the key used for indexing
      key_type tixmask = store_.size() - 1;

      // don't count copied elements
      size_ = 0;

      // iterate over existing elements and copy each one.
      for (typename storage_type::iterator cur = old_store.begin(); cur != old_store.end(); ++cur) {
        key_type ix = tixmask & cur->keyval.first;
        node_type & dst_node = store_[ix];

        // if we're not the last or first node, we remove this node 
        // from the free list by linking adjacent nodes
        if (dst_node.next != ix) { // not last?
          store_[dst_node.prev].next = dst_node.next;
        }
        else { // this was last node, turn prev-node into last
          store_[dst_node.prev].next = dst_node.prev;
        }

        if (dst_node.prev != ix) { // not first?
          store_[dst_node.next].prev = dst_node.prev;
        }
        else { // this was first node, turn next-node into first
          store_[dst_node.next].prev = dst_node.next;
        }

        // adjacent nodes linked, ready to overwrite.
        use_slot(dst_node, cur->keyval.first, cur->keyval.second);
      }
    }

    // Room has been made so call recursively and return 
    // a valid slot.
    return find_free_slot();
  }
  return free_head_;
}

template <typename value__, typename key__>
inline typename boost::tokenmaps::tokenmap<value__, key__>::key_type
  boost::tokenmaps::tokenmap<value__, key__>::generate_key(typename boost::tokenmaps::tokenmap<value__, key__>::key_type ix)
{
  // create mask of portion of the key used for indexing
  key_type key = key_type(imperfect_hash(++seed_));
  key &= ~(store_.size() - 1); // mask out index part
  key |= ix; // fill in the index into masked out portion
  return key;
}

template <typename value__, typename key__>
inline void
  boost::tokenmaps::tokenmap<value__, key__>::use_slot(typename boost::tokenmaps::tokenmap<value__, key__>::node_type & slot, 
                                                       typename boost::tokenmaps::tokenmap<value__, key__>::key_type key, 
                                                       typename boost::tokenmaps::tokenmap<value__, key__>::mapped_type * vptr)
{
  // create mask of portion of the key used for indexing
  key_type ixmask = store_.size() - 1;
  key_type ix = ixmask & key;
  slot.keyval = value_type(key, vptr);

  // Pop the slot from the free-slots list
  if (free_head_ == ix) {
    free_head_ = store_[free_head_].next;
  }

  // Push the slot onto the used-slots list
  if (size_ == 0) {
    slot.prev = slot.next = used_head_ = used_tail_ = key;
  }
  else {
    store_[used_head_ & ixmask].prev = key;
    slot.next = used_head_;
    slot.prev = key;
    used_head_ = key;
  }

  // update elements count
  ++size_;
}

template <typename node__, typename traversal_mode__>
inline 
  boost::tokenmaps::node_iterator<node__, traversal_mode__>::node_iterator(map_type * map, node__* cur) :
    map_(map), cur_(cur)
{ }

template <typename node__, typename traversal_mode__>
inline void
  boost::tokenmaps::node_iterator<node__, traversal_mode__>::increment()
{ 
  if (cur_) {
    if (cur_->keyval.first == traversal_mode__::next(cur_)) {
      cur_ = 0;
    }
    else {
      key_type ix = map_->store_.size() - 1;
      ix &= traversal_mode__::next(cur_);
      cur_ = &map_->store_[ix];
    }
  }
}

template <typename node__, typename traversal_mode__>
inline bool
  boost::tokenmaps::node_iterator<node__, traversal_mode__>::equal(boost::tokenmaps::node_iterator<node__, traversal_mode__> const & other) const
{
  return cur_ == other.cur_;
}

template <typename node__, typename traversal_mode__>
inline typename node__::value_type &
  boost::tokenmaps::node_iterator<node__, traversal_mode__>::dereference() const
{
  return cur_->keyval;
}

#endif // BOOST_TOKENMAP_TOKENMAP_HPP

