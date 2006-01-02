
   #include <boost/shmem/named_shared_object.hpp>
   #include <cstddef>

   int main ()
   {
      using namespace boost::shmem;

      //A shared memory front-end that is able to 
      //allocate raw memory buffers from a shared memory segment
      named_shared_object segment;

      //Create the shared memory segment and initialize needed resources
      if(!segment.create("/MySharedMemory",  //segment name
                        65536)){            //segment size in bytes
         return -1;
      }

      //Allocate a portion of the segment
      void * shptr   = segment.allocate(1024/*bytes to allocate*/);

      //An offset from the base address can identify any byte of the shared 
      //memory segment even if it is mapped in different base addresses
      std::ptrdiff_t offset = segment.get_offset_from_address(shptr);
      (void)offset;
      // Copy message to buffer
      // . . .
      // Send offset to other process
      // . . .
      // Wait response from other process
      // . . .

      //Deallocate the portion previously allocated
      segment.deallocate(shptr);
      return 0;
   }
