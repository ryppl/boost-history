// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_VECTOR_PORT_HPP
#define BOOST_DATAFLOW_SUPPORT_VECTOR_PORT_HPP

#include <boost/dataflow/detail/transform_remove_reference.hpp>
#include <boost/dataflow/support/fusion_port_vector.hpp>
#include <boost/dataflow/support/vector_port.hpp>

#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/type_traits/is_same.hpp>


namespace boost { namespace dataflow {

template<typename PortCategory, typename PortSequence, typename Tag=default_tag>
struct fusion_vector_port_traits
    : public port_traits<PortCategory, Tag>
{
    typename detail::transform_remove_reference<PortSequence>::type ports;
    /// MPL Sequence of result types for get_port
    typedef PortSequence fusion_ports;
    
    template<typename VectorPort>
    static fusion_ports & get_ports(const VectorPort &vector_port)
    {   return vector_port.t; }
};

template<typename PortCategory, typename T, typename Tag=default_tag>
class fusion_vector_port
{
public:
    typedef fusion_vector_port_traits<PortCategory, T, Tag> dataflow_traits;
    typedef T vector_type;
    
    fusion_vector_port(const T& t) : t(t) {}
    
private:
    mutable T t;
    
    friend struct fusion_vector_port_traits<PortCategory, T, Tag>;
};

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_VECTOR_PORT_HPP

