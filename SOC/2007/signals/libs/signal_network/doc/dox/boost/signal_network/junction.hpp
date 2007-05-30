#include <boost/signal_network/detail/defines.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

/** \brief Forwards a single signal to multiple slots, and can
		also be disabled to stop the flow of signals.
	\param Signature Signature of the signal sent and received.
	\warning Since I can't get Doxygen to unravel the preprocessed definition of this
		class, the documentation was generated through a non-functioning class Doxygen could read.

	\par Example:
	\dontinclude example.cpp
	\skip junction_test
	\until end void junction_test

*/
template<typename Signature>
class junction : public filter<Signature>
{
public:
	/// Enables the junction (signals will be forwarded).
	void enable();
	///	Disables the junction (signals will not be forwarded).
	void disable();
	///	Upon receiving this signal, the same signal will be sent out if the junction is enabled.
	typename boost::function_traits<Signature>::result_type operator()(...);
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE
