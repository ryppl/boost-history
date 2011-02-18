//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Based on the unique_threader/unique_joiner design from of Kevlin Henney (n1883)
//
// See http://www.boost.org/libs/async for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_ASYNC_TYPEOF_FUTURE__HPP
#define BOOST_ASYNC_TYPEOF_FUTURE__HPP

#include <boost/thread/future.hpp>
#include <boost/typeof/typeof.hpp>

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

BOOST_TYPEOF_REGISTER_TEMPLATE(boost::futures::unique_future, 1)
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::futures::shared_future, 1)
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::futures::promise, 1)
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::futures::packaged_task, 1)

#endif
