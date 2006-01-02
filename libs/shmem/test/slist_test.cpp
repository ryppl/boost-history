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
#include <iostream>
#include <functional>

#include <boost/shmem/named_shared_object.hpp>
#include <boost/shmem/containers/slist.hpp>
#include <boost/shmem/allocators/node_allocator.hpp>
#include <list>
#include "printcontainer.hpp"


/*****************************************************************/
/*                                                               */
/*  This example repeats the same operations with std::list and  */
/*  shmem_slist using the node allocator                         */
/*  and compares the values of both containers                   */
/*                                                               */
/*****************************************************************/

//Explicit instantiation to detect compilation errors
template class boost::shmem::slist<int, std::allocator<int> >;

using namespace boost::shmem;

//We will work with narrow characters for shared memory objects
//Alias <integer, 64 element per chunk> node allocator type
typedef node_allocator<int, 64, named_shared_object::segment_manager>
   shmem_node_allocator_t;

//Alias slist types
typedef slist<int, shmem_node_allocator_t>   MyShmSlist;
typedef std::list<int>                       MyStdList;

//Function to check if both slists are equal
bool CheckEqual(MyShmSlist *shmslist, MyStdList *stdlist)
{
   return std::equal(shmslist->begin(), shmslist->end(), stdlist->begin());
}

int main ()
{
   const int memsize = 655360;
   const char *const shMemName = "/MySharedMemory";
   const int max = 100;

   //Named new capable shared mem allocator
   named_shared_object segment;

   //Create shared memory
   segment.create(shMemName, memsize);

   segment.reserve_named_objects(100);

   //Shared memory allocator must be always be initialized
   //since it has no default constructor
   MyShmSlist *shmslist = segment.construct<MyShmSlist>("MyShmSlist")
                           (segment.get_segment_manager());

   MyStdList *stdlist = new MyStdList;

   int i, j;
   for(i = 0; i < max; ++i){
      shmslist->insert(shmslist->begin(), i);
      stdlist->insert(stdlist->begin(), i);
   }
   if(!CheckEqual(shmslist, stdlist)) return -1;

   MyShmSlist::iterator it;
   MyShmSlist::const_iterator cit = it;

   shmslist->erase(shmslist->begin()++);
   stdlist->erase(stdlist->begin()++);
   if(!CheckEqual(shmslist, stdlist)) return -1;

   shmslist->erase(shmslist->begin());
   stdlist->erase(stdlist->begin());
   if(!CheckEqual(shmslist, stdlist)) return -1;

   //Swapping test
   MyShmSlist tmpshmslist2 (segment.get_segment_manager());
   MyStdList tmpstdlist2;
   shmslist->swap(tmpshmslist2);
   stdlist->swap(tmpstdlist2);
   shmslist->swap(tmpshmslist2);
   stdlist->swap(tmpstdlist2);
   if(!CheckEqual(shmslist, stdlist)) return -1;

   //Insertion from other container
   std::vector<int> aux_vect;
   #if !BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)
   aux_vect.assign(50, -1);
   shmslist->insert(shmslist->begin(), aux_vect.begin(), aux_vect.end());
   stdlist->insert(stdlist->begin(), aux_vect.begin(), aux_vect.end());
   if(!CheckEqual(shmslist, stdlist)) return -1;
   #endif

   #if !BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)
   for(i = 0, j = static_cast<int>(shmslist->size()); i < j; ++i){
      shmslist->erase(shmslist->begin());
      stdlist->erase(stdlist->begin());
   }
   if(!CheckEqual(shmslist, stdlist)) return -1;
   #endif

   #if !BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)
   shmslist->insert(shmslist->begin(), aux_vect.begin(), aux_vect.end());
   stdlist->insert(stdlist->begin(), aux_vect.begin(), aux_vect.end());
   if(!CheckEqual(shmslist, stdlist)) return -1;
   #endif

   shmslist->remove(*shmslist->begin());
   stdlist->remove(*stdlist->begin());
   if(!CheckEqual(shmslist, stdlist)) return -1;

   #if !BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)
   shmslist->insert(shmslist->begin(), aux_vect.begin(), aux_vect.end());
   stdlist->insert(stdlist->begin(), aux_vect.begin(), aux_vect.end());
   if(!CheckEqual(shmslist, stdlist)) return -1;
   #endif

   tmpshmslist2.assign(shmslist->begin(), shmslist->end());
   tmpstdlist2.assign(stdlist->begin(), stdlist->end());
   shmslist->insert(shmslist->begin(), aux_vect.begin(), aux_vect.end());
   stdlist->insert(stdlist->begin(), aux_vect.begin(), aux_vect.end());
   shmslist->splice(++shmslist->begin(), tmpshmslist2);
   stdlist->splice(++stdlist->begin(), tmpstdlist2);
   if(!CheckEqual(shmslist, stdlist)) return -1;

   shmslist->sort();
   stdlist->sort();
   if(!CheckEqual(shmslist, stdlist)) return -1;

   tmpshmslist2.assign(shmslist->begin(), shmslist->end());
   tmpstdlist2.assign(stdlist->begin(), stdlist->end());
   shmslist->merge(tmpshmslist2);
   stdlist->merge(tmpstdlist2);
   if(!CheckEqual(shmslist, stdlist)) return -1;

   segment.destroy<MyShmSlist>("MyShmSlist");
   delete stdlist;
   return 0;
}




