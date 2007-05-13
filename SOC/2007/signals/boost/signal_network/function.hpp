// function.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_FUNCTION_HPP
#define SIGNAL_NETWORK_FUNCTION_HPP

#include <boost/signal_network/filter.hpp>
#include <boost/signal_network/detail/defines.hpp>
#include <boost/function.hpp>

#define SIGNAL_NETWORK_TEMPLATE_CLASS function
#define SIGNAL_NETWORK_TEMPLATE_BASE filter<void (typename boost::function_traits<Signature>::result_type)>
#define SIGNAL_NETWORK_TEMPLATE_NO_RETURNS
#include <boost/signal_network/detail/loader.hpp>

#endif // SIGNAL_NETWORK_FUNCTION_HPP