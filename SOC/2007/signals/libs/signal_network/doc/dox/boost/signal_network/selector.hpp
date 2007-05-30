#include <boost/signal_network/detail/defines.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

/** \brief Allows selection of signals from multiple inputs.
	\param Signature Signature of the signal sent and received.
	\warning Since I can't get Doxygen to unravel the preprocessed definition of this
		class, the documentation was generated through a non-functioning class Doxygen could read.

	This can be used to determine which signal out of a group of signals
	will continue through the selector (the others will not be forwarded).

	\par Example:
	\dontinclude example.cpp
	\skip selector_test
	\until end void selector_test
*/
template<typename Signature>
class selector : public filter<Signature>
{
public:
	///	Selects the indicated input (if source is 0, no input will be selected).
	void select(int input);
	/// If N is selected, the signal received at this slot will be forwared.
	typename boost::function_traits<Signature>::result_type inputN(...);
	/// Returns the slot selector for inputN.
	slot_selector_t<selector, Signature> slotN();
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE
