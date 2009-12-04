/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_VALIDATE_STD_ALGORITHM_COPY_HPP_JOFA_091124
#define BOOST_VALIDATE_STD_ALGORITHM_COPY_HPP_JOFA_091124

#include <algorithm>

namespace boost{namespace itl
{

//------------------------------------------------------------------------------
//- std::copy ------------------------------------------------------------------
template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_copy
{
    static void apply(const SourceT& source, TargetT& target)
    {
        std::copy(source.begin(), source.end(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
    }

    static void apply_elemental(const SourceT& source, TargetT& target)
    {
        std::copy(source.elements_begin(), source.elements_end(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
    }

    static std::string struct_abbreviation(){ return "cpy_f"; }
};

template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_copy_back
{
    static void apply(const SourceT& source, TargetT& target)
    {
        std::copy(source.rbegin(), source.rend(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
    }

    static void apply_elemental(const SourceT& source, TargetT& target)
    {
        std::copy(source.elements_rbegin(), source.elements_rend(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
    }

    static std::string struct_abbreviation(){ return "cpy_b"; }
};

//------------------------------------------------------------------------------
//- std::reverse_copy ----------------------------------------------------------
template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_reverse_copy
{
    static void apply(const SourceT& source, TargetT& target)
    {
        std::reverse_copy(source.begin(), source.end(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
    }

    static void apply_elemental(const SourceT& source, TargetT& target)
    {
        std::reverse_copy(source.elements_begin(), source.elements_end(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
    }

    static std::string struct_abbreviation(){ return "rcpy_f"; }
};

template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_reverse_copy_back
{
    static void apply(const SourceT& source, TargetT& target)
    {
        std::reverse_copy(source.rbegin(), source.rend(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
    }

    static void apply_elemental(const SourceT& source, TargetT& target)
    {
        std::reverse_copy(source.elements_rbegin(), source.elements_rend(), 
            InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
    }

    static std::string struct_abbreviation(){ return "rcpy_b"; }
};


//------------------------------------------------------------------------------
//- std::copy_backward ---------------------------------------------------------
template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_copy_backward
{
    typedef typename TargetT::value_type value_type;

    static void apply(const SourceT& source, TargetT& target)
    {
        fill_n(InsertIterator<TargetT>(target, target.end()), source.size(), neutron<value_type>::value());
        std::copy_backward(source.begin(), source.end(), target.end());
    }

    static void apply_elemental(const SourceT& source, TargetT& target)
    {
        fill_n(InsertIterator<TargetT>(target, target.end()), source.size(), neutron<value_type>::value());
        std::copy_backward(source.elements_begin(), source.elements_end(), target.end());
    }

    static std::string struct_abbreviation(){ return "cpyb_f"; }
};

template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_copy_backward_back
{
    typedef typename TargetT::value_type value_type;

    static void apply(const SourceT& source, TargetT& target)
    {
        fill_n(InsertIterator<TargetT>(target, target.end()), source.size(), neutron<value_type>::value());
        std::copy_backward(source.rbegin(), source.rend(), target.end());
    }

    static void apply_elemental(const SourceT& source, TargetT& target)
    {
        fill_n(InsertIterator<TargetT>(target, target.end()), source.size(), neutron<value_type>::value());
        std::copy_backward(source.elements_rbegin(), source.elements_rend(), target.end());
    }

    static std::string struct_abbreviation(){ return "cpyb_b"; }
};



}} // namespace itl boost

#endif BOOST_VALIDATE_STD_ALGORITHM_COPY_HPP_JOFA_091124

