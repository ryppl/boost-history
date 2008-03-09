// filter.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_FILTER_HPP
#define SIGNAL_NETWORK_FILTER_HPP

#include <boost/dataflow/signals/component/filter_base.hpp>
#include <boost/dataflow/utility/forced_sequence.hpp>

#include <boost/fusion/functional/adapter/fused.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/adapted/mpl.hpp>


#define SIGNAL_NETWORK_DEFAULT_OUT unfused

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

template<
    typename Combiner,
    typename Group=int,
    typename GroupCompare=std::less<Group> >
struct signal_args
{
    typedef Combiner combiner_type;
    typedef Group group_type;
    typedef GroupCompare group_compare_type;
};


template<typename T, typename Enable=void>
struct is_signal_args : public mpl::false_
{};

template<typename T>
struct is_signal_args<
    T,
    typename dataflow::utility::enable_if_type<
        dataflow::utility::all_of<
            typename T::combiner_type,
            typename T::group_type,
            typename T::group_compare_type
        >
    >::type>
    : public mpl::true_
{};    
    
template<typename OutSignature>
struct default_signal_args
{
    typedef boost::last_value<typename boost::function_traits<OutSignature>::result_type> combiner_type;
    typedef int group_type;
    typedef std::less<group_type> group_compare_type;

    typedef signal_args<combiner_type, group_type, group_compare_type> type;
};

template<>
struct default_signal_args<void>
{
    typedef void type;
};

template<typename OutSignature, typename SignalArgs>
struct signal_from_args
{
    typedef boost::signal<
        OutSignature,
        typename SignalArgs::combiner_type,
        typename SignalArgs::group_type,
        typename SignalArgs::group_compare_type
    > type;
};


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
class filter;

/** \brief Unfused version of the filter class
*/
template<typename Derived, typename OutSignature, typename InSignatures, typename SignalArgs>
class filter<Derived, OutSignature, InSignatures, unfused, SignalArgs>
    : public filter_base<
        Derived,
        typename signal_from_args<OutSignature, SignalArgs>::type,
        typename dataflow::utility::forced_sequence<InSignatures>::type >
{
//    BOOST_MPL_ASSERT(( mpl::is_sequence<InSignatures> ));
    BOOST_MPL_ASSERT(( is_signal_args<SignalArgs> ));
    
public:
    // the type of the signal
    typedef typename signal_from_args<OutSignature, SignalArgs>::type signal_type;
    // the signature of the output signal
	typedef OutSignature signature_type;

	filter(const filter &) {}
	filter(){}
    const filter &operator = (const filter &) {return *this;}

	///	Returns the default out signal.
  	signal_type &default_signal() const
	{	return out; }

	///	Disconnects all slots connected to the signals::filter.
	void disconnect_all_slots() {out.disconnect_all_slots();}
protected:
	mutable signal_type out;
}; // class filter

/** \brief Filter class with no output signal
*/
template<typename Derived, typename InSignatures=mpl::vector<> >
class consumer
    : public filter_base<
        Derived, 
        void,
        typename dataflow::utility::forced_sequence<InSignatures>::type >
{
}; // class filter

/** \brief Combined version of the filter class
*/
template<typename Derived, typename OutSignature, typename InSignatures, typename SignalArgs>
class filter<Derived, OutSignature, InSignatures, combined, SignalArgs>
: public filter<Derived, OutSignature, InSignatures, unfused, SignalArgs>
{
//    BOOST_MPL_ASSERT(( mpl::is_sequence<InSignatures> ));
    BOOST_MPL_ASSERT(( is_signal_args<SignalArgs> ));

public:
    filter() : fused_out(filter::out) {}
	filter(const filter &) : fused_out(filter::out){}
    const filter &operator = (const filter &) {return *this;}
    
    typedef typename boost::function_types::parameter_types<OutSignature>::type parameter_types;
    typedef typename boost::fusion::result_of::as_vector<parameter_types>::type parameter_vector;
    typedef typename filter<Derived, OutSignature, InSignatures, unfused, SignalArgs>::signal_type::result_type fused_signature_type (const parameter_vector &);

protected:
    typename signal_from_args<fused_signature_type, SignalArgs>::type fusion_out;
    boost::fusion::fused<typename filter::signal_type const &> fused_out;
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
        typedef typename signal_from_args<fused_signature_type, SignalArgs>::type signal_type;
    };
}

/** \brief Fused version of the filter class
*/
template<typename Derived, typename OutSignature, typename InSignatures, typename SignalArgs>
class filter<Derived, OutSignature, InSignatures, fused, SignalArgs>
: public filter_base<
    Derived,
    typename detail::fused_signal_type<OutSignature, SignalArgs>::signal_type,
    typename dataflow::utility::forced_sequence<InSignatures>::type >
{
//    BOOST_MPL_ASSERT(( mpl::is_sequence<InSignatures> ));
    BOOST_MPL_ASSERT(( is_signal_args<SignalArgs> ));

public:
	filter(const filter &) {}
	filter(){}
    const filter &operator = (const filter &) {return *this;}

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

#endif // SIGNAL_NETWORK_FILTER_HPP
