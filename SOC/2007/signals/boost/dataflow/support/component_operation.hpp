// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_COMPONENT_OPERATION_HPP
#define BOOST_DATAFLOW_SUPPORT_COMPONENT_OPERATION_HPP

#include <boost/dataflow/support/component.hpp>


// ***************************************
// * component_operation
// ***************************************
#define DATAFLOW_SPECIALIZABLE_OPERATION_NAME component_operation
#define DATAFLOW_SPECIALIZABLE_OPERATION_CHECK is_component_operable
#define DATAFLOW_SPECIALIZABLE_OPERATION_TYPENAME_TEMPLATES typename Operation
#define DATAFLOW_SPECIALIZABLE_OPERATION_TEMPLATES Operation
#define DATAFLOW_SPECIALIZABLE_OPERATION_ARITY 1
#define DATAFLOW_SPECIALIZABLE_OPERATION_TRAITS_OF traits_of
#define DATAFLOW_SPECIALIZABLE_OPERATION_HAS_TRAITS is_component
#include <boost/dataflow/support/detail/make_specializable_operation.hpp>

namespace boost { namespace dataflow {

namespace operations
{
    struct invoke;
}

template<typename Component>
inline typename enable_if<
    dataflow::is_component<Component>,
    void
>::type invoke(Component &component)
{
    boost::dataflow::component_operation<boost::dataflow::operations::invoke, default_tag>(component);
}

} } // namespace boost::dataflow


#endif // BOOST_DATAFLOW_SUPPORT_COMPONENT_OPERATION_HPP
