/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __itl_interval_JOFA_000626_H__
#define __itl_interval_JOFA_000626_H__

#ifdef USE_CONCEPTS
#include <bits/concepts.h>
#endif

#include <ostream>
#include <functional>
#include <limits>
#include <string>
#include <boost/assert.hpp> 
#include <boost/static_assert.hpp> 
#include <boost/next_prior.hpp> 
#include <boost/call_traits.hpp> 
#include <boost/mpl/bool.hpp> 
#include <boost/mpl/if.hpp> 
#include <boost/mpl/assert.hpp> 
#include <boost/itl/detail/notate.hpp>
#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/type_traits/neutron.hpp>
#include <boost/itl/type_traits/unon.hpp>
#include <boost/itl/type_traits/is_continuous.hpp>
#include <boost/itl/type_traits/difference.hpp>
#include <boost/itl/type_traits/size.hpp>
#include <boost/itl/type_traits/to_string.hpp>

#undef min
#undef max

#define BOUND_VALUE first
#define BOUND_TYPES second

namespace boost{namespace itl
{

/// Constants for intervalbounds
enum BoundTypes {
    /// Both open: <tt>(x,y)</tt>
    open_bounded             = 0, 
    /// Left open right closed: <tt>(x,y]</tt>
    left_open                = 1, 
    /// Left closed right open: <tt>[x,y)</tt>
    right_open               = 2,
    /// Both closed: <tt>[x,y]</tt>
    closed_bounded           = 3,
} ;

typedef unsigned char bound_type;

/** A class template for intervals */
template <class DomainT, ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT)>
#ifdef USE_CONCEPTS
	requires std::LessThanComparable<DomainT>
#endif
class interval
{
public:
    typedef interval<DomainT,Compare> type;

    /// Domain type or element type
    typedef DomainT domain_type;
    typedef DomainT codomain_type;
    typedef DomainT element_type;
    typedef type    segment_type;

	typedef DomainT key_type;
	typedef DomainT data_type;
	typedef DomainT value_type;
    typedef type    interval_type;

	/// Compare order on the data
    typedef ITL_COMPARE_DOMAIN(Compare,DomainT) domain_compare;
	typedef domain_compare key_compare;

    /// The difference type of an interval which is sometimes different form the domain_type
    typedef typename itl::difference<DomainT>::type difference_type;

    /// The size type of an interval which is mostly std::size_t
    typedef typename itl::size<DomainT>::type size_type;

public:
	//==========================================================================
	//= Construct, copy, destruct
	//==========================================================================
    /** Default constructor; yields an empty interval <tt>[1,0]</tt> */
    interval() : _lwb(unon<DomainT>::value()), _upb(neutron<DomainT>::value()), 
		         _boundtype(itl::closed_bounded) {}

    //NOTE: Compiler generated copy constructor is used

    /** Constructor for a closed singleton interval <tt>[val,val]</tt> */
    explicit interval(const DomainT& val) : 
		_lwb(val), _upb(val), _boundtype(itl::closed_bounded) {}

    /** Interval from <tt>low</tt> to <tt>up</tt> with bounds <tt>bounds</tt> */
	interval(const DomainT& low, const DomainT& up, itl::bound_type bounds = itl::closed_bounded) : 
        _lwb(low), _upb(up), _boundtype(bounds) {}

	/** Closed interval <tt>[low,up]</tt> */
	static interval closed(const DomainT& low, const DomainT& up)
	{ return interval(low, up, itl::closed_bounded); }

	/** Rightopen interval <tt>[low,up)</tt> */
	static interval rightopen(const DomainT& low, const DomainT& up)
	{ return interval(low, up, right_open); }

	/** Leftopen interval <tt>(low,up]</tt> */
	static interval leftopen(const DomainT& low, const DomainT& up)
	{ return interval(low, up, left_open); }

	/** Open interval <tt>(low,up)</tt> */
	static interval open(const DomainT& low, const DomainT& up)
	{ return interval(low, up, itl::open_bounded); }

    //NOTE: Compiler generated assignment operator = used

	//==========================================================================
	//= Emptieness, containment
	//==========================================================================
    /** Is the interval empty? */
    bool empty()const;

    /** Set the interval empty */
    void clear()
	{ set_lwb(unon<DomainT>::value()); set_upb(neutron<DomainT>::value()); 
	  _boundtype=itl::closed_bounded; }

    /** Does the interval contain <tt>x</tt>? */
    bool contains(const DomainT& x)const;
    /** <tt>*this</tt> is superset of <tt>sub</tt> */
    bool contains(const interval& sub)const;

