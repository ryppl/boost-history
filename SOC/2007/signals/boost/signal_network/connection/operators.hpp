// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_CONNECTION_OPERATORS_HPP
#define SIGNAL_NETWORK_CONNECTION_OPERATORS_HPP

#include <boost/signal_network/connection/connect.hpp>

namespace boost { namespace signals {

///	Connects two components (typically as a part of a chain of components).
/** This operator is identical to signals::operator| (it connects the
left component to the right component, and returns a reference to the left component),
except it is evaluated right to left.  This makes it semantics more suitable for
connecting a chain of components.
*/
template<typename Input, typename Output>
typename boost::enable_if<is_component<Input>, Input & >::type
operator >>= (Input &input, Output &output) { connect(input, output); return input;}

template<typename Input, typename Output>
typename boost::enable_if<is_component<Input>, Input & >::type
operator >>= (Input &input, const Output &output) { connect(input, output); return input;}

template<typename Input, typename Output>
typename boost::enable_if<is_component<Input>, const Input & >::type
operator >>= (const Input &input, Output &output) { connect(input, output); return input;}

template<typename Input, typename Output>
typename boost::enable_if<is_component<Input>, const Input & >::type
operator >>= (const Input &input, const Output &output) { connect(input, output); return input;}
    
/// Connects two components (typically as a part of branching from a single component).
/** This operator is identical to signals::operator>>=, (it connects the
left component to the right component, and returns a reference to the left component)
except it is evaluated left to right.  This makes its semantics more suitable for
branching connections.
*/
template<typename Input, typename Output>
typename boost::enable_if<is_component<Input>, Input & >::type
operator | (Input &input, Output &output) { connect(input, output); return input;}

template<typename Input, typename Output>
typename boost::enable_if<is_component<Input>, Input & >::type
operator | (Input &input, const Output &output) { connect(input, output); return input;}

template<typename Input, typename Output>
typename boost::enable_if<is_component<Input>, const Input & >::type
operator | (const Input &input, Output &output) { connect(input, output); return input;}

template<typename Input, typename Output>
typename boost::enable_if<is_component<Input>, const Input & >::type
operator | (const Input &input, const Output &output) { connect(input, output); return input;}
    
} } // namespace boost::signals

#endif SIGNAL_NETWORK_CONNECTION_OPERATORS_HPP

