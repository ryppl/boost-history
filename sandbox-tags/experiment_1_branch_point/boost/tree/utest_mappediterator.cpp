//  Boost-sandbox simple unittest implementation file  ----------------------
//  Purpose: only test of facilities in mapped_iterator.hpp
//  This file contains the actual tests performed.

//  (C) Copyright Marc Cromme 2002, marc@cromme.dk. Permission to
//  copy, use, modify, sell and distribute this software is granted
//  provided this copyright notice appears in all copies. This
//  software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.

#include "unittest.hpp"            
#include "mapped_iterator.hpp"
#include <map>
//#include <iostream>
//#include <algorithm>


void utest_mappediteratorbasics(void) 
{
   using namespace boost;

   std::map<size_t, int>  rep;

   for (std::map<size_t, int>::size_type id = 0; id < 14; id++)
    {
      std::pair<const size_t, int> valuepair =
	 std::make_pair<const size_t, int>(id, (int) id);
      rep.insert(valuepair);
   }

   mapped_iterator<std::map<size_t, int> > first(rep.begin());
   mapped_iterator<std::map<size_t, int> > last(rep.end());   
   mapped_iterator<std::map<size_t, int> > tmp;   
   mapped_iterator<std::map<size_t, int> > tmp2(last);   

   UNITTEST("mapped_iterator default constructor", tmp == tmp);
   UNITTEST("mapped_iterator copy constructor   ", tmp2 == tmp2);
   UNITTEST("std::map::iterator constructor     ", first != last);
   UNITTEST("mapped_iterator dereferencing      ", *first == 0);
   UNITTEST("mapped_iterator key                ", first.key() == 0);
   UNITTEST("mapped_iterator increment          ", *(first++) == 0);
   UNITTEST("mapped_iterator increment          ", *(++first) == 2);
   UNITTEST("mapped_iterator decrement          ", *(--last) == 13);
   UNITTEST("mapped_iterator decrement          ", *(last--) == 13);
   UNITTEST("mapped_iterator decrement          ", *last == 12);
   UNITTEST("mapped_iterator decremented key    ", last.key() == 12);
   UNITTEST("mapped_iterator assignment         ", (tmp = last) == last);
}

 

void utest_mappediteratortypes(void) 
{
   using namespace boost;

   //mapped_iterator<std::map<size_t, int> >::iterator_category cat = 1;
   //UNITTEST("mapped_iterator::iterator_category ", cat);
   mapped_iterator<std::map<size_t, int> >::value_type val = 1;
   UNITTEST("mapped_iterator::value_type        ", val);
   mapped_iterator<std::map<size_t, int> >::difference_type dif = 1;
   UNITTEST("mapped_iterator::difference_type   ", dif);
   mapped_iterator<std::map<size_t, int> >::pointer pnt = &val;
   UNITTEST("mapped_iterator::pointer           ", *pnt);
   mapped_iterator<std::map<size_t, int> >::reference ref = val;
   UNITTEST("mapped_iterator::reference         ", ref);  
}


void utest_constmappediteratorbasics(void) 
{
   using namespace boost;

   std::map<size_t, int>  rep;

   for (std::map<size_t, int>::size_type id = 0; id < 14; id++)
    {
      std::pair<const size_t, int> valuepair =
	 std::make_pair<const size_t, int>(id, (int) id);
      rep.insert(valuepair);
   }

   const_mapped_iterator<std::map<size_t, int> > first(rep.begin());
   const_mapped_iterator<std::map<size_t, int> > last(rep.end());   
   const_mapped_iterator<std::map<size_t, int> > tmp;   
   const_mapped_iterator<std::map<size_t, int> > tmp2(last);   

   UNITTEST("const_mapped_iterator default constructor", tmp == tmp);
   UNITTEST("const_mapped_iterator copy constructor   ", tmp2 == tmp2);
   UNITTEST("std::map::iterator constructor     ", first != last);
   UNITTEST("const_mapped_iterator dereferencing      ", *first == 0);
   UNITTEST("const_mapped_iterator key                ", first.key() == 0);
   UNITTEST("const_mapped_iterator increment          ", *(first++) == 0);
   UNITTEST("const_mapped_iterator increment          ", *(++first) == 2);
   UNITTEST("const_mapped_iterator decrement          ", *(--last) == 13);
   UNITTEST("const_mapped_iterator decrement          ", *(last--) == 13);
   UNITTEST("const_mapped_iterator decrement          ", *last == 12);
   UNITTEST("const_mapped_iterator decremented key    ", last.key() == 12);
   UNITTEST("const_mapped_iterator assignment         ", (tmp = last) == last);
}

 

void utest_constmappediteratortypes(void) 
{
   using namespace boost;

   //const_mapped_iterator<std::map<size_t, int> >::iterator_category cat = 1;
   //UNITTEST("const_mapped_iterator::iterator_category ", cat);
   const_mapped_iterator<std::map<size_t, int> >::value_type val = 1;
   UNITTEST("const_mapped_iterator::value_type        ", val);
   const_mapped_iterator<std::map<size_t, int> >::difference_type dif = 1;
   UNITTEST("const_mapped_iterator::difference_type   ", dif);
   const_mapped_iterator<std::map<size_t, int> >::pointer pnt = &val;
   UNITTEST("const_mapped_iterator::pointer           ", *pnt);
   const_mapped_iterator<std::map<size_t, int> >::reference ref = val;
   UNITTEST("const_mapped_iterator::reference         ", ref);  
}


void utest_mappediterator(void) 
{
   utest_mappediteratortypes();
   utest_mappediteratorbasics();
   utest_constmappediteratortypes();
   utest_constmappediteratorbasics();
}




