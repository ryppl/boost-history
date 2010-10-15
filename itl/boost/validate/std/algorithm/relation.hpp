/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_VALIDATE_STD_ALGORITHM_RELATION_HPP_JOFA_091202
#define BOOST_VALIDATE_STD_ALGORITHM_RELATION_HPP_JOFA_091202

#include <algorithm>

namespace boost{namespace icl
{

//- equal ----------------------------------------------------------------------

#ifdef BOOST_MSVC 
#pragma warning(push)
#pragma warning(disable:4996) //'std::equal': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'
#endif                        // I do guarantee here that I am using the parameters correctly :)


template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_equals
{
    typedef typename SourceT::atomized_type AtomicT;

    void operator()(const AtomicT& left, const AtomicT& right, TargetT& target)
    {
        target = false;
        if(left.size() <= right.size())
            target = std::equal(left.begin(), left.end(), right.begin());
    }

    void operator()(const SourceT& left, const SourceT& right, TargetT& target)
    {
        target = false;
        if(left.size() <= right.size())
            target = std::equal(elements_begin(left),  elements_end(left), elements_begin(right)); 
    }

    static std::string struct_abbreviation(){ return "==f"; }
};

template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_equals_back
{
    typedef typename SourceT::atomized_type AtomicT;

    void operator()(const AtomicT& left, const AtomicT& right, TargetT& target)
    {
        target = false;
        if(left.size() <= right.size())
            target = std::equal(left.rbegin(), left.rend(), right.rbegin());
    }

    void operator()(const SourceT& left, const SourceT& right, TargetT& target)
    {
        target = false;
        if(left.size() <= right.size())
            target = std::equal(elements_rbegin(left),  elements_rend(left), elements_rbegin(right)); 
    }

    static std::string struct_abbreviation(){ return "==b"; }
};

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif


//- lexicographical_compare ----------------------------------------------------

template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_lexicographical_compare
{
    typedef typename SourceT::atomized_type AtomicT;

    void operator()(const AtomicT& left, const AtomicT& right, TargetT& target)
    {
        target = std::lexicographical_compare(left.begin(), left.end(), 
                                              right.begin(), right.end());
    }

    void operator()(const SourceT& left, const SourceT& right, TargetT& target)
    {
        target = std::lexicographical_compare
            (elements_begin(left),  elements_end(left), 
             elements_begin(right), elements_end(right)); 
    }

    static std::string struct_abbreviation(){ return "<L_f"; }
};


template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_lexicographical_compare_back
{
    typedef typename SourceT::atomized_type AtomicT;

    void operator()(const AtomicT& left, const AtomicT& right, TargetT& target)
    {
        target = std::lexicographical_compare(left.rbegin(), left.rend(), 
                                              right.rbegin(), right.rend());
    }

    void operator()(const SourceT& left, const SourceT& right, TargetT& target)
    {
        target = std::lexicographical_compare
            (elements_rbegin(left),  elements_rend(left), 
             elements_rbegin(right), elements_rend(right)); 
    }

    static std::string struct_abbreviation(){ return "<L_b"; }
};


//- includes -------------------------------------------------------------------

template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_includes
{
    typedef typename SourceT::atomized_type AtomicT;

    void operator()(const AtomicT& left, const AtomicT& right, TargetT& target)
    {
        target = std::includes(left.begin(), left.end(), 
                               right.begin(), right.end());
    }

    void operator()(const SourceT& left, const SourceT& right, TargetT& target)
    {
        target = std::includes(elements_begin(left),  elements_end(left), 
                               elements_begin(right), elements_end(right)); 
    }

    static std::string struct_abbreviation(){ return "C=f"; }
};


template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_includes_back
{
    typedef typename SourceT::atomized_type AtomicT;

    void operator()(const AtomicT& left, const AtomicT& right, TargetT& target)
    {
        target = std::includes(left.rbegin(), left.rend(), 
                               right.rbegin(), right.rend());
    }

    void operator()(const SourceT& left, const SourceT& right, TargetT& target)
    {
        target = std::includes(elements_rbegin(left),  elements_rend(left), 
                               elements_rbegin(right), elements_rend(right)); 
    }

    static std::string struct_abbreviation(){ return "C=b"; }
};

}} // namespace icl boost

#endif // BOOST_VALIDATE_STD_ALGORITHM_RELATION_HPP_JOFA_091202

