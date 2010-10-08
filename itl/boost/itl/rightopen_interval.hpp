/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_RIGHTOPEN_INTERVAL_HPP_JOFA_100323
#define BOOST_ITL_RIGHTOPEN_INTERVAL_HPP_JOFA_100323

#include <functional>
#include <boost/concept/assert.hpp>
#include <boost/itl/type_traits/value_size.hpp>
#include <boost/itl/type_traits/type_to_string.hpp>
#include <boost/itl/concept/interval.hpp>

namespace boost{namespace itl
{

template <class DomainT, 
          ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT)>
class rightopen_interval
{
public:
    typedef rightopen_interval<DomainT,Compare> type;
    typedef DomainT domain_type;

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    /** Default constructor; yields an empty interval <tt>[0,0)</tt>. */
    rightopen_interval() 
        : _lwb(identity_element<DomainT>::value()), _upb(identity_element<DomainT>::value()) 
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
    }

    //NOTE: Compiler generated copy constructor is used

    /** Constructor for a singleton interval <tt>[val,val+1)</tt> */
    explicit rightopen_interval(const DomainT& val)
        : _lwb(val), _upb(succ(val))
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
        // Only for discrete types this ctor creates an interval containing 
        // a single element only.
        BOOST_STATIC_ASSERT((itl::is_discrete<DomainT>::value));
    }

    /** Interval from <tt>low</tt> to <tt>up</tt> with bounds <tt>bounds</tt> */
    rightopen_interval(const DomainT& low, const DomainT& up) :
        _lwb(low), _upb(up)
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
    }

    domain_type lower()const{ return _lwb; }
    domain_type upper()const{ return _upb; }

private:
    domain_type _lwb;
    domain_type _upb;
};

//==============================================================================
//=T rightopen_interval -> concept intervals
//==============================================================================
template<class DomainT, ITL_COMPARE Compare>
struct interval_traits< itl::rightopen_interval<DomainT, Compare> >
{
    typedef DomainT domain_type;
    typedef ITL_COMPARE_DOMAIN(Compare,DomainT) domain_compare;
    typedef itl::rightopen_interval<DomainT, Compare> interval_type;

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
struct interval_bound_type< rightopen_interval<DomainT,Compare> >
{
    typedef interval_bound_type type;
    BOOST_STATIC_CONSTANT(unsigned char, value = interval_bounds::static_rightopen);
};

template <class DomainT, ITL_COMPARE Compare>
struct type_to_string<itl::rightopen_interval<DomainT,Compare> >
{
    static std::string apply()
    { return "[I)<"+ type_to_string<DomainT>::apply() +">"; }
};

template<class DomainT, ITL_COMPARE Compare> 
struct value_size<itl::rightopen_interval<DomainT,Compare> >
{
    static std::size_t apply(const itl::rightopen_interval<DomainT>& value) 
    { return 2; }
};

}} // namespace itl boost

#endif

