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

#include <boost/dataflow/signal/connection/detail/bind_object.hpp>
#include <boost/dataflow/signal/support.hpp>

namespace boost { namespace signals {

/** \brief Reference to a class instance and pointer to a class member function.
*/
template<typename Signature, typename T>
struct slot_selector
{
    typedef Signature signature_type;
    typedef T class_type;
    typedef typename boost::dataflow::signal_consumer consumer_category;
    typedef typename boost::dataflow::signal_producer producer_category;
    
	T &object;
	typename detail::slot_type<Signature, T>::type func;
    
	slot_selector(typename detail::slot_type<Signature, T>::type func, T &object)
		: object(object), func(func) {}
};

/**	\brief Allows arbitrary member functions to serve as slots.
*/
template<typename Signature, typename T>
slot_selector<Signature, T> make_slot_selector(typename detail::slot_type<Signature, T>::type func, T &object)
{
	return slot_selector<Signature, T>(func, object);
}

} }

namespace boost { namespace dataflow {

    namespace extension { namespace signals {
/*
/// Support for slot_selector as an input component (producer).
template<typename Signature, typename T>
struct is_component<slot_selector<Signature, T> >
: public boost::true_type {};
*/

/// Support for slot_selector as an input component (producer).
template<typename Signature, typename T>
struct get_signal<boost::signals::slot_selector<Signature, T> >
{
    typename get_signal_type<T>::type &operator()(const boost::signals::slot_selector<Signature, T> &selector)
    {
        return selector.object.default_signal();
    }
};

/// Support for slot_selector as an input component (producer).
template<typename Signature, typename T>
struct get_signal_type<boost::signals::slot_selector<Signature, T> >
{
    typedef typename get_signal_type<T>::type type;
};

/// Support for slot_selector as an output component (consumer).
template<typename Signature, typename T>
struct get_slot<Signature, boost::signals::slot_selector<Signature, T> >
{
    boost::function<Signature> operator()(const boost::signals::slot_selector<Signature, T> &selector)
    {
        return boost::signals::detail::bind_object<Signature, T>()
            (static_cast<typename boost::signals::detail::slot_type<Signature, T>::type>(selector.func), selector.object);        
    }
};

} }

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_SLOT_SELECTOR_HPP