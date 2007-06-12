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
#include <boost/fusion/functional/adapter/fused.hpp>
#include <boost/fusion/sequence/conversion/as_vector.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

///	Provides a basis for filters (components that receive and send a signal).
/**	\param Signature The signature of the signal being sent out.

	Use this class as a base class for classes that produce a signal
	of a particular signature.
*/
struct combined_out_signal
{
    typedef combined_out_signal default_normal_type;
    typedef combined_out_signal default_unfused_type;
};
struct unfused_out_signal
{
    typedef combined_out_signal default_normal_type;
    typedef combined_out_signal default_unfused_type;
};
struct fused_out_signal
{
    typedef fused_out_signal default_normal_type;    
    typedef fused_out_signal default_unfused_type;
};

struct default_out_signal
{
    typedef fused_out_signal default_normal_type;
    typedef combined_out_signal default_unfused_type;
};

template<typename Signature,
typename OutSignal=unfused_out_signal,
typename Combiner = boost::last_value<typename boost::function_traits<Signature>::result_type>,
typename Group = int,
typename GroupCompare = std::less<Group>
>
class filter;

/** \brief Unfused version of the filter class
*/
template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
class filter<Signature, unfused_out_signal, Combiner, Group, GroupCompare> : public filter_base
{
public:
    // the signature of the output signal
	typedef Signature signature_type;
    // the type of the signal
	typedef boost::signal<Signature, Combiner, Group, GroupCompare> signal_type;

	filter(const filter &) {}
	filter(){}
    const filter &operator = (const filter &) {return *this;}

	///	Returns the default out signal.
	signal_type &default_signal()
	{	return out; }
	///	Disconnects all slots connected to the signet::filter.
	void disconnect_all_slots() {out.disconnect_all_slots();}
protected:
	signal_type out;
}; // class filter

/** \brief Combined version of the filter class
*/
template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
class filter<Signature, combined_out_signal, Combiner, Group, GroupCompare>
: public filter<Signature, unfused_out_signal, Combiner, Group, GroupCompare>
{
    typedef filter<Signature, unfused_out_signal, Combiner, Group, GroupCompare> base_type;
public:
    filter() : fused_out(base_type::out) {}

    typedef typename base_type::signature_type signature_type;
    typedef typename base_type::signal_type signal_type;
    
    typedef typename boost::function_types::parameter_types<Signature>::type parameter_types;
    typedef typename boost::fusion::result_of::as_vector<parameter_types>::type parameter_vector;
    typedef typename signal_type::result_type fused_signature_type (const parameter_vector &);

protected:
    boost::signal<fused_signature_type, Combiner, Group, GroupCompare> fusion_out;
    boost::fusion::fused<typename base_type::signal_type const &> fused_out;
}; // class filter

/** \brief Fused version of the filter class
*/
template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
class filter<Signature, fused_out_signal, Combiner, Group, GroupCompare>
: public filter_base
{
public:
	filter(const filter &) {}
	filter(){}
    const filter &operator = (const filter &) {return *this;}

    typedef typename boost::function_types::parameter_types<Signature>::type parameter_types;
    typedef typename boost::fusion::result_of::as_vector<parameter_types>::type parameter_vector;
    typedef typename Combiner::result_type signature_type (const parameter_vector &);
    typedef typename Combiner::result_type fused_signature_type (const parameter_vector &);
    typedef boost::signal<signature_type, Combiner, Group, GroupCompare> signal_type;

	///	Returns the default out signal.
	signal_type &default_signal()
	{	return fused_out; }
	///	Disconnects all slots connected to the signet::filter.
	void disconnect_all_slots() {fused_out.disconnect_all_slots();}
    
protected:
    signal_type fused_out;
}; // class filter

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE

#endif // SIGNAL_NETWORK_FILTER_HPP
