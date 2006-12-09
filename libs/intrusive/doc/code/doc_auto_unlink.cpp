   #include <boost/intrusive/ilist.hpp>
   #include <cassert>

   using namespace boost::intrusive;

   class MyClass
      //This hook removes the node automatically
      //in the destructor
      :  public ilist_auto_base_hook<0>
   {
      int int_;

      public:

      MyClass(int i = 0)
         :  int_(i)
      {}

      void unlink_self()
      {  ilist_auto_base_hook<0>::unlink_self();  }

      bool linked()
      {  return ilist_auto_base_hook<0>::linked();  }

      int get() const
      {  return int_;  }
   };

   //Define an ilist that will store MyClass
   //using the public base hook
   //The list can't have constant-time size!
   typedef ilist< ilist_auto_base_hook<0>::
                     value_traits<MyClass> >
      List;

   int main()
   {
      //Let's test how the MyClass destructor works:

      //Create the list
      List list;

      {
         //Create myclass and check it's linked
         MyClass myclass;
         assert(myclass.linked() == false);

         //Insert the object
         list.push_back(myclass);

         //Check that we have inserted the object
         assert(list.empty() == false);
         assert(&list.front() == &myclass);
         assert(myclass.linked() == true);

         //Now myclass' destructor will unlink it
         //automatically
      }

      //Check auto-unlink has been executed
      assert(list.empty() == true);

      {
         //Now test the unlink_self() function

         //Create myclass and check it's linked
         MyClass myclass;
         assert(myclass.linked() == false);

         //Insert the object
         list.push_back(myclass);

         //Check that we have inserted the object
         assert(list.empty() == false);
         assert(&list.front() == &myclass);
         assert(myclass.linked() == true);

         //Now unlink the node
         myclass.unlink_self();

         //Check auto-unlink has been executed
         assert(list.empty() == true);
      }
      return 0;
   }
