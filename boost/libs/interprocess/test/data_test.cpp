//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2004-2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/list.hpp>
#include <iostream>
#include <algorithm>
#include <functional>
#include "printcontainer.hpp"

using namespace boost::interprocess;

int main ()
{
   const int memsize = 65536;
   const char *const shMemName = "MySharedMemory";

   managed_shared_memory::remove(shMemName);

   //Create shared memory
   managed_shared_memory segment(create_only, shMemName, memsize);

   //STL compatible allocator object, uses allocate(), deallocate() functions
   typedef allocator<int, managed_shared_memory::segment_manager>
      shmem_allocator_int_t;

   const shmem_allocator_int_t myallocator (segment.get_segment_manager());

   const int max = 100;
   void *array[max];

   const char *allocName = "testAllocation";

   typedef boost::interprocess::vector<int, shmem_allocator_int_t > MyVect;
   typedef boost::interprocess::list<int, shmem_allocator_int_t >   MyList;


   //----   ALLOC, NAMED_ALLOC, NAMED_NEW TEST   ----//
   {
      int i;
      //Let's allocate some memory 
      for(i = 0; i < max; ++i){
         array[i] = segment.allocate(i+1);
      }

      //Deallocate allocated memory
      for(i = 0; i < max; ++i){
         segment.deallocate(array[i]);
      }

      bool res;

      MyVect *shmem_vect;

      //Construct and find
      shmem_vect = segment.construct<MyVect> (allocName) (myallocator);
      res = (shmem_vect == segment.find<MyVect>(allocName).first);
      if(!res)
         return 1;
      //Destroy and check it is not present
      segment.destroy<MyVect> (allocName);
      res = (0 == segment.find<MyVect>(allocName).first);
      if(!res)
         return 1;

      //Construct, dump to a file
      shmem_vect = segment.construct<MyVect> (allocName) (myallocator);
      segment.save_to_file("shmem_file");

/*
      //Recreate objects in a new shared memory check object is present
      bool created = segment.create_from_file("shmem_file", shMemName);
      if(!created)
         return 1;
      shmem_vect = segment.find<MyVect>(allocName).first;
      if(!shmem_vect)
         return 1;
      //Destroy and check it is not present
      segment.destroy<MyVect> (allocName);
      res = (0 == segment.find<MyVect>(allocName).first);
      if(!res)
         return 1;
*/
   }
   return 0;
}

#include <boost/interprocess/detail/config_end.hpp>
