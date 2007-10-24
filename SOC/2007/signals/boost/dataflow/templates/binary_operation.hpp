// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// DO NOT INCLUDE THIS FILE DIRECTLY

#ifndef DATAFLOW_TEMPLATE_MECHANISM
#error DATAFLOW_TEMPLATE_MECHANISM undefined.  Do not include this file directly.
#endif

template<typename Producer, typename Consumer>
inline void connect(Producer &producer, Consumer &consumer)
{
    boost::dataflow::connect< DATAFLOW_TEMPLATE_MECHANISM >(producer, consumer);
}

template<typename Producer, typename Consumer>
inline void connect_only(Producer &producer, Consumer &consumer)
{
    boost::dataflow::connect_only< DATAFLOW_TEMPLATE_MECHANISM >(producer, consumer);
}