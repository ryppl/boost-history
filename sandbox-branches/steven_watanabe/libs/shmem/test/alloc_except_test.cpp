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
#include <boost/shmem/containers/list.hpp>
#include <iostream>
#include <algorithm>
#include <functional>
#include "printcontainer.hpp"

struct InstanceCounter
{
   InstanceCounter(){++counter;}
   InstanceCounter(const InstanceCounter&){++counter;}
  ~InstanceCounter(){--counter;}
   static std::size_t counter;
};

std::size_t InstanceCounter::counter = 0;

using namespace boost::shmem;


int main ()
{
   const int memsize = 16384;
   const char *const shMemName = "/MySharedMemory";

   //Named allocate capable shared mem allocator
   named_shared_object segment;

   //Create shared memory
   segment.create(shMemName, memsize);

   //STL compatible allocator object, uses allocate(), deallocate() functions
   typedef allocator<InstanceCounter, 
                     named_shared_object::segment_manager>
      inst_allocator_t;
   const inst_allocator_t myallocator (segment.get_segment_manager());

   typedef vector<InstanceCounter, inst_allocator_t> MyVect;

   //We'll provoke an exception, let's see if exception handling works
   try{
      //Fill vector until there is no more memory
      MyVect myvec(myallocator);
      int i;
      for(i = 0; true; ++i){
         myvec.push_back(InstanceCounter());
      }  
   }
   catch(boost::shmem::bad_alloc &){
      if(InstanceCounter::counter != 0)
         return -1;
   }

   //We'll provoke an exception, let's see if exception handling works
   try{
      //Fill vector at the beginning until there is no more memory
      MyVect myvec(myallocator);
      int i;
      for(i = 0; true; ++i){
         myvec.insert(myvec.begin(), i, InstanceCounter());
      }  
   }
   catch(boost::shmem::bad_alloc &){
      if(InstanceCounter::counter != 0)
         return -1;
   }
   return 0;
}
