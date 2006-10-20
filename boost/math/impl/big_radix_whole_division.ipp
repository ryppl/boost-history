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

#include <algorithm>  // for std::count_if

#include <boost/lambda/lambda.hpp>  // for boost::lamda::_1


//! \cond
//  Implementation details  --------------------------------------------------//

namespace boost
{
namespace math
{
namespace detail
{

// Determine the even/odd status of a digit string.  The method used is
// dependent on the radix.
template < int Radix >
class big_radix_whole_parity_checker
{
    // The method used depends on the even/odd status of the radix
    // (Useless "Rx" parameter added since the compiler needs it.)
    template < int Rx, bool IsOdd >
    class parity_checker
    {
    public:
        template < class DequeType >
        int  operator ()( DequeType const &digits ) const
        {
            // Odd * Any == Any; Odd + Any == ~Any; Even + Any == Any
            // This means that the even/odd status of a particular place depends
            // on the even/odd status of the place's digit.  All those statuses
            // have to be added together for the final status; so a number is
            // odd only if there is an odd number of odd place digits.
            return ::std::count_if( digits.begin(), digits.end(), (lambda::_1 &
             1) != 0 ) % 2;
        }

    };  // parity_checker (1)

    // The method for even radices is quicker
    // (Useless "Rx" parameter added since the compiler can't accept an inner
    // template that's fully specialized.)
    template < int Rx >
    class parity_checker< Rx, false >
    {
    public:
        template < class DequeType >
        int  operator ()( DequeType const &digits ) const
        {
            // Even * Any == Even; Even + Any == Any
            // This means that all the places above the one's place contribute
            // an even number, since the radix (and therefore all its non-zero
            // powers) is even.  The even/odd attribute is depedent solely on
            // whether the one's place is even/odd.
            return digits.empty() ? 0 : ( digits.front() & 1 );
        }

    };  // parity_checker (2)

public:
    template < class DequeType >
    int  operator ()( DequeType const &digits ) const
    {  return parity_checker<Radix, Radix % 2 != 0>()( digits );  }

};  // boost::math::detail::big_radix_whole_parity_checker (1)

// Base-2 is really easy to specialize; little computation needed
template < >
class big_radix_whole_parity_checker< 2 >
{
public:
    template < class DequeType >
    int  operator ()( DequeType const &digits ) const
    {
        // Unlike other even radices, the one's digit is already either 0 or 1
        return digits.empty() ? 0 : digits.front();
    }

};  // boost::math::detail::big_radix_whole_parity_checker (2)

}  // namespace detail
}  // namespace math
}  // namespace boost
//! \endcond


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

/** Gets the resulting remainder if the current number was divided by a given
    positive value less than the radix.  It should be faster than using the full
    short-division routine #div_and_mod_single(digit_type) since this member
    function does not have to write back to, or otherwise change, the allocated
    digits for the dividend.

    \pre  <code>0 &lt; <var>divisor</var> &lt; Radix</code>

    \param divisor  The value that the current object will be divided by

    \return  The remainder after division

    \throws boost::math::big_radix_whole_divide_by_zero_error
             A zero-valued divisor is used.
 */
template < int Radix, class Allocator >
typename boost::math::big_radix_whole<Radix, Allocator>::digit_type
boost::math::big_radix_whole<Radix, Allocator>::modulo_single
(
    digit_type  divisor
) const
{
    BOOST_PRIVATE_WILD_ASSERT( this->test_invariant() );

    BOOST_ASSERT( 0 <= divisor && divisor < self_type::radix );

    if ( !divisor )
    {
        // something % 0 -> banned
        throw big_radix_whole_divide_by_zero_error( "attempted to short-modulo"
         " by zero" );
    }

    // ELSE: perform the short division, working even if the dividend is zero
    typedef typename deque_type::const_reverse_iterator  iterator;

    digit_type      remainder = 0;
    iterator const  e = this->digits_.rend();

    for ( iterator  i = this->digits_.rbegin() ; e != i ; ++i )
    {
        // Shift-in and divide the next dividend digit
        remainder *= self_type::radix;
        remainder += *i;
        remainder %= divisor;
    }

    return remainder;
}

/** Gets the current number's quality of being either even or odd.  This is the
    same as getting a modulo by two.  It should be faster than general division,
    modulo, or shift routines because this member function can take advantage of
    bit twiddling.

    \retval 0  <code>*this</code> is even.
    \retval 1  <code>*this</code> is odd.
 */
template < int Radix, class Allocator >
inline
int
boost::math::big_radix_whole<Radix, Allocator>::parity
(
) const
{
    BOOST_PRIVATE_WILD_ASSERT( this->test_invariant() );

    return detail::big_radix_whole_parity_checker<Radix>()( this->digits_ );
}

/** Gets whether or not the current number is odd (i.e. its remainder modulo two
    is one).

    \retval true   <code>*this</code> is odd.
    \retval false  <code>*this</code> is even.
 */
template < int Radix, class Allocator >
inline
bool
boost::math::big_radix_whole<Radix, Allocator>::is_odd
(
) const
{
    return static_cast<bool>( this->parity() );
}

/** Gets whether or not the current number is even (i.e. its remainder modulo
    two is zero).

    \retval true   <code>*this</code> is even.
    \retval false  <code>*this</code> is odd.
 */
template < int Radix, class Allocator >
inline
bool
boost::math::big_radix_whole<Radix, Allocator>::is_even
(
) const
{
    return !this->is_odd();
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
