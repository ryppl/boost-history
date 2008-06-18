// filter.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef DATAFLOW_SIGNALS_FUSION_FILTER_HPP
#define DATAFLOW_SIGNALS_FUSION_FILTER_HPP

#include <boost/dataflow/signals/component/filter.hpp>

#include <boost/fusion/functional/adapter/fused.hpp>
#include <boost/fusion/include/as_vector.hpp>


#define SIGNAL_NETWORK_DEFAULT_OUT output::unfused

namespace boost { namespace signals {

// the unfused, combined, and fused structs are used for specification of the filter class.
// unfused and fused are also used for specification of provided components based on the filter class.
// in the latter case, the filter_type member specifies the type of the underlying filter.

namespace output {

/** \brief Used for specification of the filter class using an internal fused adaptor for an unfused output signal.
*/
struct unfused
{
};

/** \brief Used to specify unfused versions of provided components.
    For the filter class, this means the filter will use a fused output signal only.
*/
struct fused
{
};

}

///	Provides a basis for filters (components that receive and send a signal).
/**	\param OutSignature The signature of the signal being sent out.

Use this class as a base class for classes that produce a signal
of a particular signature.
*/
template<
    typename Derived,
    typename OutSignature,
    typename InSignatures=mpl::vector<>,
    typename OutSignal=SIGNAL_NETWORK_DEFAULT_OUT,
    typename SignalArgs=typename default_signal_args<OutSignature>::type
>
class fusion_filter;

/** \brief Combined version of the filter class
*/
template<typename Derived, typename OutSignature, typename InSignatures, typename SignalArgs>
class fusion_filter<Derived, OutSignature, InSignatures, output::unfused, SignalArgs>
: public filter<Derived, OutSignature, InSignatures, SignalArgs>
{
//    BOOST_MPL_ASSERT(( mpl::is_sequence<InSignatures> ));
//    BOOST_MPL_ASSERT(( is_signal_args<SignalArgs> ));

public:
    fusion_filter() : fused_out(fusion_filter::out) {}
	fusion_filter(const fusion_filter &) : fused_out(fusion_filter::out){}
    const fusion_filter &operator = (const fusion_filter &) {return *this;}
    
    typedef typename boost::function_types::parameter_types<OutSignature>::type parameter_types;
    typedef typename boost::fusion::result_of::as_vector<parameter_types>::type parameter_vector;
    typedef typename function_types::result_type<OutSignature>::type fused_signature_type (const parameter_vector &);

protected:
    typename SignalArgs::template apply<fused_signature_type>::type fusion_out;
    boost::fusion::fused<typename fusion_filter::signal_type const &> fused_out;
}; // class filter

namespace detail
{
    template<typename OutSignature, typename SignalArgs=typename default_signal_args<OutSignature>::type>
    struct fused_signal_type
    {
        typedef typename boost::function_types::parameter_types<OutSignature>::type parameter_types;
        typedef typename boost::fusion::result_of::as_vector<parameter_types>::type parameter_vector;
        typedef typename SignalArgs::combiner_type::result_type signature_type (const parameter_vector &);
        typedef typename SignalArgs::combiner_type::result_type fused_signature_type (const parameter_vector &);
        typedef typename SignalArgs::template apply<fused_signature_type>::type signal_type;
    };
}

/** \brief Fused version of the filter class
*/
template<typename Derived, typename OutSignature, typename InSignatures, typename SignalArgs>
class fusion_filter<Derived, OutSignature, InSignatures, output::fused, SignalArgs>
: public filter_base<
    Derived,
    typename detail::fused_signal_type<OutSignature, SignalArgs>::signal_type,
    typename dataflow::utility::forced_sequence<InSignatures>::type >
{
//    BOOST_MPL_ASSERT(( mpl::is_sequence<InSignatures> ));
//    BOOST_MPL_ASSERT(( is_signal_args<SignalArgs> ));

public:
	fusion_filter(const fusion_filter &) {}
	fusion_filter(){}
    const fusion_filter &operator = (const fusion_filter &) {return *this;}

    typedef typename detail::fused_signal_type<OutSignature, SignalArgs>::parameter_types parameter_types;
    typedef typename detail::fused_signal_type<OutSignature, SignalArgs>::parameter_vector parameter_vector;
    typedef typename detail::fused_signal_type<OutSignature, SignalArgs>::signature_type signature_type;
    typedef typename detail::fused_signal_type<OutSignature, SignalArgs>::fused_signature_type fused_signature_type;
    typedef typename detail::fused_signal_type<OutSignature, SignalArgs>::signal_type signal_type;

	///	Returns the default out signal.
	signal_type &default_signal() const
	{	return fused_out; }
	///	Disconnects all slots connected to the signals::filter.
	void disconnect_all_slots() {fused_out.disconnect_all_slots();}
    
protected:
    mutable signal_type fused_out;
}; // class filter

} } // namespace boost::signals

#endif // DATAFLOW_SIGNALS_FUSION_FILTER_HPP
