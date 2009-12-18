
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_STRATEGY_H
#define BOOST_FIBERS_STRATEGY_H

#include <cstddef>

#include <boost/function.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/thread/tss.hpp>

#include <boost/fiber/detail/interrupt_flags.hpp>
#include <boost/fiber/object/id.hpp>
#include <boost/fiber/fiber.hpp>

#include <boost/config/abi_prefix.hpp>

# if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable:4251 4275)
# endif

namespace boost {

namespace this_fiber {

bool runs_as_fiber();
fiber::id get_id();
void yield();
void cancel();
int priority();
void priority( int);
void interruption_point();
bool interruption_requested();
bool interruption_enabled();
template< typename Callable >
void at_fiber_exit( Callable);
void at_fiber_exit( function< void() >);
void at_fiber_exit( void (*)() );

class disable_interruption;
class restore_interruption;

}

namespace fibers {

class auto_reset_event;
class condition;
class count_down_event;
class manual_reset_event;
class mutex;

class BOOST_FIBER_DECL strategy
{
private:
	friend bool this_fiber::runs_as_fiber();
	friend fiber::id this_fiber::get_id();
	friend void this_fiber::yield();
	friend void this_fiber::cancel();
	friend int this_fiber::priority();
	friend void this_fiber::priority( int);
	friend void this_fiber::interruption_point();
	friend bool this_fiber::interruption_requested();
	friend bool this_fiber::interruption_enabled();
	template< typename Callable >
	friend void this_fiber::at_fiber_exit( Callable);
	friend void this_fiber::at_fiber_exit( function< void() >);
	friend void this_fiber::at_fiber_exit( void (*)() );
	friend class this_fiber::disable_interruption;
	friend class this_fiber::restore_interruption;
	friend class auto_reset_event;
	friend class condition;
	friend class count_down_event;
	friend class manual_reset_event;
	friend class mutex;

	typedef function< void() >						callable_t;

	static bool runs_as_fiber_();

	static fiber::id get_id_();

	static void interruption_point_();

	static bool interruption_requested_();

	static detail::interrupt_t & interrupt_flags_();

	static bool interruption_enabled_();

	static int priority_();

	static void priority_( int);

	static void at_fiber_exit_( callable_t);

	static void yield_();

	static void cancel_();

	uint32_t	use_count_;

protected:
	typedef thread_specific_ptr< fiber >			fiber_t;

	static fiber_t		active_fiber;

	fiber			master_fiber;

	void attach( fiber &);

	void detach( fiber &);

	void switch_between( fiber &, fiber &);

	void enable_interruption( fiber &);

	bool interruption_enabled( fiber const&);

	bool is_master( fiber const&);

	bool in_state_not_started( fiber const&);

	bool in_state_ready( fiber const&);

	bool in_state_running( fiber const&);

	bool in_state_wait_for_fiber( fiber const&);

	bool in_state_wait_for_object( fiber const&);

	bool in_state_terminated( fiber const&);

	void set_state_ready( fiber &);

	void set_state_running( fiber &);

	void set_state_wait_for_fiber( fiber &);

	void set_state_wait_for_object( fiber &);

	void set_state_terminated( fiber &);

public:
	typedef intrusive_ptr< strategy >	ptr_t;

	strategy();

	virtual ~strategy();

	virtual void add( fiber) = 0;

	virtual void join( fiber::id const&) = 0;

	virtual void interrupt( fiber::id const&) = 0;

	virtual void reschedule( fiber::id const&) = 0;

	virtual void cancel( fiber::id const&) = 0;
	
	virtual void yield() = 0;

	virtual void register_object( object::id const&) = 0;

	virtual void unregister_object( object::id const&) = 0;

	virtual void wait_for_object( object::id const&) = 0;

	virtual void object_notify_one( object::id const&) = 0;

	virtual void object_notify_all( object::id const&) = 0;

	virtual fiber release( fiber::id const&) = 0;

	virtual void migrate( fiber) = 0;

	virtual void detach_all() = 0;

	virtual bool run() = 0;

	virtual bool empty() = 0;

	virtual std::size_t size() = 0;

	virtual std::size_t ready() = 0;

	inline friend void intrusive_ptr_add_ref( strategy * p)
	{ ++p->use_count_; }
	
	inline friend void intrusive_ptr_release( strategy * p)
	{ if ( --p->use_count_ == 0) delete p; }
};

}}

# if defined(BOOST_MSVC)
# pragma warning(pop)
# endif

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_STRATEGY_H
