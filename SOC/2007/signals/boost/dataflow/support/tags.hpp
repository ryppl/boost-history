// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_TAGS_HPP
#define BOOST_DATAFLOW_SUPPORT_TAGS_HPP


namespace boost { namespace dataflow {

/// PortCategory types.
namespace ports
{
    struct producer;
    struct consumer;

    struct producer
    {
        typedef consumer complement;
    };

    struct consumer
    {
        typedef producer complement;
    };
}

namespace concepts
{
    /// Tag
    struct port {};
    struct keyed_port {};
    struct complemented_port {};
}

namespace operations {
    struct connect;
    struct connect_only;
    struct disconnect;
    struct extract;
}

} } // namespace boost::dataflow


#endif // BOOST_DATAFLOW_SUPPORT_TAGS_HPP