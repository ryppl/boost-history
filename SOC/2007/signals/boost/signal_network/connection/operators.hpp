// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_CONNECTION_OPERATORS_HPP
#define SIGNAL_NETWORK_CONNECTION_OPERATORS_HPP

#include <boost/signal_network/connection/connect.hpp>

namespace boost { namespace signals {

///	Connects a sequence of components using signals.
/** This operator is identical to signals::filter::operator| (it connects the
left component to the right component, and returns a reference to the left component),
except it is evaluated right to left.  This makes it semantics more suitable for
connecting a chain of connections.
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
    
/// Allows branching in a component connection sequence.
/** This operator is identical to signals::filter::operator>>=, (it connects the
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