    /** <tt>*this</tt> is subset of <tt>super</tt> */
    bool contained_in(const interval& super)const ;

    /**  <tt>*this</tt> and <tt>x2</tt> are disjoint; their intersection is empty */
    bool is_disjoint(const interval& x2)const
    { return exclusive_less(x2) || x2.exclusive_less(*this); }

	//==========================================================================
	//= Size
	//==========================================================================
    /** Cardinality of the interval: The number of elements */
    size_type cardinality()const;

    /** Arithmetic size of the interval */
    difference_type length()const;

    /** Size of the interval */
    size_type size()const { return cardinality(); }

	//==========================================================================
	//= Range
	//==========================================================================
    /** Lower bound of the interval */
    DomainT lower()const { return _lwb; }
    /** Upper bound of the interval */
    DomainT upper()const { return _upb; }

    /** First (smallest) element of the interval */
    DomainT first()const;
    /** Last (largest) element of the interval */
    DomainT last()const;

	//==========================================================================
	//= Selection
	//==========================================================================

    /** Typ of interval bounds */
    bound_type boundtype()const { return _boundtype; }

	//==========================================================================
	//= Addition
	//==========================================================================

    /** Extend <tt>*this</tt> to <tt>x2</tt> yielding an interval from the 
	    minimum of lower bounds to the 
		maximum of upper bounds */
    interval& extend(const interval& x2);

	/** Interval spanning from lower bound of \c *this interval to the upper bound of \c rhs.
        Bordertypes according to the lower bound of \c *this and the upper bound of \c rhs.   */
    interval span(const interval& rhs)const
    {
        if(empty())          return rhs;
        else if(rhs.empty()) return *this;
        else return 
                interval(_lwb, rhs._upb, span(boundtype(), rhs.boundtype()));
    }


	//==========================================================================
	//= Subtraction
	//==========================================================================

    /** subtract \c x2 from \c *this interval on it's left side. Assign the difference 
		to \c right_over. The result \c right_over is the part of \c *this right of \c x2.
\code
right_over = x1 - x2; //on the left.
...      d) : x1
... c)      : x2; x1.left_subtract(right_over, x2);
     [c  d) : right_over
\endcode
	*/
    void left_subtract(interval& right_over, const interval& x2)const;

    /** subtract \c x2 from \c *this interval on it's right side. Assign the difference 
		to \c left_over. The result \c left_over is the part of \c *this left of \c x2.
\code
left_over = x1 - x2; //on the right side.
[a      ...  : x1
     [b ...  : x2; x1.right_subtract(left_over, x2);
[a  b)       : left_over
\endcode
	*/
    void right_subtract(interval& left_over, const interval& x2)const;

    /** subtract \c x2 from \c *this interval on it's left side.
\code
*this = *this - x2; //on the left.
...      d) : *this
... c)      : x2;
     [c  d) : *this
\endcode
	*/
	interval& left_subtract(const interval& x2);

    /** subtract \c x2 from \c *this interval on it's right side. 
\code
*this = *this - x2; //on the right side.
[a      ...  : *this
     [b ...  : x2;
[a  b)       : *this
\endcode
	*/
	interval& right_subtract(const interval& x2);

	//==========================================================================
	//= Intersection
	//==========================================================================

	/** Intersection with the interval  <tt>x2</tt>; assign result to <tt>isec</tt> */
 //   void intersect(interval& isec, const interval& x2)const
	//{
	//	isec = *this;
	//	isec &= x2;
	//}

	interval& operator &= (const interval& sectant)
	{
		set_lwb(lwb_max(sectant));
		set_upb(upb_min(sectant));
		return *this;
	}


	
	//==========================================================================
	//= Representation
	//==========================================================================
	
	/** Object as string */
    const std::string as_string()const;


	/** What type is the interval?
\code
interval.is(closed_bounded); //[x, y] a closed interval
interval.is(right_open);     //[x, y) a right-open interval (also left-closed interval)
interval.is(left_open);      //(x, y] a left-open interval  (also right-closed interval)
interval.is(open_bounded);   //(x, y) an open interval
\endcode
	*/
    bool is(bound_type bounded)const { return _boundtype == bounded; }

	/** What bound type is the left interval border? 	
\code
interval.is_left(closed_bounded); //[x, y] or [x, y)
interval.is_left(open_bounded);   //(x, y] or (x, y)
\endcode
	*/
	bool is_left(bound_type bounded)const
	{
		BOOST_ASSERT(bounded == itl::open_bounded || bounded == itl::closed_bounded);
		return ((_boundtype & right_open)>>1) == (bounded>>1); 
	}

