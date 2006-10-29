//  Boost math/impl/big_radix_whole_multiplication.ipp header file  ----------//

//  Copyright 2006 Daryle Walker.  Distributed under the Boost Software License,
//  Version 1.0.  (See the accompanying file LICENSE_1_0.txt or a copy at
//  <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/math/> for the library's home page.

/** \file
    \brief  Part of the implementation for an arbitrary-length specific-radix
            unsigned integer class template

    Contains the definitions some of the public member functions of the
    \c boost::math::big_radix_whole\<\> class template, and some related
    non-member functions.  All of these (member) functions, including some
    operators, perform various multiplications.
 */

#ifndef BOOST_MATH_BIG_RADIX_WHOLE_IMPL_MULTIPLICATION_IPP
#define BOOST_MATH_BIG_RADIX_WHOLE_IMPL_MULTIPLICATION_IPP

#ifndef BOOST_MATH_BIG_RADIX_WHOLE_CORE_HPP
#error "#Include only as part of <boost/math/big_radix_whole_core.hpp>"
#endif

#include <cstdlib>  // for std::div, div_t

#include <boost/assert.hpp>  // for BOOST_ASSERT


//  Radix/bignum/natural extra member function definitions  ------------------//

/** Amplifies the current number by a given (non-negative) value, then increases
    the result by another given (non-negative) value, with both values less than
    the radix.  It should be faster than converting either the short multiplier
    or the short addend to full \c big_radix_whole\<\> objects before
    multiplying and adding.

    \pre  <code>0 &lt;= <var>augend_multiplier</var>, <var>addend</var> &lt;
          Radix</code>

    \param augend_multiplier  The multipiler to the current object in the first
                              step.  The current object becomes the product
                              serving as the augend.
    \param addend             The addend to the current object in the second
                              step.

    \post  <code>*this == <var>old_this</var> * <var>augend_multiplier</var> +
           <var>addend</var></code>
 */
template < int Radix, class Allocator >
void
boost::math::big_radix_whole<Radix, Allocator>::multiply_single_add_single
(
    digit_type  augend_multiplier,
    digit_type  addend
)
{
    BOOST_PRIVATE_WILD_ASSERT( this->test_invariant() );

    BOOST_ASSERT( 0 <= augend_multiplier && augend_multiplier < self_type::radix
     && 0 <= addend && addend < self_type::radix );

    if ( size_type const  s = this->digits_.size() )
    {
        // Append scratch space for the carry
        this->digits_.push_back( 0 );

        // Perform one loop of multiplication
        std::div_t  qr;

        qr.quot = addend;

        for ( size_type  i = 0u ; s >= i ; ++i )
        {
            typename deque_type::reference  digit = this->digits_[ i ];

            qr = std::div( digit * augend_multiplier + qr.quot,
             self_type::radix );
            digit = qr.rem;
        }
        BOOST_ASSERT( 0 == qr.quot );  // scratch place doesn't make carry

        // Handle scratch space, or if augend-multiplier was zero
        this->clear_leading_zeros();
    }
    else if ( addend )
    {
        // 0 * anything1 + anything2 == anything2
        this->digits_.push_back( addend );
    }
    // ELSE: 0 * anything + 0 == 0 -> no change

    BOOST_ASSERT( this->test_invariant() );
}

/** Amplifies the current number by a given (non-negative) value, then decreases
    the result by another given (non-negative) value, with both values less than
    the radix.  It should be faster than converting either the short multiplier
    or the short subtrahend to full \c big_radix_whole\<\> objects before
    multiplying and subtracting.

    \pre  <code>0 &lt;= <var>minuend_multiplier</var>, <var>subtrahend</var>
          &lt; Radix</code>

    \param minuend_multiplier  The multipiler to the current object in the first
                               step.  The current object becomes the product
                               serving as the minuend.
    \param subtrahend          The subtrahend to the current object in the
                               second step.

    \throws boost::math::big_radix_whole_negative_result_error
             The current state describes a value that after multipling it by the
             minuend-multiplier is still less than the subtrahend, which would
             require a negative value for the new state, which is not
             representable.

    \post  <code>*this == <var>old_this</var> * <var>minuend_multiplier</var> -
           <var>subtrahend</var></code>
 */
