/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_VALIDATE_STD_ALGORITHM_FIND_HPP_JOFA_091205
#define BOOST_VALIDATE_STD_ALGORITHM_FIND_HPP_JOFA_091205

#include <algorithm>

namespace boost{namespace itl
{

//------------------------------------------------------------------------------
//- std::find ------------------------------------------------------------------
template<class SegmentalT, class TargetT>
class std_find
{
public:
    typedef typename SegmentalT::atomized_type AtomicT;
    typedef typename AtomicT::value_type       value_type;
private:
    std_find& operator = (const std_find&);
public:
    std_find(const TargetT& comparand)
        : _comparand(comparand){}
    
    TargetT operator()(const AtomicT& source)const
    {
        typename AtomicT::const_iterator found 
            = std::find(source.begin(), source.end(), _comparand); 
        if(found == source.end())
            return neutron<TargetT>::value();
        else
            return *found;
    }

    TargetT operator()(const SegmentalT& source)const
    {
        typename SegmentalT::element_const_iterator found 
            = std::find(source.elements_begin(), source.elements_end(), _comparand); 
        if(found == source.elements_end())
            return neutron<TargetT>::value();
        else
            return *found;
    }

    static std::string struct_abbreviation(){ return "find_f"; }

private:
    value_type _comparand;
};


template<class SegmentalT, class TargetT>
class std_find_back
{
public:
    typedef typename SegmentalT::atomized_type AtomicT;
    typedef typename AtomicT::value_type       value_type;
private:
    std_find_back& operator = (const std_find_back&);
public:
    std_find_back(const TargetT& comparand)
        : _comparand(comparand){}
    
    TargetT operator()(const AtomicT& source)const
    {
        typename AtomicT::const_reverse_iterator found 
            = std::find(source.rbegin(), source.rend(), _comparand); 
        if(found == source.rend())
            return neutron<TargetT>::value();
        else
            return *found;
    }

    TargetT operator()(const SegmentalT& source)const
    {
        typename SegmentalT::element_const_reverse_iterator found 
            = std::find(source.elements_rbegin(), source.elements_rend(), _comparand); 
        if(found == source.elements_rend())
            return neutron<TargetT>::value();
        else
            return *found;
    }

    static std::string struct_abbreviation(){ return "find_b"; }

private:
    value_type _comparand;
};


//------------------------------------------------------------------------------
//- std::lower_bound -----------------------------------------------------------
template<class SegmentalT, class TargetT>
class std_lower_bound
{
public:
    typedef typename SegmentalT::atomized_type AtomicT;
    typedef typename AtomicT::value_type       value_type;
private:
    std_lower_bound& operator = (const std_lower_bound&);
public:
    std_lower_bound(const TargetT& comparand)
        : _comparand(comparand){}
    
    TargetT operator()(const AtomicT& source)const
    {
        typename AtomicT::const_iterator found 
            = std::lower_bound(source.begin(), source.end(), _comparand); 
        if(found == source.end())
            return neutron<TargetT>::value();
        else
            return *found;
    }

    TargetT operator()(const SegmentalT& source)const
    {
        typename SegmentalT::element_const_iterator found 
            = std::lower_bound(source.elements_begin(), source.elements_end(), _comparand); 
        if(found == source.elements_end())
            return neutron<TargetT>::value();
        else
            return *found;
    }

    static std::string struct_abbreviation(){ return "lwb_f"; }

private:
    value_type _comparand;
};


template<class SegmentalT, class TargetT>
class std_lower_bound_back
{
public:
    typedef typename SegmentalT::atomized_type AtomicT;
    typedef typename AtomicT::value_type       value_type;
private:
    std_lower_bound_back& operator = (const std_lower_bound_back&);
public:
    std_lower_bound_back(const TargetT& comparand)
        : _comparand(comparand){}
    
    TargetT operator()(const AtomicT& source)const
    {
        typename AtomicT::const_reverse_iterator found 
            = std::lower_bound(source.rbegin(), source.rend(), _comparand); 
        if(found == source.rend())
            return neutron<TargetT>::value();
        else
            return *found;
    }

    TargetT operator()(const SegmentalT& source)const
    {
        typename SegmentalT::element_const_reverse_iterator found 
            = std::lower_bound(source.elements_rbegin(), source.elements_rend(), _comparand); 
        if(found == source.elements_rend())
            return neutron<TargetT>::value();
        else
            return *found;
    }

    static std::string struct_abbreviation(){ return "lwb_b"; }

private:
    value_type _comparand;
};


//------------------------------------------------------------------------------
//- std::upper_bound -----------------------------------------------------------
template<class SegmentalT, class TargetT>
class std_upper_bound
{
public:
    typedef typename SegmentalT::atomized_type AtomicT;
    typedef typename AtomicT::value_type       value_type;
private:
    std_upper_bound& operator = (const std_upper_bound&);
public:
    std_upper_bound(const TargetT& comparand)
        : _comparand(comparand){}
    
    TargetT operator()(const AtomicT& source)const
    {
        typename AtomicT::const_iterator found 
            = std::upper_bound(source.begin(), source.end(), _comparand); 
        if(found == source.end())
            return neutron<TargetT>::value();
        else
            return *found;
    }

    TargetT operator()(const SegmentalT& source)const
    {
        typename SegmentalT::element_const_iterator found 
            = std::upper_bound(source.elements_begin(), source.elements_end(), _comparand); 
        if(found == source.elements_end())
            return neutron<TargetT>::value();
        else
            return *found;
    }

    static std::string struct_abbreviation(){ return "upb_f"; }

private:
    value_type _comparand;
};


template<class SegmentalT, class TargetT>
class std_upper_bound_back
{
public:
    typedef typename SegmentalT::atomized_type AtomicT;
    typedef typename AtomicT::value_type       value_type;
private:
    std_upper_bound_back& operator = (const std_upper_bound_back&);
public:
    std_upper_bound_back(const TargetT& comparand)
        : _comparand(comparand){}
    
    TargetT operator()(const AtomicT& source)const
    {
        typename AtomicT::const_reverse_iterator found 
            = std::upper_bound(source.rbegin(), source.rend(), _comparand); 
        if(found == source.rend())
            return neutron<TargetT>::value();
        else
            return *found;
    }

    TargetT operator()(const SegmentalT& source)const
    {
        typename SegmentalT::element_const_reverse_iterator found 
            = std::upper_bound(source.elements_rbegin(), source.elements_rend(), _comparand); 
        if(found == source.elements_rend())
            return neutron<TargetT>::value();
        else
            return *found;
    }

    static std::string struct_abbreviation(){ return "upb_b"; }

private:
    value_type _comparand;
};


}} // namespace itl boost

#endif // BOOST_VALIDATE_STD_ALGORITHM_FIND_HPP_JOFA_091205

