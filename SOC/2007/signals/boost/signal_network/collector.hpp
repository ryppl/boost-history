#ifndef SIGNAL_NETWORK_COLLECTOR_HPP
#define SIGNAL_NETWORK_COLLECTOR_HPP

#include <boost/type_traits.hpp>
#include <boost/call_traits.hpp>
#include <boost/signal_network/detail/defines.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

///	Stores data received via a signal so that it can be retrieved.
/**	\param Signature Signature of the signal that collector can receive.
	\todo Currently collector only supports signatures of type void (type).

	Example:
	\dontinclude SignalNetworkTests.cpp
	\skip simple_test
	\until end void simple_test
*/
template<typename Signature>
class collector
{
public:
	/// Type of the stored value
	typedef typename boost::remove_reference<typename boost::function_traits<Signature>::arg1_type>::type arg1_value_type;
	
	/// The default constructor will not initialize the stored value.
	collector(){}
	/// This constructor will initialize the stored value to what is provided.
	collector(typename boost::call_traits<arg1_value_type>::param_type init) : arg1_value(init) {}
	/// Stores the value in the signal.
	void operator()(typename boost::function_traits<Signature>::arg1_type arg1)
	{
		arg1_value = arg1;
	}
	/// Returns the last stored value.
	typename call_traits<arg1_value_type>::const_reference operator()() {return arg1_value;}
private:
	/// Stored value.
	typename arg1_value_type arg1_value;
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE

#endif // SIGNAL_NETWORK_COLLECTOR_HPP