template < int Radix, class Allocator >
void
boost::math::big_radix_whole<Radix, Allocator>::multiply_single_subtract_single
(
    digit_type  minuend_multiplier,
    digit_type  subtrahend
)
{
    BOOST_PRIVATE_WILD_ASSERT( this->test_invariant() );

    BOOST_ASSERT( 0 <= minuend_multiplier && minuend_multiplier <
     self_type::radix && 0 <= subtrahend && subtrahend < self_type::radix );

    big_radix_whole_negative_result_error const  exception( "attempted to "
     "subtract from a smaller short-product" );

    if ( minuend_multiplier )
    {
        using std::div;

        std::div_t  qr;
        int &       scratch = qr.quot;  // just a memory-saving shortcut

        switch ( size_type const  s = this->digits_.size() )
        {
        default:
            // Since the object's value is at least Radix, the single-digit
            // minuend-multiplier is at least 1, and the subtrahend is at most
            // Radix - 1, the operation can't go negative.

            // Append scratch space for the multiplication carry
            this->digits_.push_back( 0 );

            // Perform one loop of multiplication
            qr.quot = -subtrahend;

            for ( size_type  i = 0u ; s >= i ; ++i )
            {
                typename deque_type::reference  digit = this->digits_[ i ];

                qr = div( digit * minuend_multiplier + qr.quot,
                 self_type::radix );
                digit = qr.rem;
            }
            BOOST_ASSERT( 0 == qr.quot );  // scratch place doesn't make carry

            // Make sure borrows are resolved (due to Standard not specifying
            // how division with negatives distributes the quot/rem signs)
            for ( size_type  j = 0u ; s > j ; ++j )
            {
                typename deque_type::reference  digit = this->digits_[ j ];

                while ( 0 > digit )
                {
                    digit += self_type::radix;
                    --this->digits_[ j + 1u ];
                }
            }
            BOOST_ASSERT( 0 <= this->digits_.back() );  // no negative results

            break;

        case 1u:
            if ( (scratch = this->digits_.front() * minuend_multiplier -
             subtrahend) < 0 )
            {
                throw exception;
            }
            else
            {
                if ( (qr = div( scratch, self_type::radix )).quot )
                {
                    this->digits_.push_back( qr.quot );
                }
                this->digits_.front() = qr.rem;
            }
            break;

        case 0u:
            if ( subtrahend )
            {
                // 0 * anything - any_positive == -any_positive -> banned
                throw exception;
            }
            // ELSE: 0 * anything - 0 == 0 -> no change
            break;
        }

        this->clear_leading_zeros();
    }
    else if ( subtrahend )
    {
        // anything * 0 - any_positive == -any_positive -> banned
        throw exception;
    }
    else
    {
        // anything * 0 - 0 == 0
        this->digits_.clear();
    }

    BOOST_ASSERT( this->test_invariant() );
}

/** Amplifies the current number by a given (non-negative) value that is less
    than the radix.  It should be faster than converting the short multiplier to
    a full \c big_radix_whole\<\> before multiplying.

    \pre  <code>0 &lt;= <var>multiplier</var> &lt; Radix</code>

    \param multiplier  The multipiler to the current object

    \post  <code>*this == <var>old_this</var> * <var>multiplier</var></code>

    \see  #multiply_single_add_single
 */
template < int Radix, class Allocator >
inline
void
boost::math::big_radix_whole<Radix, Allocator>::multiply_single
(
    digit_type  multiplier
)
{
    this->multiply_single_add_single( multiplier, 0 );
}

