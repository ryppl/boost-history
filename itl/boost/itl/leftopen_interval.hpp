/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_LEFTOPEN_INTERVAL_HPP_JOFA_100930
#define BOOST_ITL_LEFTOPEN_INTERVAL_HPP_JOFA_100930

#include <functional>
#include <boost/concept/assert.hpp>
#include <boost/itl/type_traits/value_size.hpp>
#include <boost/itl/type_traits/type_to_string.hpp>
#include <boost/itl/concept/interval.hpp>

namespace boost{namespace itl
{

template <class DomainT, 
          ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT)>
class leftopen_interval
{
public:
    typedef leftopen_interval<DomainT,Compare> type;
    typedef DomainT domain_type;

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    /** Default constructor; yields an empty interval <tt>(0,0]</tt>. */
    leftopen_interval() 
        : _lwb(neutron<DomainT>::value()), _upb(neutron<DomainT>::value()) 
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
    }

    //NOTE: Compiler generated copy constructor is used

    /** Constructor for a left-open singleton interval <tt>(val-1,val]</tt> */
    explicit leftopen_interval(const DomainT& val)
        : _lwb(pred(val)), _upb(val)
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
        // Only for discrete types this ctor creates an interval containing 
        // a single element only.
        BOOST_STATIC_ASSERT((itl::is_discrete<DomainT>::value));
        BOOST_ASSERT((numeric_minimum<DomainT, is_numeric<DomainT>::value >::is_less_than(val) )); 
    }

    /** Interval from <tt>low</tt> to <tt>up</tt> with bounds <tt>bounds</tt> */
    leftopen_interval(const DomainT& low, const DomainT& up) :
        _lwb(low), _upb(up)
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
    }

    DomainT lower()const{ return _lwb; }
    DomainT upper()const{ return _upb; }

private:
    DomainT _lwb;
    DomainT _upb;
};

//==============================================================================
//=T leftopen_interval -> concept intervals
//==============================================================================
template<class DomainT, ITL_COMPARE Compare>
struct interval_traits< itl::leftopen_interval<DomainT, Compare> >
{
    typedef DomainT domain_type;
    typedef ITL_COMPARE_DOMAIN(Compare,DomainT) domain_compare;
    typedef itl::leftopen_interval<DomainT, Compare> interval_type;

    static interval_type construct(const domain_type& lo, const domain_type& up)
    {
        return interval_type(lo, up);
    }

    static domain_type lower(const interval_type& inter_val){ return inter_val.lower(); };
    static domain_type upper(const interval_type& inter_val){ return inter_val.upper(); };
};


//==============================================================================
//= Type traits
//==============================================================================
template <class DomainT, ITL_COMPARE Compare> 
struct interval_bound_type< leftopen_interval<DomainT,Compare> >
{
    typedef interval_bound_type type;
    BOOST_STATIC_CONSTANT(unsigned char, value = interval_bounds::static_leftopen);
};

template <class DomainT, ITL_COMPARE Compare>
struct type_to_string<itl::leftopen_interval<DomainT,Compare> >
{
    static std::string apply()
    { return "(I]<"+ type_to_string<DomainT>::apply() +">"; }
};

template<class DomainT, ITL_COMPARE Compare> 
struct value_size<itl::leftopen_interval<DomainT,Compare> >
{
    static std::size_t apply(const itl::leftopen_interval<DomainT>& value) 
    { return 2; }
};

}} // namespace itl boost

#endif

