   #include <boost/shmem/named_shared_object.hpp>
   #include <boost/shmem/containers/vector.hpp>
   #include <boost/shmem/containers/string.hpp>
   #include <boost/shmem/allocators/allocator.hpp>

   int main ()
   {
      using namespace boost::shmem;
      //Typedefs
      typedef allocator<char, named_shared_object::segment_manager> 
         CharAllocator;
      typedef basic_string<char, std::char_traits<char>, CharAllocator>
         MyShmString;
      typedef allocator<MyShmString, named_shared_object::segment_manager>
         StringAllocator;      
      typedef vector<MyShmString, StringAllocator>
         MyShmStringVector;

      //Open shared memory
      named_shared_object shm;
      shm.create("myshm", 10000);

      //Create allocators
      CharAllocator     charallocator  (shm.get_segment_manager());
      StringAllocator   stringallocator(shm.get_segment_manager());

      //This string is in only in this process (the pointer pointing to the
      //buffer that will hold the text is not in shared memory). 
      //But the buffer that will hold "this is my text" is allocated from 
      //shared memory
      MyShmString mystring(charallocator);
      mystring = "this is my text";

      //This vector is in only in this process (the pointer pointing to the
      //buffer that will hold the MyShmString-s is not in shared memory). 
      //But the buffer that will hold 10 MyShmString-s is allocated from 
      //shared memory using StringAllocator. Since strings use a shared 
      //memory allocator (CharAllocator) the 10 buffers that hold 
      //"this is my text" text are also in shared memory.
      MyShmStringVector myvector(stringallocator);
      myvector.insert(myvector.begin(), 10, mystring);

      //This vector is fully constructed in shared memory. All pointers
      //buffers are constructed in the same shared memory segment
      //This vector can be safely accessed from other processes.
      MyShmStringVector *myshmvector = 
         shm.construct<MyShmStringVector>("myshmvector")(stringallocator);
      myshmvector->insert(myshmvector->begin(), 10, mystring);

      //Destroy vector. This will free all strings that the vector contains
      shm.destroy_ptr(myshmvector);
      return 0;
   }