/** Increases the current number by the product of two (non-negative) values,
    with both factors less than the radix.  It should be faster than converting
    either of the factors or the product to full \c big_radix_whole\<\> objects
    before (possibly multiplying and) adding.

    \pre  <code>0 &lt;= <var>addend_multiplicand</var>,
          <var>addend_multiplier</var> &lt; Radix</code>

    \param addend_multiplicand  The multiplicand in the product-addend
    \param addend_multiplier    The multiplier in the product-addend

    \post  <code>*this == <var>old_this</var> + <var>addend_multiplicand</var> *
           <var>addend_multiplier</var></code>
 */
template < int Radix, class Allocator >
void
boost::math::big_radix_whole<Radix, Allocator>::add_single_product
(
    digit_type  addend_multiplicand,
    digit_type  addend_multiplier
)
{
    BOOST_PRIVATE_WILD_ASSERT( this->test_invariant() );

    BOOST_ASSERT( 0 <= addend_multiplicand && addend_multiplicand <
     self_type::radix && 0 <= addend_multiplier && addend_multiplier <
     self_type::radix );

    if ( digit_type const  product = addend_multiplicand * addend_multiplier )
    {
        // Current size determines the allocation strategy
        switch ( size_type const  s = this->digits_.size() )
        {
        default:
            // Spare digit space for cascading addition carry
            this->digits_.push_back( 0 );
            break;

        case 1u:
        case 0u:
            // With the constraints on the factors, the final result can never
            // go to three digits, so just allocate two.
            this->digits_.insert( this->digits_.end(), 2u - s, 0 );
            break;
        }

        size_type const  ss = this->digits_.size();

        BOOST_ASSERT( ss >= 2u );

        // Determine the high and low digits of the 2-digit-max product
        std::div_t const  product_hl = std::div( product, self_type::radix );

        this->digits_[ 0u ] += product_hl.rem;
        this->digits_[ 1u ] += product_hl.quot;

        // Propagate carries, noting that the highest-placed digit should never
        // propagate a carry of its own.
        size_type const  rs = ss - 1u;

        for ( size_type  i = 0u ; rs > i ; ++i )
        {
            while ( self_type::radix <= this->digits_[i] )
            {
                this->digits_[ i ] -= self_type::radix;
                ++this->digits_[ i + 1u ];
            }
        }
        BOOST_ASSERT( this->digits_[rs] < self_type::radix );

        // Remove unused highest-placed zeros
        this->clear_leading_zeros();
    }
    // ELSE: anything + 0 == anything -> no change

    BOOST_ASSERT( this->test_invariant() );
}

/** Decreases the current number by the product of two (non-negative) values,
    with both factors less than the radix.  It should be faster than converting
    either of the factors or the product to full \c big_radix_whole\<\> objects
    before (possibly multiplying and) subtracting.

    \pre  <code>0 &lt;= <var>subtrahend_multiplicand</var>,
          <var>subtrahend_multiplier</var> &lt; Radix</code>

    \param subtrahend_multiplicand  The multiplicand in the product-subtrahend
    \param subtrahend_multiplier    The multiplier in the product-subtrahend

    \throws boost::math::big_radix_whole_negative_result_error
             The current state describes a value less than the
             product-subtrahend, which would require a negative value for the
             new state, which is not representable.

    \post  <code>*this == <var>old_this</var> -
           <var>subtrahend_multiplicand</var> *
           <var>subtrahend_multiplier</var></code>
 */
