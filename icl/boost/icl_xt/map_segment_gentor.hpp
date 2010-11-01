/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ICL_XT_MAP_SEGMENT_GENTOR_HPP_JOFA_091004
#define BOOST_ICL_XT_MAP_SEGMENT_GENTOR_HPP_JOFA_091004


#include <boost/icl/interval.hpp>
#include <boost/icl_xt/gentorit.hpp>
#include <boost/icl_xt/numbergentor.hpp>
#include <boost/icl_xt/itvgentor.hpp>
#include <boost/icl_xt/seqgentor.hpp>

namespace boost{namespace icl
{

template <class DomainT, class CodomainT, class IntervalT = typename icl::interval<DomainT>::type > 
class map_segment_gentor: public RandomGentorAT<std::pair<IntervalT, CodomainT> >
{
public:
    typedef DomainT   domain_type;
    typedef CodomainT codomain_type;
    typedef IntervalT interval_type;
    typedef typename icl::interval<domain_type>::type   range_type;
    typedef typename icl::interval<codomain_type>::type co_range_type;
    //typedef typename icl::interval<codomain_type>::type co_range_type;
    typedef std::pair<IntervalT, CodomainT> segment_type;

public:
    map_segment_gentor(): _codomain_gentor(NULL){}
    ~map_segment_gentor(){ delete _codomain_gentor; }

    virtual void some(segment_type& x);

    void set_range(const range_type& range)
    { _interval_gentor.setRange(range); }

    void set_corange(const co_range_type& range)
    { _covalue_range = range; }
    
    void setMaxIntervalLength(domain_type max_itv_length)
    { _interval_gentor.setMaxIntervalLength(max_itv_length); }

    void setCodomainGentor(RandomGentorAT<codomain_type>* gentor)
    { 
        delete _codomain_gentor;
        _codomain_gentor = gentor; 
    }

private:
    ItvGentorT<domain_type,IntervalT> _interval_gentor;
    RandomGentorAT<codomain_type>*    _codomain_gentor;
    co_range_type                     _covalue_range;
};


template <class DomainT, class CodomainT, class IntervalT>
void map_segment_gentor<DomainT,CodomainT,IntervalT>::some(segment_type& value)
{
    interval_type inter_val;
    codomain_type co_val;
    _interval_gentor.some(inter_val);
    _codomain_gentor->some(co_val);
    value = segment_type(inter_val, co_val);
};

}} // BOOST_ICL_XT_MAP_SEGMENT_GENTOR_HPP_JOFA_091004

#endif


