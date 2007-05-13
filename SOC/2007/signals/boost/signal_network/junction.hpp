// junction.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_JUNCTION_HPP
#define SIGNAL_NETWORK_JUNCTION_HPP

#include <boost/signal_network/filter.hpp>
#include <boost/signal_network/detail/defines.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>

#define SIGNAL_NETWORK_TEMPLATE_CLASS junction
#define SIGNAL_NETWORK_TEMPLATE_BASE filter<Signature>
#include <boost/signal_network/detail/loader.hpp>

#endif
