// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef DATAFLOW_SIGNALS_RUNTIME_SUPPORT_HPP
#define DATAFLOW_SIGNALS_RUNTIME_SUPPORT_HPP

#include <boost/dataflow/support/runtime.hpp>
#include <boost/dataflow/signals/support.hpp>

DATAFLOW_RUNTIME_PROPERTY(signals::tag, const char *, "Dataflow.Signals")
DATAFLOW_RUNTIME_PROPERTY(signals::tag, int, 10)

#endif // DATAFLOW_SIGNALS_RUNTIME_SUPPORT_HPP
