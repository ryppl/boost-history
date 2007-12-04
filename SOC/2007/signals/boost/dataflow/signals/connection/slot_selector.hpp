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

#include <boost/dataflow/signals/connection/detail/bind_object.hpp>
#include <boost/dataflow/signals/support.hpp>
#include <boost/dataflow/signals/component/filter_base.hpp>

namespace boost { namespace dataflow { namespace signals {

struct slot_selector_consumer
    : public boost::dataflow::port_traits<ports::consumer, concepts::keyed_port, tag> {};

} } // namespace dataflow::signals

namespace signals {

/** \brief Reference to a class instance and pointer to a class member function.
*/
template<typename Signature, typename T>
struct slot_selector
{
    typedef boost::dataflow::signals::slot_selector_consumer port_traits;

    typedef Signature signature_type;
    typedef T class_type;
    
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

namespace extension {
    
    template<typename Signature>
    struct get_keyed_port_impl<
        signals::slot_selector_consumer, signals::producer<Signature> >
    {
        typedef const boost::function<Signature> result_type;
        
        template<typename Consumer>
        result_type operator()(Consumer &consumer)
        {
            return boost::signals::detail::bind_object<Signature, typename Consumer::class_type>()
            (static_cast<typename boost::signals::detail::slot_type<Signature, typename Consumer::class_type>::type>(consumer.func), consumer.object);
        }
    };
    
}

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_SLOT_SELECTOR_HPP