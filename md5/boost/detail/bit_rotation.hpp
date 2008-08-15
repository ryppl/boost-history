// Boost detail/bit_rotation.hpp header file  --------------------------------//
/** \file
    \brief  Definitions of rotating bit-shift functions.

    \author  Daryle Walker

    Contains the definitions of class and function templates that perform
    wrap-around bit-shifts.
 
    (C) Copyright Daryle Walker 2008.  Distributed under the Boost Software
    License, Version 1.0. (See the accompanying file LICENSE_1_0.txt or a copy
    at <http://www.boost.org/LICENSE_1_0.txt>.)
 */

#ifndef BOOST_DETAIL_BIT_ROTATION_HPP
#define BOOST_DETAIL_BIT_ROTATION_HPP

#include <boost/integer.hpp>               // for boost::exact_integral, etc.
#include <boost/integer/integer_mask.hpp>  // for boost::integer_lo_mask


namespace boost
{
namespace detail
{


//  Exact-size integer bit-rotation class and function templates  ------------//

/** \brief  An exact-size left-wise bit-shift rotator.

    This class template makes function objects that left-shift a given integer
    value, but rotate the up-shifted bits back to the low-order positions.

    \pre  \p Bits must match the bit-size of an existing built-in integer type.

    \tparam Bits  The bit-size of the integers to be rotated.
 */
template < int Bits >
struct exact_left_rotator
{
    // Types
    /** \brief  Type of the affected integers

        Represents the integer type used.
     */
    typedef typename boost::exact_integral<Bits, unsigned>::type  value_type;

    /** \brief  Type of the return value

        Represents the integer type of the rotated results.
     */
    typedef value_type  result_type;
    /** \brief  Type of the first argument

        Represents the integer type of the pre-rotated value.
     */
    typedef value_type  first_argument_type;
    /** \brief  Type of the second argument

        Represents the type of the rotation amount.
     */
    typedef int         second_argument_type;

    // Operators
    /** \brief  Application

        Left-shift-rotates a value by a given amount.

        \param x  The value to be shifted.
        \param n  The shift amount.  Since the shift rotates any bits that go
                  past the top, \p n is effectively reduced modulo \p Bits.

        \return  The rotated value.  The lower <code>Bits - <var>n</var> %
                 Bits</code> bits are shifted to the high-order spots, and the
                 former <code><var>n</var> % Bits</code> higher bits go to the
                 low-order spots.
     */
    result_type
    operator ()( first_argument_type x, second_argument_type n ) const
    {
        n %= Bits;
        return ( x << n ) | ( x >> (Bits - n) );
    }

};  // exact_left_rotator

/** \brief  An exact-size right-wise bit-shift rotator.

    This class template makes function objects that right-shift a given integer
    value, but rotate the down-shifted bits back to the high-order positions.

    \pre  \p Bits must match the bit-size of an existing built-in integer type.

    \tparam Bits  The bit-size of the integers to be rotated.
 */
template < int Bits >
struct exact_right_rotator
{
    // Types
    /** \brief  Type of the affected integers

        Represents the integer type used.
     */
    typedef typename boost::exact_integral<Bits, unsigned>::type  value_type;

    /** \brief  Type of the return value

        Represents the integer type of the rotated results.
     */
    typedef value_type  result_type;
    /** \brief  Type of the first argument

        Represents the integer type of the pre-rotated value.
     */
    typedef value_type  first_argument_type;
    /** \brief  Type of the second argument

        Represents the type of the rotation amount.
     */
    typedef int         second_argument_type;

