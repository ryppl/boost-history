// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef DATAFLOW_TEMPLATE_MECHANISM
#error DATAFLOW_TEMPLATE_MECHANISM undefined.
#endif
#ifndef DATAFLOW_TEMPLATE_BINARY_OPERATION
#error DATAFLOW_TEMPLATE_BINARY_OPERATION undefined.
#endif

template<typename Producer, typename Consumer>
inline void DATAFLOW_TEMPLATE_BINARY_OPERATION(Producer &producer, Consumer &consumer)
{
    boost::dataflow::binary_operation< boost::dataflow::operations::DATAFLOW_TEMPLATE_BINARY_OPERATION, DATAFLOW_TEMPLATE_MECHANISM >(producer, consumer);
}

template<typename Producer, typename Consumer>
inline void DATAFLOW_TEMPLATE_BINARY_OPERATION(Producer &producer, const Consumer &consumer)
{
    boost::dataflow::binary_operation< boost::dataflow::operations::DATAFLOW_TEMPLATE_BINARY_OPERATION, DATAFLOW_TEMPLATE_MECHANISM >(producer, consumer);
}

template<typename Producer, typename Consumer>
inline void DATAFLOW_TEMPLATE_BINARY_OPERATION(const Producer &producer, Consumer &consumer)
{
    boost::dataflow::binary_operation< boost::dataflow::operations::DATAFLOW_TEMPLATE_BINARY_OPERATION, DATAFLOW_TEMPLATE_MECHANISM >(producer, consumer);
}

template<typename Producer, typename Consumer>
inline void DATAFLOW_TEMPLATE_BINARY_OPERATION(const Producer &producer, const Consumer &consumer)
{
    boost::dataflow::binary_operation< boost::dataflow::operations::DATAFLOW_TEMPLATE_BINARY_OPERATION, DATAFLOW_TEMPLATE_MECHANISM >(producer, consumer);
}
