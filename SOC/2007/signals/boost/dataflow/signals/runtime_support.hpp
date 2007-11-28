// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef DATAFLOW_SIGNALS_RUNTIME_SUPPORT_HPP
#define DATAFLOW_SIGNALS_RUNTIME_SUPPORT_HPP

#include <boost/dataflow/support/runtime.hpp>
#include <boost/dataflow/signals/support.hpp>

DATAFLOW_RUNTIME_PROPERTY(signals::mechanism, const char *, "signals connection")
DATAFLOW_RUNTIME_PROPERTY(signals::mechanism, int, 10)
DATAFLOW_RUNTIME_PROPERTY(signals::extract_mechanism, const char *, "signals extraction")
DATAFLOW_RUNTIME_PROPERTY(signals::extract_mechanism, int, 11)

#endif // DATAFLOW_SIGNALS_RUNTIME_SUPPORT_HPP