	/** What bound type is the right interval border? 	
\code
interval.is_right(closed_bounded); //[x, y] or (x, y]
interval.is_right(open_bounded);   //[x, y) or (x, y)
\endcode
	*/
	bool is_right(bound_type bounded)const
	{
		BOOST_ASSERT(bounded == itl::open_bounded || bounded == itl::closed_bounded);
		return ((_boundtype & left_open) == (bounded>>1)); 
	}


    /** There is no gap between <tt>*this</tt> and <tt>x2</tt> but they have no element in common */
    bool touches(const interval& x2)const;

    /** Maximal element of <tt>*this</tt> is less than the minimal element of <tt>x2</tt> */
    bool exclusive_less(const interval& x2)const;

	/** Set \c *this interval to from \c low to \c up with boundtype \c bounds */
    interval& set(const DomainT& low, const DomainT& up, bound_type bounds) 
    { _lwb=low; _upb=up; _boundtype=bounds; return *this; }

    /** Transforms the interval to the bound-type <tt>bounded</tt> without
        changing it's content. Requires Integral<domain_type>. 
\code
interval.as(closed_bounded).is(closed_bounded) &&
interval.as(right_open).is(right_open) &&
interval.as(left_open).is(left_open) &&
interval.as(open_bounded).is(open_bounded)
\endcode
	*/
    interval& as(bound_type bounded);

	/** An interval that covers the complete range of it's domain_type */
    static interval whole()
    { return interval<DomainT>::closed(std::numeric_limits<DomainT>::min(), 
                                       std::numeric_limits<DomainT>::max()); }


	/** First element of \c *this is less than first element of \c x2 */
    bool lower_less(const interval& x2)const;
	/** Last element of \c *this is less than last element of \c x2 */
    bool upper_less(const interval& x2)const;
	/** First element of \c *this is less than or equal to the first element of \c x2 */
    bool lower_less_equal(const interval& x2)const;
	/** Last element of \c *this is less than or equal to the last element of \c x2 */
    bool upper_less_equal(const interval& x2)const;
	/** First element of \c *this is equal to the first element of \c x2 */
    bool lower_equal(const interval& x2)const;
	/** Last element of \c *this is equal to the last element of \c x2 */
    bool upper_equal(const interval& x2)const;

public:
    typedef typename boost::call_traits<DomainT>::param_type DomainP;

	/** Less compare of interval elements. */
	inline static bool domain_less(DomainP left, DomainP right)       
	{return domain_compare()(left, right) ;}

	/** Less or equal compare of interval elements. */
	inline static bool domain_less_equal(DomainP left, DomainP right) 
	{return !domain_compare()(right, left );}

	/** Equality compare of interval elements. */
	inline static bool domain_equal(DomainP left, DomainP right)
	{return !domain_compare()(left, right) && !domain_compare()(right, left);}

private:
    typedef std::pair<DomainT, bound_type> BoundT;

private:
    void set_lwb(DomainT lw) { _lwb=lw; }
    void set_upb(DomainT up) { _upb=up; }

    void set_lwb_type(bound_type bounds) 
    { _boundtype = (unsigned char)((left_open & _boundtype) | (right_open & bounds)); }

    void set_upb_type(bound_type bounds) 
    { _boundtype = (unsigned char)((right_open & _boundtype) | (left_open & bounds)); }

    static bound_type span(bound_type left, bound_type right)
    { return (unsigned char)((left_open | left) & (right_open | right)) ; }

    bound_type succession_bounds()const;

    void set_lwb(const BoundT& lw) { _lwb=lw.BOUND_VALUE; set_lwb_type(lw.BOUND_TYPES); }
    void set_upb(const BoundT& up) { _upb=up.BOUND_VALUE; set_upb_type(up.BOUND_TYPES); }

    BoundT lwb_min(const interval& x2)const;
    BoundT lwb_max(const interval& x2)const;
    BoundT upb_min(const interval& x2)const;
    BoundT upb_max(const interval& x2)const;

