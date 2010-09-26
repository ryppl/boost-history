/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_RIGHTOPEN_INTERVAL_HPP_JOFA_100323
#define BOOST_ITL_RIGHTOPEN_INTERVAL_HPP_JOFA_100323

#include <boost/itl/type_traits/value_size.hpp>
#include <boost/itl/type_traits/type_to_string.hpp>
#include <boost/itl/interval_bounds.hpp> //JODO CL rightopen_interval ought to be independent on this, 
                                         //but interval_function.hpp depends partially. So we might try to minimize
                                         //dependencies here. There's also that point on generation.
                                         //Two partitions of interval_functions
                                         //  (dependen on i_b ( independent on i_b ))
#include <boost/itl/detail/base_interval.hpp>
#include <boost/itl/concept/interval.hpp>

namespace boost{namespace itl
{

template <class DomainT, 
          ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT)>
class rightopen_interval : public base_interval<DomainT,Compare> 
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
        : _lwb(neutron<DomainT>::value()), _upb(neutron<DomainT>::value()) 
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
    }

    //NOTE: Compiler generated copy constructor is used

    /** Constructor for a closed singleton interval <tt>[val,val]</tt> */
    explicit rightopen_interval(const DomainT& val)
        : _lwb(val), _upb(succ(val))
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
        BOOST_STATIC_ASSERT((!itl::is_continuous<DomainT>::value));
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


template<class CharType, class CharTraits, class DomainT, ITL_COMPARE Compare>
std::basic_ostream<CharType, CharTraits>& operator <<
  (std::basic_ostream<CharType, CharTraits> &stream, 
   rightopen_interval<DomainT,Compare> const& object)
{
    if(boost::itl::is_empty(object))
        return stream << "[)";
    else
        return stream << "[" << object.lower() << "," << object.upper()<< ")";
}

//==============================================================================
//=T rightopen_interval -> concept intervals
//==============================================================================
template<class DomainT, ITL_COMPARE Compare>
struct intervals< itl::rightopen_interval<DomainT, Compare> >
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
struct is_interval<rightopen_interval<DomainT,Compare> >
{
    typedef is_interval<rightopen_interval<DomainT,Compare> > type;
    BOOST_STATIC_CONSTANT(bool, value = true);
};

template <class DomainT, ITL_COMPARE Compare> 
struct has_static_bounds<rightopen_interval<DomainT,Compare> >
{
    typedef has_static_bounds<rightopen_interval<DomainT,Compare> > type;
    BOOST_STATIC_CONSTANT(bool, value = true);
};

template <class DomainT, ITL_COMPARE Compare> 
struct has_asymmetric_bounds<rightopen_interval<DomainT,Compare> >
{
    typedef has_asymmetric_bounds<rightopen_interval<DomainT,Compare> > type;
    BOOST_STATIC_CONSTANT(bool, value = true);
};

template <class DomainT, ITL_COMPARE Compare> 
struct is_static_rightopen<rightopen_interval<DomainT,Compare> >//JODO replace this predicate static_borders in {open, leftopen, rightopen, cloded}
{
    typedef is_static_rightopen<rightopen_interval<DomainT,Compare> > type;
    BOOST_STATIC_CONSTANT(bool, value = true);
};

template <class DomainT, ITL_COMPARE Compare>
struct type_to_string<itl::rightopen_interval<DomainT,Compare> >
{
    static std::string apply()
    { return "[I)<"+ type_to_string<DomainT>::apply() +">"; }
};

template<class DomainT> 
struct value_size<itl::rightopen_interval<DomainT> >
{
    static std::size_t apply(const itl::rightopen_interval<DomainT>& value) 
    { return 2; }
};

}} // namespace itl boost

#endif

