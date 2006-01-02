//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/shmem/ipc/shared_message_queue.hpp>
#include <boost/shmem/named_user_object.hpp>
#include <boost/shmem/containers/map.hpp>
#include <boost/shmem/containers/set.hpp>
#include <boost/shmem/allocators/node_allocator.hpp>
#include <vector>
#include <cstddef>
#include <limits>
#include <boost/thread.hpp>

#ifdef max
#undef max
#endif

/******************************************************************************/
/*                                                                            */
/*  This example tests the process shared message queue.                      */
/*                                                                            */
/******************************************************************************/

using namespace boost::shmem;

//This test inserts messages with different priority and marks them with a 
//time-stamp to check if receiver obtains highges priority messages first and
//messages with same priority are received in fifo order
bool test_priority_order()
{
   shared_message_queue mq1, mq2;
   if(!mq1.open_or_create("test_priority_order", 100, sizeof(std::size_t)) ||
      !mq2.open_or_create("test_priority_order", 100, sizeof(std::size_t)) ){
      return false;
   }

   //We test that the queue is ordered by priority and in the 
   //same priority, is a FIFO
   std::size_t recvd = 0;
   unsigned int priority = 0;
   std::size_t tstamp;

   //We will send 100 message with priority 0-9
   //The message will contain the timestamp of the message
   for(std::size_t i = 0; i < 100; ++i){
      tstamp = i;
      mq1.send(&tstamp, sizeof(tstamp), (unsigned int)(i%10));
   }

   unsigned int priority_prev = std::numeric_limits<unsigned int>::max();
   std::size_t  tstamp_prev = 0;

   //Receive all messages and test those are ordered
   //by priority and by FIFO in the same priority
   for(std::size_t i = 0; i < 100; ++i){
      mq1.receive(&tstamp, sizeof(tstamp), recvd, priority);
      if(priority > priority_prev)
         return false;
      if(priority == priority_prev &&
         tstamp   <= tstamp_prev){
         return false;
      }
      priority_prev  = priority;
      tstamp_prev    = tstamp;
   }
   return true;
};

//This test creates a in memory data-base using Shmem machinery and 
//serializes it through a message queue. Then rebuilds the data-base in 
//another buffer and checks it against the original data-base
bool test_serialize_db()
{
   //Shared memory queues
   shared_message_queue mq1, mq2;

   //Typedef data to create a Shmem map   
   typedef std::pair<const std::size_t, std::size_t> MyPair;
   typedef std::less<std::size_t>   MyLess;
   typedef node_allocator<MyPair, 64, named_user_object::segment_manager>
      node_allocator_t;
   typedef map<std::size_t, 
               std::size_t, 
               std::less<std::size_t>, 
               node_allocator_t>
               MyMap;

   //Some constants
   const std::size_t BufferSize  = 65536;
   const std::size_t MaxMsgSize  = 100;

   //Allocate two memory buffers from heap using vector<char>
   std::vector<char> buffer1(BufferSize, 0), buffer2(BufferSize, 0);

   //Create the message-queue
   if(!mq1.create("message_queue", 1, MaxMsgSize)){
      return false;
   }

   //Open previously created message-queue simulating other process
   if(!mq2.open("message_queue")){
      return false;
   }

   //Shmem machinery to manage previously allocated buffer1 and buffer2
   named_user_object db1, db2;      

   //Create Shmem machinery in the first buffer
   if(!db1.create(&buffer1[0], buffer1.size())){
      return false;
   }

   //Construct the map in the first buffer
   MyMap *map1 = db1.construct<MyMap>("MyMap")
                                     (MyLess(), 
                                      db1.get_segment_manager());
   if(!map1)
      return false;

   //Fill map1 until is full 
   try{
      std::size_t i = 0;
      while(1){
         (*map1)[i] = i;
         ++i;
      }
   }
   catch(boost::shmem::bad_alloc &){}

   //Data control data sending through the message queue
   std::size_t sent = 0;
   std::size_t recvd = 0;
   std::size_t total_recvd = 0;
   unsigned int priority;

   //Send whole first buffer through the mq1, read it 
   //through mq2 to the second buffer
   while(1){
      //Send a fragment of buffer1 through mq1
      std::size_t bytes_to_send = MaxMsgSize < (BufferSize - sent) ? 
                                    MaxMsgSize : (BufferSize - sent);
      switch(mq1.send(&buffer1[sent], bytes_to_send, 0)){
         case shared_message_queue::ok:
            sent += bytes_to_send;         
         break;

         default:
            return false;
      }
      //Receive the fragment through mq2 to buffer2
      switch(mq2.receive(&buffer2[total_recvd], BufferSize - recvd, 
                         recvd,                 priority)){   
         case shared_message_queue::ok:
            total_recvd += recvd; 
         break;

         default:
            return false;
      }

      //Check if we have received all the buffer
      if(total_recvd == BufferSize){
         break;
      }
   }
   
   //We don't need message queues anymore
   mq1.close();
   mq2.close();
    
   //The second buffer will contain a copy of the buffer1 
   //map so let's open Shmem machinery in the second buffer  
   if(!db2.open(&buffer2[0], BufferSize)){
      return false;
   }

   //Let's find the map
   std::pair<MyMap *, std::size_t> ret = db2.find<MyMap>("MyMap");
   MyMap *map2 = ret.first;

   //Check if we have found it
   if(!map2){
      return false;
   }

   //Check if it is a single variable (not an array)
   if(ret.second != 1){
      return false;
   }

   //Now let's compare size
   if(map1->size() != map2->size()){
      return false;
   }

   //Now let's compare all db values
   for(std::size_t i = 0, num_elements = map1->size(); i < num_elements; ++i){
      if((*map1)[i] != (*map2)[i]){
         return false;
      }
   }
   
   //Destroy maps from db-s
   db1.destroy_ptr(map1);
   db2.destroy_ptr(map2);

   db1.close();
   db2.close();

   return true;
}

static const int MsgSize = 10;
static const int NumMsg  = 100000;
static boost::shmem::shared_message_queue s_message_queue;
static char msgsend [10];
static char msgrecv [10];

void receiver()
{
   std::size_t recvd_size;
   unsigned int priority;
   int nummsg = NumMsg;

   while(nummsg--){
      s_message_queue.receive(msgrecv, MsgSize, recvd_size, priority);
   }
}

bool test_buffer_overflow()
{
   s_message_queue.create("mymsg", 10, 10);
   boost::thread thread(&receiver);
   boost::thread::yield();

   int nummsg = NumMsg;

   while(nummsg--){
      s_message_queue.send(msgsend, MsgSize, 0);
   }

   thread.join();
   return true;
}

int main ()
{
   if(!test_priority_order()){ 
      return -1;
   }

   if(!test_serialize_db()){ 
      return -1;
   }

   if(!test_buffer_overflow()){ 
      return -1;
   }

   return 0;
}





