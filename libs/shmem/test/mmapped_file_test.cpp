//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2004-2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/shmem/named_mfile_object.hpp>
#include <boost/shmem/allocators/allocator.hpp>
#include <boost/shmem/containers/vector.hpp>

using namespace boost::shmem;

int main ()
{

   {
      const int memsize = 65536;
      const char *const fileName = "MyMappedFile";
      const int max = 100;

      void *array[max];
      //Named allocate capable shared mem allocator
      named_mfile_object mapped_file;

      //Create shared memory
      if(!mapped_file.create(fileName, memsize)){
         return false;
      }

      int i;
      //Let's allocate some memory 
      for(i = 0; i < max; ++i){
         array[i] = mapped_file.allocate(i+1);
      }

      //Deallocate allocated memory
      for(i = 0; i < max; ++i){
         mapped_file.deallocate(array[i]);
      }
   }

   {
      //STL compatible allocator object for memory-mapped file
      typedef allocator<int, named_mfile_object::segment_manager>
         allocator_int_t;
      typedef boost::shmem::vector<int, allocator_int_t > MyVect;

      const int memsize = 65536;
      const char *const fileName = "MyMappedFile";

      //Named allocate capable memory mapped file front-end
      named_mfile_object mapped_file;

      //Create memory-mapped file and initialize Shmem machinery
      if(!mapped_file.create(fileName, memsize)){
         return false;
      }

      //Construct the STL-like allocator with the segment manager
      const allocator_int_t myallocator (mapped_file.get_segment_manager());

      //Construct vector
      MyVect *mfile_vect = mapped_file.construct<MyVect> ("MyVector") (myallocator);

      //Test that vector can be found via name
      if(mfile_vect != mapped_file.find<MyVect>("MyVector").first)
         return -1;

      //Destroy and check it is not present
      mapped_file.destroy<MyVect> ("MyVector");
      if(0 != mapped_file.find<MyVect>("MyVector").first)
         return -1;

      //Construct a vector in the memory-mapped file
      mfile_vect = mapped_file.construct<MyVect> ("MyVector") (myallocator);
      
      //Flush cached data from memory-mapped file to disk
      mapped_file.flush();

      //Close mapped file
      mapped_file.close();

      //Map preexisting file again in memory
      if(!mapped_file.open(fileName)){
         return -1;
      }

      //Check vector is still there
      mfile_vect = mapped_file.find<MyVect>("MyVector").first;
      if(!mfile_vect)
         return -1;

      //Destroy and check it is not present
      mapped_file.destroy_ptr(mfile_vect);
      if(0 != mapped_file.find<MyVect>("MyVector").first)
         return -1;
   }
   return 0;
}
