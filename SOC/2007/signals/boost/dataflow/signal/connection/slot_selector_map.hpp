// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_SLOT_SELECTOR_MAP_HPP
#define SIGNAL_NETWORK_SLOT_SELECTOR_MAP_HPP
/*
#include <boost/dataflow/signal/connection/slot_selector.hpp>

#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/sequence/intrinsic/value_at_key.hpp>
#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost { namespace dataflow {

template<typename T>
struct producer_category_of<T, typename boost::enable_if<boost::fusion::traits::is_sequence<T> >::type>
{
    typedef signal_producer type;
};
    
namespace extension { namespace signals {

// Support for fusion maps of consumer components.
template<typename Signature, typename T>
struct get_slot<Signature, T, typename boost::enable_if<boost::fusion::traits::is_sequence<T> >::type> // this should be is_map
{
    boost::function<Signature> operator()(const T &seq)
    {
        return get_slot<Signature, typename boost::fusion::result_of::value_at_key<T, Signature>::type>()
            (boost::fusion::at_key<Signature>(seq));
    }
};

// Support for slot_selector as an input component (producer).
template<typename T>
struct get_signal_type<T, typename boost::enable_if<boost::fusion::traits::is_sequence<T> >::type>
{
    typedef
        typename get_signal_type<
            typename boost::remove_reference<
                typename boost::fusion::result_of::front<T>::type
            >::type::second_type
        >::type type;
};

// Support for slot_selector as an input component (producer).
template<typename T>
struct get_signal<T, typename boost::enable_if<boost::fusion::traits::is_sequence<T> >::type>
{
    typename get_signal_type<T>::type &operator()(const T &map)
    {
        return get_signal<
            typename boost::remove_reference<
                typename boost::fusion::result_of::front<T>::type
            >::type::second_type>()(boost::fusion::front(map).second);
    }
};

    } }

} } // namespace boost::signals
*/
#endif // SIGNAL_NETWORK_SLOT_SELECTOR_MAP_HPP