//  Copright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_TASK_H
#define BOOST_TASK_TASK_H

#include <exception>
#include <ios>
#include <new>
#include <stdexcept>
#include <typeinfo>

#include <boost/exception_ptr.hpp>
#include <boost/exception.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/thread.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/utility/result_of.hpp>

#include <boost/task/detail/interrupter.hpp>
#include <boost/task/exceptions.hpp>
#include <boost/task/future.hpp>

namespace boost { namespace task
{
namespace detail
{
class callable;
}
template< typename Channel >
class pool;

template< typename R >
class task
{
private:
	template< typename Channel >
	friend class pool;
	friend class detail::callable;
	template< typename Iterator >
	friend void waitfor_all( Iterator begin, Iterator end);
	template< typename T1, typename T2 >
	friend void waitfor_all( T1 & t1, T2 & t2);
	template< typename T1, typename T2, typename T3 >
	friend void waitfor_all( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3);
	template< typename T1, typename T2, typename T3, typename T4 >
	friend void waitfor_all( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4);
	template< typename T1, typename T2, typename T3, typename T4, typename T5 >
	friend void waitfor_all( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4, task< T5 > & t5);
	template< typename Iterator >
	friend Iterator waitfor_any( Iterator begin, Iterator end);
	template< typename T1, typename T2 >
	friend unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2);
	template< typename T1, typename T2, typename T3 >
	friend unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3);
	template< typename T1, typename T2, typename T3, typename T4 >
	friend unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4);
	template< typename T1, typename T2, typename T3, typename T4, typename T5 >
	friend unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4, task< T5 > & t5);

	struct impl
	{
		promise< R >			prom;
		shared_future< R >		fut;
		detail::interrupter		intr;
		exception_ptr			excep;

		impl()
		:
		prom(),
		fut( prom.get_future() ),
		intr(),
		excep()
		{}

		virtual ~impl() {}

		virtual void operator()() = 0;
	};

	template< typename Fn >
	class impl_wrapper : public impl
	{
	private:
		Fn		fn_;

	public:
		impl_wrapper( Fn const& fn)
		: fn_( fn)
		{}

		void operator()() // throw()
		{
			try
			{ impl::prom.set_value( fn_() ); }
			catch ( promise_already_satisfied const&)
			{ impl::excep = copy_exception( task_already_executed() ); }
			catch ( thread_interrupted const&)
			{ impl::prom.set_exception( copy_exception( task_interrupted() ) ); }
			catch ( boost::exception const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::ios_base::failure const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::domain_error const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::invalid_argument const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::length_error const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::out_of_range const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::logic_error const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::overflow_error const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::range_error const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::underflow_error const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::runtime_error const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::bad_alloc const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::bad_cast const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::bad_typeid const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::bad_exception const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch(...)
			{ impl::prom.set_exception( current_exception() ); }
		}
	};

	shared_ptr< impl >	impl_;

public:
	task()
	: impl_()
	{}

	template< typename Fn >
	task( Fn const& fn)
	: impl_( new impl_wrapper< Fn >( fn) )
	{}

	void interrupt()
	{ impl_->intr.interrupt(); }

	void interrupt_and_wait()
	{ impl_->intr.interrupt_and_wait(); }

	void interrupt_and_wait( system_time const& abs_time)
	{ impl_->intr.interrupt_and_wait( abs_time); }

	template< typename Duration >
	void interrupt_and_wait( Duration const& rel_time)
	{ impl_->intr.interrupt_and_wait( rel_time); }

	bool interruption_requested()
	{ return impl_->intr.interruption_requested(); }

	R get()
	{
		if ( impl_->excep)
			rethrow_exception( impl_->excep);
		return impl_->fut.get();
	}

	bool is_ready() const
	{ return impl_->fut.is_ready(); }

	bool has_value() const
	{ return impl_->fut.has_value(); }

	bool has_exception() const
	{ return impl_->excep ? true : impl_->fut.has_exception(); }

	void wait() const
	{ impl_->fut.wait(); }

    template< typename Duration >
    bool timed_wait( Duration const& rel_time) const
	{ return impl_->fut.timed_wait( rel_time); }

    bool timed_wait_until( system_time const& abs_time) const
	{ return impl_->fut.timed_wait_until( abs_time); }

	void swap( task< R > & other) // throw()
	{ impl_.swap( other.impl_); }

	void operator()() // throw()
	{ ( * impl_)(); }

	template< typename F >
	void set_wait_callback( F const& f)
	{ impl_->prom.set_wait_callback( f); }
};

