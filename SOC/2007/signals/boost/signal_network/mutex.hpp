// signal_mutex.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_MUTEX_HPP
#define SIGNAL_NETWORK_MUTEX_HPP

#include <boost/signal_network/filter.hpp>
#include <boost/signal_network/detail/defines.hpp>
#include <boost/thread/mutex.hpp>

#define SIGNAL_NETWORK_TEMPLATE_CLASS mutex
#define SIGNAL_NETWORK_TEMPLATE_BASE filter<Signature>
#include <boost/signal_network/detail/loader.hpp>

#endif // SIGNAL_NETWORK_MUTEX_HPP
