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
#include <vector>
#include <list>
#include <iostream>
#include <functional>

#include <boost/shmem/named_shared_object.hpp>
#include <boost/shmem/allocators/cached_node_allocator.hpp>
#include <boost/shmem/containers/vector.hpp>
#include <boost/shmem/containers/list.hpp>
#include "printcontainer.hpp"

/*****************************************************************/
/*                                                               */
/*  This example repeats the same operations with std::list and  */
/*  shmem_list using the cached node allocator                   */
/*  and compares the values of both containers                   */
/*                                                               */
/*****************************************************************/

using namespace boost::shmem;

//We will work with wide characters for shared memory objects
//Alias <integer, 64 element per chunk> node allocator type
typedef cached_node_allocator
   <int, wnamed_shared_object::segment_manager>
   cached_node_allocator_t;

//Alias list types
typedef list<int, cached_node_allocator_t>   MyShmList;
typedef std::list<int>                       MyStdList;

//Function to check if both lists are equal
bool CheckEqual(MyShmList *shmlist, MyStdList *stdlist)
{
   return std::equal(shmlist->begin(), shmlist->end(), stdlist->begin());
}

int main ()
{
   const int memsize = 1048576;
   const char *const shMemName = "/MySharedMemory";
   const int max = 100;

   //Named new capable shared mem allocator
   wnamed_shared_object segment;

   //Create shared memory
   segment.create(shMemName, memsize);

   //Shared memory allocator must be always be initialized
   //since it has no default constructor
   MyShmList *shmlist = segment.construct<MyShmList>
                           (L"MyVect")
                           (segment.get_segment_manager());

   MyStdList *stdlist = new MyStdList;

   int i;
   for(i = 0; i < max; ++i){
      shmlist->push_back(i);
      stdlist->push_back(i);
   }
   if(!CheckEqual(shmlist, stdlist)) return -1;

   shmlist->erase(shmlist->begin()++);
   stdlist->erase(stdlist->begin()++);
   if(!CheckEqual(shmlist, stdlist)) return -1;

   shmlist->pop_back();
   stdlist->pop_back();
   if(!CheckEqual(shmlist, stdlist)) return -1;

   shmlist->pop_front();
   stdlist->pop_front();
   if(!CheckEqual(shmlist, stdlist)) return -1;

   std::vector<int> aux_vect;
   #if !BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)
   aux_vect.assign(50, -1);
   shmlist->assign(aux_vect.begin(), aux_vect.end());
   stdlist->assign(aux_vect.begin(), aux_vect.end());
   if(!CheckEqual(shmlist, stdlist)) return -1;
   #endif

   shmlist->sort();
   stdlist->sort();
   if(!CheckEqual(shmlist, stdlist)) return -1;

   #if !BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)
   aux_vect.assign(50, 0);
   #endif
   shmlist->insert(shmlist->begin(), aux_vect.begin(), aux_vect.end());
   stdlist->insert(stdlist->begin(), aux_vect.begin(), aux_vect.end());

   shmlist->unique();
   stdlist->unique();
   if(!CheckEqual(shmlist, stdlist)) return -1;

   shmlist->sort(std::greater<int>());
   stdlist->sort(std::greater<int>());
   if(!CheckEqual(shmlist, stdlist)) return -1;

   shmlist->resize(shmlist->size()/2);
   stdlist->resize(stdlist->size()/2);
   if(!CheckEqual(shmlist, stdlist)) return -1;

   shmlist->remove(*shmlist->begin());
   stdlist->remove(*stdlist->begin());
   if(!CheckEqual(shmlist, stdlist)) return -1;

   for(i = 0; i < max; ++i){
      shmlist->push_back(i);
      stdlist->push_back(i);
   }
   if(!CheckEqual(shmlist, stdlist)) 
      return -1;   

   MyShmList othershmlist(*shmlist);
   MyStdList otherstdlist(*stdlist);
   shmlist->splice(shmlist->begin(), othershmlist);
   stdlist->splice(stdlist->begin(), otherstdlist);
   if(!CheckEqual(shmlist, stdlist)) 
      return -1;   

   othershmlist = *shmlist;
   otherstdlist = *stdlist;
   shmlist->merge(othershmlist, std::greater<int>());
   stdlist->merge(otherstdlist, std::greater<int>());
   if(!CheckEqual(shmlist, stdlist)) return -1;   
   
   MyShmList *shmlist2 = segment.construct<MyShmList>
                           (L"MyVect2")
                           (segment.get_segment_manager());

   shmlist->swap(*shmlist2);
   segment.destroy<MyShmList>(L"MyVect2");

   //Allocators must be destroyed before shared memory is unmapped
   {
      cached_node_allocator_t al1 = shmlist->get_allocator();
      cached_node_allocator_t al2 = shmlist->get_allocator();

      if(al1 != al2)
         return -1;

      std::size_t max_chached2 = al1.get_max_cached_nodes()*2;
      al2.set_max_cached_nodes(max_chached2);

      while(max_chached2--){
         al1.deallocate(al2.allocate(1), 1);
      }

      swap(al1, al2);
   }   

   segment.destroy_ptr(shmlist);

   delete stdlist;
   return 0;
}


