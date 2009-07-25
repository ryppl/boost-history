
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_FIBER_POSIX_H
#define BOOST_TASK_DETAIL_FIBER_POSIX_H

extern "C"
{
#include <ucontext.h>
}

#include <cstddef>
#include <utility>

#include <boost/assert.hpp>
#include <boost/function.hpp>
#include <boost/shared_array.hpp>
#include <boost/system/system_error.hpp>

#include <boost/fibers/detail/context.hpp>

namespace boost { namespace task {
namespace detail
{
template< typename Fiber >
void trampoline( Fiber * fib)
{
	BOOST_ASSERT( fib);
	context< Fiber > ctxt( * fib);
	BOOST_ASSERT( ! fib->fn_.empty() );
	fib->fn_( ctxt);
	fib->exit();
}

class fiber_base
{
private:
	enum st_state
	{
		st_uninitialized = 0,
		st_ready = 1,
		st_running = 2,
		st_exited = 3
	};

	template< typename Fiber >
	friend
	void trampoline( Fiber *);

	function< void( context< fiber_base > &) >	fn_;
	std::size_t									stack_size_;
	::ucontext_t								caller_;
	::ucontext_t								callee_;
	shared_array< char >						stack_;
	st_state									state_;

	bool uninitialized_() const
	{ return state_ == st_uninitialized; }

	bool ready_() const
	{ return state_ == st_ready; }

	bool running_() const
	{ return state_ == st_running; }

	bool exited_() const
	{ return state_ == st_exited; }

	void yield_()
	{
		if ( ::swapcontext( & callee_, & caller_) == -1)
			throw system::system_error(
				system::error_code(
					errno,
					system::system_category) );
	}

	void yield_to_( fiber_base & to)
	{
		std::swap( caller_, to.caller_);
		std::swap( state_, to.state_);
		if ( ::swapcontext( & callee_, & to.callee_) == -1)
			throw system::system_error(
				system::error_code(
					errno,
					system::system_category) );
	}

	void run_()
	{
		if ( ::swapcontext( & caller_, & callee_) == -1)
			throw system::system_error(
				system::error_code(
					errno,
					system::system_category) );
	}

	void init_()
	{
		BOOST_ASSERT( state_ == st_uninitialized);

		if ( ::getcontext( & caller_) == -1)
			throw system::system_error(
				system::error_code(
					errno,
					system::system_category) );
		BOOST_ASSERT( stack_);
		if ( ::getcontext( & callee_) == -1)
			throw system::system_error(
				system::error_code(
					errno,
					system::system_category) );
		callee_.uc_stack.ss_sp = stack_.get();
		callee_.uc_stack.ss_size = stack_size_;
		callee_.uc_link = 0;
		typedef void fn_type( fiber_base *);
		typedef void ( * st_fn)();
		fn_type * fn_ptr( trampoline< fiber_base >);

		::makecontext(
			& callee_,
			( st_fn)( fn_ptr),
			1,
			this);

		state_ = st_ready;
	}

public:
	fiber_base(
		function< void( context< fiber_base > &) > fn,
		std::size_t stack_size)
	:
	fn_( fn),
	stack_size_( stack_size),
	caller_(),
	callee_(),
	stack_( new char[stack_size]),
	state_( st_uninitialized)
	{
		BOOST_ASSERT( ! fn_.empty() );
		BOOST_ASSERT( stack_size_ > 0);
	}

	~fiber_base()
	{ BOOST_ASSERT( ! running_() ); }

	bool ready() const
	{ return uninitialized_() || ready_(); }

    bool running() const
	{ return running_(); }

    bool exited() const
	{ return exited_(); }

	void yield()
	{
		BOOST_ASSERT( running_() );
		state_ = st_ready;
		yield_();
		BOOST_ASSERT( running_() );
	}

	void yield_to( fiber_base & to)
	{
		BOOST_ASSERT( running_() );
		if ( to.uninitialized_() ) to.init_();
		yield_to_( to);
		BOOST_ASSERT( running_() );
	}

	void run()
	{
		BOOST_ASSERT( uninitialized_() || ready_() );
		if ( uninitialized_() ) init_();
		BOOST_ASSERT( ready_() );
		state_ = st_running;
		run_();
		BOOST_ASSERT( ready_() || exited_() );
	}

	void exit()
	{
		BOOST_ASSERT( running_() ) ;
		state_ = st_exited;
		yield_();
		BOOST_ASSERT(!"should never be reached");
	}
};
}}}

#endif // BOOST_TASK_DETAIL_FIBER_POSIX_H

