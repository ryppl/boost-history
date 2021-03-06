   #include <boost/interprocess/detail/config_begin.hpp>
   #include <boost/interprocess/detail/workaround.hpp>

   #include <boost/interprocess/containers/vector.hpp>
   #include <boost/interprocess/containers/string.hpp>
   #include <boost/interprocess/managed_shared_memory.hpp>
   #include <boost/interprocess/streams/vectorstream.hpp>
   #include <iterator>
   
   using namespace boost::interprocess;

   typedef allocator<int, managed_shared_memory::segment_manager> 
      IntAllocator;
   typedef allocator<char, managed_shared_memory::segment_manager> 
      CharAllocator;
   typedef vector<int, IntAllocator>   MyVector;
   typedef basic_string
      <char, std::char_traits<char>, CharAllocator>   MyString;
   typedef basic_vectorstream<MyString>               MyVectorStream;

   int main ()
   {
      //Create shared memory
      shared_memory_object::remove("MySharedMemory");
      managed_shared_memory segment(
         create_only, 
         "MySharedMemory",//segment name
         65536);           //segment size in bytes

      //Construct shared memory vector
      MyVector *myvector = 
         segment.construct<MyVector>("MyVector")
         (IntAllocator(segment.get_segment_manager()));

      //Fill vector
      myvector->reserve(100);
      for(int i = 0; i < 100; ++i){
         myvector->push_back(i);
      }

      //Create the vectorstream. To create the internal shared memory
      //basic_string we need to pass the shared memory allocator as
      //a constructor argument
      MyVectorStream myvectorstream(CharAllocator(segment.get_segment_manager()));

      //Reserve the internal string
      myvectorstream.reserve(100*5);

      //Write all vector elements as text in the internal string
      //Data will be directly written in shared memory, because
      //internal string's allocator is a shared memory allocator
      for(std::size_t i = 0, max = myvector->size(); i < max; ++i){
         myvectorstream << (*myvector)[i] << std::endl;
      }

      //Auxiliary vector to compare original data
      MyVector *myvector2 =
         segment.construct<MyVector>("MyVector2")
         (IntAllocator(segment.get_segment_manager()));

      //Avoid reallocations
      myvector2->reserve(100);

      //Extract all values from the internal 
      //string directly to a shared memory vector.
      std::istream_iterator<int> it(myvectorstream), itend;
      std::copy(it, itend, std::back_inserter(*myvector2));

      //Compare vectors
      assert(std::equal(myvector->begin(), myvector->end(), myvector2->begin()));

      //Create a copy of the internal string
      MyString stringcopy (myvectorstream.vector());

      //Now we create a new empty shared memory string...
      MyString *mystring = 
         segment.construct<MyString>("MyString")
         (CharAllocator(segment.get_segment_manager()));
   
      //...and we swap vectorstream's internal string
      //with the new one: after this statement mystring
      //will be the owner of the formatted data.
      //No reallocations, no data copies
      myvectorstream.swap_vector(*mystring);

      //Let's compare both strings
      assert(stringcopy == *mystring);

      //Done, destroy and delete vectors and string from the segment
      segment.destroy_ptr(myvector2);
      segment.destroy_ptr(myvector);
      segment.destroy_ptr(mystring);
      return 0;
   }

   #include <boost/interprocess/detail/config_end.hpp>
