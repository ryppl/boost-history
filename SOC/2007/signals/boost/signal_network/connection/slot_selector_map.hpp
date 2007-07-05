/*
 *  slot_selector_map.hpp
 *  signal_network
 *
 *  Created by Stjepan Rajko on 7/3/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SIGNAL_NETWORK_SLOT_SELECTOR_MAP_HPP
#define SIGNAL_NETWORK_SLOT_SELECTOR_MAP_HPP

#include <boost/signal_network/connection/slot_selector.hpp>

#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/sequence/intrinsic/value_at_key.hpp>

namespace boost { namespace signals {

template<typename Signature, typename T>
struct get_slot<Signature, T, typename boost::enable_if<boost::fusion::traits::is_sequence<T> >::type>
{
    boost::function<Signature> operator()(const T &seq)
    {
        return get_slot<Signature, typename boost::fusion::result_of::value_at_key<T, Signature>::type>()
            (boost::fusion::at_key<Signature>(seq));
    }
};

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_SLOT_SELECTOR_MAP_HPP