//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_STM_TRANSACTION_BOOKKEEPING_H
#define BOOST_STM_TRANSACTION_BOOKKEEPING_H

#include <iostream>
#include <vector>
#include <map>
#include <cstddef>
//~ #include <pthread.h>
#include <boost/thread.hpp>

#include <boost/stm/datatypes.hpp>

namespace boost { namespace stm {

//-----------------------------------------------------------------------------
class ThreadIdAndCommitId
{
public:

   ThreadIdAndCommitId(thread_id_t threadId, std::size_t const &commitId) :
      threadId_(threadId), commitId_(commitId) {}

   thread_id_t threadId_;
   std::size_t commitId_;

   bool operator==(ThreadIdAndCommitId const &rhs) const
   { return threadId_ == rhs.threadId_ && commitId_ == rhs.commitId_; }

   bool operator<(ThreadIdAndCommitId const &rhs) const
   {
      if (threadId_ < rhs.threadId_) return true;
      if (threadId_ == rhs.threadId_)
      {
         if (commitId_ < rhs.commitId_) return true;
      }
      return false;
   }
};

//-----------------------------------------------------------------------------
class transaction_bookkeeping
{
public:

   typedef std::map<thread_id_t, std::size_t> thread_commit_map;
   typedef std::map<ThreadIdAndCommitId, std::size_t> CommitHistory;
   typedef std::map<ThreadIdAndCommitId, std::size_t> AbortHistory;

   transaction_bookkeeping() : aborts_(0), writeAborts_(0), readAborts_(0),
      abortPermDenied_(0), commits_(0), handOffs_(0), newMemoryCommits_(0),
      newMemoryAborts_(0), deletedMemoryCommits_(0), deletedMemoryAborts_(0),
      readStayedAsRead_(0), readChangedToWrite_(0), commitTimeMs_(0), lockConvoyMs_(0)
   {
      //abortTrackingMutex_ = PTHREAD_MUTEX_INITIALIZER;
   }

   clock_t const & lockConvoyMs() const { return lockConvoyMs_; }
   clock_t const & commitTimeMs() const { return commitTimeMs_; }
   std::size_t const & readAborts() const { return readAborts_; }
   std::size_t const & writeAborts() const { return writeAborts_; }
   std::size_t const & abortPermDenied() const { return abortPermDenied_; }
   std::size_t const totalAborts() const { return readAborts_ + writeAborts_ + abortPermDenied_; }
   std::size_t const & commits() const { return commits_; }
   std::size_t const & handOffs() const { return handOffs_; }
   std::size_t const & newMemoryAborts() const { return newMemoryAborts_; }
   std::size_t const & newMemoryCommits() const { return newMemoryCommits_; }
   std::size_t const & deletedMemoryAborts() const { return deletedMemoryAborts_; }
   std::size_t const & deletedMemoryCommits() const { return deletedMemoryCommits_; }
   std::size_t const & readChangedToWrite() const { return readChangedToWrite_; }
   std::size_t const & readStayedAsRead() const { return readStayedAsRead_; }

   void inc_read_aborts() { ++readAborts_; }
   void inc_write_aborts() { ++writeAborts_; }

   void inc_thread_commits(thread_id_t threadId)
   {
#if 0
      std::map<thread_id_t, std::size_t>::iterator i = threadedCommits_.find(threadId);

      if (threadedCommits_.end() == i) threadedCommits_[threadId] = 1;
      else i->second = i->second + 1;
#endif
   }

   void inc_thread_aborts(thread_id_t threadId)
   {
#if 0
      std::map<thread_id_t, std::size_t>::iterator i = threadedAborts_.find(threadId);

      if (threadedAborts_.end() == i)
      {
         threadedAborts_.insert(std::make_pair(threadId, 1));
      }
      else
      {
         i->second += 1;
      }
#endif
   }

   thread_commit_map const & threadedCommits() const { return threadedCommits_; }
   thread_commit_map const & threadedAborts() const { return threadedAborts_; }

   void inc_lock_convoy_ms(clock_t const &rhs) { lockConvoyMs_ += rhs; }
   void inc_commit_time_ms(clock_t const &rhs) { commitTimeMs_ += rhs; }
   void inc_commits() { ++commits_; inc_thread_commits(this_thread::get_id()); }
   void inc_abort_perm_denied(thread_id_t const &threadId) { ++abortPermDenied_; inc_thread_aborts(threadId); }
   void inc_handoffs() { ++handOffs_; }
   void inc_new_mem_aborts_by(std::size_t const &rhs) { newMemoryAborts_ += rhs; }
   void inc_new_mem_commits_by(std::size_t const &rhs) { newMemoryCommits_ += rhs; }
   void inc_del_mem_aborts_by(std::size_t const &rhs) { deletedMemoryAborts_ += rhs; }
   void inc_del_mem_commits_by(std::size_t const &rhs) { deletedMemoryCommits_ += rhs; }
   void incrementReadChangedToWrite() { ++readChangedToWrite_; }
   void incrementReadStayedAsRead() { ++readStayedAsRead_; }

