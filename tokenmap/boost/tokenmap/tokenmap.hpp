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
#include <utility>
#include <vector>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <memory>
#include <cmath>
#include <stdint.h>

/// \brief The namespace where all the boost libraries lives.

namespace boost {

/// \brief Boost.Tokemap library namespace
/**
All the entities in the library are defined in this namespace.
*/
namespace tokenmaps {

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
  typedef uint64_t                           hash_type;
  typedef key__                              key_type;
  typedef value__                            mapped_type;
  typedef std::pair<key_type, mapped_type*>  value_type;

  /** Creates tokenmap with a capacity of at least specified value.

    @param capacity A hint on initial size of the container.
    @param load_factor The average number of elements per bucket
    @param seed A value passed to the pseudo-random number generator.
  */
  tokenmap(size_t capacity, float load_factor, hash_type seed);

  /** Erases all stored elements.
  */
  ~tokenmap();

  /** Adds element to the container by value.
    
    @param value Value to be copied and stored in the tokenmap.

    @returns A unique integral key identifying element's location
             within the tokenmap.
  */
  value_type insert(mapped_type const & value);

  /** Adds element to the container without copying (transfer of ownership).
    
    @param value A ptr to be inserted into the tokenmap.

    @returns A unique integral key identifying element's location
             within the tokenmap.
  */
  value_type insert(std::auto_ptr<mapped_type> value);

  /** Finds element identified by `key'.
    
    @returns If found, pointer to the element, otherwise NULL.
  */
  mapped_type * find(key_type key) const;

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

private:
  typedef std::vector<value_type> storage_type;

  hash_type imperfect_hash(hash_type key) const;
  std::pair<typename storage_type::iterator, key_type> find_free_slot();

  const float load_factor_;
  key_type filled_, at_count_, prev_ix_;
  hash_type seed_;
  storage_type store_;
};

} // namespace tokenmaps
} // namespace boost

template <typename value__, typename key__>
inline boost::tokenmaps::tokenmap<value__, key__>::tokenmap(size_t capacity, float load_factor, hash_type seed) :
  load_factor_(load_factor), filled_(0), at_count_(0), prev_ix_(0), seed_(seed)
{
  // set minimum capacity
  capacity = std::max(size_t(8), capacity);

  // We round up the capacity to optimize modulo computation later on.
  //
  double target_cap = (1 << key_type(ceil(log(capacity)/log(2))));
  if (target_cap > std::numeric_limits<key_type>::max())
    throw std::range_error("tokenmap: requested capacity (after rounding) exceeds maximum key length");
  store_.resize(typename storage_type::size_type(target_cap));
}

