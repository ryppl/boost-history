#ifndef BOOST_FSM_DETAIL_EVENT_PROCESSOR_INCLUDED
#define BOOST_FSM_DETAIL_EVENT_PROCESSOR_INCLUDED
//////////////////////////////////////////////////////////////////////////////
// Copyright (c) Andreas Huber Doenni 2002-2004.
// Use, modification and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////



namespace boost
{
namespace fsm
{
namespace detail
{



//////////////////////////////////////////////////////////////////////////////
template< class Worker >
class event_processor
{
  public:
    //////////////////////////////////////////////////////////////////////////
    typedef typename Worker::event_ptr_type event_ptr_type;

    void queue_event( const event_ptr_type & pEvent )
    {
      myWorker_.queue_event( *this, pEvent );
    }

  protected:
    //////////////////////////////////////////////////////////////////////////
    event_processor( Worker & myWorker ) : myWorker_( myWorker )
    {
      myWorker_.add_processor( *this );
    }

    ~event_processor()
    {
      myWorker_.remove_processor( *this );
    }

  public:
    //////////////////////////////////////////////////////////////////////////
    // The following declarations should be private.
    // They are only public because many compilers lack template friends.
    //////////////////////////////////////////////////////////////////////////
    bool initiate()
    {
      return initiate_impl();
    }

    bool process_event( const event_ptr_type & pEvent )
    {
      return process_event_impl( pEvent );
    }

    void terminate()
    {
      terminate_impl();
    }

  private:
    //////////////////////////////////////////////////////////////////////////
    virtual bool initiate_impl() = 0;
    virtual bool process_event_impl( const event_ptr_type & pEvent ) = 0;
    virtual void terminate_impl() = 0;

    Worker & myWorker_;
};



} // namespace detail
} // namespace fsm
} // namespace boost



#endif