   CommitHistory const& getCommitReadSetList() const { return committedReadSetSize_; }
   CommitHistory const& getCommitWriteSetList() const { return committedWriteSetSize_; }
   AbortHistory const& getAbortReadSetList() const { return abortedReadSetSize_; }
   AbortHistory const& getAbortWriteSetList() const { return abortedWriteSetSize_; }

   void pushBackSizeOfReadSetWhenAborting(std::size_t const &size)
   {
      //lock(&abortTrackingMutex_);

      ThreadIdAndCommitId tcId(this_thread::get_id(), ++aborts_);

      // if waiting for commit read from thread is already true, it means there
      // was no commit on the last abort, so drop it from the map

      if (waitingForCommitReadFromThread[this_thread::get_id()])
      {
         abortedReadSetSize_.erase(ThreadIdAndCommitId(this_thread::get_id(), aborts_-1));
         abortedWriteSetSize_.erase(ThreadIdAndCommitId(this_thread::get_id(), aborts_-1));
      }

      abortedReadSetSize_[tcId] = size;
      waitingForCommitReadFromThread[this_thread::get_id()] = true;
      //unlock(&abortTrackingMutex_);
   }

   void pushBackSizeOfWriteSetWhenAborting(std::size_t const &size)
   {
      //lock(&abortTrackingMutex_);
      ThreadIdAndCommitId tcId(this_thread::get_id(), aborts_);
      abortedWriteSetSize_[tcId] = size;
      waitingForCommitWriteFromThread[this_thread::get_id()] = true;
      //unlock(&abortTrackingMutex_);
   }

   void pushBackSizeOfReadSetWhenCommitting(std::size_t const &size)
   {
      //lock(&abortTrackingMutex_);
      ThreadIdAndCommitId tcId(this_thread::get_id(), aborts_);

      // only insert this commit if an abort made an entry at this commit point
      if (waitingForCommitReadFromThread[this_thread::get_id()])
      {
         committedReadSetSize_[tcId] = size;
         waitingForCommitReadFromThread[this_thread::get_id()] = false;
      }
      //unlock(&abortTrackingMutex_);
   }

   void pushBackSizeOfWriteSetWhenCommitting(std::size_t const &size)
   {
      //lock(&abortTrackingMutex_);
      ThreadIdAndCommitId tcId(this_thread::get_id(), aborts_);

      // only insert this commit if an abort made an entry at this commit point
      if (waitingForCommitWriteFromThread[this_thread::get_id()])
      {
         committedWriteSetSize_[tcId] = size;
         waitingForCommitWriteFromThread[this_thread::get_id()] = false;
      }
      //unlock(&abortTrackingMutex_);
   }

   bool isLoggingAbortAndCommitSize() const { return isLoggingAbortAndCommitSize_; }
   void setIsLoggingAbortAndCommitSize(bool const &in) { isLoggingAbortAndCommitSize_ = in; }

   ////////////////////////////////////////////////////////////////////////////
   friend std::ostream& operator<<(std::ostream& out, transaction_bookkeeping const &that)
   {
      using namespace std;

      out << "########################################" << endl;
      //out << "total_aborts=" << that.totalAborts() << endl;


      for (thread_commit_map::const_iterator i = that.threadedCommits().begin();
         i != that.threadedCommits().end(); ++i)
      {
         out << " thread [" << i->first << "]:  commits: " << i->second << "  aborts: ";

         bool found = false;

         for (thread_commit_map::const_iterator j = that.threadedAborts().begin();
            j != that.threadedAborts().end(); ++j)
         {
            if (j->first == i->first)
            {
               out << j->second;
               found = true;
               break;
            }
         }

         if (!found) out << 0;

         out << endl;
      }

      return out;
   }

private:

   bool isLoggingAbortAndCommitSize_;

   AbortHistory abortedReadSetSize_;
   AbortHistory abortedWriteSetSize_;
   CommitHistory committedReadSetSize_;
   CommitHistory committedWriteSetSize_;

   std::map<thread_id_t, bool> waitingForCommitReadFromThread;
   std::map<thread_id_t, bool> waitingForCommitWriteFromThread;

   thread_commit_map threadedCommits_;
   thread_commit_map threadedAborts_;

   std::size_t aborts_;
   std::size_t writeAborts_;
   std::size_t readAborts_;
   std::size_t abortPermDenied_;
   std::size_t commits_;
   std::size_t handOffs_;
   std::size_t newMemoryCommits_;
   std::size_t newMemoryAborts_;
   std::size_t deletedMemoryCommits_;
   std::size_t deletedMemoryAborts_;
   std::size_t readStayedAsRead_;
   std::size_t readChangedToWrite_;
   clock_t commitTimeMs_;
   clock_t lockConvoyMs_;

   //Mutex abortTrackingMutex_;
};

}}
#endif // TRANSACTION_BOOKKEEPING_H

