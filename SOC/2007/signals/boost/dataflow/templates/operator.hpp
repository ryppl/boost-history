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
#ifndef DATAFLOW_TEMPLATE_BINARY_OPERATION
#error DATAFLOW_TEMPLATE_OPERATOR undefined.
#endif
    
template<typename OutgoingPort, typename IncomingPort>
inline typename boost::enable_if<
    boost::mpl::and_<
        boost::dataflow::has_default_port<OutgoingPort, boost::dataflow::args::left, DATAFLOW_TEMPLATE_MECHANISM, DATAFLOW_TEMPLATE_TAG>,
        boost::dataflow::has_default_port<IncomingPort, boost::dataflow::args::right, DATAFLOW_TEMPLATE_MECHANISM, DATAFLOW_TEMPLATE_TAG>
    >, OutgoingPort &
>::type
operator DATAFLOW_TEMPLATE_OPERATOR (OutgoingPort &left, IncomingPort &right)
{ DATAFLOW_TEMPLATE_BINARY_OPERATION(left, right); return left;}

template<typename OutgoingPort, typename IncomingPort>
inline typename boost::enable_if<
    boost::mpl::and_<
        boost::dataflow::has_default_port<OutgoingPort, boost::dataflow::args::left, DATAFLOW_TEMPLATE_MECHANISM, DATAFLOW_TEMPLATE_TAG>,
        boost::dataflow::has_default_port<IncomingPort, boost::dataflow::args::right, DATAFLOW_TEMPLATE_MECHANISM, DATAFLOW_TEMPLATE_TAG>
    >, OutgoingPort &
>::type
operator DATAFLOW_TEMPLATE_OPERATOR (OutgoingPort &left, const IncomingPort &right)
{ DATAFLOW_TEMPLATE_BINARY_OPERATION(left, right); return left;}

template<typename OutgoingPort, typename IncomingPort>
inline typename boost::enable_if<
    boost::mpl::and_<
        boost::dataflow::has_default_port<OutgoingPort, boost::dataflow::args::left, DATAFLOW_TEMPLATE_MECHANISM, DATAFLOW_TEMPLATE_TAG>,
        boost::dataflow::has_default_port<IncomingPort, boost::dataflow::args::right, DATAFLOW_TEMPLATE_MECHANISM, DATAFLOW_TEMPLATE_TAG>
    >, const OutgoingPort &
>::type
operator DATAFLOW_TEMPLATE_OPERATOR (const OutgoingPort &left, IncomingPort &right)
{ DATAFLOW_TEMPLATE_BINARY_OPERATION(left, right); return left;}

template<typename OutgoingPort, typename IncomingPort>
inline typename boost::enable_if<
    boost::mpl::and_<
        boost::dataflow::has_default_port<OutgoingPort, boost::dataflow::args::left, DATAFLOW_TEMPLATE_MECHANISM, DATAFLOW_TEMPLATE_TAG>,
        boost::dataflow::has_default_port<IncomingPort, boost::dataflow::args::right, DATAFLOW_TEMPLATE_MECHANISM, DATAFLOW_TEMPLATE_TAG>
    >, const OutgoingPort &
>::type
operator DATAFLOW_TEMPLATE_OPERATOR (const OutgoingPort &left, const IncomingPort &right)
{ DATAFLOW_TEMPLATE_BINARY_OPERATION(left, right); return left;}
