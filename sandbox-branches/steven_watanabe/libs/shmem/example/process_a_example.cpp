//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/shmem/offset_ptr.hpp>
#include <boost/shmem/allocators/allocator.hpp>
#include <boost/shmem/named_shared_object.hpp>
#include "printcontainer.hpp"
#include <algorithm>
#include <boost/shmem/sync/named_semaphore.hpp>
#include <boost/shmem/containers/vector.hpp>
#include <boost/shmem/sync/shared_condition.hpp>

using namespace boost::shmem;

int main ()
{
   //Shared memory attributes
   const int memsize = 65536;
   const char *const shMemName = "/MySharedMemory";

   //Create sems for synchronization
   named_semaphore semA;
   if(!semA.create("/processAsem", 0)){
     	std::cout << "error creating semA\n";
   	return -1;
   }
   named_semaphore semB;
   if(!semB.create("/processBsem", 0)){
   	std::cout << "error creating semB\n";
   	return -1;
   }   
   //Named new capable shared mem allocator
   named_shared_object segment;

   //Create shared memory
   if(!segment.open_or_create(shMemName, memsize)){
   	std::cout << "error creating shared memory\n";
   	return -1;
   };

   //STL compatible allocator object, uses allocate(), deallocate() functions
   typedef allocator<int, named_shared_object::segment_manager>
      shmem_allocator_int_t;

   const int num_elements = 100;

   //Type of shared memory vector
   typedef vector<int, shmem_allocator_int_t > MyVect;

   const shmem_allocator_int_t &alloc_ref (segment.get_segment_manager());

   //Creating the vector in shared memory
   std::cout << "Named New of ShmVect\n\n";
   MyVect *shmem_vect = segment.construct<MyVect> ("ShmVect")(alloc_ref);

   offset_ptr<MyVect> shmptr_vect = 0;
   offset_ptr<MyVect> other_shmptr_vect = 0;

   //Fill the vector
   std::cout << "Filling ShmVect\n\n";
   int i;
   for(i = 0; i < num_elements; ++i){
      shmem_vect->push_back(i);
   }

   //Printing contents before waiting to second process
   PrintContents(*shmem_vect, "ShmVect");

   //Wake up other process and sleeping until notified
   semB.post();
   std::cout << "Waking up processB and waiting sorting\n\n";
   semA.wait();

   //Notification received, let's see the changes
   std::cout << "processB sorting complete\n\n";
   PrintContents(*shmem_vect, "ShmVect");

   //Let's delete the vector from memory
   std::cout << "Deleting the vector with destroy\n\n";
   segment.destroy<MyVect> ("ShmVect");
   std::cout << "vector deleted\n\n";
   return 0;
}

