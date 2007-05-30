#include <boost/signal_network/detail/defines.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

/** \brief Converts a function into a Signal Network filter.
	\param Signature Signature of the function to be converted.
	\warning Since I can't get Doxygen to unravel the preprocessed definition of this
		class, the documentation was generated through a non-functioning class Doxygen could read.

	The signet::function object will receive signals of signature void(<i>function arguments</i>),
	and send signals of signature void(<i>function return type</i>).

	\par Example:
	\dontinclude example.cpp
	\skip DoublerFunc
	\until end void function_test

*/

template<typename Signature>
class function : public filter<Signature>
{
public:
	///	Initializes the object with the specified function.
	function(boost::function<Signature> f);
	/// Calls the function with the arguments of the signal, and
	/// then sends a signal with the return value of the function
	void operator()(...);
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE
