   #include <boost/shmem/containers/list.hpp>
   #include <boost/shmem/named_mfile_object.hpp>
   #include <boost/shmem/allocators/allocator.hpp>
   #include <cstddef>

   using namespace boost::shmem;
   typedef list<int, allocator<int, named_mfile_object::segment_manager> > 
      MyList;

   int main ()
   {
      named_mfile_object mfile_memory;

      //We will create a mapped file of 1000 bytes to store a list
      mfile_memory.create("./mapped_file", 1000);
      MyList * mylist = mfile_memory.construct<MyList>("MyList")
                           (mfile_memory.get_segment_manager());

      //Obtain offset, that identifies the list in the buffer
      std::ptrdiff_t list_offset = mfile_memory.get_offset_from_address(mylist);

      //Fill list until there is no more room in the file
      try{
         while(1) {
            mylist->insert(mylist->begin(), 0);
         }
      }
      catch(const bad_alloc &){
         //mapped file is full
      }
      //Let's obtain the size of the list
      std::size_t old_size = mylist->size();

      //To make the list bigger, let's increase the mapped file
      //in 1000 bytes more.
      mfile_memory.grow(1000);

      //If mapping address has changed, the old pointer is invalid,
      //so use previously obtained offset to find the new pointer.
      mylist = static_cast<MyList *>
                  (mfile_memory.get_address_from_offset(list_offset));
      
      //Fill list until there is no more room in the file
      try{
         while(1) {
            mylist->insert(mylist->begin(), 0);
         }
      }
      catch(const bad_alloc &){
         //mapped file is full
      }

      //Let's obtain the new size of the list      
      std::size_t new_size = mylist->size();

      assert(new_size > old_size);

      //Destroy list
      mfile_memory.destroy_ptr(mylist);

      return 0;
   }