template < int Radix, class Allocator >
void
boost::math::big_radix_whole<Radix, Allocator>::subtract_single_product
(
    digit_type  subtrahend_multiplicand,
    digit_type  subtrahend_multiplier
)
{
    BOOST_PRIVATE_WILD_ASSERT( this->test_invariant() );

    BOOST_ASSERT( 0 <= subtrahend_multiplicand && subtrahend_multiplicand <
     self_type::radix && 0 <= subtrahend_multiplier && subtrahend_multiplier <
     self_type::radix );

    if ( digit_type const  product = subtrahend_multiplicand *
     subtrahend_multiplier )
    {
        big_radix_whole_negative_result_error const  exception( "attempted to"
         " subtract a larger short-product" );

        // Determine the high and low digits of the 2-digit-max product
        std::div_t const  product_hl = std::div( product, self_type::radix );

        // Do the subtraction
        switch ( size_type const  s = this->digits_.size() )
        {
        default:
            this->digits_.front() -= product_hl.rem;
            if ( 0 > this->digits_.front() )
            {
                this->digits_.front() += self_type::radix;
                --this->digits_[ 1u ];
            }
            this->digits_[ 1u ] -= product_hl.quot;

            for ( size_type  i = 2u ; s > i ; ++i )
            {
                if ( 0 > this->digits_[i - 1u] )
                {
                    this->digits_[ i - 1u ] += self_type::radix;
                    --this->digits_[ i ];
                }
                else
                {
                    // The loop should always end here, not at "s > i"
                    break;
                }
            }
            BOOST_ASSERT( 0 <= this->digits_.back() );

            break;

        case 2u:
            if ( this->digits_.back() > product_hl.quot || this->digits_.back()
             == product_hl.quot && this->digits_.front() >= product_hl.rem )
            {
                typename deque_type::reference  r_digit = this->digits_.back(),
                 o_digit = this->digits_.front();

                r_digit -= product_hl.quot;
                o_digit -= product_hl.rem;

                if ( 0 > o_digit )
                {
                    o_digit += self_type::radix;
                    --r_digit;
                }
                BOOST_ASSERT( 0 <= r_digit );

                break;
            }
            else
            {
                throw exception;
            }

        case 1u:
            if ( !product_hl.quot && this->digits_.front() >= product_hl.rem )
            {
                this->digits_.front() -= product_hl.rem;
                break;
            }
            else
            {
                throw exception;
            }

        case 0u:
            // Since the short product is non-zero, it's too big for a
            // zero-valued minuend.
            throw exception;
        }

        // Remove unused highest-placed zeros
        this->clear_leading_zeros();
    }
    // ELSE: anything - 0 == anything -> no change

    BOOST_ASSERT( this->test_invariant() );
}

/** Replaces the current value with the absolute difference between that value
    and the product of two (non-negative) values, with both factors less than
    the radix.  It should be faster than converting either of the factors or
    their product to full \c big_radix_whole\<\> objects before subtracting.

    \pre  <code>0 &lt;= <var>subtrahend_multiplicand</var>,
          <var>subtrahend_multiplier</var> &lt; Radix</code>

    \param subtrahend_multiplicand  The multiplicand in the product-subtrahend
    \param subtrahend_multiplier    The multiplier in the product-subtrahend

    \retval true   The difference was originally negative (i.e.
                   <code><var>subtrahend_multiplicand</var> *
                   <var>subtrahend_multiplier</var> &gt; *this</code>).
    \retval false  The difference was originally non-negative.

    \post  <code>*this == |<var>old_this</var> -
           <var>subtrahend_multiplicand</var> *
           <var>subtrahend_multiplier</var>|</code>
 */
template < int Radix, class Allocator >
bool
boost::math::big_radix_whole<Radix,
 Allocator>::subtract_single_product_absolutely
