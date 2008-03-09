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
#ifndef DATAFLOW_TEMPLATE_UNARY_OPERATION
#error DATAFLOW_TEMPLATE_UNARY_OPERATION undefined.
#endif

template<typename Producer>
inline void DATAFLOW_TEMPLATE_UNARY_OPERATION(Producer &producer)
{
    boost::dataflow::default_unary_operation< boost::dataflow::operations::DATAFLOW_TEMPLATE_UNARY_OPERATION, DATAFLOW_TEMPLATE_MECHANISM, DATAFLOW_TEMPLATE_TAG >(producer);
}

template<typename Producer, typename Consumer>
inline void DATAFLOW_TEMPLATE_UNARY_OPERATION(const Producer &producer)
{
    boost::dataflow::default_unary_operation< boost::dataflow::operations::DATAFLOW_TEMPLATE_UNARY_OPERATION, DATAFLOW_TEMPLATE_MECHANISM, DATAFLOW_TEMPLATE_TAG >(producer);
}
