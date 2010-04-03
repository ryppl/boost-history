/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_CONTINUOUS_INTERVAL_HPP_JOFA_100327
#define BOOST_ITL_CONTINUOUS_INTERVAL_HPP_JOFA_100327

#include <boost/itl/type_traits/is_continuous.hpp>
#include <boost/itl/type_traits/is_continuous_interval.hpp>
#include <boost/itl/detail/base_interval.hpp>
#include <boost/itl/interval_bounds.hpp>
#include <boost/itl/interval_functions.hpp>

namespace boost{namespace itl
{

typedef unsigned char bound_type; //JODO

template <class DomainT, 
          ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT)>
class continuous_interval : public base_interval<DomainT,Compare> 
{
public:
    typedef continuous_interval<DomainT,Compare> type;
    typedef DomainT domain_type;

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    /** Default constructor; yields an empty interval <tt>[0,0)</tt>. */
    continuous_interval()
		: _lwb(neutron<DomainT>::value()), _upb(neutron<DomainT>::value())
		, _bounds(interval_bounds::right_open())
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
        //JODO URG BOOST_STATIC_ASSERT((itl::is_continuous<DomainT>::value)); 
		//JODO we must assert this via appropriate defaults for template parameters
    }

    //NOTE: Compiler generated copy constructor is used

    /** Constructor for a closed singleton interval <tt>[val,val]</tt> */
    explicit continuous_interval(const DomainT& val)
        : _lwb(val), _upb(val), _bounds(interval_bounds::closed())
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
        //JODO BOOST_STATIC_ASSERT((itl::is_continuous<DomainT>::value));
    }

    /** Interval from <tt>low</tt> to <tt>up</tt> with bounds <tt>bounds</tt> */
    continuous_interval(const DomainT& low, const DomainT& up, 
                        interval_bounds bounds = interval_bounds::right_open())
        : _lwb(low), _upb(up), _bounds(bounds)
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
        //JODO BOOST_STATIC_ASSERT((itl::is_continuous<DomainT>::value));
    }

    domain_type     lower()const { return _lwb; }
    domain_type     upper()const { return _upb; }
    interval_bounds bounds()const{ return _bounds; }

    domain_type first()const{ return _lwb; }

    DomainT last()const
    {
        BOOST_STATIC_ASSERT((!itl::is_continuous<DomainT>::value));
        return pred(_upb);
    }

private:
    domain_type     _lwb;
    domain_type     _upb;
    interval_bounds _bounds;
};


template<class CharType, class CharTraits, class DomainT, ITL_COMPARE Compare>
std::basic_ostream<CharType, CharTraits>& operator <<
  (std::basic_ostream<CharType, CharTraits> &stream, 
   continuous_interval<DomainT,Compare> const& object)
{
    if(itl::is_empty(object))
        return stream << "[)";
    else
        return stream << "[" << object.lower() << "," << object.upper()<< ")";
}


//==============================================================================
//= Type traits
//==============================================================================
template <class DomainT, ITL_COMPARE Compare> 
struct is_interval<continuous_interval<DomainT,Compare> >
{
    typedef is_interval<continuous_interval<DomainT,Compare> > type;
    BOOST_STATIC_CONSTANT(bool, value = true);
};

template <class DomainT, ITL_COMPARE Compare> 
struct is_continuous_interval<continuous_interval<DomainT,Compare> >
{
    typedef is_continuous_interval<continuous_interval<DomainT,Compare> > type;
	BOOST_STATIC_CONSTANT(bool, value = is_continuous<DomainT>::value);
};

template <class DomainT, ITL_COMPARE Compare> 
struct has_dynamic_bounds<continuous_interval<DomainT,Compare> >
{
    typedef has_dynamic_bounds<continuous_interval<DomainT,Compare> > type;
    BOOST_STATIC_CONSTANT(bool, value = true);
};

template <class DomainT, ITL_COMPARE Compare>
struct type_to_string<itl::continuous_interval<DomainT,Compare> >
{
    static std::string apply()
    { return "[R)<"+ type_to_string<DomainT>::apply() +">"; }
};

template<class DomainT> 
struct value_size<itl::continuous_interval<DomainT> >
{
    static std::size_t apply(const itl::continuous_interval<DomainT>& value) 
    { return 2; }
};

}} // namespace itl boost

#endif

