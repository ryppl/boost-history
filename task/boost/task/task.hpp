
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASKS_TASK_H
#define BOOST_TASKS_TASK_H

#include <boost/bind.hpp>
#include <boost/config.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/move/move.hpp>
#include <boost/thread/future.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/utility/result_of.hpp>

#include <boost/task/exceptions.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace tasks {
namespace detail {

template< typename R >
class task_base
{
protected:
	bool	done_;

protected:
	promise< R >	prom_;

	virtual void do_run() = 0;

public:
	task_base() :
		done_( false), prom_()
	{}

	virtual ~task_base() {}

	void run()
	{
		if ( this->done_) throw task_already_executed();
		do_run();
		done_ = true;
	}

	unique_future< R > get_future()
	{ return prom_.get_future(); }

	template< typename Cb >
	void set_wait_callback( Cb const& cb)
	{ prom_.set_wait_callback( cb); }
};

template< typename R, typename Fn >
class task_wrapper : public task_base< R >
{
private:
	Fn		fn_;

	void do_run()
	{
		try
		{ this->prom_.set_value( fn_() ); }
		catch ( promise_already_satisfied const&)
		{ throw task_already_executed(); }
		catch ( thread_interrupted const&)
		{ this->prom_.set_exception( copy_exception( task_interrupted() ) ); }
		catch ( task_interrupted const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( boost::exception const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::ios_base::failure const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::domain_error const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::invalid_argument const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::length_error const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::out_of_range const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::logic_error const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::overflow_error const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::range_error const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::underflow_error const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::runtime_error const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::bad_alloc const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::bad_cast const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::bad_typeid const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::bad_exception const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch(...)
		{ this->prom_.set_exception( current_exception() ); }
	}

public:
	task_wrapper( Fn const& fn) :
		task_base< R >(), fn_( fn)
	{}
};

template< typename Fn >
class task_wrapper< void, Fn > : public task_base< void >
{
private:
	Fn		fn_;

	void do_run()
	{
		try
		{
			fn_();
			this->prom_.set_value();
		}
		catch ( promise_already_satisfied const&)
		{ throw task_already_executed(); }
		catch ( thread_interrupted const&)
		{ this->prom_.set_exception( copy_exception( task_interrupted() ) ); }
		catch ( task_interrupted const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( boost::exception const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::ios_base::failure const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::domain_error const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::invalid_argument const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::length_error const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::out_of_range const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::logic_error const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::overflow_error const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::range_error const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::underflow_error const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::runtime_error const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::bad_alloc const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::bad_cast const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::bad_typeid const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch ( std::bad_exception const& e)
		{ this->prom_.set_exception( copy_exception( e) ); }
		catch(...)
		{ this->prom_.set_exception( current_exception() ); }
	}

public:
	task_wrapper( Fn const& fn) :
		task_base< void >(), fn_( fn)
	{}
};

}

template< typename R >
class task
{
private:
	BOOST_MOVABLE_BUT_NOT_COPYABLE( task);	

	shared_ptr< detail::task_base< R > >	task_;

public:
	task() :
		task_()
	{}

	explicit task( R( * fn)()) :
		task_( new detail::task_wrapper< R, R( *)() >( fn) )
	{}

	template< typename Fn >
	explicit task( BOOST_RV_REF( Fn) fn) :
		task_( new detail::task_wrapper< R, Fn >( fn) )
	{}

	task( BOOST_RV_REF( task) other) :
		task_()
	{ task_.swap( other.task_); }

	task & operator=( BOOST_RV_REF( task) other)
	{
		task tmp( other);
		swap( tmp);
		return * this;
	}

# ifndef BOOST_TASKS_MAX_ARITY
#   define BOOST_TASKS_MAX_ARITY 10
# endif

# define BOOST_TASKS_ARG(z, n, unused) \
   BOOST_PP_CAT(A, n) BOOST_PP_CAT(a, n)
# define BOOST_ENUM_TASK_ARGS(n) BOOST_PP_ENUM(n, BOOST_TASKS_ARG, ~)

# define BOOST_TASKS_CTOR(z, n, unused)	\
template<								\
	typename Fn,						\
	BOOST_PP_ENUM_PARAMS(n, typename A)	\
>										\
explicit task( Fn fn, BOOST_ENUM_TASK_ARGS(n))	\
	: task_(									\
			new detail::task_wrapper< R, function< R() > >(	\
				bind( fn, BOOST_PP_ENUM_PARAMS(n, a)) ) )	\
	{}

BOOST_PP_REPEAT_FROM_TO( 1, BOOST_TASKS_MAX_ARITY, BOOST_TASKS_CTOR, ~)

# undef BOOST_TASKS_CTOR

	void operator()()
	{
		if ( ! task_)
			throw task_moved();
		task_->run();
	}

	unique_future< R > get_future()
	{
		if ( ! task_)
			throw task_moved();
		return task_->get_future();	
	}

	template< typename Cb >
	void set_wait_callback( Cb const& cb)
	{
		if ( ! task_)
			throw task_moved();
		task_->set_wait_callback( cb);
	}

	typedef typename shared_ptr< detail::task_base< R > >::unspecified_bool_type	unspecified_bool_type;

	operator unspecified_bool_type() const // throw()
	{ return task_; }

	bool operator!() const // throw()
	{ return ! task_; }

	void swap( task & other) // throw()
	{ task_.swap( other.task_); }
};

template< typename Fn >
task< typename result_of< Fn() >::type > make_task( Fn fn)
{ return task< typename boost::result_of< Fn() >::type >( fn); }

# define BOOST_TASKS_MAKE_TASK_FUNCTION(z, n, unused)	\
template<												\
	typename Fn,										\
	BOOST_PP_ENUM_PARAMS(n, typename A)					\
>														\
task< typename result_of< Fn( BOOST_PP_ENUM_PARAMS(n, A)) >::type >		\
make_task( Fn fn, BOOST_ENUM_TASK_ARGS(n))				\
{ return task< typename result_of< Fn( BOOST_PP_ENUM_PARAMS(n, A)) >::type >( fn, BOOST_PP_ENUM_PARAMS(n, a)); }

BOOST_PP_REPEAT_FROM_TO( 1, BOOST_TASKS_MAX_ARITY, BOOST_TASKS_MAKE_TASK_FUNCTION, ~)

# undef BOOST_TASKS_MAKE_TASK_FUNCTION
# undef BOOST_ENUM_TASK_ARGS
# undef BOOST_TASKS_ARG
# undef BOOST_TASKS_MAX_ARITY

}

using tasks::task;

template< typename R >
void swap( task< R > & l, task< R > & r)
{ return l.swap( r); }

}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASKS_TASK_H
