//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <memory>
#include <vector>
#include <set>
#include <iostream>
#include <functional>

#include <boost/shmem/named_shared_object.hpp>
#include <boost/shmem/containers/flat_set.hpp>
#include <boost/shmem/containers/flat_map.hpp>
#include <boost/shmem/indexes/flat_map_index.hpp>
#include <boost/shmem/allocators/allocator.hpp>
#include "printcontainer.hpp"

/*****************************************************************/
/*                                                               */
/*  This example repeats the same operations with std::set and   */
/*  shmem_set using the node allocator                           */
/*  and compares the values of both containers                   */
/*                                                               */
/*****************************************************************/

//Explicit instantiation to detect compilation errors
template class boost::shmem::flat_set<int, std::less<int>, std::allocator<int> >;
template class boost::shmem::flat_multiset<int, std::less<int>, std::allocator<int> >;
template class boost::shmem::flat_map<int, int, std::less<int>, std::allocator<std::pair<int, int> > >;
template class boost::shmem::flat_multimap<int, int, std::less<int>, std::allocator<std::pair<int, int> > >;

using namespace boost::shmem;

//Customize named_shared_object class
typedef basic_named_shared_object
   <char,
    simple_seq_fit<shared_mutex_family>,
    flat_map_index
   > my_named_shared_object;

//Alias allocator type
typedef allocator<int, named_shared_object::segment_manager> 
   shmem_node_allocator_t;

//Alias set types
typedef flat_set<int, std::less<int>, shmem_node_allocator_t>        MyShmSet;
typedef std::set<int>                                                MyStdSet;
typedef flat_multiset<int, std::less<int>, shmem_node_allocator_t>   MyShmMultiSet;
typedef std::multiset<int>                                           MyStdMultiSet;

//Function to check if both sets are equal
bool CheckEqual(MyShmSet *shmset, MyStdSet *stdset)
{
   if(shmset->size() != stdset->size())
      return false;
   return std::equal(shmset->begin(), shmset->end(), stdset->begin());
}

//Function to check if both sets are equal
bool CheckEqual(MyShmMultiSet *shmmultiset, MyStdMultiSet *stdmultiset)
{
   if(shmmultiset->size() != stdmultiset->size())
      return false;
   return std::equal(shmmultiset->begin(), shmmultiset->end(), stdmultiset->begin());
}

int main ()
{
   const int memsize = 655360;
   const char *const shMemName = "/MySharedMemory";
   const int max = 100;

   //Named new capable shared mem allocator
   my_named_shared_object segment;

   //Create shared memory
   segment.create(shMemName, memsize);

   segment.reserve_named_objects(100);

   //Shared memory allocator must be always be initialized
   //since it has no default constructor
   MyShmSet *shmset = segment.construct<MyShmSet>("MyShmSet")
                           (std::less<int>(), segment.get_segment_manager());

   MyStdSet *stdset = new MyStdSet;

   MyShmMultiSet *shmmultiset = segment.construct<MyShmMultiSet>("MyShmMultiSet")
                           (std::less<int>(), segment.get_segment_manager());

   MyStdMultiSet *stdmultiset = new MyStdMultiSet;


   int i, j;
   for(i = 0; i < max; ++i){
      shmset->insert(i);
      stdset->insert(i);
      shmmultiset->insert(i);
      stdmultiset->insert(i);
   }
   if(!CheckEqual(shmset, stdset)) return -1;
   if(!CheckEqual(shmmultiset, stdmultiset)) return -1;

   MyShmSet::iterator it;
   MyShmSet::const_iterator cit = it;

   shmset->erase(shmset->begin()++);
   stdset->erase(stdset->begin()++);
   shmmultiset->erase(shmmultiset->begin()++);
   stdmultiset->erase(stdmultiset->begin()++);
   if(!CheckEqual(shmset, stdset)) return -1;
   if(!CheckEqual(shmmultiset, stdmultiset)) return -1;

   shmset->erase(shmset->begin());
   stdset->erase(stdset->begin());
   shmmultiset->erase(shmmultiset->begin());
   stdmultiset->erase(stdmultiset->begin());
   if(!CheckEqual(shmset, stdset)) return -1;
   if(!CheckEqual(shmmultiset, stdmultiset)) return -1;

   std::vector<int> aux_vect;
   #if !BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)
   aux_vect.assign(50, -1);
   shmset->insert(aux_vect.begin(), aux_vect.end());
   stdset->insert(aux_vect.begin(), aux_vect.end());
   shmmultiset->insert(aux_vect.begin(), aux_vect.end());
   stdmultiset->insert(aux_vect.begin(), aux_vect.end());
   if(!CheckEqual(shmset, stdset)) return -1;
   if(!CheckEqual(shmmultiset, stdmultiset)) return -1;
   #endif

   #if !BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)
   for(i = 0, j = static_cast<int>(shmset->size()); i < j; ++i){
      shmset->erase(aux_vect[i]);
      stdset->erase(aux_vect[i]);
      shmmultiset->erase(aux_vect[i]);
      stdmultiset->erase(aux_vect[i]);
   }
   if(!CheckEqual(shmset, stdset)) return -1;
   if(!CheckEqual(shmmultiset, stdmultiset)) return -1;
   #endif

   #if !BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)
   shmset->insert(aux_vect.begin(), aux_vect.end());
   shmset->insert(aux_vect.begin(), aux_vect.end());
   stdset->insert(aux_vect.begin(), aux_vect.end());
   stdset->insert(aux_vect.begin(), aux_vect.end());
   shmmultiset->insert(aux_vect.begin(), aux_vect.end());
   shmmultiset->insert(aux_vect.begin(), aux_vect.end());
   stdmultiset->insert(aux_vect.begin(), aux_vect.end());
   stdmultiset->insert(aux_vect.begin(), aux_vect.end());
   if(!CheckEqual(shmset, stdset)) return -1;
   if(!CheckEqual(shmmultiset, stdmultiset)) return -1;
   #endif

   shmset->erase(*shmset->begin());
   stdset->erase(*stdset->begin());
   shmmultiset->erase(*shmmultiset->begin());
   stdmultiset->erase(*stdmultiset->begin());

   if(!CheckEqual(shmset, stdset)) return -1;
   if(!CheckEqual(shmmultiset, stdmultiset)) return -1;

   for(i = 0; i < max; ++i){
      shmset->insert(i);
      stdset->insert(i);
      shmmultiset->insert(i);
      stdmultiset->insert(i);
   }
   if(!CheckEqual(shmset, stdset)) return -1;
   if(!CheckEqual(shmmultiset, stdmultiset)) return -1;

   segment.destroy<MyShmSet>("MyShmSet");
   delete stdset;
   segment.destroy<MyShmMultiSet>("MyShmMultiSet");
   delete stdmultiset;

   return 0;
}