template <typename value__, typename key__>
inline boost::tokenmaps::tokenmap<value__, key__>::~tokenmap()
{
  for (typename storage_type::iterator it = store_.begin(); it != store_.end(); ++it) {
    if (it->second != 0) 
      delete (it->second); 
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
  std::pair<typename storage_type::iterator, key_type> found = 
    find_free_slot();

  // are we full?
  if (found.first == store_.end())
  {
    // create temp. storage to expand into
    storage_type tstore_;

    // check if the new capacity is within the key-type's range
    double target_cap = store_.capacity() << 1;
    if (target_cap > std::numeric_limits<key_type>::max())
      throw std::range_error("tokenmap: capacity limit reached");

    // resize to fill up the new capacity
    tstore_.resize(store_.size() << 1);

    // create mask from portion of the key used for indexing
    key_type tixmask = tstore_.size() - 1;

    // iterate over existing elements and copy each one.
    for (typename storage_type::iterator cur = store_.begin(); cur != store_.end(); ++cur)
      tstore_[tixmask & cur->first] = *cur;

    // all copied, swap the containers
    store_.swap(tstore_);

    // call recursively, this time insertion will work
    return insert(value);
  }
  else // not filled up, used returned slot
  {
    value_type val(found.second, value.release());
    *(found.first) = val;
  }

  return *(found.first);
}

template <typename value__, typename key__>
inline typename boost::tokenmaps::tokenmap<value__, key__>::mapped_type *
  boost::tokenmaps::tokenmap<value__, key__>::find(typename boost::tokenmaps::tokenmap<value__, key__>::key_type key) const
{
  // create mask of portion of the key used for indexing
  key_type ixmask = store_.capacity() - 1;

  // mask out random portion of the key
  key_type ix = key & ixmask;

  // fetch the stored element
  value_type res = store_[ix];

  return (res.first == key) ? res.second : 0;
}

template <typename value__, typename key__>
inline std::auto_ptr<typename boost::tokenmaps::tokenmap<value__, key__>::mapped_type>
  boost::tokenmaps::tokenmap<value__, key__>::pop(typename boost::tokenmaps::tokenmap<value__, key__>::key_type key)
{
  // create mask of portion of the key used for indexing
  key_type ixmask = store_.capacity() - 1;

  // mask out random portion of the key
  key_type ix = key & ixmask;

  // fetch the stored element
  value_type & res = store_[ix];
  std::auto_ptr<mapped_type> ptr;

  if (res.first == key)
  {
    --filled_;
    ptr.reset(res.second);
    res = value_type();
  }
  return ptr;
}

template <typename value__, typename key__>
inline bool
  boost::tokenmaps::tokenmap<value__, key__>::exists(typename boost::tokenmaps::tokenmap<value__, key__>::key_type key) const
{
  return find(key) != 0;
}

template <typename value__, typename key__>
inline typename boost::tokenmaps::tokenmap<value__, key__>::key_type
  boost::tokenmaps::tokenmap<value__, key__>::size() const
{
  return filled_;
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
inline std::pair<
  typename boost::tokenmaps::tokenmap<value__, key__>::storage_type::iterator, 
  typename boost::tokenmaps::tokenmap<value__, key__>::key_type
  >  
  boost::tokenmaps::tokenmap<value__, key__>::find_free_slot()
{
  // create mask of portion of the key used for indexing
  key_type ixmask = store_.capacity() - 1;

  // check if load factor exceeded 
  if ((++at_count_ & (ixmask>>2)) == ixmask>>2)
    if ((float(filled_)/store_.capacity()) >= load_factor_)
      return std::pair<typename storage_type::iterator, key_type>(store_.end(), 0);

  // generate random key and mask out index portion
  key_type hash = key_type(imperfect_hash(++seed_));
  key_type rand = hash & ~ixmask; // variable part

  // first attempt: to avoid cache misses, we derive index position from 
  // prev position
  key_type ix = ++prev_ix_;
  typename storage_type::iterator pos = store_.begin();
  pos += ix;

  if (pos->first == 0) // if key == 0 then empty slot
  {
    ++filled_;
    return std::pair<typename storage_type::iterator, key_type>(pos, rand|ix);
  }
  
  // second attempt: this time we generate a random index pos.
  //
  pos = store_.begin();
  ix = hash & ixmask;
  pos += ix;

  // store random pos in case we reach end and 
  // have to start from beg, down below.
  typename storage_type::iterator const rndpos = pos;

  // scan until we reach end of container
  //
  for (; pos != store_.end(); ++pos, ++ix)
    if (pos->first == 0) // if key == 0 then empty slot
    {
      ++filled_;
      prev_ix_ = ix;
      return std::pair<typename storage_type::iterator, key_type>(pos, rand|ix);
    }
  
  // last attempt: re-start search from beginning...
  pos = store_.begin(); ix = 0;
  for (; pos != rndpos; ++pos, ++ix)
    if (pos->first == 0) // if key == 0 then empty slot
    {
      ++filled_;
      prev_ix_ = ix;
      return std::pair<typename storage_type::iterator, key_type>(pos, rand|ix);
    }

  // give up, resize
  return std::pair<typename storage_type::iterator, key_type>(store_.end(), 0);
}

#endif // BOOST_TOKENMAP_TOKENMAP_HPP

