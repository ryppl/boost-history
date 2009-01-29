#ifndef VISITABLE_POLICIES_H
#define VISITABLE_POLICIES_H
// Copyright 2008 Christophe Henry
// henry UNDERSCORE christophe AT hotmail DOT com
// This is an extended version of the state machine available in the boost::mpl library
// Distributed under the same license as the original.
// Copyright for the original version:
// Copyright 2005 David Abrahams and Aleksey Gurtovoy. Distributed
// under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <vector>
#include <boost/function.hpp>


namespace boost { namespace msm
{
// base class for states wanting to be visited
struct VisitorBase 
{
    virtual ~VisitorBase(){}
};

 // dummy policy when visitable states are not desired
 class NotVisitableStates 
 {
 public:
     NotVisitableStates(){}
 protected:
     void fill_visitors(int number_of_states)
     {
     }
     template <class FCT>
     void insert(int index,FCT fct)
     {
     }
     void execute(int index,VisitorBase& vis)
     {
     }
 };

 // policy adding support of visitable states 
 class VisitableStates 
 {
 public:
     VisitableStates():m_state_visitors(){}
 protected:
     void fill_visitors(int number_of_states)
     {
         m_state_visitors.resize(number_of_states);
     }
     template <class FCT>
     void insert(int index,FCT fct)
     {
         m_state_visitors[index]=fct;
     }
     void execute(int index,VisitorBase& vis)
     {
        m_state_visitors[index](vis);
     }
 private:
    typedef boost::function<void (VisitorBase&)>    visitor_fct;
    typedef std::vector<visitor_fct>                visitors;
    visitors                                        m_state_visitors;
 };    
} } //boost::msm


#endif //VISITABLE_POLICIES_H
