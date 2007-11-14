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
#ifndef DATAFLOW_TEMPLATE_BINARY_OPERATION
#error DATAFLOW_TEMPLATE_OPERATOR undefined.
#endif

template<typename Producer, typename Consumer>
typename boost::enable_if<
    boost::mpl::and_<
        boost::dataflow::is_port<DATAFLOW_TEMPLATE_MECHANISM, boost::dataflow::ports::producer, Producer>,
        boost::dataflow::is_port<DATAFLOW_TEMPLATE_MECHANISM, boost::dataflow::ports::consumer, Consumer>
    >,
    Producer &
>::type
operator DATAFLOW_TEMPLATE_OPERATOR (Producer &producer, Consumer &consumer)
{ DATAFLOW_TEMPLATE_BINARY_OPERATION(producer, consumer); return producer;}

template<typename Producer, typename Consumer>
typename boost::enable_if<
    boost::mpl::and_<
        boost::dataflow::is_port<DATAFLOW_TEMPLATE_MECHANISM, boost::dataflow::ports::producer, Producer>,
        boost::dataflow::is_port<DATAFLOW_TEMPLATE_MECHANISM, boost::dataflow::ports::consumer, Consumer>
    >,
    Producer &
>::type
operator DATAFLOW_TEMPLATE_OPERATOR (Producer &producer, const Consumer &consumer)
{ DATAFLOW_TEMPLATE_BINARY_OPERATION(producer, consumer); return producer;}

template<typename Producer, typename Consumer>
typename boost::enable_if<
    boost::mpl::and_<
        boost::dataflow::is_port<DATAFLOW_TEMPLATE_MECHANISM, boost::dataflow::ports::producer, Producer>,
        boost::dataflow::is_port<DATAFLOW_TEMPLATE_MECHANISM, boost::dataflow::ports::consumer, Consumer>
    >,
    const Producer &
>::type
operator DATAFLOW_TEMPLATE_OPERATOR (const Producer &producer, Consumer &consumer)
{ DATAFLOW_TEMPLATE_BINARY_OPERATION(producer, consumer); return producer;}

template<typename Producer, typename Consumer>
typename boost::enable_if<
    boost::mpl::and_<
        boost::dataflow::is_port<DATAFLOW_TEMPLATE_MECHANISM, boost::dataflow::ports::producer, Producer>,
        boost::dataflow::is_port<DATAFLOW_TEMPLATE_MECHANISM, boost::dataflow::ports::consumer, Consumer>
    >,
    const Producer &
>::type
operator DATAFLOW_TEMPLATE_OPERATOR (const Producer &producer, const Consumer &consumer)
{ DATAFLOW_TEMPLATE_BINARY_OPERATION(producer, consumer); return producer;}
