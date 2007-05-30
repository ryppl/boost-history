namespace boost {
namespace signal_network {

/** \mainpage

\section sec_tutorial Tutorial
	This tutorial explains the use of the Signal Network library, and is based on elements of the library
	that have working prototypes.

\section signal_network_namespace Namespace use
	The connection operators (and a few other things) are located in the boost::signal_network namespace.
	All of the components are in the boost::signal_network::signet namespace.
	
	It is recommended that you do
\code
using namespace boost::signal_network
\endcode
	so that your code can use the operators >>= and |, and you can access the components via signet::\em component.
	
	Otherwise, the following might be helpful:
\code
namespace signet = boost::signal_network::signet;
\endcode
	
\section signal_network_examples Examples

\par Creating connections and components

	\li \ref signal_network_simple_example
	\li \ref signal_network_receiving_example
	\li \ref signal_network_branching_example
	\li \ref signal_network_pull_example
	\li \ref signal_network_disconnect_example
	\li \ref signal_network_multi_in_simple_example
	\li \ref signal_network_multi_in_example
	\li \ref signal_network_multi_out_example

\par Flow control and signal storage components

The following components have examples in their documentation:

	\li signet::storage for storing and forwarding values.
	\li signet::junction for signal branching and flow control.
	\li signet::selector for selection between multiple signals.

\par Classes for creating new components

The following classes have examples in their documentation:

	\li signet::filter is a base class for filters (components that receive and send a signal).
	\li signet::function can be used to convert functions into filters.

\par Threading components

The following components have examples in their documentation:

	\li signet::timed_generator for periodic sending of signals in a separate thread.
	\li signet::mutex for thread control when using signals from separate threads.

\par Network topologies

The following components have examples in their documentation:

	\li signet::chain implements the chain topology.

\par Signal networks accross multiple computers using Boost.Serialization and the Boost asio library.

The following components have examples in their documentation:

	\li signet::socket_sender sends serializable signals accross the network.
	\li signet::socket_receiver receives serializable signals from the network.

\subsection signal_network_simple_example Simple connections using Signal Network components

	The Sigal Network library provides many common building block components for a
	signal network.  The most basic is \link signet::storage storage\endlink, which
	can be used to store a value received through a signal, send it on, and/or retreive it.
	
	By default, components in the library use operator() to receive a signal.
	For example, \link boost::signal_network::signet::storage storage\endlink objects can receive
	signals through \link boost::signal_network::signet::storage::operator()()
	operator()()\endlink.  Upon receiving this signal,
	they will output their stored value through another signal.
	
	The value stored inside a \link boost::signal_network::signet::storage storage\endlink object
	can be retrieved via the \link boost::signal_network::signet::storage::valueN() valueN()\endlink
	function.
	
	Using a few \link boost::signal_network::signet::storage storage\endlink objects, it is easy to
	create a network using \link boost::signal_network::operator>>=() operator>>=\endlink:
	
	\dontinclude example.cpp
	\skip simple_test
	\until end void simple_test
	
\subsection signal_network_receiving_example Creating your own signal receiver (slot)

	The easiest way to create your own signal receiver which can be used with the Signal Network
	library is to create a class with operator() of the signal signature you wish to receive.
	
	For example, consider the signature "void ()".  This is the signature
	of a function that returns void and takes no arguments.
	A class that can receive signals of such a signature would be
	defined as follows:

	\dontinclude example.cpp
	\skip class SignalVoidCounter
	\until end class SignalVoidCounter
	
	

\subsection signal_network_branching_example Branching connections
	More complex connections can also be made relatively easily using
	both \link boost::signal_network::operator>>=() operator >>= \endlink
	and \link boost::signal_network::operator|() operator | \endlink:

	\dontinclude example.cpp
	\skip branching_test
	\until end void branching_test
	
\subsection signal_network_pull_example Building a pull-based network
	The Signal Network library was built with push-based networks in
	mind (the data producer sends data through a signal), but it can also
	be used for pull-based networks (the data consumer requests data through
	a signal).

	The following example illustrates this:
	
	\dontinclude example.cpp
	\skip PullDoubler
	\until end void pull_test

\subsection	signal_network_disconnect_example Disconnecting connections
	Connections can be terminated in two ways.  One is through the "trackable"
	mechanism of Boost.Signals, which will automatically destroy connections
	to a trackable object when the object is destroyed.  The other
	way is through the disconnect_all_slots method of the signal sender.

	\dontinclude example.cpp
	\skip void disconnect_test
	\until end void disconnect_test

\subsection signal_network_multi_in_simple_example Multiple inputs of different signatures

	It is simple to have an object provide multiple slots through operator() functions
	of different signatures.  The following class does so through providing 1-ary slots
	of different types:

	\dontinclude example.cpp
	\skip class SignalIntFloatCollector
	\until end class

	The following class, on the other hand, uses slots of different number of arguments:
	\dontinclude example.cpp
	\skip class SignalMultiCollector
	\until end class

	In such cases, where the operator() functions differ in their signature,
	standard connection operators will work out the correct connection:

	\dontinclude example.cpp
	\skip void multi_type_test
	\until end void multi_type_test

\subsection signal_network_multi_in_example Multiple inputs of the same signature

	In some cases, a class may want to receive multiple signals of the same
	signature.  For example, the following class can receive a void() signal
	through its inherited operator() function, as well as through the
	operator() function of member "other":
	\dontinclude example.cpp
	\skip class Signal2VoidCounter
	\until end class
	
	Similarly, the following class could receive void() signals both
	through operator() and through AltInput:
	\dontinclude example.cpp
	\skip class Signal2VoidInputs
	\until end class

	The following example shows how to connect signals to all of the above slots.
	For the class Signal2VoidInputs, this is accomplished using the
	slot_selector function:
	\skip void multi_in_test
	\until end void multi_in_test
	
\subsection signal_network_multi_out_example Multiple outputs

	\dontinclude example.cpp
	\skip class SignalOutIntFloat
	\until end void multi_out_test
	
*/

}
}