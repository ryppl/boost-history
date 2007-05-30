#include <boost/signal_network/filter.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

/** \brief Connects a number of components of the same type in a chain.
	\param T Type of the component.
	\param Signature Signature of the signal sent and received.
	\warning Since I can't get Doxygen to unravel the preprocessed definition of this
		class, the documentation was generated through a non-functioning class Doxygen could read.

	\par Example:
	\dontinclude example.cpp
	\skip chain_test
	\until end void chain_test
*/
template<typename T, typename Signature>
class chain : public filter_base
{
public:
	///	Constructs a chain composed of instances of T.
	chain(size_t copies) : SIGNAL_NETWORK_TEMPLATE_CLASS_IMPL_BASE(copies) {}
	///	Constructs a chain composed of copies of component.
	chain(size_t copies, T &component) : SIGNAL_NETWORK_TEMPLATE_CLASS_IMPL_BASE(copies, &component) {}
	/// Sending a signal to the chain will forward it to the first component in the chain.
	operator()(...);
	///	The default signal coming out of the chain is the default signal of the last component in the chain.
	boost::signal<Signature> &default_signal();
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE
