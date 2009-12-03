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

namespace boost{namespace itl
{

//- equal ----------------------------------------------------------------------

#if BOOST_WORKAROUND(BOOST_MSVC, >= 1400) // 1500=MSVC-9.0 1400=MSVC-8.0; 1310=MSVC-7.1; 1300=MSVC-7.0; 
#pragma warning(push)
#pragma warning(disable:4996) //'std::equal': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'
#endif                        // I do guarantee here that I am using the parameters correctly :)


template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_equals
{
    static void apply(const SourceT& left, const SourceT& right, TargetT& target)
    {
        target = false;
        if(left.size() <= right.size())
            target = std::equal(left.begin(), left.end(), right.begin());
    }

    static void apply_elemental(const SourceT& left, const SourceT& right, TargetT& target)
    {
        target = false;
        if(left.size() <= right.size())
            target = std::equal(left.elements_begin(),  left.elements_end(), right.elements_begin()); 
    }

    static std::string struct_abbreviation(){ return "==f"; }
};

template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_equals_back
{
    static void apply(const SourceT& left, const SourceT& right, TargetT& target)
    {
        target = false;
        if(left.size() <= right.size())
            target = std::equal(left.rbegin(), left.rend(), right.rbegin());
    }

    static void apply_elemental(const SourceT& left, const SourceT& right, TargetT& target)
    {
        target = false;
        if(left.size() <= right.size())
            target = std::equal(left.elements_rbegin(),  left.elements_rend(), right.elements_rbegin()); 
    }

    static std::string struct_abbreviation(){ return "==b"; }
};

#if BOOST_WORKAROUND(BOOST_MSVC, >= 1400)
#pragma warning(pop)
#endif


//- lexicographical_compare ----------------------------------------------------

template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_lexicographical_compare
{
    static void apply(const SourceT& left, const SourceT& right, TargetT& target)
    {
        target = std::lexicographical_compare(left.begin(), left.end(), 
                                              right.begin(), right.end());
    }

    static void apply_elemental(const SourceT& left, const SourceT& right, TargetT& target)
    {
        target = std::lexicographical_compare
            (left.elements_begin(),  left.elements_end(), 
             right.elements_begin(), right.elements_end()); 
    }

    static std::string struct_abbreviation(){ return "<L_f"; }
};


template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_lexicographical_compare_back
{
    static void apply(const SourceT& left, const SourceT& right, TargetT& target)
    {
        target = std::lexicographical_compare(left.rbegin(), left.rend(), 
                                              right.rbegin(), right.rend());
    }

    static void apply_elemental(const SourceT& left, const SourceT& right, TargetT& target)
    {
        target = std::lexicographical_compare
            (left.elements_rbegin(),  left.elements_rend(), 
             right.elements_rbegin(), right.elements_rend()); 
    }

    static std::string struct_abbreviation(){ return "<L_b"; }
};


//- includes -------------------------------------------------------------------

template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_includes
{
    static void apply(const SourceT& left, const SourceT& right, TargetT& target)
    {
        target = std::includes(left.begin(), left.end(), 
                               right.begin(), right.end());
    }

    static void apply_elemental(const SourceT& left, const SourceT& right, TargetT& target)
    {
        target = std::includes(left.elements_begin(),  left.elements_end(), 
                               right.elements_begin(), right.elements_end()); 
    }

    static std::string struct_abbreviation(){ return "C=f"; }
};


template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_includes_back
{
    static void apply(const SourceT& left, const SourceT& right, TargetT& target)
    {
        target = std::includes(left.rbegin(), left.rend(), 
                               right.rbegin(), right.rend());
    }

    static void apply_elemental(const SourceT& left, const SourceT& right, TargetT& target)
    {
        target = std::includes(left.elements_rbegin(),  left.elements_rend(), 
                               right.elements_rbegin(), right.elements_rend()); 
    }

    static std::string struct_abbreviation(){ return "C=b"; }
};

}} // namespace itl boost

#endif BOOST_VALIDATE_STD_ALGORITHM_RELATION_HPP_JOFA_091202