    BoundT lwb_rightOf(const interval& x2)const;
    BoundT upb_leftOf(const interval& x2)const;

private:
    DomainT _lwb;
    DomainT _upb;
    bound_type _boundtype;
} ;


template <class DomainT, ITL_COMPARE Compare>
bound_type interval<DomainT,Compare>::succession_bounds()const
{
	if(_boundtype==itl::right_open)     return itl::right_open;
    if(_boundtype==itl::left_open)      return itl::left_open;
    if(_boundtype==itl::closed_bounded) return itl::open_bounded;
	return itl::closed_bounded;
}


template<class IntervalT> 
struct continuous_type
{
	typedef typename IntervalT::domain_type domain_type;
    typedef typename boost::call_traits<domain_type>::param_type DomainP;

	static bool open_bound_less_equal(DomainP x, DomainP y) 
	{ return IntervalT::domain_less_equal(x,y); } //{ return x <= y; }
    static bool open_bound_less      (DomainP x, DomainP y) 
	{ return IntervalT::domain_less(x,y); } //{ return x < y; }
};

template<class IntervalT> 
struct discrete_type
{
	typedef typename IntervalT::domain_type domain_type;
    typedef typename boost::call_traits<domain_type>::param_type DomainP;

    static bool open_bound_less_equal(DomainP x, DomainP y) 
	{ return IntervalT::domain_less_equal(x, succ(y)); } //{ return      x  <= succ(y); }
    static bool open_bound_less      (DomainP x, DomainP y) 
	{ return IntervalT::domain_less(succ(x),y); }        //{ return succ(x) <       y ; }
};

template <class DomainT, ITL_COMPARE Compare>
bool interval<DomainT,Compare>::empty()const
{
    using namespace boost::mpl;

    if(is_right(closed_bounded) && is_left(closed_bounded)) return domain_less(_upb, _lwb);       //_upb <  _lwb;
    if(is_right(open_bounded)   && is_left(closed_bounded)) return domain_less_equal(_upb, _lwb); //_upb <= _lwb;
    if(is_right(closed_bounded) && is_left(open_bounded))   return domain_less_equal(_upb, _lwb); //_upb <= _lwb;

    // OTHERWISE (is_right(open_bounded) && is_left(open_bounded))
    return 
        if_<
            bool_<is_continuous<DomainT>::value>, 
            continuous_type<interval<DomainT,Compare> >, 
            discrete_type<interval<DomainT,Compare> > 
           >
           ::type::open_bound_less_equal(_upb, _lwb);
}

template<class IntervalT> 
struct continuous_interval
{
    static typename IntervalT::size_type cardinality(const IntervalT& x) 
    {
		typedef typename IntervalT::size_type SizeT;
		if(x.empty()) 
			return itl::neutron<SizeT>::value();
        else if(x.is(itl::closed_bounded) && IntervalT::domain_equal(x.lower(), x.upper()))
            return itl::unon<SizeT>::value();
        else 
            return std::numeric_limits<SizeT>::infinity();
	}

    static typename IntervalT::difference_type 
        length(const IntervalT& x) 
    {
		return x.empty() ? itl::neutron<typename IntervalT::difference_type>::value() 
			             : x.upper() - x.lower();
	}

    static bool unaligned_lwb_equal(const IntervalT& x1, const IntervalT& x2)
    { return false; }

    static bool unaligned_upb_equal(const IntervalT& x1, const IntervalT& x2)
    { return false; }

    static bool has_equal_border_touch(const IntervalT& x1, const IntervalT& x2)
    { return false; }
};

template<class IntervalT> 
struct discrete_interval
{
	typedef typename IntervalT::domain_type domain_type;

    static typename IntervalT::size_type 
        cardinality(const IntervalT& x) 
    { 
		return x.empty()? itl::neutron<typename IntervalT::size_type>::value() 
			            : succ(x.last()-x.first());
	}

    static typename IntervalT::difference_type length(const IntervalT& x) 
    {
		return x.empty() ? 
            itl::neutron<typename IntervalT::difference_type>::value() : 
            succ(x.last() - x.first()); 
	}

    static bool unaligned_lwb_equal(const IntervalT& x1, const IntervalT& x2)
    { 
        if(x1.is_left(open_bounded) &&  x2.is_left(closed_bounded)) 
			 return IntervalT::domain_equal(succ(x1.lower()),     x2.lower() );
        else return IntervalT::domain_equal(     x1.lower(), succ(x2.lower()));
    }

    static bool unaligned_upb_equal(const IntervalT& x1, const IntervalT& x2)
    { 
        if(x1.is_right(closed_bounded) && x2.is_right(open_bounded))  
             return IntervalT::domain_equal(succ(x1.upper()),      x2.upper() );
        else return IntervalT::domain_equal(     x1.upper(),  succ(x2.upper()));
    }

