#ifndef SIGNAL_NETWORK_BASE_HPP
#define SIGNAL_NETWORK_BASE_HPP

#include <boost/signal.hpp>
#ifdef _THREAD_SAFE_SIGNAL_HPP
#define SIGNAL_NETWORK_THREAD_SAFE
#else
#define SIGNAL_NETWORK_TRACKABLE
#endif
#include <boost/signal_network/detail/defines.hpp>

// Constructs a type corresponding to pointer to member of T with signature Signature.
// e.g. slot_type<some_class, void(float)>::type is void (some_class::*) (float)

#define SIGNAL_NETWORK_TEMPLATE_CLASS slot_type
#define SIGNAL_NETWORK_TEMPLATE_NO_RETURNS
#define SIGNAL_NETWORK_TEMPLATE_TYPENAME_T
#define SIGNAL_NETWORK_TEMPLATE_DETAIL
#include <boost/signal_network/detail/loader.hpp>

/// \namespace boost::signal_network \brief Connection operators
/// \namespace boost::signal_network::signet \brief Signal Network components
/// \namespace boost::signal_network::signet::detail \brief Detail
SIGNAL_NETWORK_OPEN_SIGNAL_NETWORK_NAMESPACE

template<typename T, typename Signature>
struct slot_selector_t
{
	T &link;
	typename signet::detail::slot_type<T, Signature>::type func;

	slot_selector_t<T, Signature>(T &link, typename signet::detail::slot_type<T, Signature>::type func)
		: link(link), func(func) {}
	operator T &() {return link;}
};

///	Allows functions other than operator() to serve as signet::filter slots.
/**	\ingroup Utility
	\sa signet::filter for an \ref signet::filter_multi_in_example "example"
*/
template<typename Signature, typename T>
slot_selector_t<T, Signature> slot_selector(T &link, typename signet::detail::slot_type<T, Signature>::type func)
{
	return slot_selector_t<T, Signature>(link, func);
}

SIGNAL_NETWORK_CLOSE_SIGNAL_NETWORK_NAMESPACE

#define SIGNAL_NETWORK_TEMPLATE_CLASS connect
#define SIGNAL_NETWORK_TEMPLATE_NO_RETURNS
#define SIGNAL_NETWORK_TEMPLATE_TYPENAME_T
#define SIGNAL_NETWORK_TEMPLATE_NO_MAIN_CLASS
#include <boost/signal_network/detail/loader.hpp>

#define SIGNAL_NETWORK_TEMPLATE_CLASS connect_slot
#define SIGNAL_NETWORK_TEMPLATE_NO_RETURNS
#define SIGNAL_NETWORK_TEMPLATE_TYPENAME_T
#define SIGNAL_NETWORK_TEMPLATE_NO_MAIN_CLASS
#include <boost/signal_network/detail/loader.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

namespace detail {
	
template<typename T, typename Signature> void connect(boost::signal<Signature> &signal, T &link)
{
	connect_impl<T, Signature, boost::function_traits<Signature>::arity>::connect(signal, link);
}

template<typename T, typename Signature> void connect_slot(boost::signal<Signature> &signal, const slot_selector_t<T, Signature> &link)
{
	connect_slot_impl<T, Signature, boost::function_traits<Signature>::arity>::connect_slot(signal, link);
}

} // namespace detail

class filter_base
#ifdef SIGNAL_NETWORK_TRACKABLE
	: public boost::signals::trackable
#endif
{};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE

SIGNAL_NETWORK_OPEN_SIGNAL_NETWORK_NAMESPACE

///	Connects a sequence of components using signals.
/** This operator is identical to signet::filter::operator>= (it connects the
left component to the right component, and returns a reference to the left component),
except it is evaluated right to left.  This makes it semantics more suitable for
connecting a chain of connections.
*/
template<typename Filter, typename T>
typename boost::enable_if<boost::is_base_of<signet::filter_base, Filter>, Filter & >::type
operator >>= (Filter &filter, T &link) { signet::detail::connect(filter.default_signal(), link); return filter;}

/// Allows branching in a component connection sequence.
/** This operator is identical to signet::filter::operator>>=, (it connects the
left component to the right component, and returns a reference to the left component)
except it is evaluated left to right.  This makes its semantics more suitable for
branching connections.
*/
template<typename Filter, typename T>
typename boost::enable_if<boost::is_base_of<signet::filter_base, Filter>, Filter & >::type
operator | (Filter &filter, T &link) { signet::detail::connect(filter.default_signal(), link); return filter;}

///	Allows slot functions other than operator() to be used in a sequence of components.
/**	\sa slot_selector()
*/
template<typename Filter, typename T, typename Signature>
typename boost::enable_if<boost::is_base_of<signet::filter_base, Filter>, Filter & >::type
operator >>= (Filter &filter, slot_selector_t<T, Signature> link) {signet::detail::connect_slot(filter.default_signal(), link); return filter;}

/// Allows slot functions other than operator() to be used with branching.
/** \sa slot_selector()
*/
template<typename Filter, typename T, typename Signature>
typename boost::enable_if<boost::is_base_of<signet::filter_base, Filter>, Filter & >::type
operator | (Filter &filter, slot_selector_t<T, Signature> link) {signet::detail::connect_slot(filter.default_signal(), link); return filter;}

template<typename Signature, typename T>
boost::signal<Signature> &operator >>= (boost::signal<Signature> &signal, T &link)
{signet::detail::connect(signal, link); return signal;}

template<typename Signature, typename T>
boost::signal<Signature> &operator >= (boost::signal<Signature> &signal, T &link)
{signet::detail::connect(signal, link); return signal;}	

SIGNAL_NETWORK_CLOSE_SIGNAL_NETWORK_NAMESPACE

#endif // SIGNAL_NETWORK_BASE_HPP