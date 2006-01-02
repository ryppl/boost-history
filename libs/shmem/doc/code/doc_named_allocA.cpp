   #include <boost/shmem/named_shared_object.hpp>

   int main ()
   {
      using namespace boost::shmem;
      typedef std::pair<double, int> MyType;

      //A shared memory front-end that is able to construct 
      //objects associated with a c-string
      named_shared_object segment;

      //Create the shared memory segment and initialize resources
      if(!segment.create("/MySharedMemory",  //segment name
                         65536)){            //segment size in bytes
         return -1;
      }
                     

      //Create an object of MyType initialized to {0.0, 0}
      MyType *instance = segment.construct<MyType>
         ("MyType instance")  /*name of the object*/
         (0.0                 /*ctor first argument*/,
          0                   /*ctor second argument*/);    

      //Create an array of 10 elements of MyType initialized to {0.0, 0}
      MyType *array = segment.construct<MyType>
         ("MyType array")     /*name of the object*/
         [10]                 /*number of elements*/
         (0.0                 /*ctor first argument*/,
          0                   /*ctor second argument*/);    
      return 0;
   }
