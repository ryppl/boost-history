//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_RESULT_H
#define BOOST_FIBERS_RESULT_H

#include <boost/exception.hpp>
#include <boost/exception_ptr.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>

#include <boost/fibers/context.hpp>

namespace boost {
namespace fibers
{
#define CATCH_ENABLE_CURRENT_EXCEPTION( Exception)	\
  catch ( Exception const& e)						\
  { throw boost::enable_current_exception( e); }

template< typename R >
class result
{
private:
	struct impl
	{
		virtual ~impl() {}
		virtual void operator()( context &) = 0;
		virtual bool ready() const = 0;
		virtual bool has_value() const = 0;
		virtual bool has_exception() const = 0;
		virtual R get() const = 0;
	};

	template< typename Act >
	class impl_wrapper
	: public impl
	{
	private:
		Act				act_;
		R				r_;
		bool			ready_;
		exception_ptr	except_ptr_;

	public:
		impl_wrapper( Act const& act)
		:
		act_( act),
		r_(),
		ready_( false),
		except_ptr_()
		{}

		void operator()( context & ctxt)
		{
			try
			{
				try
				{ r_ = act_( ctxt); }
				CATCH_ENABLE_CURRENT_EXCEPTION( thread_interrupted )
				CATCH_ENABLE_CURRENT_EXCEPTION( exception )

				CATCH_ENABLE_CURRENT_EXCEPTION( std::domain_error )
				CATCH_ENABLE_CURRENT_EXCEPTION( std::invalid_argument )
				CATCH_ENABLE_CURRENT_EXCEPTION( std::length_error )
				CATCH_ENABLE_CURRENT_EXCEPTION( std::out_of_range )
				CATCH_ENABLE_CURRENT_EXCEPTION( std::logic_error )

				CATCH_ENABLE_CURRENT_EXCEPTION( std::overflow_error )
				CATCH_ENABLE_CURRENT_EXCEPTION( std::range_error )
				CATCH_ENABLE_CURRENT_EXCEPTION( std::underflow_error)
				CATCH_ENABLE_CURRENT_EXCEPTION( std::runtime_error )

				CATCH_ENABLE_CURRENT_EXCEPTION( std::bad_alloc )
				CATCH_ENABLE_CURRENT_EXCEPTION( std::bad_cast )
				CATCH_ENABLE_CURRENT_EXCEPTION( std::bad_typeid )
				CATCH_ENABLE_CURRENT_EXCEPTION( std::bad_exception )

				// iostreams library
				CATCH_ENABLE_CURRENT_EXCEPTION( std::ios_base::failure )
			}
			catch ( ... )
			{ except_ptr_ = current_exception(); }
			ready_ = true;
		}

		bool ready() const
		{ return ready_; }

		bool has_value() const
		{ return ready() && ! except_ptr_; }

		bool has_exception() const
		{ return ready() && except_ptr_; }

		R get() const
		{
			if ( except_ptr_) rethrow_exception( except_ptr_);
			return r_;
		}
	};

	shared_ptr< impl >	impl_;

public:
	template< typename Act >
	result( Act act)
	: impl_( new impl_wrapper< Act >( act) )
	{}

	void operator()( context & ctxt)
	{ ( * impl_)( ctxt); }

	bool ready() const
	{ return impl_->ready(); }

	bool has_value() const
	{ return impl_->has_value(); }

	bool has_exception() const
	{ return impl_->has_exception(); }

	R get() const
	{ return impl_->get(); }
};


template<>
class result< void >
{
private:
	struct impl
	{
		virtual ~impl() {}
		virtual void operator()( context &) = 0;
		virtual bool ready() const = 0;
		virtual bool has_value() const = 0;
		virtual bool has_exception() const = 0;
		virtual void get() const = 0;
	};

	template< typename Act >
	class impl_wrapper
	: public impl
	{
	private:
		Act				act_;
		bool			ready_;
		exception_ptr	except_ptr_;

	public:
		impl_wrapper( Act const& act)
		:
		act_( act),
		ready_( false),
		except_ptr_()
		{}

		void operator()( context & ctxt)
		{
			try
			{
				try
				{ act_( ctxt); }
				CATCH_ENABLE_CURRENT_EXCEPTION( thread_interrupted )
				CATCH_ENABLE_CURRENT_EXCEPTION( exception )

				CATCH_ENABLE_CURRENT_EXCEPTION( std::overflow_error )
				CATCH_ENABLE_CURRENT_EXCEPTION( std::range_error )
				CATCH_ENABLE_CURRENT_EXCEPTION( std::underflow_error)
				CATCH_ENABLE_CURRENT_EXCEPTION( std::runtime_error )

				// logic_error standard subclasses
				CATCH_ENABLE_CURRENT_EXCEPTION( std::domain_error )
				CATCH_ENABLE_CURRENT_EXCEPTION( std::invalid_argument )
				CATCH_ENABLE_CURRENT_EXCEPTION( std::length_error )
				CATCH_ENABLE_CURRENT_EXCEPTION( std::out_of_range )
				CATCH_ENABLE_CURRENT_EXCEPTION( std::logic_error )

				CATCH_ENABLE_CURRENT_EXCEPTION( std::bad_alloc )
				CATCH_ENABLE_CURRENT_EXCEPTION( std::bad_cast )
				CATCH_ENABLE_CURRENT_EXCEPTION( std::bad_typeid )
				CATCH_ENABLE_CURRENT_EXCEPTION( std::bad_exception )

				// iostreams library
				CATCH_ENABLE_CURRENT_EXCEPTION( std::ios_base::failure )
			}
			catch ( ... )
			{ except_ptr_ = current_exception(); }
			ready_ = true;
		}

		bool ready() const
		{ return ready_; }

		bool has_value() const
		{ return ready() && ! except_ptr_; }

		bool has_exception() const
		{ return ready() && except_ptr_; }

		void get() const
		{ if ( except_ptr_) rethrow_exception( except_ptr_); }
	};

	shared_ptr< impl >	impl_;

public:
	template< typename Act >
	result( Act act)
	: impl_( new impl_wrapper< Act >( act) )
	{}

	void operator()( context & ctxt)
	{ ( * impl_)( ctxt); }

	bool ready() const
	{ return impl_->ready(); }

	bool has_value() const
	{ return impl_->has_value(); }

	bool has_exception() const
	{ return impl_->has_exception(); }

	void get() const
	{ impl_->get(); }
};

#undef CATCH_ENABLE_CURRENT_EXCEPTION
} }

#endif // BOOST_FIBERS_RESULT_H

