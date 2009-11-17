
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_SCHEDULER_H
#define BOOST_FIBERS_SCHEDULER_H

#include <cstddef>
#include <memory>

#include <boost/function.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/thread/tss.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/attributes.hpp>
#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/fiber_state.hpp>
#include <boost/fiber/detail/scheduler_impl.hpp>
#include <boost/fiber/fiber.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {

namespace this_fiber {

bool runs_as_fiber();
boost::fiber::id get_id();
void yield();
void cancel();
void suspend();
int priority();
void priority( int);
void interruption_point();
bool interruption_requested();
bool interruption_enabled();
template< typename Callable >
void at_fiber_exit( Callable);
void at_fiber_exit( function< void() >);
void at_fiber_exit( void (*)() );

}

namespace fibers {

class disable_interruption;
class restore_interruption;

class BOOST_FIBER_DECL scheduler : private noncopyable
{
private:
	friend bool this_fiber::runs_as_fiber();
	friend fiber::id this_fiber::get_id();
	friend void this_fiber::yield();
	friend void this_fiber::cancel();
	friend void this_fiber::suspend();
	friend int this_fiber::priority();
	friend void this_fiber::priority( int);
	friend void this_fiber::interruption_point();
	friend bool this_fiber::interruption_requested();
	friend bool this_fiber::interruption_enabled();
	template< typename Callable >
	friend void this_fiber::at_fiber_exit( Callable);
	friend void this_fiber::at_fiber_exit( function< void() >);
	friend void this_fiber::at_fiber_exit( void (*)() );
	friend class fiber;
	friend class disable_interruption;
	friend class restore_interruption;

	typedef thread_specific_ptr< detail::scheduler_impl >	impl_t;
	typedef function< void() >								callable_t;

	static impl_t	impl_;

	static bool runs_as_fiber();

	static fiber::id id_active_fiber();

	static void yield_active_fiber();

	static void cancel_active_fiber();

	static void suspend_active_fiber();

	static void interrupt_active_fiber();

	static bool interruption_requested_active_fiber();

	static bool interruption_enabled_active_fiber();

	static detail::fiber_interrupt_t & interrupt_flags_active_fiber();

	static int priority_active_fiber();

	static void priority_active_fiber( int);

	static void at_exit_active_fiber( callable_t);

	static void cancel_fiber( fiber::id const&);

	static void suspend_fiber( fiber::id const&);

	static void resume_fiber( fiber::id const&);

	static void reschedule_fiber( fiber::id const&);

	static void join_fiber( fiber::id const&);

	detail::scheduler_impl * access_();

public:
	~scheduler();

	bool run();

	bool empty();

	std::size_t size();

	void submit_fiber( fiber);

	template< typename Fn >
	void make_fiber( Fn fn)
	{ access_()->add_fiber( fiber( fn) ); }

	template< typename Fn >
	void make_fiber( attributes attrs, Fn fn)
	{ access_()->add_fiber( fiber( attrs, fn) ); }

#ifndef BOOST_FIBER_MAX_ARITY
#define BOOST_FIBER_MAX_ARITY 10
#endif

#define BOOST_FIBER_ARG(z, n, unused) \
   BOOST_PP_CAT(A, n) BOOST_PP_CAT(a, n)
#define BOOST_ENUM_FIBER_ARGS(n) BOOST_PP_ENUM(n, BOOST_FIBER_ARG, ~)

#define BOOST_FIBER_MAKE_FIBER_FUNCTION(z, n, unused) \
	template< typename Fn, BOOST_PP_ENUM_PARAMS(n, typename A) > \
	void make_fiber( Fn fn, BOOST_ENUM_FIBER_ARGS(n)) \
	{ access_()->add_fiber( fiber( fn, BOOST_PP_ENUM_PARAMS(n, a) ) ); } \
	template< typename Fn, BOOST_PP_ENUM_PARAMS(n, typename A) > \
	void make_fiber( attributes const& attrs, Fn fn, BOOST_ENUM_FIBER_ARGS(n)) \
	{ access_()->add_fiber( fiber( attrs, fn, BOOST_PP_ENUM_PARAMS(n, a) ) ); }

BOOST_PP_REPEAT_FROM_TO( 1, BOOST_FIBER_MAX_ARITY, BOOST_FIBER_MAKE_FIBER_FUNCTION, ~)

#undef BOOST_FIBER_MAKE_FIBER_FUNCTION
#undef BOOST_FIBER_MAX_ARITY
};

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_SCHEDULER_H
