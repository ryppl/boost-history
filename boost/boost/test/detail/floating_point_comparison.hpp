//  (C) Copyright Gennadiy Rozental 2001.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_FLOATING_POINT_COMAPARISON_HPP
#define BOOST_FLOATING_POINT_COMAPARISON_HPP

#include <boost/limits.hpp>

#include <boost/test/detail/class_properties.hpp>

template<typename FPT>
inline FPT
fpt_abs( FPT arg ) 
{
    return arg < 0 ? -arg : arg;
}

//____________________________________________________________________________//

// both f1 and f2 are unsigned here
template<typename FPT>
inline FPT 
safe_fpt_division( FPT f1, FPT f2 )
{
    return  (f2 < 1 && f1 > f2 * std::numeric_limits<FPT>::max())   ? std::numeric_limits<FPT>::max() :
           ((f2 > 1 && f1 < f2 * std::numeric_limits<FPT>::min() || 
             f1 == 0)                                               ? 0                               :
                                                                      f1/f2 );
}

//____________________________________________________________________________//

template<typename FPT>
class close_at_tolerance {
public:
    explicit    close_at_tolerance( FPT tolerance, bool strong_or_weak = true ) 
    : p_tolerance( tolerance ), m_strong_or_weak( strong_or_weak ) {}

    explicit    close_at_tolerance( int number_of_rounding_errors, bool strong_or_weak = true ) 
    : p_tolerance( std::numeric_limits<FPT>::epsilon() * number_of_rounding_errors/2 ), 
      m_strong_or_weak( strong_or_weak ) {}

    bool        operator()( FPT left, FPT right ) const
    {
        FPT diff = fpt_abs( left - right );
        FPT d1   = safe_fpt_division( diff, fpt_abs( right ) );
        FPT d2   = safe_fpt_division( diff, fpt_abs( left ) );
        
        return m_strong_or_weak ? (d1 <= p_tolerance.get() && d2 <= p_tolerance.get()) 
                                : (d1 <= p_tolerance.get() || d2 <= p_tolerance.get());
    }

    // Data members
    BOOST_READONLY_PROPERTY( FPT, 0, () )
                p_tolerance;
private:
    bool        m_strong_or_weak;
};

//  Revision History
//   9 Dec 01  Initial version (Gennadiy Rozental)

#endif // BOOST_FLOATING_POINT_COMAPARISON_HPP