template<>
class task< void >
{
private:
	template< typename Channel >
	friend class pool;
	friend class detail::callable;
	template< typename Iterator >
	friend void waitfor_all( Iterator begin, Iterator end);
	template< typename T1, typename T2 >
	friend void waitfor_all( T1 & t1, T2 & t2);
	template< typename T1, typename T2, typename T3 >
	friend void waitfor_all( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3);
	template< typename T1, typename T2, typename T3, typename T4 >
	friend void waitfor_all( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4);
	template< typename T1, typename T2, typename T3, typename T4, typename T5 >
	friend void waitfor_all( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4, task< T5 > & t5);
	template< typename Iterator >
	friend Iterator waitfor_any( Iterator begin, Iterator end);
	template< typename T1, typename T2 >
	friend unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2);
	template< typename T1, typename T2, typename T3 >
	friend unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3);
	template< typename T1, typename T2, typename T3, typename T4 >
	friend unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4);
	template< typename T1, typename T2, typename T3, typename T4, typename T5 >
	friend unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4, task< T5 > & t5);

	struct impl
	{
		promise< void >			prom;
		shared_future< void >	fut;
		detail::interrupter		intr;
		exception_ptr			excep;

		impl()
		:
		prom(),
		fut( prom.get_future() ),
		intr(),
		excep()
		{}

		virtual ~impl() {}

		virtual void operator()() = 0;
	};

	template< typename Fn >
	class impl_wrapper : public impl
	{
	private:
		Fn		fn_;

	public:
		impl_wrapper( Fn const& fn)
		: fn_( fn)
		{}

		void operator()() // throw()
		{
			try
			{
				fn_();
				impl::prom.set_value();
			}
			catch ( promise_already_satisfied const&)
			{ impl::excep = copy_exception( task_already_executed() ); }
			catch ( thread_interrupted const&)
			{ impl::prom.set_exception( copy_exception( task_interrupted() ) ); }
			catch ( boost::exception const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::ios_base::failure const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::domain_error const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::invalid_argument const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::length_error const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::out_of_range const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::logic_error const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::overflow_error const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::range_error const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::underflow_error const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::runtime_error const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::bad_alloc const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::bad_cast const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::bad_typeid const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch ( std::bad_exception const& e)
			{ impl::prom.set_exception( copy_exception( e) ); }
			catch(...)
			{ impl::prom.set_exception( current_exception() ); }
		}
	};

	shared_ptr< impl >	impl_;

public:
	task()
	: impl_()
	{}

	template< typename Fn >
	task( Fn const& fn)
	: impl_( new impl_wrapper< Fn >( fn) )
	{}

	void interrupt()
	{ impl_->intr.interrupt(); }

	void interrupt_and_wait()
	{ impl_->intr.interrupt_and_wait(); }

	void interrupt_and_wait( system_time const& abs_time)
	{ impl_->intr.interrupt_and_wait( abs_time); }

	template< typename Duration >
	void interrupt_and_wait( Duration const& rel_time)
	{ impl_->intr.interrupt_and_wait( rel_time); }

	bool interruption_requested()
	{ return impl_->intr.interruption_requested(); }

	void get()
	{
		if ( impl_->excep)
			rethrow_exception( impl_->excep);
		impl_->fut.get();
	}

	bool is_ready() const
	{ return impl_->fut.is_ready(); }

	bool has_value() const
	{ return impl_->fut.has_value(); }

	bool has_exception() const
	{ return impl_->excep ? true : impl_->fut.has_exception(); }

	void wait() const
	{ impl_->fut.wait(); }

    template< typename Duration >
    bool timed_wait( Duration const& rel_time) const
	{ return impl_->fut.timed_wait( rel_time); }

    bool timed_wait_until( system_time const& abs_time) const
	{ return impl_->fut.timed_wait_until( abs_time); }

	void swap( task< void > & other) // throw()
	{ impl_.swap( other.impl_); }

	void operator()() // throw()
	{ ( * impl_)(); }

	template< typename F >
	void set_wait_callback( F const& f)
	{ impl_->prom.set_wait_callback( f); }
};

