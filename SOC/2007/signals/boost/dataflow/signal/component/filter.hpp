// filter.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_FILTER_HPP
#define SIGNAL_NETWORK_FILTER_HPP

#include <boost/dataflow/signal/component/filter_base.hpp>

#include <boost/fusion/functional/adapter/fused.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/adapted/mpl.hpp>

#ifndef SIGNAL_NETWORK_DEFAULT_OUT
#define SIGNAL_NETWORK_DEFAULT_OUT unfused
#endif

namespace boost { namespace signals {

// the unfused, combined, and fused structs are used for specification of the filter class.
// unfused and fused are also used for specification of provided components based on the filter class.
// in the latter case, the filter_type member specifies the type of the underlying filter.

/** \brief Used for specification of the filter class using an internal fused adaptor for an unfused output signal.
*/
struct combined {};

/** \brief Used to specify unfused versions of provided components.
    For the filter class, this means the filter will use an unfused output signal only,
*/
struct unfused
{
    typedef combined filter_type;
};

/** \brief Used to specify unfused versions of provided components.
    For the filter class, this means the filter will use a fused output signal only.
*/
struct fused
{
    typedef fused filter_type;
};

///	Provides a basis for filters (components that receive and send a signal).
/**	\param Signature The signature of the signal being sent out.

Use this class as a base class for classes that produce a signal
of a particular signature.
*/
template<typename Signature,
typename OutSignal=SIGNAL_NETWORK_DEFAULT_OUT,
typename Combiner = boost::last_value<typename boost::function_traits<Signature>::result_type>,
typename Group = int,
typename GroupCompare = std::less<Group>
>
class filter;

/** \brief Unfused version of the filter class
*/
template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
class filter<Signature, unfused, Combiner, Group, GroupCompare>
    : public filter_base<boost::signal<Signature, Combiner, Group, GroupCompare> >
{
public:
    // the type of the signal
    typedef boost::signal<Signature, Combiner, Group, GroupCompare> signal_type;

    // the signature of the output signal
	typedef Signature signature_type;

	filter(const filter &) {}
	filter(){}
    const filter &operator = (const filter &) {return *this;}

	///	Returns the default out signal.
  	signal_type &get_proxied_producer() const
	{	return out; }

	///	Disconnects all slots connected to the signals::filter.
	void disconnect_all_slots() {out.disconnect_all_slots();}
protected:
	mutable signal_type out;
}; // class filter

/** \brief Combined version of the filter class
*/
template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
class filter<Signature, combined, Combiner, Group, GroupCompare>
: public filter<Signature, unfused, Combiner, Group, GroupCompare>
{
    typedef filter<Signature, unfused, Combiner, Group, GroupCompare> base_type;
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

namespace detail
{
    template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
    struct fused_signal_type
    {
        typedef typename boost::function_types::parameter_types<Signature>::type parameter_types;
        typedef typename boost::fusion::result_of::as_vector<parameter_types>::type parameter_vector;
        typedef typename Combiner::result_type signature_type (const parameter_vector &);
        typedef typename Combiner::result_type fused_signature_type (const parameter_vector &);
        typedef boost::signal<signature_type, Combiner, Group, GroupCompare> signal_type;
    };
}

/** \brief Fused version of the filter class
*/
template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
class filter<Signature, fused, Combiner, Group, GroupCompare>
: public filter_base<typename detail::fused_signal_type<Signature, Combiner, Group, GroupCompare>::signal_type>
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
    typedef signal_type proxy_producer_for;

	///	Returns the default out signal.
	proxy_producer_for &get_proxied_producer() const
	{	return fused_out; }
	///	Disconnects all slots connected to the signals::filter.
	void disconnect_all_slots() {fused_out.disconnect_all_slots();}
    
protected:
    mutable signal_type fused_out;
}; // class filter

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_FILTER_HPP
