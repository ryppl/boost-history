//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2006-2007. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>
//[run_ipc_message
#include <boost/interprocess/managed_shared_memory.hpp>
#include <cstdlib> //std::system
#include <sstream>

int main (int argc, char *argv[])
{
   using namespace boost::interprocess;
   if(argc == 1){  //Parent process
      //Remove shared memory on construction and destruction
      struct shm_remove 
      {
         shm_remove() {  shared_memory_object::remove("MySharedMemory"); }
         ~shm_remove(){  shared_memory_object::remove("MySharedMemory"); }
      } remover;

      //Create a managed shared memory segment
      managed_shared_memory segment(create_only, "MySharedMemory", 65536);

      //Allocate a portion of the segment (raw memory)
      std::size_t free_memory = segment.get_free_memory();
      void * shptr = segment.allocate(1024/*bytes to allocate*/);

      //Check invariant
      if(free_memory <= segment.get_free_memory())
         return 1;

      //An handle from the base address can identify any byte of the shared 
      //memory segment even if it is mapped in different base addresses
      managed_shared_memory::handle_t handle = segment.get_handle_from_address(shptr);
      std::stringstream s;
      s << argv[0] << " " << handle << std::ends;
      
      //Launch child process
      if(0 != std::system(s.str().c_str()))
         return 1;
      //Check memory has been freed
      if(free_memory != segment.get_free_memory())
         return 1;
   }
   else{
      //Open managed segment
      managed_shared_memory segment(open_only, "MySharedMemory");

      //An handle from the base address can identify any byte of the shared 
      //memory segment even if it is mapped in different base addresses
      managed_shared_memory::handle_t handle = 0;

      //Obtain handle value
      std::stringstream s; s << argv[1]; s >> handle;

      //Get buffer local address from handle
      void *msg = segment.get_address_from_handle(handle);

      //Deallocate previously allocated memory
      segment.deallocate(msg);
   }
   return 0;
}
//]
#include <boost/interprocess/detail/config_end.hpp>
