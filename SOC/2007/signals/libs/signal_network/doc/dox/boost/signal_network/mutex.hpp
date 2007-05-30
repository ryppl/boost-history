#include <boost/signal_network/detail/defines.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

/** \brief Ensures a component is processing only one signal at a time when using multiple threads.
	\param Signature Signature of the signal sent and received.
	\warning Since I can't get Doxygen to unravel the preprocessed definition of this
		class, the documentation was generated through a non-functioning class Doxygen could read.

	\par Example:
	\dontinclude example.cpp
	\skip mutex_test
	\until end void mutex_test
*/

template<typename Signature>
class mutex : public filter<Signature>
{
public:
	/// Locks an internal mutex and forwards the signal.
	typename boost::function_traits<Signature>::result_type operator()(...);
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE
