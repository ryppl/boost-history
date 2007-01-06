//[doc_ihashset_code
#include <boost/intrusive/ihashset.hpp>
#include <vector>
#include <algorithm>
#include <boost/functional/hash.hpp>

using namespace boost::intrusive;

class MyClass
   :  public ihashset_base_hook<0>
{
   int int_;

   public:
   ihashset_member_hook<MyClass> member_hook_;

   MyClass(int i)
      :  int_(i)
   {}

   int get() const
   {  return int_;  }

   friend bool operator== (const MyClass &a, const MyClass &b)
   {  return a.get() == b.get();  }

   friend bool operator> (const MyClass &a, const MyClass &b)
   {  return a.get() > b.get();  }
};

std::size_t hash_value(const MyClass &value)
{  return std::size_t(value.get()); }

//Define an ihashset that will store MyClass
//in reverse order using the public base hook
typedef ihashset< ihashset_base_hook<0>::
                  value_traits<MyClass> >
   BaseSet;

//Define an ihashmultiset that will store MyClass
//using the public member hook
typedef ihashmultiset< ihashset_member_hook<MyClass>::
                       value_traits<&MyClass::member_hook_> >
   MemberIMultihashset;

int main()
{
   typedef std::vector<MyClass> Vect;
   typedef Vect::iterator VectIt;
   typedef Vect::reverse_iterator VectRit;

   //Create a vector with 100 different MyClass objects,
   //each one with a different internal number
   Vect myclassvector;
   for(int i = 0; i < 100; ++i)
      myclassvector.push_back(MyClass(i));

   //Create a copy of the vector
   Vect myclassvector2(myclassvector);

   //Create a bucket array for baseset
   BaseSet::bucket_type base_buckets[100];

   //Create a bucket array for membermultihashset
   MemberIMultihashset::bucket_type member_buckets[200];

   //Create a the hashset and hashmultiset,
   //taking buckets as arguments
   BaseSet baseset(base_buckets, 100);
   MemberIMultihashset membermultihashset(member_buckets, 200);

   //Now insert myclassvector's elements in the hashset
   for(VectIt it(myclassvector.begin()), itend(myclassvector.end())
      ; it != itend
      ; ++it){
      baseset.insert(*it);
   }

   //Now insert myclassvector's and myclassvector2's elements in the hashmultiset
   for(VectIt it(myclassvector.begin()), itend(myclassvector.end()),
             it2(myclassvector2.begin()),itend2(myclassvector2.end())
      ; it != itend
      ; ++it, ++it2){
      membermultihashset.insert(*it);
      membermultihashset.insert(*it2);
   }

   //Now find every element
   {
      VectIt it(myclassvector.begin()), itend(myclassvector.end());

      for(; it != itend; ++it){
         //baseset should contain one element for each key
         if(baseset.count(*it) != 1){
            return 1;
         }
         //membermultihashset should contain two elements for each key
         if(membermultihashset.count(*it) != 2){
            return 1;
         }
      }
   }
   return 0;
}
//]

