   #include <boost/interprocess/detail/config_begin.hpp>
   #include <boost/interprocess/detail/workaround.hpp>

   #include <boost/interprocess/managed_shared_memory.hpp>
   #include <boost/interprocess/smart_ptr/scoped_ptr.hpp>

   using namespace boost::interprocess;

   class my_class
   {};

   class my_exception
   {};

   //A functor that destroys the shared memory object
   template<class T>
   class my_deleter
   {
      private:
      //A typedef to save typing
      typedef managed_shared_memory::segment_manager segment_manager;
      //This my_deleter is created in the stack, not in shared memory,
      //so we can use raw pointers
      segment_manager *mp_segment_manager;
   
      public:
      //This typedef will specify the pointer type that
      //scoped_ptr will store
      typedef T *pointer;
      //Constructor
      my_deleter(segment_manager *s_mngr)
      : mp_segment_manager(s_mngr){}

      void operator()(pointer object_to_delete)
      {  mp_segment_manager->destroy_ptr(object_to_delete);  }
   };

   int main ()
   {
      //Create shared memory
      shared_memory_object::remove("my_shmem");
      managed_shared_memory shmem(create_only, "my_shmem", 10000);

      //In the first try, there will be no exceptions
      //in the second try we will throw an exception
      for(int i = 0; i < 2; ++i){
         //Create an object in shared memory
         my_class * my_object = shmem.construct<my_class>("my_object")();
         
         //Since the next shared memory allocation can throw
         //assign it to a scoped_ptr so that if an exception occurs
         //we destroy the object automatically
         my_deleter<my_class> d(shmem.get_segment_manager());

         try{
            scoped_ptr<my_class, my_deleter<my_class> > s_ptr(my_object, d);
            //Let's emulate a exception capable operation
            //In the second try, throw an exception
            if(i == 1){
               throw(my_exception());
            }
            //If we have passed the dangerous zone
            //we can release the scoped pointer
            //to avoid destruction
            s_ptr.release();
         }
         catch(const my_exception &){}

         //Here, scoped_ptr is destroyed
         //so it we haven't thrown an exception
         //the object should be there, otherwise, destroyed
         if(i == 0){
            //Make sure the object is alive
            if(!shmem.find<my_class>("my_object").first){
               return 1;
            }
            //Now we can use it and delete it manually
            shmem.destroy<my_class>("my_object");
         }
         else{
            //Make sure the object has been deleted
            if(shmem.find<my_class>("my_object").first){
               return 1;
            }
         }
      }
      return 0;
   }

   #include <boost/interprocess/detail/config_end.hpp>
