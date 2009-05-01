
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include <boost/bind.hpp>

#include "boost/task.hpp"

namespace tsk = boost::task;

inline
int fibonacci_fn( int n)
{
	if ( n == 0) return 0;
	if ( n == 1) return 1;
	int k1( 1), k2( 0);
	for ( int i( 2); i <= n; ++i)
	{
		boost::this_thread::interruption_point();
		int tmp( k1);
		k1 = k1 + k2;
		k2 = tmp;
	}
	boost::this_thread::interruption_point();
	return k1;
}

int main( int argc, char *argv[])
{
	try
	{
		tsk::handle< int > h1(
			tsk::async(
				tsk::own_thread(),
				tsk::make_task(
					fibonacci_fn,
					10) ) );
		tsk::handle< int > h2(
			tsk::async(
				tsk::new_thread(),
				tsk::make_task(
					fibonacci_fn,
					10) ) );
		tsk::handle< int > h3(
			tsk::async(
				tsk::default_pool(),
				tsk::make_task(
					fibonacci_fn,
					10) ) );
		tsk::pool<
			tsk::unbounded_channel< tsk::priority< int > >
		> pool( tsk::poolsize( 3) );
		tsk::handle< int > h4(
			tsk::async(
				pool,
				tsk::make_task(
					fibonacci_fn,
					10),
				2) );
		std::cout << h1.get() << std::endl;
		std::cout << h2.get() << std::endl;
		std::cout << h3.get() << std::endl;
		std::cout << h4.get() << std::endl;

		return EXIT_SUCCESS;
	}
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch ( ... )
	{ std::cerr << "unhandled" << std::endl; }

	return EXIT_FAILURE;
}
