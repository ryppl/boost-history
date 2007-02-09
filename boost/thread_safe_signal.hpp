/*
	A thread-safe (partial) implementation of boost.signals.

	Author: Frank Hess <frank.hess@nist.gov>
	Begin: 2007-01-23
*/
/* This software was developed at the National Institute of Standards and
 * Technology by employees of the Federal Government in the course of
 * their official duties. Pursuant to title 17 Section 105 of the United
 * States Code this software is not subject to copyright protection and is
 * in the public domain. This is an experimental system. NIST assumes no
 * responsibility whatsoever for its use by other parties, and makes no
 * guarantees, expressed or implied, about its quality, reliability, or
 * any other characteristic. We would appreciate acknowledgement if the
 * software is used.
 */
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#ifndef _EPG_SIGNALS_H
#define _EPG_SIGNALS_H

#include <algorithm>
#include <boost/function.hpp>
#include <boost/last_value.hpp>
#include <boost/preprocessor/arithmetic.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/type_traits.hpp>
#include <boost/thread_safe_signals/connection.hpp>
#include <boost/thread_safe_signals/detail/signals_common.hpp>
#include <boost/thread_safe_signals/detail/slot_groups.hpp>
#include <boost/thread_safe_signals/detail/slot_call_iterator.hpp>
#include <functional>
#include <list>

#ifndef EPG_SIGNALS_MAX_ARGS
#define EPG_SIGNALS_MAX_ARGS 10
#endif

#define BOOST_PP_ITERATION_LIMITS (0, EPG_SIGNALS_MAX_ARGS)
#define BOOST_PP_FILENAME_1 <boost/thread_safe_signals/detail/signal_template.hpp>
#include BOOST_PP_ITERATE()

namespace EPG
{
	template<typename Signature,
		typename Combiner = boost::last_value<typename boost::function_traits<Signature>::result_type >,
		typename Group = int,
		typename GroupCompare = std::less<Group>,
		typename SlotFunction = boost::function<Signature> >
	class signal: public ::EPG::signalslib::detail::SignalN<boost::function_traits<Signature>::arity,
		Signature, Combiner, Group, GroupCompare, SlotFunction>::type
	{
	private:
		typedef typename signalslib::detail::SignalN<boost::function_traits<Signature>::arity,
			Signature, Combiner, Group, GroupCompare, SlotFunction>::type base_type;
	public:
		signal(const Combiner &combiner = Combiner(), const GroupCompare &group_compare = GroupCompare()):
			base_type(combiner, group_compare)
		{}
	};
}

#endif	// _EPG_SIGNALS_H
