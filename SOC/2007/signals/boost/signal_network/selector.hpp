// selector.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_SELECTOR_HPP
#define SIGNAL_NETWORK_SELECTOR_HPP

#include <boost/signal_network/filter.hpp>
#include <boost/signal_network/detail/defines.hpp>

#define SIGNAL_NETWORK_SELECTOR_INPUT(z,n,_arity) \
	SIGNAL_NETWORK_FUNCTION(input##n, Signature, _arity) \
	{ \
		if (selected == n) \
			SIGNAL_NETWORK_TEMPLATE_BASE::out(SIGNAL_NETWORK_ARG_NAMES(_arity)); \
	} \
	slot_selector_t<selector_impl<_inputs, Signature, _arity>, Signature> slot##n () \
	{return slot_selector<Signature>(*this, &selector_impl<_inputs, Signature, _arity>::input##n);}

#define SIGNAL_NETWORK_TEMPLATE_CLASS selector
#define SIGNAL_NETWORK_TEMPLATE_BASE filter<Signature>
#define SIGNAL_NETWORK_TEMPLATE_ITERATE2

#include <boost/signal_network/detail/loader.hpp>

#endif // SIGNAL_NETWORK_SELECTOR_HPP
