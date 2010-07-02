////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
////////////////////////////////////////

#ifndef BOOST_CONTAINER_TEST_LIST_TEST_HEADER
#define BOOST_CONTAINER_TEST_LIST_TEST_HEADER

#include <boost/container/detail/config_begin.hpp>
#include "check_equal_containers.hpp"
#include <memory>
#include <list>
#include <vector>
#include <functional>
#include "print_container.hpp"
#include <boost/move/move.hpp>
#include <string>

namespace boost{
namespace container {
namespace test{

template<class V1, class V2>
bool list_copyable_only(V1 *, V2 *, boost::container::containers_detail::false_type)
{
   return true;
}

//Function to check if both sets are equal
template<class V1, class V2>
bool list_copyable_only(V1 *shmlist, V2 *stdlist, boost::container::containers_detail::true_type)
{
   typedef typename V1::value_type IntType;
   shmlist->insert(shmlist->end(), 50, IntType(1));
   stdlist->insert(stdlist->end(), 50, 1);
   if(!test::CheckEqualContainers(shmlist, stdlist)) return false;

   {
      IntType move_me(1);
      shmlist->insert(shmlist->begin(), 50, boost::move(move_me));
      stdlist->insert(stdlist->begin(), 50, 1);
      if(!test::CheckEqualContainers(shmlist, stdlist)) return false;
   }
   {
      IntType move_me(2);
      shmlist->assign(shmlist->size()/2, boost::move(move_me));
      stdlist->assign(stdlist->size()/2, 2);
      if(!test::CheckEqualContainers(shmlist, stdlist)) return false;
   }
   {
      IntType move_me(3);
      shmlist->assign(shmlist->size()*3-1, boost::move(move_me));
      stdlist->assign(stdlist->size()*3-1, 3);
      if(!test::CheckEqualContainers(shmlist, stdlist)) return false;
   }

   {
      IntType copy_me(3);
      const IntType ccopy_me(3);
      shmlist->push_front(copy_me);
      stdlist->push_front(int(3));
      shmlist->push_front(ccopy_me);
      stdlist->push_front(int(3));
      if(!test::CheckEqualContainers(shmlist, stdlist)) return false;
   }

   return true;
}

template<bool DoublyLinked>
struct list_push_data_function
{
   template<class MyShmList, class MyStdList>
   static int execute(int max, MyShmList *shmlist, MyStdList *stdlist)
   {
      typedef typename MyShmList::value_type IntType;
      for(int i = 0; i < max; ++i){
         IntType move_me(i);
         shmlist->push_back(boost::move(move_me));
         stdlist->push_back(i);
         shmlist->push_front(IntType(i));
         stdlist->push_front(int(i));
      }
      if(!CheckEqualContainers(shmlist, stdlist))
         return 1;
      return 0;
   }
};

template<>
struct list_push_data_function<false>
{
   template<class MyShmList, class MyStdList>
   static int execute(int max, MyShmList *shmlist, MyStdList *stdlist)
   {
      typedef typename MyShmList::value_type IntType;
      for(int i = 0; i < max; ++i){
         IntType move_me(i);
         shmlist->push_front(boost::move(move_me));
         stdlist->push_front(i);
         shmlist->push_front(IntType(i));
         stdlist->push_front(int(i));
      }
      if(!CheckEqualContainers(shmlist, stdlist))
         return 1;
      return 0;
   }
};

template<bool DoublyLinked>
struct list_pop_back_function
{
   template<class MyStdList, class MyShmList>
   static int execute(MyShmList *shmlist, MyStdList *stdlist)
   {
      shmlist->pop_back();
      stdlist->pop_back();
      if(!CheckEqualContainers(shmlist, stdlist))
         return 1;
      return 0;
   }
};

template<>
struct list_pop_back_function<false>
{
   template<class MyStdList, class MyShmList>
   static int execute(MyShmList *shmlist, MyStdList *stdlist)
   {
      (void)shmlist; (void)stdlist;
      return 0;
   }
};

template<class MyShmList
        ,bool  DoublyLinked>
int list_test (bool copied_allocators_equal = true)
{
   typedef std::list<int> MyStdList;
   typedef typename MyShmList::value_type IntType;
   const int max = 100;
   typedef list_push_data_function<DoublyLinked> push_data_t;

   try{
      MyShmList *shmlist = new MyShmList;
      MyStdList *stdlist = new MyStdList;

      if(push_data_t::execute(max, shmlist, stdlist)){
         return 1;
      }

      shmlist->erase(shmlist->begin()++);
      stdlist->erase(stdlist->begin()++);
      if(!CheckEqualContainers(shmlist, stdlist)) return 1;

      if(list_pop_back_function<DoublyLinked>::execute(shmlist, stdlist)){
         return 1;
      }

      shmlist->pop_front();
      stdlist->pop_front();
      if(!CheckEqualContainers(shmlist, stdlist)) return 1;

      {
         IntType aux_vect[50];
         for(int i = 0; i < 50; ++i){
            IntType move_me(-1);
            aux_vect[i] = boost::move(move_me);
         }
         int aux_vect2[50];
         for(int i = 0; i < 50; ++i){
            aux_vect2[i] = -1;
         }
         shmlist->assign(boost::make_move_iterator(&aux_vect[0])
                        ,boost::make_move_iterator(&aux_vect[50]));
         stdlist->assign(&aux_vect2[0], &aux_vect2[50]);
         if(!CheckEqualContainers(shmlist, stdlist)) return 1;
      }

      if(copied_allocators_equal){
         shmlist->sort();
         stdlist->sort();
         if(!CheckEqualContainers(shmlist, stdlist)) return 1;
      }

      shmlist->reverse();
      stdlist->reverse();
      if(!CheckEqualContainers(shmlist, stdlist)) return 1;

      shmlist->reverse();
      stdlist->reverse();
      if(!CheckEqualContainers(shmlist, stdlist)) return 1;

      {
         IntType aux_vect[50];
         for(int i = 0; i < 50; ++i){
            IntType move_me(-1);
            aux_vect[i] = boost::move(move_me);
         }
         int aux_vect2[50];
         for(int i = 0; i < 50; ++i){
            aux_vect2[i] = -1;
         }
         shmlist->insert(shmlist->begin()
                        ,boost::make_move_iterator(&aux_vect[0])
                        ,boost::make_move_iterator(&aux_vect[50]));
         stdlist->insert(stdlist->begin(), &aux_vect2[0], &aux_vect2[50]);
      }

      shmlist->unique();
      stdlist->unique();
      if(!CheckEqualContainers(shmlist, stdlist))
         return 1;

      if(copied_allocators_equal){
         shmlist->sort(std::greater<IntType>());
         stdlist->sort(std::greater<int>());
         if(!CheckEqualContainers(shmlist, stdlist))
            return 1;
      }

      for(int i = 0; i < max; ++i){
         IntType new_int(i);
         shmlist->insert(shmlist->end(), boost::move(new_int));
         stdlist->insert(stdlist->end(), i);
         if(!test::CheckEqualContainers(shmlist, stdlist)) return 1;
      }
      if(!test::CheckEqualContainers(shmlist, stdlist)) return 1;

      shmlist->resize(25);
      stdlist->resize(25);
      shmlist->resize(50);
      stdlist->resize(50);
      shmlist->resize(0);
      stdlist->resize(0);
      if(!CheckEqualContainers(shmlist, stdlist))
         return 1;

      if(push_data_t::execute(max, shmlist, stdlist)){
         return 1;
      }
      {
         MyShmList othershmlist(shmlist->get_allocator());
         MyStdList otherstdlist;

         int listsize = (int)shmlist->size();

         if(push_data_t::execute(listsize, shmlist, stdlist)){
            return 1;
         }

         if(copied_allocators_equal){
            shmlist->splice(shmlist->begin(), othershmlist);
            stdlist->splice(stdlist->begin(), otherstdlist);
            if(!CheckEqualContainers(shmlist, stdlist))
               return 1;   
         }

         listsize = (int)shmlist->size();

         if(push_data_t::execute(listsize, shmlist, stdlist)){
            return 1;
         }

         if(push_data_t::execute(listsize, &othershmlist, &otherstdlist)){
            return 1;
         }

         if(copied_allocators_equal){
            shmlist->sort(std::greater<IntType>());
            stdlist->sort(std::greater<int>());
            if(!CheckEqualContainers(shmlist, stdlist))
               return 1;

            othershmlist.sort(std::greater<IntType>());
            otherstdlist.sort(std::greater<int>());
            if(!CheckEqualContainers(&othershmlist, &otherstdlist))
               return 1;

            shmlist->merge(othershmlist, std::greater<IntType>());
            stdlist->merge(otherstdlist, std::greater<int>());
            if(!CheckEqualContainers(shmlist, stdlist))
               return 1;
         }

         if(!list_copyable_only(shmlist, stdlist
                        ,containers_detail::bool_<!is_movable<IntType>::value>())){
            return 1;
         }
      }

      delete shmlist;
      delete stdlist;
   }
   catch(...){
      throw;
   }
   return 0;
}

}  //namespace test{
}  //namespace container {
}  //namespace boost{

#include <boost/container/detail/config_end.hpp>

#endif