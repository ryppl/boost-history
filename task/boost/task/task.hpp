
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_TASK_H
#define BOOST_TASK_TASK_H

#include <boost/bind.hpp>
#include <boost/config.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/thread.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/utility/result_of.hpp>

#include <boost/task/exceptions.hpp>
#include <boost/task/future.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace task {
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
	struct dummy;

	shared_ptr< detail::task_base< R > >	task_;

	task( task &);
	task & operator=( task &);

public:
	task() :
		task_()
	{}

	explicit task( R( * fn)()) :
		task_( new detail::task_wrapper< R, R( *)() >( fn) )
	{}

# if defined(BOOST_HAS_RVALUE_REFS)
	template< typename Fn >
	task( Fn && fn) :
		task_( new detail::task_wrapper< R, Fn >( fn) )
	{}

	task( task && other) :
		task_()
	{ task_.swap( other.task_); }

	task & operator=( task && other)
	{
		task tmp( static_cast< task && >( other) );
		swap( tmp);
		return * this;
	}

	task && move()
	{ return static_cast< task && >( * this); }
# else
#ifdef BOOST_NO_SFINAE
	template< typename Fn >
	explicit task( Fn fn) :
		task_( new detail::task_wrapper< R, Fn >( fn) )
	{}
#else
	template< typename Fn >
	explicit task(
			Fn fn,
			typename disable_if< boost::is_convertible< Fn &, boost::detail::thread_move_t< Fn > >, dummy * >::type = 0) :
		task_( new detail::task_wrapper< R, Fn >( fn) )
	{}
#endif
	template< typename Fn >
	explicit task( boost::detail::thread_move_t< Fn > fn) :
		task_( new detail::task_wrapper< R, Fn >( fn) )
	{}

	task( boost::detail::thread_move_t< task > other) :
		task_()
	{ task_.swap( other->task_); }

	task & operator=( boost::detail::thread_move_t< task > other)
	{
		task tmp( other);
		swap( tmp);
		return * this;
	}

	operator boost::detail::thread_move_t< task >()
	{ return move(); }

	boost::detail::thread_move_t< task > move()
	{
		boost::detail::thread_move_t< task > t( * this);
		return t;
	}
# endif

# ifndef BOOST_TASK_MAX_ARITY
#   define BOOST_TASK_MAX_ARITY 10
# endif

# define BOOST_TASK_ARG(z, n, unused) \
   BOOST_PP_CAT(A, n) BOOST_PP_CAT(a, n)
# define BOOST_ENUM_TASK_ARGS(n) BOOST_PP_ENUM(n, BOOST_TASK_ARG, ~)

# define BOOST_TASK_CTOR(z, n, unused)	\
template<								\
	typename Fn,						\
	BOOST_PP_ENUM_PARAMS(n, typename A)	\
>										\
explicit task( Fn fn, BOOST_ENUM_TASK_ARGS(n))	\
	: task_(									\
			new detail::task_wrapper< R, function< R() > >(	\
				bind( fn, BOOST_PP_ENUM_PARAMS(n, a)) ) )	\
	{}

BOOST_PP_REPEAT_FROM_TO( 1, BOOST_TASK_MAX_ARITY, BOOST_TASK_CTOR, ~)

# undef BOOST_TASK_CTOR

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

# define BOOST_TASK_MAKE_TASK_FUNCTION(z, n, unused)	\
template<												\
	typename Fn,										\
	BOOST_PP_ENUM_PARAMS(n, typename A)					\
>														\
task< typename result_of< Fn( BOOST_PP_ENUM_PARAMS(n, A)) >::type >		\
make_task( Fn fn, BOOST_ENUM_TASK_ARGS(n))				\
{ return task< typename result_of< Fn( BOOST_PP_ENUM_PARAMS(n, A)) >::type >( fn, BOOST_PP_ENUM_PARAMS(n, a)); }

BOOST_PP_REPEAT_FROM_TO( 1, BOOST_TASK_MAX_ARITY, BOOST_TASK_MAKE_TASK_FUNCTION, ~)

# undef BOOST_TASK_MAKE_TASK_FUNCTION
# undef BOOST_ENUM_TASK_ARGS
# undef BOOST_TASK_ARG
# undef BOOST_TASK_MAX_ARITY

}

template< typename R >
void swap( task::task< R > & l, task::task< R > & r)
{ return l.swap( r); }

# if defined(BOOST_HAS_RVALUE_REFS)
template< typename R >
task::task< R > && move( task::task< R > && t)
{ return t; }
# else
template< typename R >
task::task< R > move( boost::detail::thread_move_t< task::task< R > > t)
{ return task::task< R >( t); }
# endif
}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_TASK_H