(
    digit_type  subtrahend_multiplicand,
    digit_type  subtrahend_multiplier
)
{
    BOOST_PRIVATE_WILD_ASSERT( this->test_invariant() );

    size_type const  index = 0u;  // for later

    BOOST_ASSERT( 0 <= subtrahend_multiplicand && subtrahend_multiplicand <
     self_type::radix && 0 <= subtrahend_multiplier && subtrahend_multiplier <
     self_type::radix );
    BOOST_ASSERT( index < this->digits_.max_size() - 1u );

    bool  result = false;

    if ( digit_type const  product = subtrahend_multiplicand *
     subtrahend_multiplier )
    {
        typedef typename deque_type::reference  reference;

        // Determine the high and low digits of the 2-digit-max product
        std::div_t const  product_hl = std::div( product, self_type::radix );
        int const &       product_h = product_hl.quot;
        int const &       product_l = product_hl.rem;

        // Do the subtraction
        size_type  s = this->digits_.size();

        if ( index >= s )  // both digits outside block
        {
            // Append space for product digits and any intermediate zeros
            // (in one shot to reduce number of throw spots)
            this->digits_.insert( this->digits_.end(), index - s + 1u +
             static_cast<unsigned>(0u != product_h), 0 );

            // Add (actually subtract) the product's digits
            reference  back_digit = this->digits_.back();

            s = this->digits_.size();
            if ( product_h )
            {
                reference  next_to_back = this->digits_[ s - 2u ];

                back_digit = -product_h;
                next_to_back = -product_l;

                // Handle borrows
                while ( 0 > next_to_back )
                {
                    next_to_back += self_type::radix;
                    --back_digit;
                }
            }
            else
            {
                // Leave out zero-valued upper digit, just handle lower digit
                back_digit -= product_l;
            }
            BOOST_ASSERT( 0 > back_digit );
        }
        else if ( index + 1u == s )  // digits straddle block
        {
            // Append the new digit space for the subtracted higher digit
            if ( product_h )
            {
                this->digits_.push_back( -product_h );
                s = this->digits_.size();

                // Finish the subtraction
                reference  back_digit = this->digits_.back();
                reference  next_to_back = this->digits_[ s - 2u ];

                next_to_back -= product_l;

                // Handle borrows
                while ( 0 > next_to_back )
                {
                    next_to_back += self_type::radix;
                    --back_digit;
                }
                BOOST_ASSERT( 0 > back_digit );
            }
            else
            {
                // Don't append a zero digit, just handle the lower digit
                this->digits_.back() -= product_l;
                this->clear_leading_zeros();
            }
        }
        else  // both digits inside block
        {
            // Just do the subtraction
            this->digits_[ index + 1u ] -= product_h;
            this->digits_[ index ] -= product_l;

            // Handle borrows
            for ( size_type  i = index + 1u ; i < s ; ++i )
            {
                reference  current = this->digits_[ i ];
                reference  previous = this->digits_[ i - 1u ];

                while ( 0 > previous )
                {
                    previous += self_type::radix;
                    --current;
                }
            }
            this->clear_leading_zeros();
        }

        // A larger subtrahend gives a negative value in the highest place
        // (copied from member function "subtract_shifted_single_absolutely")
        if ( result = (s && ( 0 > this->digits_.back() )) )
        {
            typedef typename deque_type::iterator  iterator;

            // Get the absolute value by negating all the digits...
            iterator const  last_j = this->digits_.end() - 1;
            bool            borrow = false;

            for ( iterator  j = this->digits_.begin() ; last_j > j ; ++j )
            {
                reference  digit = *j;

                // ...and then normalize
                digit += static_cast<int>( borrow );
                if ( borrow = static_cast<bool>(digit) )
                {
                    digit = self_type::radix - digit;
                }
            }

            // Propagate the remaining borrow when negating highest digit
            reference  last_d = *last_j;

            last_d = -last_d - static_cast<int>( borrow );
            this->clear_leading_zeros();
        }
    }
    // ELSE: anything - 0 == anything -> no change

    BOOST_ASSERT( this->test_invariant() );
    return result;
}


//  Radix/bignum/natural operator member function definitions  ---------------//

/*template < int Radix, class Allocator >
boost::math::big_radix_whole<Radix, Allocator> &
boost::math::big_radix_whole<Radix, Allocator>::operator *=
(
    big_radix_whole const &  multiplier
)
{
}*/


//  Radix/bignum/natural miscellaneous function definitions  -----------------//

// Put something here?


#endif  // BOOST_MATH_BIG_RADIX_WHOLE_IMPL_MULTIPLICATION_IPP