template< typename Iterator >
void waitfor_all( Iterator begin, Iterator end)
{
	for ( Iterator i = begin; i != end; ++i)
		i->wait();
}

template< typename T1, typename T2 >
void waitfor_all( T1 & t1, T2 & t2)
{ wait_for_all( t1.fut_, t2.fut_); }

template< typename T1, typename T2, typename T3 >
void waitfor_all( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3)
{ wait_for_all( t1.fut_, t2.fut_, t3.fut_); }

template< typename T1, typename T2, typename T3, typename T4 >
void waitfor_all( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4)
{ wait_for_all( t1.fut_, t2.fut_, t3.fut_, t4.fut_); }

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
void waitfor_all( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4, task< T5 > & t5)
{ wait_for_all( t1.fut_, t2.fut_, t3.fut_, t4.fut_, t5.fut_); }

template< typename Iterator >
Iterator waitfor_any( Iterator begin, Iterator end)
{
	boost::detail::future_waiter waiter;
	for ( Iterator i = begin; i != end; ++i)
		waiter.add( i->fut_);
	return next( begin, waiter.wait() );
}

template< typename T1, typename T2 >
unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2)
{ return wait_for_any( t1.fut_, t2.fut_); }

template< typename T1, typename T2, typename T3 >
unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3)
{ return wait_for_any( t1.fut_, t2.fut_, t3.fut_); }

template< typename T1, typename T2, typename T3, typename T4 >
unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4)
{ return wait_for_any( t1.fut_, t2.fut_, t3.fut_, t4.fut_); }

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4, task< T5 > & t5)
{ return wait_for_any( t1.fut_, t2.fut_, t3.fut_, t4.fut_, t5.fut_); }

template< typename Fn >
task< typename result_of< Fn() >::type > make_task( Fn fn)
{ return task< typename boost::result_of< Fn() >::type >( fn); }

# ifndef BOOST_TASK_MAKE_TASK_MAX_ARITY
#   define BOOST_TASK_MAKE_TASK_MAX_ARITY 10
# endif

# define BOOST_TASK_MAKE_TASK_FUNC_ARG(z, n, unused) \
   BOOST_PP_CAT(A, n) BOOST_PP_CAT(a, n)
# define BOOST_ENUM_TASK_MAKE_TASK_FUNC_ARGS(n) BOOST_PP_ENUM(n, BOOST_TASK_MAKE_TASK_FUNC_ARG, ~)

# define BOOST_TASK_MAKE_TASK_FUNCTION(z, n, unused)	\
template<												\
	typename Fn,										\
	BOOST_PP_ENUM_PARAMS(n, typename A)					\
>														\
task< typename result_of< Fn( BOOST_PP_ENUM_PARAMS(n, A)) >::type >		\
make_task( Fn fn, BOOST_ENUM_TASK_MAKE_TASK_FUNC_ARGS(n))				\
{ return make_task( boost::bind( fn, BOOST_PP_ENUM_PARAMS(n, a))); }

BOOST_PP_REPEAT_FROM_TO( 1, BOOST_TASK_MAKE_TASK_MAX_ARITY, BOOST_TASK_MAKE_TASK_FUNCTION, ~)

# undef BOOST_TASK_MAKE_TASK_FUNCTION
# undef BOOST_TASK_MAKE_TASK_FUNC_ARG
# undef BOOST_ENUM_TASK_MAKE_TASK_FUNC_ARGS

}}

#endif // BOOST_TASK_TASK_H
