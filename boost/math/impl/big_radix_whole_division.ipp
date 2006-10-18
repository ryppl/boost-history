//  Boost math/impl/big_radix_whole_division.ipp header file  ----------------//

//  Copyright 2006 Daryle Walker.  Distributed under the Boost Software License,
//  Version 1.0.  (See the accompanying file LICENSE_1_0.txt or a copy at
//  <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/math/> for the library's home page.

/** \file
    \brief  Part of the implementation for an arbitrary-length specific-radix
            unsigned integer class template

    Contains the definitions some of the public member functions of the
    \c boost::math::big_radix_whole\<\> class template, and some related
    non-member functions.  All of these (member) functions perform various
    divisions.
 */

#ifndef BOOST_MATH_BIG_RADIX_WHOLE_IMPL_DIVISION_IPP
#define BOOST_MATH_BIG_RADIX_WHOLE_IMPL_DIVISION_IPP

#ifndef BOOST_MATH_BIG_RADIX_WHOLE_CORE_HPP
#error "#Include only as part of <boost/math/big_radix_whole_core.hpp>"
#endif

#include <cstdlib>  // for std::div, div_t

#include <boost/assert.hpp>  // for BOOST_ASSERT


//  Radix/bignum/natural extra member function definitions  ------------------//

/** Attenuates the current number by a given positive value less than the radix.
    It should be faster than converting the short divisor to a full
    \c big_radix_whole\<\> object before dividing.

    \pre  <code>0 &lt; <var>divisor</var> &lt; Radix</code>

    \param divisor  The value that the current object will be divided by

    \return  The remainder after division

    \throws boost::math::big_radix_whole_divide_by_zero_error
             A zero-valued divisor is used.

    \post  <code><var>old_this</var> == *this * <var>divisor</var> +
           <var>return_value</var></code>
 */
template < int Radix, class Allocator >
typename boost::math::big_radix_whole<Radix, Allocator>::digit_type
boost::math::big_radix_whole<Radix, Allocator>::div_and_mod_single
(
    digit_type  divisor
)
{
    BOOST_PRIVATE_WILD_ASSERT( this->test_invariant() );

    BOOST_ASSERT( 0 <= divisor && divisor < self_type::radix );

    if ( !divisor )
    {
        // something / 0 -> banned
        throw big_radix_whole_divide_by_zero_error( "attempted to short-divide"
         " by zero" );
    }

    // ELSE: perform the short division, working even if the dividend is zero
    typedef typename deque_type::reverse_iterator  iterator;

    std::div_t     qr = { 0, 0 };
    iterator const  e = this->digits_.rend();

    for ( iterator  i = this->digits_.rbegin() ; e != i ; ++i )
    {
        qr = std::div( qr.rem * self_type::radix + *i, divisor );
        *i = qr.quot;
    }

    this->clear_leading_zeros();

    BOOST_ASSERT( this->test_invariant() );
    return qr.rem;
}


//  Radix/bignum/natural operator member function definitions  ---------------//

/*template < int Radix, class Allocator >
boost::math::big_radix_whole<Radix, Allocator> &
boost::math::big_radix_whole<Radix, Allocator>::operator /=
(
    big_radix_whole const &  divisor
)
{
}*/

/*template < int Radix, class Allocator >
boost::math::big_radix_whole<Radix, Allocator> &
boost::math::big_radix_whole<Radix, Allocator>::operator %=
(
    big_radix_whole const &  divisor
)
{
}*/


//  Radix/bignum/natural miscellaneous function definitions  -----------------//

// Put something here?


#endif  // BOOST_MATH_BIG_RADIX_WHOLE_IMPL_DIVISION_IPP
