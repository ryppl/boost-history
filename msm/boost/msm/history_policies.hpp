// Copyright 2008 Christophe Henry
// henry UNDERSCORE christophe AT hotmail DOT com
// This is an extended version of the state machine available in the boost::mpl library
// Distributed under the same license as the original.
// Copyright for the original version:
// Copyright 2005 David Abrahams and Aleksey Gurtovoy. Distributed
// under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MSM_HISTORY_POLICIES_H
#define BOOST_MSM_HISTORY_POLICIES_H

#include <vector>
#include <boost/mpl/contains.hpp>

namespace boost { namespace msm
{

// policy classes

// Default: no history used
class NoHistory
{
public:
    NoHistory():m_initialStates(){}
    ~NoHistory(){}
    void set_initial_states(std::vector<int>& initial_states)
    {
	    m_initialStates = initial_states;
    }
    void history_exit(std::vector<int>& )
    {
	    // ignore
    }
    // returns the state where the state machine should be at start
	template <class Event>
    std::vector<int>& history_entry(Event const& )
    {
	    // always come back to the original state
	    return m_initialStates;
    }
private:
    std::vector<int> m_initialStates;
};

// not UML standard. Always activates history, no matter which event generated the transition
class AlwaysHistory
{
public:
    AlwaysHistory():m_currentStates(){}
    ~AlwaysHistory(){}
    void set_initial_states(std::vector<int>& initial_states)
    {
	    m_currentStates = initial_states;
    }
    void history_exit(std::vector<int>& current_states)
    {
	    m_currentStates = current_states;
    }
    // returns the state where the state machine should be at start
	template <class Event>
    std::vector<int>& history_entry(Event const& )
    {
	    // always load back the last active state
	    return m_currentStates;
    }
private:
    std::vector<int> m_currentStates;
};

// UML Shallow history. For deep history, just use this policy for all the contained state machines
template <class Events>
class ShallowHistory
{
public:
    ShallowHistory():m_initialStates(),m_currentStates(){}
    ~ShallowHistory(){}
    void set_initial_states(std::vector<int>& initial_states)
    {
	    m_currentStates = initial_states;
	    m_initialStates = initial_states;
    }
    void history_exit(std::vector<int>& current_states)
    {
	    m_currentStates = current_states;
    }
    // returns the state where the state machine should be at start
    template <class Event>
	std::vector<int>& history_entry(Event const&)
    {
        if ( ::boost::mpl::contains<Events,Event>::value)
		{
		    return m_currentStates;
	    }
	    // not one of our events, no history
	    return m_initialStates;
    }
private:
    std::vector<int>	m_initialStates;
    std::vector<int>	m_currentStates;
};

} }//boost::msm
#endif //BOOST_MSM_HISTORY_POLICIES_H
