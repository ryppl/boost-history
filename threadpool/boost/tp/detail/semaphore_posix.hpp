//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_DETAIL_SEMAPHORE_POSIX_H
#define BOOST_TP_DETAIL_SEMAPHORE_POSIX_H

extern "C"
{
#include <semaphore.h>
}

#include <boost/utility.hpp>

namespace boost { namespace tp {
namespace detail
{
class semaphore : private boost::noncopyable
{
private:
	sem_t	sem_;

public:
	semaphore( int);

	~semaphore();

	void post();

	bool try_wait();
	void wait();

	int value();
};
}}}

#endif // BOOST_TP_DETAIL_SEMAPHORE_POSIX_H
