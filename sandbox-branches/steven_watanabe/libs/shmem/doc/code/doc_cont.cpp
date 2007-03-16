   #include <boost/shmem/containers/vector.hpp>
   #include <boost/shmem/named_shared_object.hpp>

   int main ()
   {
      using namespace boost::shmem;

      //Shared memory front-end that is able to construct objects
      //associated with a c-string
      named_shared_object segment;

      //Create the memory segment and initialize resources
      if(!segment.create("/MySharedMemory",  //segment name
                         65536)){            //segment size in bytes
         return -1;
      }

      //Alias an STL-like allocator of ints that allocates ints from the segment
      typedef allocator<int, named_shared_object::segment_manager> 
         ShmemAllocator;

      //Alias a vector that uses the previous STL-like allocator
      typedef vector<int, ShmemAllocator> MyVector;

      int initVal[]        = {0, 1, 2, 3, 4, 5, 6 };
      const int *begVal    = initVal;
      const int *endVal    = initVal + sizeof(initVal)/sizeof(initVal[0]);

      //Initialize the STL-like allocator
      const ShmemAllocator alloc_inst (segment.get_segment_manager());

      //Construct the vector in the shared memory segment with the STL-like allocator 
      //from a range of iterators
      MyVector *myvector = 
         segment.construct<MyVector>
           ("MyVector")/*object name*/
           (begVal     /*first ctor parameter*/,
            endVal     /*second ctor parameter*/, 
            alloc_inst /*third ctor parameter*/); 

      //Use vector as your want
      std::sort(myvector->rbegin(), myvector->rend());
      // . . .
      //When done, destroy and delete vector from the segment
      segment.destroy<MyVector>("MyVector");
      return 0;
   }
