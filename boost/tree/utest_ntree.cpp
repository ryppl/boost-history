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

void utest_default_constructor(void);
void utest_iterator_constructor(void);
void utest_ntree_copy_assignment(void);
void utest_ntree_equality_inequality_swap(void);
void utest_ntree_empty_and_root(void);
void utest_ntree_dense(void);

void utest_ntree(void) 
{
   utest_default_constructor();
   utest_iterator_constructor();
   utest_ntree_copy_assignment();
   utest_ntree_equality_inequality_swap();   
   utest_ntree_empty_and_root();
   utest_ntree_dense();
}


void utest_default_constructor(void) 
{
   boost::ntree<3, int> itree;
   
   UNITTEST("default constructor ntree<3, int>", itree.empty());
   UNITTEST("default constructor ntree<3, int>", itree.size() == 0);
   UNITTEST("default constructor ntree<3, int>", itree.begin() == itree.end());
   UNITTEST("default constructor ntree<3, int>", 
	    itree.rbegin() == itree.rend()); 

}


void utest_iterator_constructor(void) 
{
    std::list<int> ilist;
    for (int i = 0; i < 13; i++)
       ilist.push_back(i);

    boost::ntree<3, int> itree(ilist.begin(), ilist.end());   
    UNITTEST("iterator constructor              ", itree.size() == 13);

    boost::ntree<3, int>::iterator iter(itree.root());
    UNITTEST("iterator constructor root element ", iter->first == 0);
    UNITTEST("iterator constructor root element ", iter->second == 0);
    std::advance(iter, 3);
    UNITTEST("iterator constructor third child  ", iter->first == 3);
    UNITTEST("iterator constructor third child  ", iter->second == 3);
    std::advance(iter, 4);
    UNITTEST("iterator constructor grandchild   ", iter->first == 7);
    UNITTEST("iterator constructor grandchild   ", iter->second == 7);
    
    boost::ntree<3, int>::reverse_iterator riter(itree.rbegin());
    UNITTEST("iterator constructor last child   ", riter->first == 12);
    UNITTEST("iterator constructor last child   ", riter->second == 12);
}


void utest_ntree_copy_assignment(void) 
{
   boost::ntree<3, int> itree;

   boost::ntree<3, int> itree2(itree);
   UNITTEST("copy constructor empty tree      ", itree == itree2);

   boost::ntree<3, int> itree4;
   itree4 = itree;
   UNITTEST("assignment empty tree            ", itree == itree4);


   itree.insert_root();

   boost::ntree<3, int> itree3(itree);
   UNITTEST("copy constructor root node only  ", itree == itree3);

   boost::ntree<3, int> itree5;
   itree5 = itree;
   UNITTEST("assignment root node only        ", itree == itree5);


   std::list<int> ilist;
   for (int i = 0; i < 13; i++)
      ilist.push_back(i);
   
   boost::ntree<3, int> itree13(ilist.begin(), ilist.end());   
   boost::ntree<3, int> itree6(itree13);
   UNITTEST("copy constructor dense tree      ", itree6 == itree13);

   boost::ntree<3, int> itree7;
   itree7 = itree13;
   UNITTEST("assignment dense tree            ", itree7 == itree13);

}


void utest_ntree_equality_inequality_swap(void) 
{
   boost::ntree<3, int> itree;
   boost::ntree<3, int> itree2;
   itree2.insert_root();

   UNITTEST("equality empty ntree<3, int>     ", itree == itree);
   UNITTEST("equality with root ntree<3, int> ", itree2 == itree2);
   UNITTEST("inequality ntree<3, int>         ", (itree < itree2));

   swap(itree, itree2);
   UNITTEST("swapped inequality ntree<3, int> ", (itree > itree2));

   itree.clear();
   UNITTEST("cleared ntree<3, int>            ", itree.empty());
}


void utest_ntree_empty_and_root(void) 
{
   boost::ntree<3, int> itree;
   boost::ntree<3, int> itree2;
   itree2.insert_root();
   UNITTEST("ntree<3, int> with root          ", itree2.size() == 1);
   UNITTEST("insert_root twice                ",
	    itree2.end() == itree2.insert_root());
   UNITTEST("insert_root twice                ", itree2.size() == 1);

}



void utest_ntree_dense(void)
{
   std::list<int> ilist;
   for (int i = 0; i < 13; i++)
      ilist.push_back(i);
   
   boost::ntree<3, int> itree13(ilist.begin(), ilist.end());   

   boost::ntree<3, int> itree;
   boost::ntree<3, int>::iterator iroot, ichild0, ichild1, ichild2;
   iroot = itree.insert_root(0);
   ichild0 = itree.insert_child(iroot, 0 , 1);
   ichild1 = itree.insert_child(iroot, 1 , 2);
   ichild2 = itree.insert_child(iroot, 2 , 3);
   itree.insert_child(ichild0, 0 , 4);
   itree.insert_child(ichild0, 1 , 5);
   itree.insert_child(ichild0, 2 , 6);
   itree.insert_child(ichild1, 0 , 7);
   itree.insert_child(ichild1, 1 , 8);
   itree.insert_child(ichild1, 2 , 9);
   itree.insert_child(ichild2, 0 , 10);
   itree.insert_child(ichild2, 1 , 11);
   itree.insert_child(ichild2, 2 , 12);
   
   UNITTEST("dense tree insert root & children", itree == itree13);
}









