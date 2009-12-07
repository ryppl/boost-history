
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_SCHEDULER_H
#define BOOST_FIBERS_SCHEDULER_H

#include <cstddef>
#include <memory>

#include <boost/preprocessor/repetition.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/fiber.hpp>
#include <boost/fiber/round_robin.hpp>
#include <boost/fiber/strategy.hpp>

#include <boost/config/abi_prefix.hpp>

# if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable:4251 4275)
# endif

namespace boost {
namespace fibers {

class auto_reset_event;
class condition;
class count_down_event;
class manual_reset_event;
class mutex;

template< typename Strategy = round_robin >
class BOOST_FIBER_DECL scheduler : private noncopyable
{
private:
	friend class auto_reset_event;
	friend class condition;
	friend class count_down_event;
	friend class manual_reset_event;
	friend class mutex;

	typedef intrusive_ptr< strategy >	strategy_t;

	strategy_t	strategy_;

public:
	scheduler() :
		strategy_( new Strategy() )
	{}

	~scheduler()
	{} 

	bool run()
	{ return strategy_->run(); }

	bool empty()
	{ return strategy_->empty(); }

	std::size_t size()
	{ return strategy_->size(); }

	void submit_fiber( fiber f)
	{ strategy_->add( f); }

	template< typename Fn >
	void make_fiber( Fn fn)
	{ strategy_->add( fiber( fn) ); }

	template< typename Fn >
	void make_fiber( std::size_t stack_size, Fn fn)
	{ strategy_->add( fiber( stack_size, fn) ); }

#ifndef BOOST_FIBER_MAX_ARITY
#define BOOST_FIBER_MAX_ARITY 10
#endif

#define BOOST_FIBER_ARG(z, n, unused) \
   BOOST_PP_CAT(A, n) BOOST_PP_CAT(a, n)
#define BOOST_ENUM_FIBER_ARGS(n) BOOST_PP_ENUM(n, BOOST_FIBER_ARG, ~)

#define BOOST_FIBER_MAKE_FIBER_FUNCTION(z, n, unused) \
	template< typename Fn, BOOST_PP_ENUM_PARAMS(n, typename A) > \
	void make_fiber( Fn fn, BOOST_ENUM_FIBER_ARGS(n)) \
	{ strategy_->add( fiber( fn, BOOST_PP_ENUM_PARAMS(n, a) ) ); } \
	template< typename Fn, BOOST_PP_ENUM_PARAMS(n, typename A) > \
	void make_fiber( std::size_t stack_size, Fn fn, BOOST_ENUM_FIBER_ARGS(n)) \
	{ strategy_->add( fiber( stack_size, fn, BOOST_PP_ENUM_PARAMS(n, a) ) ); }

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
