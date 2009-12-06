/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_VALIDATE_STD_ALGORITHM_SET_ALGO_HPP_JOFA_091125
#define BOOST_VALIDATE_STD_ALGORITHM_SET_ALGO_HPP_JOFA_091125

#include <algorithm>

namespace boost{namespace itl
{


//- set_union -----------------------------------------------------------------

template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_set_union
{
    typedef typename SourceT::atomized_type AtomicT;

    void operator()(const AtomicT& left, const AtomicT& right, TargetT& target)
    {
        std::set_union(left.begin(), left.end(), right.begin(), right.end(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
    }

    void operator()(const SourceT& left, const SourceT& right, TargetT& target)
    {
        std::set_union(
            left.elements_begin(),  left.elements_end(), 
            right.elements_begin(), right.elements_end(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end()))
        );
    }

    static std::string struct_abbreviation(){ return "+f"; }
};


template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_set_union_back
{
    typedef typename SourceT::atomized_type AtomicT;

    void operator()(const AtomicT& left, const AtomicT& right, TargetT& target)
    {
        std::set_union(left.rbegin(), left.rend(), right.rbegin(), right.rend(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
    }

    void operator()(const SourceT& left, const SourceT& right, TargetT& target)
    {
        std::set_union(
            left.elements_rbegin(),  left.elements_rend(), 
            right.elements_rbegin(), right.elements_rend(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end()))
        );
    }

    static std::string struct_abbreviation(){ return "+b"; }
};


//- set_difference ------------------------------------------------------------

template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_set_difference
{
    typedef typename SourceT::atomized_type AtomicT;

    void operator()(const AtomicT& left, const AtomicT& right, TargetT& target)
    {
        std::set_difference(left.begin(), left.end(), right.begin(), right.end(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
    }

    void operator()(const SourceT& left, const SourceT& right, TargetT& target)
    {
        std::set_difference(
            left.elements_begin(),  left.elements_end(), 
            right.elements_begin(), right.elements_end(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end()))
        );
    }

    static std::string struct_abbreviation(){ return "-f"; }
};


template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_set_difference_back
{
    typedef typename SourceT::atomized_type AtomicT;

    void operator()(const AtomicT& left, const AtomicT& right, TargetT& target)
    {
        std::set_difference(left.rbegin(), left.rend(), right.rbegin(), right.rend(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
    }

    void operator()(const SourceT& left, const SourceT& right, TargetT& target)
    {
        std::set_difference(
            left.elements_rbegin(),  left.elements_rend(), 
            right.elements_rbegin(), right.elements_rend(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end()))
        );
    }

    static std::string struct_abbreviation(){ return "-b"; }
};


//- set_intersection -----------------------------------------------------------

template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_set_intersection
{
    typedef typename SourceT::atomized_type AtomicT;

    void operator()(const AtomicT& left, const AtomicT& right, TargetT& target)
    {
        std::set_intersection(left.begin(), left.end(), right.begin(), right.end(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
    }

    void operator()(const SourceT& left, const SourceT& right, TargetT& target)
    {
        std::set_intersection(
            left.elements_begin(),  left.elements_end(), 
            right.elements_begin(), right.elements_end(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end()))
        );
    }

    static std::string struct_abbreviation(){ return "^f"; }
};


template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_set_intersection_back
{
    typedef typename SourceT::atomized_type AtomicT;

    void operator()(const AtomicT& left, const AtomicT& right, TargetT& target)
    {
        std::set_intersection(left.rbegin(), left.rend(), right.rbegin(), right.rend(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
    }

    void operator()(const SourceT& left, const SourceT& right, TargetT& target)
    {
        std::set_intersection(
            left.elements_rbegin(),  left.elements_rend(), 
            right.elements_rbegin(), right.elements_rend(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end()))
        );
    }

    static std::string struct_abbreviation(){ return "^b"; }
};


//- set_symmetric_difference ---------------------------------------------------

template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_set_symmetric_difference
{
    typedef typename SourceT::atomized_type AtomicT;

    void operator()(const AtomicT& left, const AtomicT& right, TargetT& target)
    {
        std::set_symmetric_difference(left.begin(), left.end(), right.begin(), right.end(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
    }

    void operator()(const SourceT& left, const SourceT& right, TargetT& target)
    {
        std::set_symmetric_difference(
            left.elements_begin(),  left.elements_end(), 
            right.elements_begin(), right.elements_end(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end()))
        );
    }

    static std::string struct_abbreviation(){ return "&f"; }
};


template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_set_symmetric_difference_back
{
    typedef typename SourceT::atomized_type AtomicT;

    void operator()(const AtomicT& left, const AtomicT& right, TargetT& target)
    {
        std::set_symmetric_difference(left.rbegin(), left.rend(), right.rbegin(), right.rend(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
    }

    void operator()(const SourceT& left, const SourceT& right, TargetT& target)
    {
        std::set_symmetric_difference(
            left.elements_rbegin(),  left.elements_rend(), 
            right.elements_rbegin(), right.elements_rend(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end()))
        );
    }

    static std::string struct_abbreviation(){ return "&b"; }
};


}} // namespace itl boost

#endif // BOOST_VALIDATE_STD_ALGORITHM_SET_ALGO_HPP_JOFA_091125

