// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_TAGS_HPP
#define BOOST_DATAFLOW_SUPPORT_TAGS_HPP


namespace boost { namespace dataflow {

/// The default Tag argument for Dataflow templates.
struct default_tag {};
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

namespace concepts
{
    /// PortConcept type specifying the Port concept.
    struct port {};
    /// PortConcept tag specifying the KeyedPort concept.
    struct keyed_port : public port {};
    /// PortConcept tag specifying the ComplementedPort concept.
    struct complemented_port : public port {};
    /// ComponentConcept tag specifying the Component concept.
    struct component {};
    /// ComponentConcept tag specifying the FusionComponent concept.
    struct fusion_component : public component {};
}

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

namespace directions {
    /// Tag specifying the outgoing direction in binary operations.
    struct outgoing {};
    /// Tag specifying the incoming direction in binary operations.
    struct incoming {};
}

} } // namespace boost::dataflow


#endif // BOOST_DATAFLOW_SUPPORT_TAGS_HPP