// selector.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_SELECTOR_HPP
#define SIGNAL_NETWORK_SELECTOR_HPP

#include <boost/signal_network/filter.hpp>
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

/*
template<typename Signature>
class selector : public fused_filter<Signature>
{
	selector() : selected(0) {}

    ///	Selects the indicated input (if source is 0, no input will be selected).
	void select(int source) {selected = source;}
	
    template <class Seq>
    struct result
    {
        typedef typename boost::function_traits<Signature>::result_type type;
    };
    /// If N is selected, the signal received at this slot will be forwared.
    typename boost::function_traits<Signature>::result_type 
    operator()(const VecPar &vec_par)
    {
    	if (selected == n)
             return static_cast<typename boost::function_traits<Signature>::result_type>
                 (fused_out(vec_par));
    }
    /// Returns the slot selector for inputN.
    template<int N>
    slot_selector_t<selector<Signature>, Signature> slot()
	{return slot_selector<Signature>(*this, &selector_impl<_inputs, Signature, _arity>::input##n);}
private:
	volatile int selected;
};
*/
SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE

#endif // SIGNAL_NETWORK_SELECTOR_HPP
