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
/*
template<typename Signature>
class selector : public fused_filter<Signature>
{
	selector() : selected(0) {}
	void select(int source) {selected = source;}
	
    template <class Seq>
    struct result
    {
        typedef typename boost::function_traits<Signature>::result_type type;
    };
    typename boost::function_traits<Signature>::result_type 
    operator()(const VecPar &vec_par)
    {
    	if (selected == n)
             return static_cast<typename boost::function_traits<Signature>::result_type>
                 (fused_out(vec_par));
    }
    template<int N>
    slot_selector_t<selector<Signature>, Signature> slot()
	{return slot_selector<Signature>(*this, &selector_impl<_inputs, Signature, _arity>::input##n);}
private:
	volatile int selected;
};
*/
SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE

#endif // SIGNAL_NETWORK_SELECTOR_HPP
