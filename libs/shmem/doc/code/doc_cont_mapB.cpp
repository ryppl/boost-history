   #include <vector>
   #include <boost/shmem/named_shared_object.hpp>

   int main ()
   {
      using namespace boost::shmem;
       //Shared memory front-end that is able to construct objects
      //associated with a c-string
      fixed_named_shared_object segment;

      //Open the memory segment at the specified address and initialize resources
      if(!segment.open("/MySharedMemory",    //segment name
                       (void*)0x03000000)){  //mapping address
         return -1;
      }

      //Alias an STL compatible allocator of ints that allocates ints from the segment
      //and that defines allocator::pointer as "int *". This allocator will be compatible
      //with most STL implementations.
      typedef allocator<int, fixed_named_shared_object::segment_manager> 
         ShmemAllocator;

      //Alias a vector that uses the previous STL-like allocator
      typedef std::vector<int, ShmemAllocator> MyVector;

      //Find the vector using the c-string name
      MyVector *myvector = segment.find<MyVector>("MyVector").first;

      //Use vector as you want
      std::sort(myvector->rbegin(), myvector->rend());
      // . . .

      //When done, destroy and delete vector
      segment.destroy<MyVector>("MyVector");
   }
