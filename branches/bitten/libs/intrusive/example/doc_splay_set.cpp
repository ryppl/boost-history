/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2006-2007
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////
//[doc_splay_set_code
#include <boost/intrusive/splay_set.hpp>
#include <vector>
#include <algorithm>

using namespace boost::intrusive;

                  //This is a base hook
class MyClass : public splay_set_base_hook<>
{
   int int_;

   public:
   //This is a member hook
   splay_set_member_hook<> member_hook_;

   MyClass(int i)
      :  int_(i)
      {}
   friend bool operator< (const MyClass &a, const MyClass &b)
      {  return a.int_ < b.int_;  }
   friend bool operator> (const MyClass &a, const MyClass &b)
      {  return a.int_ > b.int_;  }
   friend bool operator== (const MyClass &a, const MyClass &b)
      {  return a.int_ < b.int_;  }
};

//Define an set using the base hook that will store values in reverse order
typedef splay_set< MyClass, compare<std::greater<MyClass> > >     BaseSplaySet;

//Define an multiset using the member hook
typedef member_hook<MyClass, splay_set_member_hook<>, &MyClass::member_hook_> MemberOption;
typedef splay_multiset< MyClass, MemberOption>   MemberSplayMultiset;

int main()
{
   typedef std::vector<MyClass>::iterator VectIt;
   typedef std::vector<MyClass>::reverse_iterator VectRit;

   //Create several MyClass objects, each one with a different value
   std::vector<MyClass> values;
   for(int i = 0; i < 100; ++i)  values.push_back(MyClass(i));

   BaseSplaySet baseset;
   MemberSplayMultiset membermultiset;

   //Now insert them in the reverse order in the base hook set
   for(VectIt it(values.begin()), itend(values.end()); it != itend; ++it)
      baseset.insert(*it);

   //Now insert them in the same order as in vector in the member hook set
   for(VectIt it(values.begin()), itend(values.end()); it != itend; ++it)
      membermultiset.insert(*it);

   //Now test sets
   {
      BaseSplaySet::reverse_iterator rbit(baseset.rbegin()), rbitend(baseset.rend());
      MemberSplayMultiset::iterator mit(membermultiset.begin()), mitend(membermultiset.end());
      VectIt it(values.begin()), itend(values.end());

      //Test the objects inserted in the base hook set
      for(; it != itend; ++it, ++rbit)
         if(&*rbit != &*it)   return 1;

      //Test the objects inserted in the member hook set
      for(it = values.begin(); it != itend; ++it, ++mit)
         if(&*mit != &*it) return 1;
   }
   return 0;
}
//]
