
   #include <boost/shmem/named_shared_object.hpp>
   #include <cstddef>

   int main ()
   {
      using namespace boost::shmem;

      //A shared memory front-end that is able to 
      //allocate raw memory buffers from a shared memory segment
      named_shared_object segment;

      //Connect to the shared memory segment and initialize needed resources
      if(!segment.open("/MySharedMemory")){  //segment name
         return -1;
      }

      //An offset from the base address can identify any byte of the shared 
      //memory segment even if it is mapped in different base addresses
      std::ptrdiff_t offset = 0;

      //Wait offset msg from the other process and put it in
      //"offset" local variable
      //Get buffer local address from offset
      void *msg = segment.get_address_from_offset(offset);
      (void)msg;
      //Do anything with msg
      //. . .
      //Send ack to sender process
      return 0;
   }