    // Operators
    /** \brief  Application

        Right-shift-rotates a value by a given amount.

        \param x  The value to be shifted.
        \param n  The shift amount.  Since the shift rotates any bits that go
                  past the bottom, \p n is effectively reduced modulo \p Bits.

        \return  The rotated value.  The higher <code>Bits - <var>n</var> %
                 Bits</code> bits are shifted to the low-order spots, and the
                 former <code><var>n</var> % Bits</code> lower bits go to the
                 high-order spots.
     */
    result_type
    operator ()( first_argument_type x, second_argument_type n ) const
    {
        n %= Bits;
        return ( x >> n ) | ( x << (Bits - n) );
    }

};  // exact_right_rotator

/** \brief  Rotating left-bit-shift, exact size

    Left-shift-rotates a value by a given amount.

    \pre  \p Bits must match the bit-size of an existing built-in integer type.

    \tparam Bits  The bit-size of the integers to be rotated.

    \param x  The value to be shifted.
    \param n  The shift amount.  Since the shift rotates any bits that go past
              the top, \p n is effectively reduced modulo \p Bits.

    \return  The rotated value.  The lower <code>Bits - <var>n</var> %
             Bits</code> bits are shifted to the high-order spots, and the
             former <code><var>n</var> % Bits</code> higher bits go to the
             low-order spots.

    \see  boost::detail::exact_left_rotator
 */
template < int Bits >
inline typename boost::exact_integral<Bits, unsigned>::type
left_rotate_exact(typename boost::exact_integral<Bits, unsigned>::type x, int n)
{ return exact_left_rotator<Bits>()( x, n ); } 

/** \brief  Rotating right-bit-shift, exact size

    Right-shift-rotates a value by a given amount.

    \pre  \p Bits must match the bit-size of an existing built-in integer type.

    \tparam Bits  The bit-size of the integers to be rotated.

    \param x  The value to be shifted.
    \param n  The shift amount.  Since the shift rotates any bits that go past
              the bottom, \p n is effectively reduced modulo \p Bits.

    \return  The rotated value.  The higher <code>Bits - <var>n</var> %
             Bits</code> bits are shifted to the low-order spots, and the former
             <code><var>n</var> % Bits</code> lower bits go to the high-order
             spots.

    \see  boost::detail::exact_right_rotator
 */
template < int Bits >
inline typename boost::exact_integral<Bits, unsigned>::type
right_rotate_exact(typename boost::exact_integral<Bits,unsigned>::type x, int n)
{ return exact_right_rotator<Bits>()( x, n ); } 

//  Integer bit-rotation class templates  ------------------------------------//

/** \brief  A left-wise bit-shift rotator.

    This class template makes function objects that left-shift a given integer
    value, but rotate the up-shifted bits back to the low-order positions.

    \pre  <code><var>Bits</var> &lt;=
          std::numeric_limits&lt;boost::uintmax_t&gt;::digits</code>.

    \tparam Bits  The bit-size of the integers to be rotated.  If the integer
                  type is longer than \p Bits, the high-order bits are ignored
                  during rotation.

    \todo  When \p Bits is an exact integer size, use
           \c boost::detail::exact_left_rotator to eliminate masking code.
 */
template < int Bits >
struct left_rotator
{
    // Types
    /** \brief  Type of the affected integers

        Represents the integer type used.
     */
    typedef typename boost::sized_integral<Bits, unsigned>::type  value_type;
    /** \brief  Type of the return value

        Represents the integer type of the rotated results.
     */
    typedef typename boost::fast_integral<value_type>::type  result_type;
    /** \brief  Type of the first argument

        Represents the integer type of the pre-rotated value.
     */
    typedef typename boost::fast_integral<value_type>::type first_argument_type;
    /** \brief  Type of the second argument

        Represents the type of the rotation amount.
     */
    typedef int  second_argument_type;

