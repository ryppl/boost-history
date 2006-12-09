#include <boost/intrusive/islist.hpp>
#include <vector>

using namespace boost::intrusive;

class MyClass
   :  public islist_base_hook<0>
{
   int int_;

   public:
   islist_member_hook<MyClass> member_hook_;

   MyClass(int i)
      :  int_(i)
   {}

   int get() const
   {  return int_;  }
};

//Define an islist that will store MyClass
//using the public base hook
typedef islist< islist_base_hook<0>::
                  value_traits<MyClass> >
   BaseList;

//Define an islist that will store MyClass
//using the public member hook
typedef islist< islist_member_hook<MyClass>::
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
   for(BaseList::iterator it(baselist.begin()), itend(baselist.end())
      ; it != itend
      ; ++it){
      memberlist.push_front(*it);
   }

   //Now test lists
   {
      BaseList::iterator bit(baselist.begin()), bitend(baselist.end());
      MemberList::iterator mit(memberlist.begin()), mitend(memberlist.end());
      VectRit rit(myclassvector.rbegin()), ritend(myclassvector.rend());
      VectIt  it(myclassvector.begin()), itend(myclassvector.end());

      //Test the objects inserted in the base hook list
      for(; rit != ritend; ++rit, ++bit){
         if(&*bit != &*rit)
            return 1;
      }

      //Test the objects inserted in the member hook list
      for(; it != itend; ++it, ++mit){
         if(&*mit != &*it)
            return 1;
      }
   }

   return 0;
}
