// filter.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_FILTER_HPP
#define SIGNAL_NETWORK_FILTER_HPP

#include <boost/bind.hpp>
#include <boost/signal_network/base.hpp>
#include <boost/type_traits/is_base_of.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

///	Provides a basis for filters (components that receive and send a signal).
/**	\param Signature The signature of the signal being sent out.

	Use this class as a base class for classes that produce a \e signal
	of a particular signature.  

	\par Example:
	\dontinclude example.cpp
	\skip DoublerClass
	\until end void filter_test
*/
template<typename Signature>
class filter : public filter_base
{
public:
	typedef Signature signature_type;
	typedef boost::signal<Signature> signal_type;
	typedef typename boost::function_traits<Signature>::result_type result_type;

	filter(const filter &) {}
	filter &operator = (const filter &) {return *this;}
	filter(){}
	///	Returns the default out signal.
	signal_type &default_signal()
	{	return out; }
	///	Disconnects all slots connected to the signet::filter.
	void disconnect_all_slots() {out.disconnect_all_slots();}
protected:
	signal_type out;
}; // class filter

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE

#endif // SIGNAL_NETWORK_FILTER_HPP
