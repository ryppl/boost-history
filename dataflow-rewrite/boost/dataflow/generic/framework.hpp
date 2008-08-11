// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_TAGS_HPP
#define BOOST_DATAFLOW_SUPPORT_TAGS_HPP

#include <boost/type_traits/is_base_of.hpp>

namespace boost { namespace dataflow {

/// The default Tag argument for Dataflow templates.
struct tag {};

/// The default Tag argument for Dataflow templates.
struct default_tag : tag {};

/// Boolean metafunction determining whether a type is a Tag.
template<typename T>
struct is_tag
    : public is_base_of<tag, T> {};

/// The default Mechanism argument for Dataflow templates.
struct default_mechanism {};

/// PortCategory types.
namespace ports
{
    struct producer;
    struct consumer;

    /// producer PortCategory.
    struct producer
    {
        typedef consumer complement; ///< complement PortCategory type.
    };

    /// consumer PortCategory.
    struct consumer
    {
        typedef producer complement; ///< complement PortCategory type.
    };
}

/*namespace concepts
{
    struct entity {};
    
    struct port_vector : public entity {};
    struct fusion_port_vector : public port_vector {};
    
    /// PortConcept type specifying the Port concept.
    struct port : public entity {};
    /// PortConcept tag specifying the KeyedPort concept.
    struct vector_port : public port {};
    struct fusion_vector_port : public port, public fusion_port_vector {};
    /// PortConcept tag specifying the KeyedPort concept.
    struct keyed_port : public vector_port {};
    /// PortConcept tag specifying the ComplementedPort concept.
    struct complemented_port : public port {};
    /// ComponentConcept tag specifying the Component concept.
    struct component : port_vector {};
    /// ComponentConcept tag specifying the FusionComponent concept.
    struct fusion_component : public component, public fusion_port_vector {};
}

template<typename T>
struct is_concept
    : public is_base_of<concepts::entity, T> {};*/


namespace operations {
    /// Tag specifying the connect binary operation.
    struct connect {};
    /// Tag specifying the connect_only binary operation.
    struct connect_only {};
    /// Tag specifying the disconnect binary operation.
    struct disconnect {};
    /// Tag specifying the extract binary operation.
    struct extract {};

    /// Tag specifying the disconnect_all unary port operation.
    struct disconnect_all {};
}

namespace args {
    /// Tag specifying the left direction in binary operations.
    struct left {};
    /// Tag specifying the right direction in binary operations.
    struct right {};
}

} } // namespace boost::dataflow


#endif // BOOST_DATAFLOW_SUPPORT_TAGS_HPP
