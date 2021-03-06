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
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <cstdio>
#include <string>
#include "get_compiler_name.hpp"

using namespace boost::interprocess;

int main ()
{
   const int FileSize          = 65536;
   const char *const FileName = test::get_compiler_name();

   //STL compatible allocator object for memory-mapped file
   typedef allocator<int, managed_mapped_file::segment_manager>
      allocator_int_t;
   //A vector that uses that allocator
   typedef boost::interprocess::vector<int, allocator_int_t> MyVect;

   {
      //Remove the file it is already created
      std::remove(FileName);

      const int max              = 100;
      void *array[max];
      //Named allocate capable shared memory allocator
      managed_mapped_file mfile(create_only, FileName, FileSize);

      int i;
      //Let's allocate some memory 
      for(i = 0; i < max; ++i){
         array[i] = mfile.allocate(i+1);
      }

      //Deallocate allocated memory
      for(i = 0; i < max; ++i){
         mfile.deallocate(array[i]);
      }
   }

   {
      //Remove the file it is already created
      std::remove(FileName);

      //Named allocate capable memory mapped file managed memory class
      managed_mapped_file mfile(create_only, FileName, FileSize);

      //Construct the STL-like allocator with the segment manager
      const allocator_int_t myallocator (mfile.get_segment_manager());

      //Construct vector
      MyVect *mfile_vect = mfile.construct<MyVect> ("MyVector") (myallocator);

      //Test that vector can be found via name
      if(mfile_vect != mfile.find<MyVect>("MyVector").first)
         return -1;

      //Destroy and check it is not present
      mfile.destroy<MyVect> ("MyVector");
      if(0 != mfile.find<MyVect>("MyVector").first)
         return -1;

      //Construct a vector in the memory-mapped file
      mfile_vect = mfile.construct<MyVect> ("MyVector") (myallocator);
      
      //Flush cached data from memory-mapped file to disk
      mfile.flush();
   }
   {

      //Map preexisting file again in memory
      managed_mapped_file mfile(open_only, FileName);

      //Check vector is still there
      MyVect *mfile_vect = mfile.find<MyVect>("MyVector").first;
      if(!mfile_vect)
         return -1;

      //Destroy and check it is not present
      mfile.destroy_ptr(mfile_vect);
      if(0 != mfile.find<MyVect>("MyVector").first)
         return -1;
   }
   std::remove(FileName);
   return 0;
}

#include <boost/interprocess/detail/config_end.hpp>