    // Operators
    /** \brief  Application

        Left-shift-rotates a value by a given amount.

        \param x  The value to be shifted.
        \param n  The shift amount.  Since the shift rotates any bits that go
                  past the top, \p n is effectively reduced modulo \p Bits.

        \return  The rotated value.  The lower <code>Bits - <var>n</var> %
                 Bits</code> bits are shifted to the high-order spots, and the
                 former <code><var>n</var> % Bits</code> higher bits go to the
                 low-order spots.
     */
    result_type
    operator ()( first_argument_type x, second_argument_type n ) const
    {
        typedef boost::integer_lo_mask<Bits>  mask_type;

        n %= Bits;
        return ( (x << n) | (( x & mask_type::value ) >> ( Bits - n )) ) &
         mask_type::value;
    }

};  // left_rotator

/** \brief  A right-wise bit-shift rotator.

    This class template makes function objects that right-shift a given integer
    value, but rotate the down-shifted bits back to the high-order positions.

    \pre  <code><var>Bits</var> &lt;=
          std::numeric_limits&lt;boost::uintmax_t&gt;::digits</code>.

    \tparam Bits  The bit-size of the integers to be rotated.  If the integer
                  type is longer than \p Bits, the high-order bits are ignored
                  during rotation.

    \todo  When \p Bits is an exact integer size, use
           \c boost::detail::exact_right_rotator to eliminate masking code.
 */
template < int Bits >
struct right_rotator
{
    // Types
    /** \brief  Type of the affected integers

        Represents the integer type used.
     */
    typedef typename boost::sized_integral<Bits, unsigned>::type  value_type;
    /** \brief  Type of the return value

        Represents the integer type of the rotated results.
     */
    typedef typename boost::fast_integral<value_type>::type  result_type;
    /** \brief  Type of the first argument

        Represents the integer type of the pre-rotated value.
     */
    typedef typename boost::fast_integral<value_type>::type first_argument_type;
    /** \brief  Type of the second argument

        Represents the type of the rotation amount.
     */
    typedef int  second_argument_type;

    // Operators
    /** \brief  Application

        Right-shift-rotates a value by a given amount.

        \param x  The value to be shifted.
        \param n  The shift amount.  Since the shift rotates any bits that go
                  past the bottom, \p n is effectively reduced modulo \p Bits.

        \return  The rotated value.  The higher <code>Bits - <var>n</var> %
                 Bits</code> bits are shifted to the low-order spots, and the
                 former <code><var>n</var> % Bits</code> lower bits go to the
                 high-order spots.
     */
    result_type
    operator ()( first_argument_type x, second_argument_type n ) const
    {
        typedef boost::integer_lo_mask<Bits>  mask_type;

        n %= Bits;
        return ( (( x & mask_type::value ) >> n) | (x << ( Bits - n )) ) &
         mask_type::value;
    }

};  // right_rotator

/** \brief  Rotating left-bit-shift

    Left-shift-rotates a value by a given amount.

    \pre  There must be an integer type with a bit length of \p Bits at least.

    \tparam Bits  The bit-size of the integers to be rotated.

    \param x  The value to be shifted.
    \param n  The shift amount.  Since the shift rotates any bits that go past
              the top, \p n is effectively reduced modulo \p Bits.

    \return  The rotated value.  The lower <code>Bits - <var>n</var> %
             Bits</code> bits are shifted to the high-order spots, and the
             former <code><var>n</var> % Bits</code> higher bits go to the
             low-order spots.

    \see  boost::detail::left_rotator
 */
template < int Bits >
inline typename boost::sized_integral<Bits, unsigned>::type
left_rotate( typename boost::sized_integral<Bits, unsigned>::type x, int n )
{ return left_rotator<Bits>()( x, n ); } 

/** \brief  Rotating right-bit-shift

    Right-shift-rotates a value by a given amount.

    \pre  There must be an integer type with a bit length of \p Bits at least.

    \tparam Bits  The bit-size of the integers to be rotated.

    \param x  The value to be shifted.
    \param n  The shift amount.  Since the shift rotates any bits that go past
              the bottom, \p n is effectively reduced modulo \p Bits.

    \return  The rotated value.  The higher <code>Bits - <var>n</var> %
             Bits</code> bits are shifted to the low-order spots, and the former
             <code><var>n</var> % Bits</code> lower bits go to the high-order
             spots.

    \see  boost::detail::right_rotator
 */
template < int Bits >
inline typename boost::sized_integral<Bits, unsigned>::type
right_rotate( typename boost::sized_integral<Bits, unsigned>::type x, int n )
{ return right_rotator<Bits>()( x, n ); } 


}  // namespace detail
}  // namespace boost


#endif  // BOOST_DETAIL_BIT_ROTATION_HPP
