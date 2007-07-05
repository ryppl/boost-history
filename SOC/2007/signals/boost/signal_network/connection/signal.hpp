/*
 *  signal.hpp
 *  signal_network
 *
 *  Created by Stjepan Rajko on 7/3/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SIGNAL_NETWORK_CONNECTION_SIGNAL_HPP
#define SIGNAL_NETWORK_CONNECTION_SIGNAL_HPP

#include <boost/signal_network/connection/connect.hpp>
#include <boost/signal_network/connection/detail/bind_object.hpp>
#include <boost/signal_network/connection/detail/result_of_defined.hpp>
#include <boost/signal_network/component/traits.hpp>
#include <boost/signal.hpp>

#include <boost/mpl/aux_/has_type.hpp>
#include <boost/utility/result_of.hpp>

namespace boost { namespace signals {

template<typename Input, typename Output>
struct connect_impl<Input, Output>
{
    void operator()(Input &input, Output &output)
    {
        get_signal<Input>()(input).connect(get_slot<typename get_signature<Input>::type, Output>()(output));
    }
    void operator()(Input &input, const Output &output)
    {
        get_signal<Input>()(input).connect(get_slot<typename get_signature<Input>::type, Output>()(output));
    }
    void operator()(const Input &input, Output &output)
    {
        get_signal<Input>()(input).connect(get_slot<typename get_signature<Input>::type, Output>()(output));
    }
    void operator()(const Input &input, const Output &output)
    {
        get_signal<Input>()(input).connect(get_slot<typename get_signature<Input>::type, Output>()(output));
    }
};

template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
struct get_signal<boost::signal<Signature, Combiner, Group, GroupCompare> >
{
    boost::signal<Signature, Combiner, Group, GroupCompare> & operator()
        (boost::signal<Signature, Combiner, Group, GroupCompare> &s) {return s;}
};

template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
struct get_signature<boost::signal<Signature, Combiner, Group, GroupCompare> >
{
    typedef Signature type;
};

template<typename Signature, typename T>
struct get_slot<Signature, T, typename boost::enable_if<detail::result_of_defined<
typename detail::replace_return_type<Signature, T>::type > >::type>
{
    boost::function<Signature> operator()(T &object)
    {
        return detail::bind_object<Signature, T>()
            (static_cast<typename detail::slot_type<Signature, T>::type>(&T::operator()), object);
    }
    boost::function<Signature> operator()(const T &object)
    {
        return detail::bind_object<Signature, T>()
            (static_cast<typename detail::slot_type<Signature, T>::type>(&T::operator()), object);
    }
};

template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
struct is_component<boost::signal<Signature, Combiner, Group, GroupCompare> >
: public boost::true_type {};

} } // namespace boost::signals
#endif // SIGNAL_NETWORK_CONNECTION_SIGNAL_HPP