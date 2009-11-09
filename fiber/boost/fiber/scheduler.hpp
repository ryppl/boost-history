
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBER_SCHEDULER_H
#define BOOST_FIBER_SCHEDULER_H

#include <cstddef>
#include <memory>

#include <boost/preprocessor/repetition.hpp>
#include <boost/thread/tss.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/attributes.hpp>
#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/fiber.hpp>
#include <boost/fiber/policy.hpp>
#include <boost/fiber/rrp.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {

namespace this_fiber {

bool runs_as_fiber();
fiber::fiber::id get_id();
void yield();
void cancel();

}

namespace fiber {

class BOOST_FIBER_DECL scheduler : private noncopyable
{
private:
	friend void trampoline( fiber *);
	friend bool this_fiber::runs_as_fiber();
	friend fiber::id this_fiber::get_id();
	friend void this_fiber::yield();
	friend void this_fiber::cancel();

	typedef thread_specific_ptr< policy >	tss_policy_t;

	static tss_policy_t	impl_;

	static bool runs_as_fiber();

	static fiber::id get_id();

	static void yield();

	static void exit();

	static void failed();

	policy * access_();

public:
	~scheduler();

	template< typename Fn >
	void make_fiber( Fn fn)
	{
		attributes attrs;
		access_()->add_fiber(
			std::auto_ptr< fiber >(
				new fiber( fn, attrs) ) );
	}

	template< typename Fn >
	void make_fiber( attributes attrs, Fn fn)
	{
		access_()->add_fiber(
			std::auto_ptr< fiber >(
				new fiber( fn, attrs) ) );
	}

#ifndef BOOST_FIBER_MAX_ARITY
#define BOOST_FIBER_MAX_ARITY 10
#endif

#define BOOST_FIBER_ARG(z, n, unused) \
   BOOST_PP_CAT(A, n) BOOST_PP_CAT(a, n)
#define BOOST_ENUM_FIBER_ARGS(n) BOOST_PP_ENUM(n, BOOST_FIBER_ARG, ~)

#define BOOST_FIBER_MAKE_FIBER_FUNCTION(z, n, unused)	\
	template<											\
		typename Fn,									\
		BOOST_PP_ENUM_PARAMS(n, typename A)				\
	>													\
	void make_fiber( Fn fn, BOOST_ENUM_FIBER_ARGS(n))	\
	{													\
		attributes attrs;								\
		access_()->add_fiber(							\
			std::auto_ptr< fiber >(						\
				new fiber(								\
					boost::bind(						\
						boost::type< void >(), fn, BOOST_PP_ENUM_PARAMS(n, a) ),\
					attrs) ) );													\
	}

BOOST_PP_REPEAT_FROM_TO( 1, BOOST_FIBER_MAX_ARITY, BOOST_FIBER_MAKE_FIBER_FUNCTION, ~)

#undef BOOST_FIBER_MAKE_FIBER_FUNCTION

#define BOOST_FIBER_MAKE_FIBER_FUNCTION(z, n, unused)	\
	template<											\
		typename Fn,									\
		BOOST_PP_ENUM_PARAMS(n, typename A)				\
	>													\
	void make_fiber( attributes const& attrs, Fn fn, BOOST_ENUM_FIBER_ARGS(n))	\
	{																			\
		access_()->add_fiber(													\
			std::auto_ptr< fiber >(												\
				new fiber(														\
					boost::bind(												\
						boost::type< void >(), fn, BOOST_PP_ENUM_PARAMS(n, a) ),\
					attrs) ) );													\
	}

BOOST_PP_REPEAT_FROM_TO( 1, BOOST_FIBER_MAX_ARITY, BOOST_FIBER_MAKE_FIBER_FUNCTION, ~)

#undef BOOST_FIBER_MAKE_FIBER_FUNCTION
#undef BOOST_FIBER_MAX_ARITY

	bool run();

	bool empty();

	std::size_t size();
};

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBER_SCHEDULER_H
