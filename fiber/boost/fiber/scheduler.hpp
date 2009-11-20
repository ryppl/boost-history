
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

#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/fiber_state.hpp>
#include <boost/fiber/detail/strategy.hpp>
#include <boost/fiber/fiber.hpp>

#include <boost/config/abi_prefix.hpp>

# if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable:4251 4275)
# endif

namespace boost {

namespace this_fiber {

bool runs_as_fiber();
boost::fiber::id get_id();
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

	typedef thread_specific_ptr< detail::strategy >	impl_t;
	typedef function< void() >								callable_t;

	static impl_t	impl_;

	static bool runs_as_fiber();

	static fiber::id get_id();

	static void interrupt();

	static bool interruption_requested();

	static bool interruption_enabled();

	static detail::fiber_interrupt_t & interrupt_flags();

	static void yield();

	static void cancel();

	static int priority();

	static void priority( int);

	static void at_exit( callable_t);

	static void interrupt( fiber::id const&);

	static bool interruption_requested( fiber::id const&);

	static void cancel( fiber::id const&);

	static void join( fiber::id const&);

	static void reschedule( fiber::id const&);

	detail::strategy * access_();

public:
	~scheduler();

	bool run();

	bool empty();

	std::size_t size();

	void submit_fiber( fiber);

	template< typename Fn >
	void make_fiber( Fn fn)
	{ access_()->add( fiber( fn) ); }

	template< typename Fn >
	void make_fiber( std::size_t stack_size, Fn fn)
	{ access_()->add( fiber( stack_size, fn) ); }

#ifndef BOOST_FIBER_MAX_ARITY
#define BOOST_FIBER_MAX_ARITY 10
#endif

#define BOOST_FIBER_ARG(z, n, unused) \
   BOOST_PP_CAT(A, n) BOOST_PP_CAT(a, n)
#define BOOST_ENUM_FIBER_ARGS(n) BOOST_PP_ENUM(n, BOOST_FIBER_ARG, ~)

#define BOOST_FIBER_MAKE_FIBER_FUNCTION(z, n, unused) \
	template< typename Fn, BOOST_PP_ENUM_PARAMS(n, typename A) > \
	void make_fiber( Fn fn, BOOST_ENUM_FIBER_ARGS(n)) \
	{ access_()->add( fiber( fn, BOOST_PP_ENUM_PARAMS(n, a) ) ); } \
	template< typename Fn, BOOST_PP_ENUM_PARAMS(n, typename A) > \
	void make_fiber( std::size_t stack_size, Fn fn, BOOST_ENUM_FIBER_ARGS(n)) \
	{ access_()->add( fiber( stack_size, fn, BOOST_PP_ENUM_PARAMS(n, a) ) ); }

BOOST_PP_REPEAT_FROM_TO( 1, BOOST_FIBER_MAX_ARITY, BOOST_FIBER_MAKE_FIBER_FUNCTION, ~)

#undef BOOST_FIBER_MAKE_FIBER_FUNCTION
#undef BOOST_FIBER_MAX_ARITY
};

}}

# if defined(BOOST_MSVC)
# pragma warning(pop)
# endif

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_SCHEDULER_H
