// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_BINARY_OPERATION_HPP
#define BOOST_DATAFLOW_SUPPORT_BINARY_OPERATION_HPP

#include <boost/dataflow/support/port.hpp>
#include <boost/call_traits.hpp>
#include <boost/dataflow/utility/forwardable.hpp>

// ***************************************
// * binary_operation, are_binary_operable
// ***************************************
#define DATAFLOW_SPECIALIZABLE_OPERATION_NAME binary_operation
#define DATAFLOW_SPECIALIZABLE_OPERATION_CHECK are_binary_operable
#define DATAFLOW_SPECIALIZABLE_OPERATION_TYPENAME_TEMPLATES typename Operation
#define DATAFLOW_SPECIALIZABLE_OPERATION_TEMPLATES Operation
#define DATAFLOW_SPECIALIZABLE_OPERATION_ARITY 2
#define DATAFLOW_SPECIALIZABLE_OPERATION_TRAITS_OF traits_of
#define DATAFLOW_SPECIALIZABLE_OPERATION_HAS_TRAITS is_port
#include <boost/dataflow/support/detail/make_specializable_operation.hpp>


namespace boost { namespace dataflow {

template<typename Operation, typename Mechanism, typename Tag, typename OutgoingPort, typename IncomingPort>
inline void
default_binary_operation(OutgoingPort &left, IncomingPort &right)
{
    extension::binary_operation_impl<
        typename default_traits_of<OutgoingPort, args::left, Mechanism, Tag>::type,
        typename default_traits_of<IncomingPort, args::right, Mechanism, Tag>::type,
        Operation>
            ()(static_cast<typename utility::forwardable<
                    typename result_of::get_default_port<OutgoingPort, args::left, Mechanism, Tag>::type>::type >
                (get_default_port<args::left, Mechanism, Tag>(left)),
               static_cast<typename utility::forwardable<
                    typename result_of::get_default_port<IncomingPort, args::right, Mechanism, Tag>::type>::type >
                (get_default_port<args::right, Mechanism, Tag>(right)));
}

template<typename OutgoingPort, typename IncomingPort>
inline typename enable_if<
    mpl::and_<
        has_default_port<OutgoingPort, args::left, default_mechanism, default_tag>,
        has_default_port<IncomingPort, args::right, default_mechanism, default_tag>
    > >::type    
connect(OutgoingPort &left, IncomingPort &right)
{
    default_binary_operation<operations::connect, default_mechanism, default_tag>(left, right);
}

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_BINARY_OPERATION_HPP
