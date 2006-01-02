//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/shmem/allocators/allocator.hpp>
#include <boost/shmem/named_shared_object.hpp>
#include <boost/shmem/containers/vector.hpp>
#include <boost/shmem/sync/named_semaphore.hpp>

#include "printcontainer.hpp"
#include <algorithm>

using namespace boost::shmem;

int main ()
{
   //Shared memory attributes
   const char *const shMemName = "/MySharedMemory";

   //Create sems for synchronization
   named_semaphore semA;
   if(!semA.open("/processAsem")){
     	std::cout << "error opening semA\n";
   	return -1;
   }
   named_semaphore semB;
   if(!semB.open("/processBsem")){
     	std::cout << "error opening semA\n";
   	return -1;
   }

   //Wait until the shared memory is ready
   semB.wait();

   //Named new capable shared mem allocator
   named_shared_object segment;

   //Create shared memory
   if(!segment.open_or_create(shMemName, 65536)){
   	std::cout << "error opening shared memory\n";
   	return -1;
   }

   //STL compatible allocator object, uses allocate(), deallocate() functions
   typedef allocator<int, named_shared_object::segment_manager>
      shmem_allocator_int_t;

   //This is the shared memory vector type
   typedef vector<int, shmem_allocator_int_t >  MyVect;

   //Finding vector in shared memory and printing contents
   std::cout << "Connecting to object ShmVect\n\n";
   MyVect *shmem_vect = segment.find<MyVect>("ShmVect").first;
   PrintContents(*shmem_vect, "ShmVect");

   //Reverse sorting the vector with std::sort
   std::cout << "Reverse sorting ShmVect\n\n";
   MyVect::reverse_iterator rbeg = 
      shmem_vect->rbegin(), rend = shmem_vect->rend();
   std::sort(shmem_vect->rbegin(), shmem_vect->rend());
   std::sort(rbeg, rend);

   //Printing values after sorting
   std::cout << "Sorting complete\n\n";
   PrintContents(*shmem_vect, "ShmVect");

   //Waking up process A
   std::cout << "Waking up processA\n\n";
   semA.post();

   //We're done, closing shared memory
   std::cout << "Closing shmem segment\n\n";
   segment.close();
   std::cout << "segment closed\n\n";

   return 0;
}