    static bool has_equal_border_touch(const IntervalT& x1, const IntervalT& x2)
    {
        if(x1.is_right(closed_bounded) && x2.is_left(closed_bounded)) 
            return IntervalT::domain_equal(succ(x1.upper()), x2.lower());
        if(x1.is_right(open_bounded)  && x2.is_left(open_bounded) ) 
			return IntervalT::domain_equal(x1.upper(), succ(x2.lower()));
        return false;    
    }

};

// NOTE structural similarities between empty and exclusive_less! 
// emptieness can be defined as being exclusive less to oneself.
template <class DomainT, ITL_COMPARE Compare>
bool interval<DomainT,Compare>::exclusive_less(const interval& x2)const
{
    using namespace boost::mpl;
    if(is_right(closed_bounded) && x2.is_left(closed_bounded)) return domain_less(_upb, x2._lwb); //_upb < x2._lwb
    if(is_right(open_bounded)   && x2.is_left(closed_bounded)) return domain_less_equal(_upb, x2._lwb); //_upb <= x2._lwb;
    if(is_right(closed_bounded) && x2.is_left(open_bounded) )  return domain_less_equal(_upb, x2._lwb); //_upb <= x2._lwb;

    return 
        if_<
            bool_<is_continuous<DomainT>::value>, 
            continuous_type<interval<DomainT,Compare> >, 
            discrete_type<interval<DomainT,Compare> > 
           >
           ::type::open_bound_less_equal(_upb, x2._lwb);
}


template <class DomainT, ITL_COMPARE Compare>
bool interval<DomainT,Compare>::lower_less(const interval& x2)const
{
    using namespace boost::mpl;
    if(is_left(closed_bounded) && x2.is_left(closed_bounded)) return domain_less(_lwb, x2._lwb);
    if(is_left(open_bounded)   && x2.is_left(open_bounded))   return domain_less(_lwb, x2._lwb);
    if(is_left(closed_bounded) && x2.is_left(open_bounded))   return domain_less_equal(_lwb, x2._lwb);//domain_less_equal(_lwb, x2._lwb);

    // OTHERWISE (is_left(open_bounded)  && x2.is_left(closed_bounded))
    return 
        if_<
            bool_<is_continuous<DomainT>::value>, 
            continuous_type<interval<DomainT,Compare> >, 
            discrete_type<interval<DomainT,Compare> > 
           >
           ::type::open_bound_less(_lwb, x2._lwb);
}

template <class DomainT, ITL_COMPARE Compare>
bool interval<DomainT,Compare>::upper_less(const interval& x2)const
{
    using namespace boost::mpl;
    if(is_right(closed_bounded) && x2.is_right(closed_bounded)) return domain_less(_upb, x2._upb);
    if(is_right(open_bounded)   && x2.is_right(open_bounded))   return domain_less(_upb, x2._upb);
    if(is_right(open_bounded)   && x2.is_right(closed_bounded)) return domain_less_equal(_upb, x2._upb);//domain_less_equal(_upb, x2._upb);

    // OTHERWISE (is_right(closed_bounded)  && x2.is_right(open_bounded))
    return 
        if_<
            bool_<is_continuous<DomainT>::value>, 
            continuous_type<interval<DomainT,Compare> >, 
            discrete_type<interval<DomainT,Compare> > 
           >
           ::type::open_bound_less(_upb, x2._upb);
}


template <class DomainT, ITL_COMPARE Compare>
bool interval<DomainT,Compare>::lower_less_equal(const interval& x2)const
{
    using namespace boost::mpl;
    if(is_left(closed_bounded) && x2.is_left(closed_bounded)) return domain_less_equal(_lwb, x2._lwb);
    if(is_left(open_bounded)   && x2.is_left(open_bounded))   return domain_less_equal(_lwb, x2._lwb);
    if(is_left(open_bounded)   && x2.is_left(closed_bounded)) return domain_less(_lwb, x2._lwb);

    // OTHERWISE (is_left(closed_bounded) && x2.is_left(open_bounded))
    return 
        if_<
            bool_<is_continuous<DomainT>::value>, 
            continuous_type<interval<DomainT,Compare> >, 
            discrete_type<interval<DomainT,Compare> > 
           >
           ::type::open_bound_less_equal(_lwb, x2._lwb);
}


template <class DomainT, ITL_COMPARE Compare>
bool interval<DomainT,Compare>::upper_less_equal(const interval& x2)const
{
    using namespace boost::mpl;
    if(is_right(closed_bounded) && x2.is_right(closed_bounded)) return domain_less_equal(_upb, x2._upb);
    if(is_right(open_bounded)   && x2.is_right(open_bounded))   return domain_less_equal(_upb, x2._upb);
    if(is_right(closed_bounded) && x2.is_right(open_bounded))   return domain_less(_upb, x2._upb);

    // OTHERWISE (is_right(open_bounded)  && x2.is_right(closed_bounded))
    return 
        if_<
            bool_<is_continuous<DomainT>::value>, 
            continuous_type<interval<DomainT,Compare> >, 
            discrete_type<interval<DomainT,Compare> > 
           >
           ::type::open_bound_less_equal(_upb, x2._upb);
}


//NOTE THINK: This implementation is rather interesting wrt. continuous value types.
// An alternative implementation was x.lwb_equal(y)={return x.lower_less_equal(y) && y.lower_less_equal(x)}
template <class DomainT, ITL_COMPARE Compare>
bool interval<DomainT,Compare>::lower_equal(const interval& x2)const
{
    using namespace boost::mpl;
    if(is_left(closed_bounded) && x2.is_left(closed_bounded)) return domain_equal(_lwb, x2._lwb);
    if(is_left(open_bounded)   && x2.is_left(open_bounded)  ) return domain_equal(_lwb, x2._lwb);

    return 
        if_<
            bool_<is_continuous<DomainT>::value>, 
            continuous_interval<interval<DomainT,Compare> >, 
            discrete_interval<interval<DomainT,Compare> > 
           >
           ::type::unaligned_lwb_equal(*this, x2);
}

//NOTE THINK: This implementation is rather interesting wrt. continuous value types.
// An alternative implementation was x.lwb_equal(y)={return x.lower_less_equal(y) && y.lower_less_equal(x)}
template <class DomainT, ITL_COMPARE Compare>
bool interval<DomainT,Compare>::upper_equal(const interval& x2)const
{
    using namespace boost::mpl;
    if(is_right(closed_bounded) && x2.is_right(closed_bounded)) return domain_equal(_upb, x2._upb);
    if(is_right(open_bounded)   && x2.is_right(open_bounded)  ) return domain_equal(_upb, x2._upb);

    return 
        if_<
            bool_<is_continuous<DomainT>::value>, 
            continuous_interval<interval<DomainT,Compare> >, 
            discrete_interval<interval<DomainT,Compare> > 
           >
           ::type::unaligned_upb_equal(*this, x2);
}



template <class DomainT, ITL_COMPARE Compare>
typename interval<DomainT,Compare>::BoundT interval<DomainT,Compare>::lwb_min(const interval& x2)const
{
    if( x2.lower_less(*this) )
        return BoundT(x2._lwb, x2.boundtype());
    else
        return BoundT(_lwb, boundtype());
}

template <class DomainT, ITL_COMPARE Compare>
typename interval<DomainT,Compare>::BoundT interval<DomainT,Compare>::upb_max(const interval& x2)const
{
    if( upper_less(x2) )
        return BoundT(x2._upb, x2.boundtype());
    else
        return BoundT(_upb, boundtype());
}


template <class DomainT, ITL_COMPARE Compare>
typename interval<DomainT,Compare>::BoundT interval<DomainT,Compare>::lwb_max(const interval& x2)const
{
    if( lower_less(x2) )
        return BoundT(x2._lwb, x2.boundtype());
    else
        return BoundT(_lwb, boundtype());
}

template <class DomainT, ITL_COMPARE Compare>
typename interval<DomainT,Compare>::BoundT interval<DomainT,Compare>::upb_min(const interval& x2)const
{
    if( x2.upper_less(*this) )
        return BoundT(x2._upb, x2.boundtype());
    else
        return BoundT(_upb, boundtype());
}


template <class DomainT, ITL_COMPARE Compare>
typename interval<DomainT,Compare>::BoundT interval<DomainT,Compare>::upb_leftOf(const interval& x2)const
{
    return BoundT(x2._lwb, x2.succession_bounds());
}

template <class DomainT, ITL_COMPARE Compare>
typename interval<DomainT,Compare>::BoundT interval<DomainT,Compare>::lwb_rightOf(const interval& x2)const
{
    return BoundT(x2._upb, x2.succession_bounds());
}


// NOTE non symmetric version: *this[upb].touches(x2[lwb])   
template <class DomainT, ITL_COMPARE Compare>
bool interval<DomainT,Compare>::touches(const interval& x2)const
{
    using namespace boost::mpl;
    if(is_right(open_bounded) && x2.is_left(closed_bounded)) return domain_equal(_upb, x2._lwb);
    if(is_right(closed_bounded) && x2.is_left(open_bounded)) return domain_equal(_upb, x2._lwb);

    return 
        if_<
            bool_<is_continuous<DomainT>::value>, 
            continuous_interval<interval<DomainT,Compare> >, 
            discrete_interval<interval<DomainT,Compare> > 
           >
           ::type::has_equal_border_touch(*this, x2);
}

template <class DomainT, ITL_COMPARE Compare>
bool interval<DomainT,Compare>::contains(const DomainT& x)const
{
    if(is_right(closed_bounded) && is_left(closed_bounded)) return domain_less_equal(_lwb, x) && domain_less_equal(x, _upb);
    if(is_right(closed_bounded) && is_left(open_bounded)  ) return domain_less(_lwb, x)       && domain_less_equal(x, _upb);
    if(is_right(open_bounded)   && is_left(closed_bounded)) return domain_less_equal(_lwb, x) && domain_less(x, _upb);
                                                  return domain_less(_lwb, x)       && domain_less(x, _upb);
}

template <class DomainT, ITL_COMPARE Compare>
bool interval<DomainT,Compare>::contained_in(const interval& super)const
{ return super.lower_less_equal(*this) && upper_less_equal(super); }

template <class DomainT, ITL_COMPARE Compare>
bool interval<DomainT,Compare>::contains(const interval& sub)const
{ return lower_less_equal(sub) && sub.upper_less_equal(*this); }


template <class DomainT, ITL_COMPARE Compare>
interval<DomainT,Compare>& interval<DomainT,Compare>::extend(const interval<DomainT,Compare>& x2)
{
    if(x2.empty()) return *this;
    else if(empty())
    {
        *this = x2; 
        return *this;
    }
    else 
    {
        set_lwb(lwb_min(x2));
        set_upb(upb_max(x2));
        return *this; 
    } 
}


template <class DomainT, ITL_COMPARE Compare>
inline interval<DomainT,Compare>& interval<DomainT,Compare>::left_subtract(const interval& x2)
{
	if(!x2.exclusive_less(*this))
		set_lwb( BoundT(x2._upb, x2.succession_bounds()) );
    return *this; 
}

template <class DomainT, ITL_COMPARE Compare>
inline interval<DomainT,Compare>& interval<DomainT,Compare>::right_subtract(const interval& x2)
{
	if(!exclusive_less(x2))
		set_upb( BoundT(x2._lwb, x2.succession_bounds()) );
    return *this; 
}

template <class DomainT, ITL_COMPARE Compare>
void interval<DomainT,Compare>::right_subtract(interval<DomainT,Compare>& lsur, const interval<DomainT,Compare>& x2)const
{
    if(lower_less(x2)) {
        lsur.set_lwb( BoundT(_lwb,boundtype()) );
		if(exclusive_less(x2))
			lsur.set_upb( BoundT(_upb,boundtype()) );
		else
            lsur.set_upb( upb_leftOf(x2) );
    }
    else lsur.clear();
}

template <class DomainT, ITL_COMPARE Compare>
void interval<DomainT,Compare>::left_subtract(interval<DomainT,Compare>& rsur, const interval<DomainT,Compare>& x2)const
{
    if(x2.upper_less(*this)) {
		if(x2.exclusive_less(*this))
            rsur.set_lwb( BoundT(_lwb,boundtype()) ); 
		else
            rsur.set_lwb(lwb_rightOf(x2)); 
        rsur.set_upb( BoundT(_upb,boundtype()) );
    }
    else rsur.clear();
}


template <class DomainT, ITL_COMPARE Compare>
const std::string interval<DomainT,Compare>::as_string()const
{
    std::string itvRep("");
    std::string lwbRep, ubpRep;

    itvRep += is_left(open_bounded) ? "(" : "[" ;
    itvRep += itl::to_string<DomainT>::apply(_lwb);
    itvRep += ",";
    itvRep += itl::to_string<DomainT>::apply(_upb);
    itvRep += is_right(open_bounded) ? ")" : "]" ;

    return itvRep;
}

template <class DomainT, ITL_COMPARE Compare>
DomainT interval<DomainT,Compare>::first()const
{
    BOOST_ASSERT((!itl::is_continuous<DomainT>::value));
    return is_left(closed_bounded) ? _lwb : succ(_lwb); 
}

template <class DomainT, ITL_COMPARE Compare>
DomainT interval<DomainT,Compare>::last()const
{ 
    BOOST_ASSERT((!itl::is_continuous<DomainT>::value));
    return is_right(closed_bounded) ? _upb : pred(_upb); 
}

template <class DomainT, ITL_COMPARE Compare>
typename interval<DomainT,Compare>::size_type interval<DomainT,Compare>::cardinality()const
{
    using namespace boost::mpl;
    return if_<
                bool_<is_continuous<DomainT>::value>,
                continuous_interval<interval<DomainT,Compare> >,
                discrete_interval<interval<DomainT,Compare> >
              >
              ::type::cardinality(*this);
}

template <class DomainT, ITL_COMPARE Compare>
typename interval<DomainT,Compare>::difference_type interval<DomainT,Compare>::length()const
{
    using namespace boost::mpl;
    return if_<
                bool_<is_continuous<DomainT>::value>,
                continuous_interval<interval<DomainT,Compare> >,
                discrete_interval<interval<DomainT,Compare> >
              >
              ::type::length(*this);
}

template <class DomainT, ITL_COMPARE Compare>
interval<DomainT,Compare>& interval<DomainT,Compare>::as(bound_type bounded)
{ 
    switch(bounded)
    {
	case itl::closed_bounded: set(first(), last(), bounded);              break;
    case itl::right_open    : set(first(), succ(last()), bounded);        break;
    case itl::left_open     : set(pred(first()), last(), bounded);        break;
    case itl::open_bounded  : set(pred(first()), succ(last()), bounded);  break;
    }
	return *this;
}

//==============================================================================
//= Equivalences and Orderings
//==============================================================================
/** Equality on intervals */
template <class DomainT, ITL_COMPARE Compare>
inline bool operator == (const interval<DomainT,Compare>& lhs, const interval<DomainT,Compare>& rhs)
{
	return (lhs.empty() && rhs.empty()) || (lhs.lower_equal(rhs) && lhs.upper_equal(rhs));
}

template <class DomainT, ITL_COMPARE Compare>
inline bool operator != (const interval<DomainT,Compare>& lhs, const interval<DomainT,Compare>& rhs)
{ return !(lhs == rhs); }

/** Strict weak less oredering on intervals */
template <class DomainT, ITL_COMPARE Compare>
inline bool operator < (const interval<DomainT,Compare>& lhs, const interval<DomainT,Compare>& rhs)
{
	if(lhs.empty()) return !rhs.empty();
	else return lhs.lower_less(rhs) || (lhs.lower_equal(rhs) && lhs.upper_less(rhs)); 
}

template <class DomainT, ITL_COMPARE Compare>
inline bool operator > (const interval<DomainT,Compare>& lhs, const interval<DomainT,Compare>& rhs)
{ return rhs < lhs; }

template <class DomainT, ITL_COMPARE Compare>
inline bool operator <= (const interval<DomainT,Compare>& lhs, const interval<DomainT,Compare>& rhs)
{ return !(lhs > rhs); }

template <class DomainT, ITL_COMPARE Compare>
inline bool operator >= (const interval<DomainT,Compare>& lhs, const interval<DomainT,Compare>& rhs)
{ return !(lhs < rhs); }


/// Comparison functor on intervals implementing an overlap free less 
template <class IntervalType>
struct exclusive_less {
    /** Operator <tt>operator()</tt> implements a strict weak ordering on intervals. */
    bool operator()(const IntervalType& x1, const IntervalType& x2)const
    { return x1.exclusive_less(x2); }
};


// ----------------------------------------------------------------------------
// operators
// ----------------------------------------------------------------------------
template <class DomainT, ITL_COMPARE Compare>
inline itl::interval<DomainT,Compare> operator & (const itl::interval<DomainT,Compare>& left, 
                                           const itl::interval<DomainT,Compare>& right)
{
	return itl::interval<DomainT,Compare>(left) &= right;
}

template<class CharType, class CharTraits, class DomainT, ITL_COMPARE Compare>
std::basic_ostream<CharType, CharTraits> &operator<<
  (std::basic_ostream<CharType, CharTraits> &stream, interval<DomainT,Compare> const& x)
{
    if(x.empty())
        return stream << "[]";
    else
    {
        return stream << std::string(x.is_left(open_bounded) ? "(" : "[")
                      << x.lower() << "," << x.upper()
                      << std::string(x.is_right(open_bounded)? ")" : "]");
    }
}



}} // namespace itl boost

#endif

