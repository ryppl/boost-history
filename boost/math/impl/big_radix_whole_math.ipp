//  Boost math/impl/big_radix_whole_math.ipp header file  --------------------//

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
    common mathematical integer functions.
 */

#ifndef BOOST_MATH_BIG_RADIX_WHOLE_IMPL_MATH_IPP
#define BOOST_MATH_BIG_RADIX_WHOLE_IMPL_MATH_IPP

#ifndef BOOST_MATH_BIG_RADIX_WHOLE_CORE_HPP
#error "#Include only as part of <boost/math/big_radix_whole_core.hpp>"
#endif


//  Radix/bignum/natural extra member function definitions  ------------------//

/** Replaces the current value with its absolute value.

    \post  <code>*this == |<var>old_this</var>|</code>

    \note  Since all the potential values this object can have are nonnegative,
           their absolute values map to themselves.  This means that this
           object's state never changes and no actual action is done.  So this
           member function can be <code>const</code>.
 */
template < int Radix, class Allocator >
inline
void
boost::math::big_radix_whole<Radix, Allocator>::absolve_self
(
) const
{
    // All values map to themselves.  This is the same mapping done in the
    // "identify_self" member function.
    this->identify_self();
}

/** Replaces the current value with its (arithmetic) sign.  That is the
    direction to the value's place in the number line relative to zero.  The
    sign can be zero (for a zero value) or one (for positive values).

    \post  <code>*this == |<var>old_this</var>| / <var>old_this</var></code> if
           <var>old_this</var> represented a non-zero value.
    \post  <code>*this == <var>old_this</var></code> if <var>old_this</var>
           represented zero.
 */
template < int Radix, class Allocator >
inline
void
boost::math::big_radix_whole<Radix, Allocator>::sign_self
(
)
{
    // Non-zero (i.e. positive) values map to one, while zero maps to itself.
    // This mapping is also done in the "verify_self" member function.
    this->verify_self();
}


//  Radix/bignum/natural miscellaneous function definitions  -----------------//

/** \brief  Absolute-value function for \c big_radix_whole\<\>

    Computes the absolute value for the given \c big_radix_whole\<\> object.
    The returned value is the distance between the origin and the given value's
    position on the number line, regardless of direction.  Since all objects of
    the \c big_radix_whole\<\> type represent nonnegative values, this function
    returns its argument.

    \param x  The function's argument.

    \return  \p x

    \see  boost::math::big_radix_whole::absolve_self

    \relates  boost::math::big_radix_whole
 */
template < int Radix, class Allocator >
inline
boost::math::big_radix_whole<Radix, Allocator>
boost::math::abs
(
    boost::math::big_radix_whole<Radix, Allocator> const &  x
)
{
    return x;
}

/** \brief  (Arithmetic) sign function for \c big_radix_whole\<\>

    Computes the sign for the given \c big_radix_whole\<\> object, i.e. the
    signum function.  This is the direction the object's value is on the number
    line, relative to the origin.  Since all objects of the
    \c big_radix_whole\<\> type represent nonnegative values, this function
    returns either 0 or 1, and never -1.

    \param x  The function's argument.

    \retval 0  \p x represents zero.
    \retval 1  \p x represents a positive value.

    \see  boost::math::big_radix_whole::sign_self

    \relates  boost::math::big_radix_whole
 */
template < int Radix, class Allocator >
inline
int
boost::math::sgn
(
    boost::math::big_radix_whole<Radix, Allocator> const &  x
)
{
    return x ? +1 : 0;
}


#endif  // BOOST_MATH_BIG_RADIX_WHOLE_IMPL_MATH_IPP
