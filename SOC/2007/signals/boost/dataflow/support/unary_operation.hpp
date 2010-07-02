// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_UNARY_OPERATION_HPP
#define BOOST_DATAFLOW_SUPPORT_UNARY_OPERATION_HPP

#include <boost/dataflow/support/port.hpp>
#include <boost/dataflow/utility/forwardable.hpp>

#include <boost/static_assert.hpp>

// ***************************************
// * unary_operation
// ***************************************
#define DATAFLOW_SPECIALIZABLE_OPERATION_NAME unary_operation
#define DATAFLOW_SPECIALIZABLE_OPERATION_CHECK is_unary_operable
#define DATAFLOW_SPECIALIZABLE_OPERATION_TYPENAME_TEMPLATES typename Operation
#define DATAFLOW_SPECIALIZABLE_OPERATION_TEMPLATES Operation
#define DATAFLOW_SPECIALIZABLE_OPERATION_ARITY 1
#define DATAFLOW_SPECIALIZABLE_OPERATION_TRAITS_OF traits_of
#define DATAFLOW_SPECIALIZABLE_OPERATION_HAS_TRAITS is_port
#include <boost/dataflow/support/detail/make_specializable_operation.hpp>


namespace boost { namespace dataflow {

template<typename Operation, typename Mechanism, typename Tag, typename Producer>
inline void
default_unary_operation(Producer &producer)
{
    extension::unary_operation_impl<
        typename default_traits_of<Producer, args::left, Mechanism, Tag>::type,
        Operation>
            ()(static_cast<typename utility::forwardable<
                    typename result_of::get_default_port<Producer, args::left, Mechanism, Tag>::type>::type >
                (get_default_port<args::left, Mechanism, Tag>(producer)));
}

template<typename Port>
inline typename enable_if<is_port<Port> >::type disconnect_all(Port &port)
{
    unary_operation<operations::disconnect_all, default_tag>(port);
}


} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_UNARY_OPERATION_HPP