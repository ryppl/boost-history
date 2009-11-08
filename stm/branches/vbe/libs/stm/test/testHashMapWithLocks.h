//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
#ifndef HASH_MAP_WITH_LOCKS_H
#define HASH_MAP_WITH_LOCKS_H

///////////////////////////////////////////////////////////////////////////////
#include <boost/stm/transaction.hpp>
#include "testLinkedListWithLocks.h"

void TestHashMapWithLocks();

namespace LATM
{
   namespace nHashMap
   {
      int const kBuckets2 = 256;
   }

///////////////////////////////////////////////////////////////////////////////
template <typename T>
class HashMap
{
public:

   bool lock_lookup(T &val)
   {
      return buckets_[val % nHashMap::kBuckets2].lock_lookup(val);
   }

   bool lock_insert(list_node<T> &element)
   {
      return buckets_[element.value() % nHashMap::kBuckets2].lock_insert(element);
   }

   bool lookup(T &val)
   {
      return buckets_[val % nHashMap::kBuckets2].lookup(val);
   }

   bool insert(list_node<T> &element)
   {
      return buckets_[element.value() % nHashMap::kBuckets2].insert(element);
   }

   bool remove(list_node<T> &v)
   {
      return buckets_[v.value() % nHashMap::kBuckets2].remove(v);
   }

   size_t walk_size()
   {
      size_t count = 0;
      for (int i = 0; i < nHashMap::kBuckets2; ++i)
      {
         count += buckets_[i].walk_size();
      }

      return count;
   }

   ////////////////////////////////////////////////////////////////////////////
   void outputList(std::ofstream &o)
   {
      for (int i = 0; i < nHashMap::kBuckets2; ++i)
      {
         buckets_[i].outputList(o);
      }
   }

   pthread_mutex_t* get_hash_lock(int val) { return buckets_[val].get_list_lock(); }

private:
   LATM::LinkedList<T> buckets_[nHashMap::kBuckets2];
};

} // namespace LATM

#endif // HASH_MAP_H
