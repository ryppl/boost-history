//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/shmem/named_shared_object.hpp>
#include <cstddef>

int main ()
{
   using namespace boost::shmem;

   //Named allocate capable shared memory allocator
   named_shared_object segment;

   //Create shared memory
   segment.create("/MySharedMemory",//segment name
                  65536);           //segment size in bytes

   //Allocate a portion of the segment
   void * shptr   = segment.allocate(1024/*bytes to allocate*/);
   std::ptrdiff_t offset = segment.get_offset_from_address(shptr);
   (void)offset;

   // Copy message to buffer
   // . . .
   // Send offset to other process
   // . . .
   // Wait response from other process 
   // . . .
   
 {
   using namespace boost::shmem;

   //Named allocate capable shared memory allocator
   named_shared_object segment;

   //Open shared memory
   if(!segment.open("/MySharedMemory")){ //segment name
      return -1;
   }

   std::ptrdiff_t offset = 0;
   (void)offset;

   //Wait offset msg from other process and put it in
   //"offset" local variable

   //Get buffer local address from offset
   void *msg = segment.get_address_from_offset(offset);
   (void)msg;
 
   //Do anything with msg
   //. . .
   //Send ack to sender process
 }

   segment.deallocate(shptr);
   return 0;
}
