//  Boost-sandbox simple unittest implementation file  ----------------------
//  Purpose: only test of facilities in ntree.hpp
//  This file contains the actual tests performed.

//  (C) Copyright Marc Cromme 2002, marc@cromme.dk. Permission to
//  copy, use, modify, sell and distribute this software is granted
//  provided this copyright notice appears in all copies. This
//  software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.

#include "unittest.hpp"            
#include "ntree.hpp"
#include <list>
//#include <algorithm>

void utest_ntree_empty_and_root(void);
void utest_ntree_dense(void);
void utest_ntree_copy_assignment(void);

void utest_ntree(void) 
{
   utest_ntree_empty_and_root();
   utest_ntree_dense();
   utest_ntree_copy_assignment();
}


void utest_ntree_empty_and_root(void) 
{
   boost::ntree<3, int> itree;
   
   UNITTEST("empty ntree<3, int>              ", itree.empty());
   UNITTEST("empty ntree<3, int>              ", itree.size() == 0);
   UNITTEST("empty ntree<3, int>              ", itree.begin() == itree.end());
   UNITTEST("empty ntree<3, int>              ", 
	    itree.rbegin() == itree.rend());

   boost::ntree<3, int> itree2;
   itree2.insert_root();
   UNITTEST("ntree<3, int> with root          ", itree2.size() == 1);
   UNITTEST("insert_root twice                ",
	    itree2.end() == itree2.insert_root());
   UNITTEST("ntree<3, int> with root          ", itree2.size() == 1);
   UNITTEST("ntree<3, int> equality           ", itree == itree);
   UNITTEST("ntree<3, int> equality           ", itree2 == itree2);
   UNITTEST("ntree<3, int> inequality         ", (itree < itree2));

   swap(itree, itree2);
   UNITTEST("ntree<3, int> swapped inequality ", (itree > itree2));

   itree.clear();
   UNITTEST("cleared ntree<3, int>            ", itree.empty());
}

void utest_ntree_dense(void) 
{
//    std::list<int> ilist;
//    for (int i = 0; i < 12; i++)
//       ilist.push_back(i);
   

//    boost::ntree<3, int> itree(ilist.begin(), ilist.end());   
//    UNITTEST("iterator constructor              ", itree.size() == 12);
}

void utest_ntree_copy_assignment(void) 
{
   boost::ntree<3, int> itree;

   boost::ntree<3, int> itree2(itree);
   UNITTEST("copy constructor empty tree      ", itree == itree2);

   itree.insert_root();
   boost::ntree<3, int> itree3(itree);
   UNITTEST("copy constructor root node only  ", itree == itree3);
}










