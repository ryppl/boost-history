#ifndef BOOST_FSM_RESULT_HPP_INCLUDED
#define BOOST_FSM_RESULT_HPP_INCLUDED
//////////////////////////////////////////////////////////////////////////////
// Copyright (c) Andreas Huber Doenni 2002-2004.
// Use, modification and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////



namespace boost
{
namespace fsm
{



//////////////////////////////////////////////////////////////////////////////
enum result
{
  no_reaction,
  do_forward_event,
  do_discard_event,
  do_defer_event
};



} // namespace fsm
} // namespace boost



#endif
