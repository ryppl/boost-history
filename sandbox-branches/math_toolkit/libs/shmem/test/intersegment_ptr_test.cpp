//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/shmem/intersegment_ptr.hpp>
#include <boost/shmem/multi_named_shared_object.hpp>
#include <boost/shmem/mem_algo/multi_simple_seq_fit.hpp>

//Instantiation headers
#include <boost/shmem/sync/shared_mutex.hpp>
#include <boost/shmem/allocators/allocator.hpp>
#include <boost/shmem/containers/vector.hpp>
#include <boost/shmem/containers/flat_set.hpp>
#include <boost/shmem/containers/list.hpp>
#include <boost/shmem/containers/set.hpp>
#include <boost/shmem/indexes/map_index.hpp>
#include <boost/shmem/streams/bufferstream.hpp>
#include <stdio.h>
#include <deque>

using namespace boost::shmem;

typedef flat_map_intersegment<shared_mutex> basic_is_ptr;
template class intersegment_ptr<int, basic_is_ptr>;
typedef class intersegment_ptr<int, basic_is_ptr> is_ptr;
template class basic_multi_named_shared_object
   <char
   ,multi_simple_seq_fit<shared_mutex_family, intersegment_ptr<void, basic_is_ptr> >
   ,map_index>;
typedef basic_multi_named_shared_object
   <char
   ,multi_simple_seq_fit<shared_mutex_family, intersegment_ptr<void, basic_is_ptr> >
   ,map_index> my_named_shared_object;
typedef allocator<int, my_named_shared_object::segment_manager> allocator_int_t;

//Force instantiation of several containers to catch compile-time errors
template class vector<int, allocator_int_t >;
template class flat_set<int, std::less<int>, allocator_int_t >;
template class set<int, std::less<int>, allocator_int_t >;
template class list<int, allocator_int_t>;

//Multi-segment, shared memory linked list node
struct list_node
{
   list_node(){}
   list_node(list_node* ptr, int val)
      : next(ptr), value(val){}
   intersegment_ptr<list_node>   next;
   int                           value;
};

static int slist_test()
{
   //Create shared memory
   const std::size_t segment_size = 65536;
   my_named_shared_object segment0, segment1;
   segment0.create("/MySharedMemory0",//segment0 name
                   65536);            //segment0 size in bytes
   segment1.create("/MySharedMemory1",//segment1 name
                   65536);            //segment1 size in bytes

   //Create linked list shared memory
   intersegment_ptr<list_node> prev = 0, current, first;

   //Let's make sure we need more than 1 segment
   int i, max = 4*(segment_size/sizeof(list_node));

   for(i = 0; i < max; ++i, prev = current){
      //Allocate even nodes from segment0 and odds from segment1
      if((i%2)==0)
         current = static_cast<list_node*>(segment0.allocate(sizeof(list_node)));
      else
         current = static_cast<list_node*>(segment1.allocate(sizeof(list_node)));
      current->value = i;
      current->next  = 0;

      if(!prev)
         first = current;
      else
         prev->next = current;
   }

   //Communicate list to other processes
   //. . .
   //When done, destroy list
   i = 0;
   for(current = first; current; ++i){
      prev = current;
      current = current->next;
      assert(prev->value == i);
      //Deallocate even nodes from segment0 and odds from segment1
      if((i%2)==0)
         segment0.deallocate(prev.get());
      else
         segment1.deallocate(prev.get());
   }

   return 0;
}

static int named_alloc_test()
{
   my_named_shared_object multi_named_segment;
   multi_named_segment.create("named_object_segments", 10000);
   const std::size_t Name_Buf_Size = 100;
   char name[Name_Buf_Size];
   obufferstream formatter(name, Name_Buf_Size);

   try{  
      typedef std::deque<list_node*> object_stack_t;
      std::deque<list_node*> object_stack;
      for(std::size_t i = 0; i < 10000; ++i){
         formatter.seekp(0, std::ios::beg);
         formatter << "my_object" << static_cast<unsigned int>(i) << std::ends;
         object_stack.push_back
            (multi_named_segment.construct<list_node>(name)());
      }

      object_stack_t::reverse_iterator 
         rit    = object_stack.rbegin(), 
         ritend = object_stack.rend();

      for(; rit != ritend; ++rit){
         multi_named_segment.destroy_ptr(*rit);
      }
   }
   catch(const bad_alloc &){
      return -1;
   }
   return 0;   
}


int main ()
{
   if(slist_test()==-1){
      return -1;
   }

   if(named_alloc_test()==-1){
      return -1;
   }

   return 0;
}
