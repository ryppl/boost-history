// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_UNARY_OPERATION_HPP
#define BOOST_DATAFLOW_SUPPORT_UNARY_OPERATION_HPP

#include <boost/dataflow/support/port.hpp>

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

template<typename Port>
inline void disconnect_all(Port &port)
{
    unary_operation<operations::disconnect_all, default_tag>(port);
}


} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_UNARY_OPERATION_HPP
