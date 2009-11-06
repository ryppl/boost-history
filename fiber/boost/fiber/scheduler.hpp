
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
#include <boost/fiber/detail/fiber.hpp>
#include <boost/fiber/detail/scheduler_data.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {

namespace this_fiber {

fiber::id get_id();
void yield();

}

namespace fiber {

class BOOST_FIBER_DECL scheduler : private noncopyable
{
private:
	friend void detail::trampoline( detail::fiber *);
	friend id this_fiber::get_id();
	friend void this_fiber::yield();

	typedef thread_specific_ptr< detail::scheduler_data >	data_ptr_t;

	static data_ptr_t	data;

	static fiber::id get_id();

	static void yield();

	static void exit();

	detail::scheduler_data * access_data();

	void add( std::auto_ptr< detail::fiber >);

public:
	scheduler();

	template< typename Fn >
	void make_fiber( Fn fn, attributes attrs = attributes() )
	{
		add(
			std::auto_ptr< detail::fiber >(
				new detail::fiber( fn, attrs) ) );
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
	void make_fiber( Fn fn, attributes const& attribs, BOOST_ENUM_FIBER_ARGS(n))	\
	{																				\
		add(																		\
			std::auto_ptr< detail::fiber >(											\
				new detail::fiber(													\
					boost::bind(													\
						boost::type< void >(), fn, BOOST_PP_ENUM_PARAMS(n, a) ),	\
					attribs) ) );													\
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
