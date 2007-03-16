//[doc_iset_code
#include <boost/intrusive/iset.hpp>
#include <vector>
#include <algorithm>

using namespace boost::intrusive;

class MyClass
   //This is a derivation hook
   :  public iset_base_hook<0>
{
   int int_;

   public:
   //This is a member hook
   iset_member_hook<MyClass> member_hook_;

   MyClass(int i)
      :  int_(i)
   {}

   int get() const
   {  return int_;  }

   friend bool operator< (const MyClass &a, const MyClass &b)
   {  return a.get() < b.get();  }

   friend bool operator> (const MyClass &a, const MyClass &b)
   {  return a.get() > b.get();  }
};

//Define an iset that will store MyClass
//in reverse order using the public base hook
typedef iset< iset_base_hook<0>::
               value_traits<MyClass>
            , std::greater<MyClass> >
   BaseSet;

//Define an imultiset that will store MyClass
//using the public member hook
typedef imultiset< iset_member_hook<MyClass>::
                     value_traits<&MyClass::member_hook_>
                 , std::less<MyClass> >
   MemberIMultiset;

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

   BaseSet baseset;
   MemberIMultiset membermultiset;

   //Now insert them in the reverse order
   //in the base hook intrusive set
   for(VectIt it(myclassvector.begin()), itend(myclassvector.end())
      ; it != itend
      ; ++it){
      baseset.insert(*it);
   }

   //Now insert them in the same order as in vector in the
   //member hook intrusive set
   for(VectIt it(myclassvector.begin()), itend(myclassvector.end())
      ; it != itend
      ; ++it){
      membermultiset.insert(*it);
   }

   //Now test sets
   {
      BaseSet::reverse_iterator rbit(baseset.rbegin()), rbitend(baseset.rend());
      MemberIMultiset::iterator mit(membermultiset.begin()), mitend(membermultiset.end());
      VectIt it(myclassvector.begin()), itend(myclassvector.end());

      //Test the objects inserted in the base hook set
      for(; it != itend; ++it, ++rbit){
         if(&*rbit != &*it)
            return 1;
      }

      //Test the objects inserted in the member hook set
      for(it = myclassvector.begin(); it != itend; ++it, ++mit){
         if(&*mit != &*it)
            return 1;
      }
   }
   return 0;
}
//]
