//[doc_how_to_use_code

#include <boost/intrusive/ilist.hpp>
#include <vector>

using namespace boost::intrusive;

class MyClass
   :  public ilist_base_hook<0>
{
   int int_;

   public:
   ilist_member_hook<MyClass> member_hook_;

   MyClass(int i)
      :  int_(i)
   {}

   int get() const
   {  return int_;  }
};

//Define an ilist that will store MyClass
//using the public base hook
typedef ilist< ilist_base_hook<0>::
                  value_traits<MyClass> >
   BaseList;

//Define an ilist that will store MyClass
//using the public member hook
typedef ilist< ilist_member_hook<MyClass>::
                  value_traits<&MyClass::member_hook_> >
   MemberList;

int main()
{
   typedef std::vector<MyClass> Vect;
   typedef Vect::iterator VectIt;
   typedef Vect::reverse_iterator VectRit;

   //Create several MyClass objects, each one
   //with a different internal number
   Vect myclassvector;
   for(int i = 0; i < 100; ++i)
      myclassvector.push_back(MyClass(i));

   BaseList baselist;
   MemberList memberlist;

   //Now insert them in the reverse order
   //in the base hook intrusive list
   for(VectIt it(myclassvector.begin()), itend(myclassvector.end())
      ; it != itend
      ; ++it){
      baselist.push_front(*it);
   }

   //Now insert them in the same order as in vector in the
   //member hook intrusive list
   for(VectIt it(myclassvector.begin()), itend(myclassvector.end())
      ; it != itend
      ; ++it){
      memberlist.push_back(*it);
   }

   //Now test lists
   {
      BaseList::reverse_iterator rbit(baselist.rbegin()), rbitend(baselist.rend());
      MemberList::iterator mit(memberlist.begin()), mitend(memberlist.end());
      VectIt  it(myclassvector.begin()), itend(myclassvector.end());

      //Test the objects inserted in the base hook list
      for(; it != itend; ++it, ++rbit){
         if(&*rbit != &*it)
            return 1;
      }

      //Test the objects inserted in the member hook list
      for(it = myclassvector.begin(); it != itend; ++it, ++mit){
         if(&*mit != &*it)
            return 1;
      }
   }

   return 0;
}
//]
