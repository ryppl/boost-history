#ifndef BOOST_FSM_FIFO_WORKER_HPP_INCLUDED
#define BOOST_FSM_FIFO_WORKER_HPP_INCLUDED
//////////////////////////////////////////////////////////////////////////////
// Copyright (c) Andreas Huber Doenni 2002-2004.
// Use, modification and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////



#include <boost/config.hpp>
#include <boost/assert.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function/function0.hpp>
#include <boost/bind.hpp>

#ifdef BOOST_HAS_THREADS
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#endif

#include <list>
#include <memory>   // std::allocator


namespace boost
{
namespace fsm
{



template< class Allocator = std::allocator< void > >
class fifo_worker : noncopyable
{
  public:
    //////////////////////////////////////////////////////////////////////////
    #ifdef BOOST_HAS_THREADS
    fifo_worker( bool waitOnEmptyQueue = true ) :
      waitOnEmptyQueue_( waitOnEmptyQueue ),
    #else
    fifo_worker() :
    #endif
      terminated_( false )
    {
    }

    typedef function0< void, Allocator > work_item;

    // We take a non-const reference so that we can move (i.e. swap) the item
    // into the queue, what avoids copying the (possibly heap-allocated)
    // implementation object inside work_item.
    void queue_work_item( work_item & item )
    {
      BOOST_ASSERT( !item.empty() );

      #ifdef BOOST_HAS_THREADS
      mutex::scoped_lock lock( mutex_ );
      #endif

      workQueue_.push_back( work_item() );
      workQueue_.back().swap( item );

      #ifdef BOOST_HAS_THREADS
      queueNotEmpty_.notify_one();
      #endif
    }

    // Convenience overload so that temporary objects can be passed directly
    // instead of having to create a work_item object first. Under most
    // circumstances, this will lead to one unnecessary copy of the
    // function implementation object.
    void queue_work_item( const work_item & item )
    {
      work_item copy = item;
      queue_work_item( copy );
    }

    void terminate()
    {
      work_item item = bind( &fifo_worker::terminate_impl, this );
      queue_work_item( item );
    }

    // Is not mutex-protected! Must only be called from the thread that also
    // calls operator().
    bool terminated() const
    {
      return terminated_;
    }

    unsigned long operator()( unsigned long maxItemCount = 0 )
    {
      unsigned long itemCount = 0;

      while ( !terminated() &&
        ( ( maxItemCount == 0 ) || ( itemCount < maxItemCount ) ) )
      {
        work_item item = dequeue_item();

        if ( item.empty() )
        {
          // item can only be empty when the queue is empty, which only
          // happens in ST builds or when users pass false to the fifo_worker
          // constructor
          return itemCount;
        }

        item();
      }

      return itemCount;
    }

  private:
    //////////////////////////////////////////////////////////////////////////
    work_item dequeue_item()
    {
      #ifdef BOOST_HAS_THREADS
      mutex::scoped_lock lock( mutex_ );

      if ( !waitOnEmptyQueue_ && workQueue_.empty() )
      {
        return work_item();
      }

      while ( workQueue_.empty() )
      {
        queueNotEmpty_.wait( lock );
      }
      #else
      // If the queue happens to run empty in a single-threaded system,
      // waiting for new work items (which means to loop indefinitely!) is
      // pointless as there is no way that new work items could find their way
      // into the queue. The only sensible thing is to exit the loop and
      // return to the caller in this case.
      // Users can then queue new work items before calling operator() again.
      if ( workQueue_.empty() )
      {
        return work_item();
      }
      #endif

      // Optimization: Swap rather than assign to avoid the copy of the
      // implementation object inside function
      work_item result;
      result.swap( workQueue_.front() );
      workQueue_.pop_front();
      return result;
    }

    void terminate_impl()
    {
      terminated_ = true;
    }


    typedef std::list<
      work_item, 
      typename Allocator::template rebind< work_item >::other
    > work_queue_type;

    work_queue_type workQueue_;

    #ifdef BOOST_HAS_THREADS
    mutex mutex_;
    condition queueNotEmpty_;
    const bool waitOnEmptyQueue_;
    #endif

    bool terminated_;
};



} // namespace fsm
} // namespace boost



#endif
