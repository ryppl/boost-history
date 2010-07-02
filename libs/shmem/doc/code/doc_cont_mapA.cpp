   #include <vector>
   #include <boost/shmem/named_shared_object.hpp>

   int main ()
   {
      using namespace boost::shmem;
      //Shared memory front-end that is able to construct objects
      //associated with a c-string
      fixed_named_shared_object segment;

      //Create the memory segment at the specified address and initialize resources
      segment.create("/MySharedMemory",  //segment name
                     65536,              //segment size in bytes
                     (void*)0x03000000); //mapping address

      //Alias an STL compatible allocator of ints that allocates ints from the segment
      //and that defines allocator::pointer as "int *". This allocator will be compatible
      //with most STL implementations.
      typedef allocator<int, fixed_named_shared_object::segment_manager> 
         ShmemAllocator;

      //Alias a vector that uses the previous STL-like allocator
      typedef std::vector<int, ShmemAllocator> MyVector;

      //Initialize shared memory STL-compatible allocator
      const ShmemAllocator alloc_inst (segment.get_segment_manager());

      //Initialize vector
      MyVector *myvector = 
         segment.construct<MyVector>("MyVector") //object name
                                    (alloc_inst);//first ctor parameter 
      (void)myvector;
      return 0;
   }