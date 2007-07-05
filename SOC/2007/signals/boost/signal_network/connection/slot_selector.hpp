/*
 *  signal.hpp
 *  signal_network
 *
 *  Created by Stjepan Rajko on 7/3/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SIGNAL_NETWORK_SLOT_SELECTOR_HPP
#define SIGNAL_NETWORK_SLOT_SELECTOR_HPP

#include <boost/signal_network/connection/detail/bind_object.hpp>
#include <boost/signal_network/component/traits.hpp>

namespace boost { namespace signals {

template<typename Signature, typename T>
struct slot_selector
{
    typedef Signature signature_type;
    typedef T class_type;
    
	T &object;
	typename detail::slot_type<Signature, T>::type func;
    
	slot_selector(typename detail::slot_type<Signature, T>::type func, T &object)
		: object(object), func(func) {}
};

///	Allows functions other than operator() to serve as signals::filter slots.
template<typename Signature, typename T>
slot_selector<Signature, T> make_slot_selector(typename detail::slot_type<Signature, T>::type func, T &object)
{
	return slot_selector<Signature, T>(func, object);
}

template<typename Signature, typename T>
struct is_component<slot_selector<Signature, T> >
: public boost::true_type {};

template<typename Signature, typename T>
struct get_signal<slot_selector<Signature, T> >
{
    typename T::signal_type &operator()(const slot_selector<Signature, T> &selector)
    {
        return selector.object.default_signal();
    }
};

template<typename Signature, typename T>
struct get_slot<Signature, slot_selector<Signature, T> >
{
    boost::function<Signature> operator()(const slot_selector<Signature, T> &selector)
    {
        return detail::bind_object<Signature, T>()
            (static_cast<typename detail::slot_type<Signature, T>::type>(selector.func), selector.object);        
    }
};

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_SLOT_SELECTOR_HPP