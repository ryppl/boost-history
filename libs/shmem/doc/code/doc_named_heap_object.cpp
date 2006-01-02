   #include <boost/shmem/containers/list.hpp>
   #include <boost/shmem/named_heap_object.hpp>
   #include <boost/shmem/allocators/allocator.hpp>
   #include <cstddef>

   using namespace boost::shmem;
   typedef list<int, allocator<int, named_heap_object::segment_manager> > 
      MyList;

   int main ()
   {
      named_heap_object heap_memory;

      //We will create a buffer of 1000 bytes to store a list
      heap_memory.create(1000);
      MyList * mylist = heap_memory.construct<MyList>("MyList")
                           (heap_memory.get_segment_manager());

      //Obtain offset, that identifies the list in the buffer
      std::ptrdiff_t list_offset = heap_memory.get_offset_from_address(mylist);

      //Fill list until there is no more memory in the buffer
      try{
         while(1) {
            mylist->insert(mylist->begin(), 0);
         }
      }
      catch(const bad_alloc &){
         //memory is full
      }
      //Let's obtain the size of the list
      std::size_t old_size = mylist->size();

      //To make the list bigger, let's increase the heap buffer
      //in 1000 bytes more.
      heap_memory.grow(1000);

      //If memory has been reallocated, the old pointer is invalid, so
      //use previously obtained offset to find the new pointer.
      mylist = static_cast<MyList *>
                  (heap_memory.get_address_from_offset(list_offset));
      
      //Fill list until there is no more memory in the buffer
      try{
         while(1) {
            mylist->insert(mylist->begin(), 0);
         }
      }
      catch(const bad_alloc &){
         //memory is full
      }

      //Let's obtain the new size of the list      
      std::size_t new_size = mylist->size();

      assert(new_size > old_size);

      //Destroy list
      heap_memory.destroy_ptr(mylist);

      return 0;
   }
