/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_CLOSED_INTERVAL_HPP_JOFA_100324
#define BOOST_ITL_CLOSED_INTERVAL_HPP_JOFA_100324

#include <boost/itl/detail/base_interval.hpp>

namespace boost{namespace itl
{

typedef unsigned char bound_type;

template <class SubType, class DomainT, 
          ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT)>
class asymmetric_interval : public base_interval<DomainT,Compare>
{
public:
    typedef base_interval<DomainT,Compare>          base_type;
    typedef asymmetric_interval<SubType,DomainT,Compare> type;
    typedef SubType                                  sub_type;

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    /** Default constructor; yields an empty interval <tt>[0,0)</tt> or <tt>(0,0]</tt> */
    asymmetric_interval() : _lwb(unon<DomainT>::value()), _upb(neutron<DomainT>::value()) 
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
        BOOST_STATIC_ASSERT((!itl::is_continuous<DomainT>::value));
    }

    //NOTE: Compiler generated copy constructor is used

    /** Constructor for a closed singleton interval <tt>[val,val]</tt> */
    explicit asymmetric_interval(const DomainT& val) : 
        _lwb(val), _upb(val)
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
        BOOST_STATIC_ASSERT((!itl::is_continuous<DomainT>::value));
    }

    /** Interval from <tt>low</tt> to <tt>up</tt> with bounds <tt>bounds</tt> */
    asymmetric_interval(const DomainT& low, const DomainT& up) : 
        _lwb(low), _upb(up)
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
        BOOST_STATIC_ASSERT((!itl::is_continuous<DomainT>::value));
    }

    domain_type lower()const{ return _lwb; }
    domain_type upper()const{ return _upb; }

    /** Is the interval empty? */
    bool empty()const{ return domain_less(_upb, _lwb); }

    /** Maximal element of <tt>*this</tt> is less than the minimal element of <tt>x2</tt> */
    bool exclusive_less(const asymmetric_interval& x2)const
    { return domain_less(_upb, x2._lwb); }

    /** There is no gap between <tt>*this</tt> and <tt>x2</tt> but they have no element in common */
    bool touches(const asymmetric_interval& x2)const
    { return domain_equal(_upb, succ(x2._lwb)); }

    bool is_disjoint(const asymmetric_interval& x2)const //JODO ex
    { return exclusive_less(x2) || x2.exclusive_less(*this); }

    sub_type& extend(const sub_type& x2) //JODO ex
    {
        if  (x2.empty()) return *that();
        else if(empty()) return *that() = x2; 
        else 
        {
            if(domain_less(x2._lwb, _lwb))
                _lwb = x2._lwb;
            if(domain_less(_upb, x2._upb))
                _upb = x2._upb;
            return *that(); 
        } 
    }

    /** subtract \c left_minuend from the \c *this interval on it's left side. 
        *this becomes the difference: The part of \c *this right of \c left_minuend.
    \code
    right_over = *this - left_minuend; //on the left.
    ...        d] : right
    ...c]         : left_minuend
         [c+1  d] : right_over = *this
    \endcode
    */
    sub_type& left_subtract(const sub_type& left_minuend)
    {
        if(!left_minuend.exclusive_less(*this))
            _lwb = succ(left_minuend._upb);
        return *that(); 
    }


    /** subtract \c right_minuend from the \c *this interval on it's right side. 
        *this becomes the difference: The part of \c *this right of \c right_minuend.
    \code
    left_over = *this - right_minuend; //on the right side.
    [a         ...  : left
            [b ...  : right_minuend
    [a  b-1]        : left_over = *this
    \endcode
    */
    sub_type& right_subtract(const sub_type& right_minuend)
    {
        if(!exclusive_less(right_minuend))
            _upb = pred(right_minuend._lwb);
        return *that(); 
    }

    /*JODO
    [a           c]
            [b           d]
    [a  b-1][b   c][c+1  d]
    */
    sub_type& operator &= (const sub_type& sectant)
    {
        _lwb = (std::max)(_lwb, sectant._lwb);
        _upb = (std::min)(_upb, sectant._upb);
        return *that();
    }

    //CL REV -- For representation
    bool is_left(bound_type bounded)const
    { return 0 != (bounded & right_open); }

    bool is_right(bound_type bounded)const
    { return bounded==open_bounded || bounded==right_open; }

    //CL REV -- For the generator
    void set(DomainP left, DomainP right, bound_type bounds)
    { _lwb = left; _upb = right; }

    //JODO
    std::string as_string()const
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

    bool lower_equal(const sub_type& x2)const{ return _lwb==x2._lwb; }
    bool upper_equal(const sub_type& x2)const{ return _upb==x2._upb; }
    bool lower_less (const sub_type& x2)const{ return domain_less(_lwb, x2._lwb); }
    bool upper_less (const sub_type& x2)const{ return domain_less(_upb, x2._upb); }


protected:
          sub_type* that()      { return static_cast<      sub_type*>(this); }
    const sub_type* that()const { return static_cast<const sub_type*>(this); }

private:
    domain_type _lwb;
    domain_type _upb;
};


}} // namespace itl boost

#endif

