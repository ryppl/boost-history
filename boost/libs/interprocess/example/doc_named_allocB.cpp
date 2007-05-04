//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gazta�aga 2006-2007. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>
//[doc_named_allocB
#include <boost/interprocess/managed_shared_memory.hpp>
#include <cstddef>
#include <utility>
#include <assert.h>

int main ()
{
   using namespace boost::interprocess;
   typedef std::pair<double, int> MyType;

   try{
      //An special shared memory where we can
      //construct objects associated with a name.
      //Connect to the already created shared memory segment
      //and initialize needed resources
      managed_shared_memory segment(open_only, "MySharedMemory");

      //Find the array and object
      std::pair<MyType*, std::size_t> res;
      res = segment.find<MyType> ("MyType array");   

      std::size_t array_len   = res.second;
      //Length should be 10
      assert(array_len == 10);

      //Find the array and the object
      res = segment.find<MyType> ("MyType instance");   

      std::size_t len   = res.second;

      //Length should be 1
      assert(len == 1);

      //Use data
      // . . . 

      //We're done, delete array from memory
      segment.destroy<MyType>("MyType array");

      //We're done, delete object from memory
      segment.destroy<MyType>("MyType instance");
   }
   catch(...){
      shared_memory_object::remove("MySharedMemory");
      throw;
   }
   shared_memory_object::remove("MySharedMemory");
   return 0;
}
//]
#include <boost/interprocess/detail/config_end.hpp>
