/*
	A thread-safe (partial) implementation of boost.signals.

	Author: Frank Mori Hess <fmhess@users.sourceforge.net>
	Begin: 2007-01-23
*/
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#ifndef _THREAD_SAFE_SIGNAL_HPP
#define _THREAD_SAFE_SIGNAL_HPP

#define BOOST_SIGNALS_NAMESPACE signalslib

#include <algorithm>
#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <boost/function.hpp>
#include <boost/last_value.hpp>
#include <boost/preprocessor/arithmetic.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits.hpp>
#include <boost/thread_safe_signals/detail/signals_common.hpp>
#include <boost/thread_safe_signals/detail/signals_common_macros.hpp>
#include <boost/thread_safe_signals/detail/slot_groups.hpp>
#include <boost/thread_safe_signals/detail/slot_call_iterator.hpp>
#include <boost/thread_safe_signals/auto_threaded.hpp>
#include <boost/thread_safe_signals/connection.hpp>
#include <boost/thread_safe_signals/shared_connection_block.hpp>
#include <boost/thread_safe_signals/single_threaded.hpp>
#include <boost/thread_safe_signals/slot.hpp>
#include <functional>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

#define BOOST_PP_ITERATION_LIMITS (0, BOOST_SIGNALS_MAX_ARGS)
#define BOOST_PP_FILENAME_1 <boost/thread_safe_signals/detail/signal_template.hpp>
#include BOOST_PP_ITERATE()

namespace boost
{
#ifndef signals
  // for backward compatibility
  namespace signals = signalslib;
#endif
  template<typename Signature,
    typename Combiner = last_value<typename boost::function_traits<Signature>::result_type>,
    typename Group = int,
    typename GroupCompare = std::less<Group>,
    typename SlotFunction = function<Signature>,
    typename ThreadingModel = signalslib::auto_threaded >
  class signal: public signalslib::detail::signalN<function_traits<Signature>::arity,
    Signature, Combiner, Group, GroupCompare, SlotFunction, ThreadingModel>::type
  {
  private:
    typedef typename signalslib::detail::signalN<boost::function_traits<Signature>::arity,
      Signature, Combiner, Group, GroupCompare, SlotFunction, ThreadingModel>::type base_type;
  public:
    signal(const Combiner &combiner = Combiner(), const GroupCompare &group_compare = GroupCompare()):
      base_type(combiner, group_compare)
    {}
  };
}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif	// _THREAD_SAFE_SIGNAL_HPP
