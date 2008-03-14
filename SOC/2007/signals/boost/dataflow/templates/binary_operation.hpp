// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef DATAFLOW_TEMPLATE_TAG
#error DATAFLOW_TEMPLATE_TAG undefined.
#endif
#ifndef DATAFLOW_TEMPLATE_MECHANISM
#error DATAFLOW_TEMPLATE_MECHANISM undefined.
#endif
#ifndef DATAFLOW_TEMPLATE_BINARY_OPERATION
#error DATAFLOW_TEMPLATE_BINARY_OPERATION undefined.
#endif

template<typename Producer, typename Consumer>
inline typename boost::dataflow::result_of::default_binary_operation<
    Producer, Consumer, 
    boost::dataflow::operations::DATAFLOW_TEMPLATE_BINARY_OPERATION, DATAFLOW_TEMPLATE_MECHANISM, DATAFLOW_TEMPLATE_TAG
>::type
    DATAFLOW_TEMPLATE_BINARY_OPERATION(Producer &producer, Consumer &consumer)
{
    return boost::dataflow::default_binary_operation< boost::dataflow::operations::DATAFLOW_TEMPLATE_BINARY_OPERATION, DATAFLOW_TEMPLATE_MECHANISM, DATAFLOW_TEMPLATE_TAG >(producer, consumer);
}

template<typename Producer, typename Consumer>
inline typename boost::dataflow::result_of::default_binary_operation<
    Producer, const Consumer, 
    boost::dataflow::operations::DATAFLOW_TEMPLATE_BINARY_OPERATION, DATAFLOW_TEMPLATE_MECHANISM, DATAFLOW_TEMPLATE_TAG
>::type
     DATAFLOW_TEMPLATE_BINARY_OPERATION(Producer &producer, const Consumer &consumer)
{
    return boost::dataflow::default_binary_operation< boost::dataflow::operations::DATAFLOW_TEMPLATE_BINARY_OPERATION, DATAFLOW_TEMPLATE_MECHANISM, DATAFLOW_TEMPLATE_TAG >(producer, consumer);
}

template<typename Producer, typename Consumer>
inline typename boost::dataflow::result_of::default_binary_operation<
    const Producer, Consumer, 
    boost::dataflow::operations::DATAFLOW_TEMPLATE_BINARY_OPERATION, DATAFLOW_TEMPLATE_MECHANISM, DATAFLOW_TEMPLATE_TAG
>::type
     DATAFLOW_TEMPLATE_BINARY_OPERATION(const Producer &producer, Consumer &consumer)
{
    return boost::dataflow::default_binary_operation< boost::dataflow::operations::DATAFLOW_TEMPLATE_BINARY_OPERATION, DATAFLOW_TEMPLATE_MECHANISM, DATAFLOW_TEMPLATE_TAG >(producer, consumer);
}

template<typename Producer, typename Consumer>
inline typename boost::dataflow::result_of::default_binary_operation<
    const Producer, const Consumer, 
    boost::dataflow::operations::DATAFLOW_TEMPLATE_BINARY_OPERATION, DATAFLOW_TEMPLATE_MECHANISM, DATAFLOW_TEMPLATE_TAG
>::type
     DATAFLOW_TEMPLATE_BINARY_OPERATION(const Producer &producer, const Consumer &consumer)
{
    return boost::dataflow::default_binary_operation< boost::dataflow::operations::DATAFLOW_TEMPLATE_BINARY_OPERATION, DATAFLOW_TEMPLATE_MECHANISM, DATAFLOW_TEMPLATE_TAG >(producer, consumer);
}
