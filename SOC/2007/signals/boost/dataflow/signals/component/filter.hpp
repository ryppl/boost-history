// filter.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_FILTER_HPP
#define SIGNAL_NETWORK_FILTER_HPP

#include <boost/dataflow/signals/component/filter_base.hpp>
#include <boost/dataflow/utility/forced_sequence.hpp>

#include <boost/function_types/result_type.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/adapted/mpl.hpp>

namespace boost { namespace signals {

template<
    typename Combiner,
    typename Group=int,
    typename GroupCompare=std::less<Group> >
struct signal_args
{
    typedef Combiner combiner_type;
    typedef Group group_type;
    typedef GroupCompare group_compare_type;
    
    template<typename OutSignature>
    struct apply
    {
        typedef boost::signal<
            OutSignature,
            combiner_type,
            group_type,
            group_compare_type
        > type;
    };
};

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


///	Provides a basis for filters (components that receive and send a signal).
/**	\param OutSignature The signature of the signal being sent out.

Use this class as a base class for classes that produce a signal
of a particular signature.
*/
template<
    typename Derived,
    typename OutSignature,
    typename InSignatures=mpl::vector<>,
    typename SignalArgs=typename default_signal_args<OutSignature>::type
>
class filter
    : public filter_base<
        Derived,
        typename SignalArgs::template apply<OutSignature>::type,
        typename dataflow::utility::forced_sequence<InSignatures>::type >
{
//    BOOST_MPL_ASSERT(( mpl::is_sequence<InSignatures> ));
//    BOOST_MPL_ASSERT(( is_signal_args<SignalArgs> ));
    
public:
    // the type of the signal
    typedef typename SignalArgs::template apply<OutSignature>::type signal_type;
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

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_FILTER_HPP
