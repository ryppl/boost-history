// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_BINARY_OPERATION_HPP
#define BOOST_DATAFLOW_SUPPORT_BINARY_OPERATION_HPP

#include <boost/dataflow/support/port.hpp>

// ***************************************
// * binary_operation, are_binary_operable
// ***************************************
#define DATAFLOW_SPECIALIZABLE_OPERATION_NAME binary_operation
#define DATAFLOW_SPECIALIZABLE_OPERATION_CHECK are_binary_operable
#define DATAFLOW_SPECIALIZABLE_OPERATION_TYPENAME_TEMPLATES typename Operation
#define DATAFLOW_SPECIALIZABLE_OPERATION_TEMPLATES Operation
#define DATAFLOW_SPECIALIZABLE_OPERATION_ARITY 2
#define DATAFLOW_SPECIALIZABLE_OPERATION_TRAITS_OF port_traits_of
#define DATAFLOW_SPECIALIZABLE_OPERATION_HAS_TRAITS is_port
#include <boost/dataflow/support/detail/make_specializable_operation.hpp>


namespace boost { namespace dataflow {

template<typename Operation, typename Mechanism, typename Tag, typename OutgoingPort, typename IncomingPort>
inline void
binary_operation(OutgoingPort &outgoing, IncomingPort &incoming)
{
    extension::binary_operation_impl<
        typename default_port_traits_of<OutgoingPort, directions::outgoing, Mechanism, Tag>::type,
        typename default_port_traits_of<IncomingPort, directions::incoming, Mechanism, Tag>::type,
        Operation>
            ()(get_default_port<directions::outgoing, Mechanism, Tag>(outgoing),
                get_default_port<directions::incoming, Mechanism, Tag>(incoming));
}

template<typename OutgoingPort, typename IncomingPort>
inline typename enable_if<
    mpl::and_<
        has_default_port<OutgoingPort, directions::outgoing, default_mechanism, default_tag>,
        has_default_port<IncomingPort, directions::incoming, default_mechanism, default_tag>
    > >::type    
connect(OutgoingPort &outgoing, IncomingPort &incoming)
{
    binary_operation<operations::connect, default_mechanism, default_tag>(outgoing, incoming);
}

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_BINARY_OPERATION_HPP
