// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_CONNECTION_SIGNAL_HPP
#define SIGNAL_NETWORK_CONNECTION_SIGNAL_HPP

#include <boost/dataflow/signal/connection/connect.hpp>
#include <boost/dataflow/signal/connection/detail/bind_object.hpp>
#include <boost/dataflow/signal/connection/detail/result_of_defined.hpp>
#include <boost/dataflow/signal/connection/detail/replace_return_type.hpp>
#include <boost/dataflow/signal/detail/enable_if_defined.hpp>
#include <boost/dataflow/signal/component/traits.hpp>
#include <boost/signal.hpp>

#include <boost/mpl/aux_/has_type.hpp>
#include <boost/utility/result_of.hpp>

namespace boost { namespace signals {

namespace detail
{
    template<typename T>
    struct get_signature;
    
    template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
    struct get_signature<boost::signal<Signature, Combiner, Group, GroupCompare> >
    {
        typedef Signature type;
    };
}
    
/** \brief Support for components using the signal data transport mechanism.
*/
template<typename Input, typename Output>
struct connect_impl<Input, Output>
{
    void operator()(Input &input, Output &output)
    {
        get_signal<Input>()(input).connect(get_slot<typename detail::get_signature<typename get_signal_type<Input>::type>::type, Output>()(output));
    }
    void operator()(Input &input, const Output &output)
    {
        get_signal<Input>()(input).connect(get_slot<typename detail::get_signature<typename get_signal_type<Input>::type>::type, Output>()(output));
    }
    void operator()(const Input &input, Output &output)
    {
        get_signal<Input>()(input).connect(get_slot<typename detail::get_signature<typename get_signal_type<Input>::type>::type, Output>()(output));
    }
    void operator()(const Input &input, const Output &output)
    {
        get_signal<Input>()(input).connect(get_slot<typename detail::get_signature<typename get_signal_type<Input>::type>::type, Output>()(output));
    }
};

/** \brief Support for boost::signal.
*/
template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
struct is_component<boost::signal<Signature, Combiner, Group, GroupCompare> >
    : public boost::true_type {};

/** \brief Support for boost::signal.
*/
template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
struct get_signal<boost::signal<Signature, Combiner, Group, GroupCompare> >
{
    boost::signal<Signature, Combiner, Group, GroupCompare> & operator()
        (boost::signal<Signature, Combiner, Group, GroupCompare> &s) {return s;}
};

/** \brief Support for boost::signal.
*/
template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
struct get_signal_type<boost::signal<Signature, Combiner, Group, GroupCompare> >
{
    typedef boost::signal<Signature, Combiner, Group, GroupCompare> type;
};

/** \brief Support for function objects that for which boost::result_of is defined.
*/
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

} } // namespace boost::signals
#endif // SIGNAL_NETWORK_CONNECTION_SIGNAL_HPP