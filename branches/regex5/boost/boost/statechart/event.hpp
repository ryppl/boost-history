#ifndef BOOST_FSM_EVENT_HPP_INCLUDED
#define BOOST_FSM_EVENT_HPP_INCLUDED
//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2002-2003 Andreas Huber Doenni, Switzerland
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//////////////////////////////////////////////////////////////////////////////



#include <boost/fsm/event_base.hpp>
#include <boost/fsm/detail/rtti_policy.hpp>



namespace boost
{
namespace fsm
{



//////////////////////////////////////////////////////////////////////////////
template< class MostDerived >
class event : public detail::rtti_policy::derived_type<
  MostDerived, event_base >
{
  protected:
    //////////////////////////////////////////////////////////////////////////
    event() {}
};



} // namespace fsm
} // namespace boost



#endif
