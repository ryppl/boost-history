// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_CONNECTION_PORT_MAP_HPP
#define BOOST_DATAFLOW_CONNECTION_PORT_MAP_HPP

#include <boost/dataflow/support/keyed_port.hpp>

//#include <boost/fusion/sequence/intrinsic/value_at.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/call_traits.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace dataflow {

template<typename PortCategory, typename Tag=default_tag>
struct fusion_map_port
    : public port_traits<PortCategory, concepts::keyed_port, Tag> {};

template<typename PortCategory, typename T, typename Tag=default_tag>
class port_map
{

public:
    typedef T map_type;
    
    port_map(const T& t) : t(t) {}
    map_type &map() const {return t;}
    
    typedef fusion_map_port<PortCategory, Tag> port_traits;

private:
    mutable map_type t;
};

namespace extension
{
    template<typename PortCategory, typename Tag, typename KeyPortTraits>
    struct get_keyed_port_impl<fusion_map_port<PortCategory, Tag>, KeyPortTraits>
    {
        template<typename FArgs> struct result;
        
        template<typename F, typename KeyedPort>
        struct result<F(KeyedPort &)>
        {
            typedef typename boost::fusion::result_of::at_key<
                typename KeyedPort::map_type,
                KeyPortTraits
            >::type type;
        };

        template<typename KeyedPort>
        typename result<get_keyed_port_impl(KeyedPort &)>::type
            operator()(KeyedPort &port)
        {
            return boost::fusion::at_key<
                    KeyPortTraits
                >(port.map());
        }
    };
}
    
} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_CONNECTION_PORT_MAP_HPP


