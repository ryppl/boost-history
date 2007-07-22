//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2004-2007. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/interprocess/detail/config_begin.hpp>

#ifdef BOOST_WINDOWS

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_windows_shared_memory.hpp>
#include <cstdio>
#include <string>
#include "get_compiler_name.hpp"

using namespace boost::interprocess;

int main ()
{
   const int MemSize          = 65536;
   const char *const MemName  = test::get_compiler_name();

   //STL compatible allocator object for shared memory
   typedef allocator<int, managed_windows_shared_memory::segment_manager>
      allocator_int_t;
   //A vector that uses that allocator
   typedef boost::interprocess::vector<int, allocator_int_t> MyVect;

   {
      const int max              = 100;
      void *array[max];
      //Named allocate capable shared memory allocator
      managed_windows_shared_memory w_shm(create_only, MemName, MemSize);

      int i;
      //Let's allocate some memory 
      for(i = 0; i < max; ++i){
         array[i] = w_shm.allocate(i+1);
      }

      //Deallocate allocated memory
      for(i = 0; i < max; ++i){
         w_shm.deallocate(array[i]);
      }
   }

   {
      //Named allocate capable shared memory managed memory class
      managed_windows_shared_memory w_shm(create_only, MemName, MemSize);

      //Construct the STL-like allocator with the segment manager
      const allocator_int_t myallocator (w_shm.get_segment_manager());

      //Construct vector
      MyVect *w_shm_vect = w_shm.construct<MyVect> ("MyVector") (myallocator);

      //Test that vector can be found via name
      if(w_shm_vect != w_shm.find<MyVect>("MyVector").first)
         return -1;

      //Destroy and check it is not present
      w_shm.destroy<MyVect> ("MyVector");
      if(0 != w_shm.find<MyVect>("MyVector").first)
         return -1;

      //Construct a vector in the shared memory
      w_shm_vect = w_shm.construct<MyVect> ("MyVector") (myallocator);

      //Map preexisting segment again in memory
      managed_windows_shared_memory w_shm_new(open_only, MemName);

      //Check vector is still there
      w_shm_vect = w_shm_new.find<MyVect>("MyVector").first;
      if(!w_shm_vect)
         return -1;

      if(w_shm_new.get_size() != w_shm.get_size())
         return 1;
      //Destroy and check it is not present
      w_shm_new.destroy_ptr(w_shm_vect);
      if(0 != w_shm_new.find<MyVect>("MyVector").first)
         return 1;
   }

   return 0;
}

#else

int main()
{
   return 0;
}

#endif

#include <boost/interprocess/detail/config_end.hpp>
