#include <boost/signal_network/detail/defines.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

/** \brief Upon receiving an empty signal, outputs the stored value.
	\param Signature signature of the signal sent by the storage.
	\warning Since I can't get Doxygen to unravel the preprocessed definition of this
		class, the documentation was generated through a non-functioning class Doxygen could read.

	\par Example:
	\dontinclude example.cpp
	\skip simple_test
	\until end void simple_test

*/
template<typename Signature>
class storage : public filter<Signature>
{
public:
	/// Stored value type.
	typedef typename detail::storable<typename boost::function_traits<Signature>::arg1_type>::type argN_value_type;

	///	Sends a signal containing the stored values.
	typename boost::function_traits<Signature>::result_type operator()();
	///	Sets the stored values.
	void operator()(typename boost::call_traits<arg1_value_type>::param_type arg1,
		typename boost::call_traits<arg2_value_type>::param_type arg2, ...);
	///	Returns the stored value of parameter N using a param_type cast.
	typename boost::call_traits<argN_value_type>::param_type valueN();
	///	Returns the stored value of parameter N.
	typename arg##n##_value_type value_exactN();
	/// Returns the slot selector for the valueN function.
	slot_selector_t<storage, typename boost::call_traits<argN_value_type>::param_type ()> slotN ();
	/// Returns the slot selector for the value_exactN function.
	slot_selector_t<storage, typename argN_value_type ()> slot_exactN ();

};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE