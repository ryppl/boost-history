// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_CONNECT_HPP
#define SIGNAL_NETWORK_CONNECT_HPP

namespace boost { namespace signals {

/** \brief A functor which connects two components. It must be specialized to support each component to be used with the library.
*/
template<typename Input, typename Output, typename Enable=void>
struct connect_impl
#ifdef DOXYGEN_DOCS_ONLY
{
    /** Connects input to output.
    */
    void operator()(Input &input, Output &output);
    /** Connects input to output.
    */
    void operator()(Input &input, const Output &output);
    /** Connects input to output.
    */
    void operator()(const Input &input, Output &output);
    /** Connects input to output.
    */
    void operator()(const Input &input, const Output &output);
}
#endif
    ;

/** Connects two components using boost::signals::connect_impl.
*/
template<typename Input, typename Output>
void connect(Input &input, Output &output)
{
    connect_impl<Input, Output>()(input, output);
};

/** Connects two components using boost::signals::connect_impl.
*/
template<typename Input, typename Output>
void connect(Input &input, const Output &output)
{
    connect_impl<Input, Output>()(input, output);
};

/** Connects two components using boost::signals::connect_impl.
*/
template<typename Input, typename Output>
void connect(const Input &input, Output &output)
{
    connect_impl<Input, Output>()(input, output);
};

/** Connects two components using boost::signals::connect_impl.
*/
template<typename Input, typename Output>
void connect(const Input &input, const Output &output)
{
    connect_impl<Input, Output>()(input, output);
};

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_CONNECT_HPP