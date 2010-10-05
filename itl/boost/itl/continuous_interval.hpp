/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_CONTINUOUS_INTERVAL_HPP_JOFA_100327
#define BOOST_ITL_CONTINUOUS_INTERVAL_HPP_JOFA_100327

#include <functional> 
#include <boost/static_assert.hpp> 
#include <boost/concept/assert.hpp>
#include <boost/itl/detail/concept_check.hpp>
#include <boost/itl/concept/container.hpp>
#include <boost/itl/type_traits/value_size.hpp>
#include <boost/itl/type_traits/type_to_string.hpp>
#include <boost/itl/type_traits/is_continuous.hpp>
#include <boost/itl/type_traits/is_continuous_interval.hpp>
#include <boost/itl/interval_bounds.hpp>
#include <boost/itl/concept/interval.hpp>

namespace boost{namespace itl
{

template <class DomainT, 
          ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT)>
class continuous_interval
{
public:
    typedef continuous_interval<DomainT,Compare> type;
    typedef DomainT domain_type;
    typedef typename bounded_value<DomainT>::type bounded_domain_type;

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
        BOOST_STATIC_ASSERT((itl::is_continuous<DomainT>::value)); 
    }

    //NOTE: Compiler generated copy constructor is used

    /** Constructor for a closed singleton interval <tt>[val,val]</tt> */
    explicit continuous_interval(const DomainT& val)
        : _lwb(val), _upb(val), _bounds(interval_bounds::closed())
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
        BOOST_STATIC_ASSERT((itl::is_continuous<DomainT>::value));
    }

    /** Interval from <tt>low</tt> to <tt>up</tt> with bounds <tt>bounds</tt> */
    continuous_interval(const DomainT& low, const DomainT& up, 
                      interval_bounds bounds = interval_bounds::right_open(),
                      continuous_interval* = 0)
        : _lwb(low), _upb(up), _bounds(bounds)
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
        BOOST_STATIC_ASSERT((itl::is_continuous<DomainT>::value));
    }

    domain_type     lower()const { return _lwb; }
    domain_type     upper()const { return _upb; }
    interval_bounds bounds()const{ return _bounds; }

    static continuous_interval open     (const DomainT& lo, const DomainT& up){ return continuous_interval(lo, up, interval_bounds::open());      }
    static continuous_interval rightopen(const DomainT& lo, const DomainT& up){ return continuous_interval(lo, up, interval_bounds::right_open());}
    static continuous_interval leftopen (const DomainT& lo, const DomainT& up){ return continuous_interval(lo, up, interval_bounds::left_open()); }
    static continuous_interval closed   (const DomainT& lo, const DomainT& up){ return continuous_interval(lo, up, interval_bounds::closed());    }

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
        return stream        << left_bracket(object.bounds()) << object.lower()
                      << "," << object.upper()<< right_bracket(object.bounds());
}


//==============================================================================
//=T continuous_interval -> concept interval
//==============================================================================
template<class DomainT, ITL_COMPARE Compare>
struct interval_traits< itl::continuous_interval<DomainT, Compare> >
{
    typedef interval_traits type;
    typedef DomainT domain_type;
    typedef ITL_COMPARE_DOMAIN(Compare,DomainT) domain_compare;
    typedef itl::continuous_interval<DomainT, Compare> interval_type;

    static interval_type construct(const domain_type& lo, const domain_type& up)
    {
        return interval_type(lo, up);
    }

    static domain_type lower(const interval_type& inter_val){ return inter_val.lower(); };
    static domain_type upper(const interval_type& inter_val){ return inter_val.upper(); };
};


//==============================================================================
//=T continuous_interval -> concept dynamic_interval
//==============================================================================
template<class DomainT, ITL_COMPARE Compare>
struct dynamic_interval_traits<boost::itl::continuous_interval<DomainT,Compare> >
{
    typedef dynamic_interval_traits type;
    typedef boost::itl::continuous_interval<DomainT,Compare> interval_type;
    typedef DomainT domain_type;
    typedef ITL_COMPARE_DOMAIN(Compare,DomainT) domain_compare;

    static interval_type construct(const domain_type lo, const domain_type up, interval_bounds bounds)
    {
        return itl::continuous_interval<DomainT,Compare>(lo, up, bounds,
            static_cast<itl::continuous_interval<DomainT,Compare>* >(0) );
    }

    static interval_type construct_bounded(const bounded_value<DomainT>& lo, 
                                           const bounded_value<DomainT>& up)
    {
        return  itl::continuous_interval<DomainT,Compare>
                (
                    lo.value(), up.value(),
                    lo.bound().left() | up.bound().right(),
                    static_cast<itl::continuous_interval<DomainT,Compare>* >(0) 
                );
    }
};

//==============================================================================
//= Type traits
//==============================================================================
template <class DomainT, ITL_COMPARE Compare> 
struct interval_bound_type< continuous_interval<DomainT,Compare> >
{
    typedef interval_bound_type type;
    BOOST_STATIC_CONSTANT(unsigned char, value = interval_bounds::dynamic);
};

template <class DomainT, ITL_COMPARE Compare> 
struct is_continuous_interval<continuous_interval<DomainT,Compare> >
{
    typedef is_continuous_interval<continuous_interval<DomainT,Compare> > type;
    BOOST_STATIC_CONSTANT(bool, value = true);
};

template <class DomainT, ITL_COMPARE Compare>
struct type_to_string<itl::continuous_interval<DomainT,Compare> >
{
    static std::string apply()
    { return "cI<"+ type_to_string<DomainT>::apply() +">"; }
};

template<class DomainT> 
struct value_size<itl::continuous_interval<DomainT> >
{
    static std::size_t apply(const itl::continuous_interval<DomainT>& value) 
    { return 2; }
};

}} // namespace itl boost

#endif

