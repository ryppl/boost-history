//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/shmem/indexes/null_map_index.hpp>
#include <boost/shmem/named_shared_object.hpp>
#include <boost/shmem/mem_algo/simple_seq_fit.hpp>
#include <cstddef>
#include <assert.h>

using namespace boost::shmem;
typedef basic_named_shared_object
   <char, simple_seq_fit<shared_mutex_family>, null_index>
my_shared_objects_t;

int main ()
{
   //Named allocate capable shared mem allocator
   my_shared_objects_t segment;

   //Create shared memory
   segment.create("/MySharedMemory",//segment name
                  65536);           //segment size in bytes

   //Allocate a portion of the segment
   void * shptr   = segment.allocate(1024/*bytes to allocate*/);
   std::ptrdiff_t offset = segment.get_offset_from_address(shptr);
   if(!segment.belongs_to_segment(shptr)){
      return -1;
   }
   if(shptr != segment.get_address_from_offset(offset)){
      return -1;
   }

   segment.deallocate(shptr);
   return 0;
}
