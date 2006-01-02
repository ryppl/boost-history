   #include <boost/shmem/named_shared_object.hpp>
   #include <boost/shmem/streams/bufferstream.hpp>
   #include <vector>
   #include <iterator>
   
   using namespace boost::shmem;

   int main ()
   {
      //Create shared memory
      named_shared_object segment;
      if(!segment.create("/MySharedMemory",  //segment name
                         65536)){            //segment size in bytes
         return -1;
      }

      //Fill data
      std::vector<int> data, data2;
      data.reserve(100);
      for(int i = 0; i < 100; ++i){
         data.push_back(i);
      }

      //Allocate a buffer in shared memory to write data
      char *my_cstring = 
         segment.construct<char>("MyCString")[100*5](0);
      bufferstream mybufstream(my_cstring, 100*5);

      //Now write data to the buffer
      for(int i = 0; i < 100; ++i){
         mybufstream << data[i] << std::endl;
      }

      //Check there was no overflow attempt
      assert(mybufstream.good());

      //Extract all values from the shared memory string
      //directly to a shared memory vector.
      data2.reserve(100);
      std::istream_iterator<int> it(mybufstream), itend;
      std::copy(it, itend, std::back_inserter(data2));

      //This extraction should have ended will fail error since 
      //the numbers formatted in the buffer end before the end
      //of the buffer. (Otherwise it would trigger eofbit)
      assert(mybufstream.fail());

      //Compare data
      assert(std::equal(data.begin(), data.end(), data2.begin()));

      //Clear errors and rewind
      mybufstream.clear();
      mybufstream.seekp(0, std::ios::beg);
      
      //Now write again the data trying to do a buffer overflow
      for(int i = 0; i < 500; ++i){
         mybufstream << data[i] << std::endl;
      }

      //Now make sure badbit is active
      //which means overflow attempt.
      assert(!mybufstream.good());
      assert(mybufstream.bad());
      segment.destroy_ptr(my_cstring);      
      return 0;
   }
