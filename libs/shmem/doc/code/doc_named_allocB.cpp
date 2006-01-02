   #include <boost/shmem/named_shared_object.hpp>
   #include <cstddef>
   #include <assert.h>

   int main ()
   {
      using namespace boost::shmem;
      typedef std::pair<double, int> MyType;

      //A shared memory front-end that is able to construct 
      //objects associated with a c-string
      named_shared_object segment;

      //Connect to the shared memory segment and initialize resources
      if(!segment.open("/MySharedMemory")){
         return -1;
      }

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
      return 0;
   }
