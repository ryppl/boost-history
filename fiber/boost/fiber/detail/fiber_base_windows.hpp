//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_DETAIL_FIBER_BASE_WINDOWS_H
#define BOOST_FIBERS_DETAIL_FIBER_BASE_WINDOWS_H

extern "C"
{
#include <windows.h>
#include <winnt.h>
}

#include <cstddef>

#include <boost/assert.hpp>
#include <boost/function.hpp>
#include <boost/system/system_error.hpp>

#include <boost/fibers/detail/context.hpp>

namespace boost { namespace fibers {
namespace detail
{
template< typename Fiber >
VOID CALLBACK trampoline( LPVOID vp)
{
	Fiber * fib( static_cast< Fiber * >( vp) );
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
	VOID CALLBACK trampoline( LPVOID);

    function< void( context< fiber_base > &) >	fn_;
	std::size_t									stack_size_;
	LPVOID										caller_;
	LPVOID										callee_;
	st_state									state_;

	bool uninitialized_() const
	{ return state_ == st_uninitialized; }

	bool ready_() const
	{ return state_ == st_ready; }

	bool running_() const
	{ return state_ == st_running; }

	bool exited_() const
	{ return state_ == st_exited; }

	bool is_fiber_()
	{
#if (_WIN32_WINNT >= 0x0600)
		return ::IsThreadAFiber() == TRUE;
#else
		LPVOID current( ::GetCurrentFiber() );
		return current != 0 && current != reinterpret_cast< LPVOID >( 0x1E00);
#endif
	}

	void yield_()
	{
		if( ! is_fiber_() )
		{
			BOOST_ASSERT( ! callee_);
			callee_ = ::ConvertThreadToFiber( 0);
			if ( ! callee_)
				throw system::system_error(
					system::error_code(
						::GetLastError(),
						system::system_category) );
			::SwitchToFiber( caller_);
			BOOL result = ::ConvertFiberToThread();
			if ( ! result)
				throw system::system_error(
					system::error_code(
						::GetLastError(),
						system::system_category) );
			caller_ = 0;
		}
		else
		{
			if ( ! callee_)
				callee_ = ::GetCurrentFiber();
			::SwitchToFiber( caller_);
			if ( ! callee_)
				callee_ = 0;
		}
	}

	void yield_to_( fiber_base & to)
	{
		std::swap( caller_, to.caller_);
		std::swap( state_, to.state_);
		if( ! is_fiber_() )
		{
			BOOST_ASSERT( ! callee_);
			callee_ = ::ConvertThreadToFiber( 0);
			if ( ! callee_)
				throw system::system_error(
					system::error_code(
						::GetLastError(),
						system::system_category) );
			::SwitchToFiber( to.callee_);
			BOOL result = ::ConvertFiberToThread();
			if ( ! result)
				throw system::system_error(
					system::error_code(
						::GetLastError(),
						system::system_category) );
			caller_ = 0;
		}
		else
		{
			if ( ! callee_)
				callee_ = ::GetCurrentFiber();
			::SwitchToFiber( to.callee_);
			if ( ! callee_)
				callee_ = 0;
		}
	}

	void run_()
	{
		if( ! is_fiber_() )
		{
			BOOST_ASSERT( ! caller_);
			caller_ = ::ConvertThreadToFiber( 0);
			if ( ! caller_)
				throw system::system_error(
					system::error_code(
						::GetLastError(),
						system::system_category) );
			::SwitchToFiber( callee_);
			BOOL result = ::ConvertFiberToThread();
			if ( ! result)
				throw system::system_error(
					system::error_code(
						::GetLastError(),
						system::system_category) );
			caller_ = 0;
		}
		else
		{
			if ( ! caller_)
				caller_ = ::GetCurrentFiber();
			::SwitchToFiber( callee_);
			if ( ! caller_)
				caller_ = 0;
		}
	}

	void init_()
	{
		BOOST_ASSERT( state_ == st_uninitialized);

		callee_ = ::CreateFiber(
			stack_size_,
			static_cast< LPFIBER_START_ROUTINE >( & trampoline< fiber_base >),
			static_cast< LPVOID >( this) );
		if ( ! callee_)
			throw system::system_error(
				system::error_code(
					::GetLastError(),
					system::system_category) );

		state_ = st_ready;
	}

public:
	fiber_base(
		function< void( context< fiber_base > &) > fn,
		std::size_t stack_size)
	:
	fn_( fn),
	stack_size_( stack_size),
	caller_( 0),
	callee_( 0),
	state_( st_uninitialized)
	{
		BOOST_ASSERT( ! fn_.empty() );
		BOOST_ASSERT( stack_size_ > 0);
	}

	~fiber_base()
	{
		BOOST_ASSERT( ! running_() );
		::DeleteFiber( callee_);
	}

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
		BOOST_ASSERT( running_() || ready_() ) ;
		state_ = st_exited;
		yield_();
		BOOST_ASSERT(!"should never be reached");
	}
};
} } }

#endif // BOOST_FIBERS_DETAIL_FIBER_BASE_WINDOWS_H

