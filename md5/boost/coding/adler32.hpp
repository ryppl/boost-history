// Boost coding/adler32.hpp header file  -------------------------------------//
/** \file
    \brief  Declarations of Adler-32 computation components.

    \author  Daryle Walker

    Contains the declaration of types and functions used for computing Adler-32
    checksums of given data blocks and granting I/O capability to any applicable
    types.
 
    (C) Copyright Daryle Walker 2008.  Distributed under the Boost Software
    License, Version 1.0. (See the accompanying file LICENSE_1_0.txt or a copy
    at <http://www.boost.org/LICENSE_1_0.txt>.)
 */
// See <http://www.boost.org/libs/coding> for documentation.

#ifndef BOOST_CODING_ADLER32_HPP
#define BOOST_CODING_ADLER32_HPP

#include <boost/coding_fwd.hpp>

#include <boost/assert.hpp>               // for BOOST_ASSERT
#include <boost/coding/coding_shell.hpp>  // for boost:coding:byte_coding_shell
#include <boost/concept/assert.hpp>       // for BOOST_CONCEPT_ASSERT
#include <boost/concept_check.hpp>        // for boost::Integer
#include <boost/cstdint.hpp>              // for boost::uint_least32_t
#include <boost/integer.hpp>              // for b:sized_integral, fast_integral
#include <boost/integer_traits.hpp>       // for boost::integer_traits
#include <boost/mpl/integral_c.hpp>       // for boost::mpl::integral_c
#include <boost/mpl/size_t.hpp>           // for boost::mpl::size_t
#include <boost/serialization/access.hpp> // for boost::serialization::access
#include <boost/serialization/nvp.hpp>    // for boost::serialization::make_nvp
#include <boost/serialization/split_member.hpp>  // for boost:s11n:split_member

#include <climits>  // for CHAR_BIT
#include <cstddef>  // for std::size_t


//  Control-macro definitions  -----------------------------------------------//

// Control macro for sum-queuing
#ifndef BOOST_CODING_ADLER32_CONTROL_USES_THRESHOLD
/** \brief  Preprocessor control flag for less-frequent modulo reduction for
            Adler-32.

    Indicates whether \c boost::coding::adler32_context should keep its sums
    running in over-sized registers as long as possible before a modulo
    reduction or always reduce each sum as each byte comes in.  Even if active,
    the optimization will not happen unless the bits-per-byte count,
    \c CHAR_BIT, is eight.  (The checksum was designed the common 8/16/32-bit
    computer system setup.)  If not overriden, it defaults to 1, activating the
    optimization if possible.

    \see  boost::coding::adler32_context
 */
#define BOOST_CODING_ADLER32_CONTROL_USES_THRESHOLD  1
#endif


namespace boost
{
namespace coding
{


//  Adler-32 checksum core computation class declaration  --------------------//

/** \brief  A computer that produces Adler-32 checksums from consuming bytes.

    This class is the bare-bones engine for the Adler-32 checksum algorithm
    described in RFC 1950.  Besides computation, it also supports comparisons
    (equivalence only, not ordering) and serialization.

    \see  BOOST_CODING_ADLER32_CONTROL_USES_THRESHOLD
 */
class adler32_context
{
    typedef adler32_context  self_type;

public:
    // Types
    /** \brief  Number of bits for checksums, as an integral meta-constant

        Represents the number of significant (low-order) bits kept for the
        checksum, as given in RFC 1950, section 2.2, last two paragraphs.
     */
    typedef mpl::size_t< 32u >  bits_per_checksum;

    /** \brief  Type of the produced output

        Represents the result type, the checksums from hashing.
     */
    typedef sized_integral< bits_per_checksum::value, unsigned >::type
      product_type;
    /** \brief  Type of the consumed input

        Represents the argument type, the data to hash.
     */
    typedef unsigned char  consumed_type;

    // Lifetime management (use automatic copy constructor and destructor)
    //! Default construction
              adler32_context();
    //! Previous-run construction
    explicit  adler32_context( product_type checksum );

    /*! \name Operators */ //@{
    // Operators (use automatic copy-assignment)
    //! Application, consumer
    void          operator ()( consumed_type byte );
    //! Application, consumer via another context
    template < typename SizeType >
    void          operator ()( self_type const &o, SizeType length );
    //! Equals
    bool          operator ==( self_type const &o ) const;
    //! Not-equals
    bool          operator !=( self_type const &o ) const;
    //! Application, producer
    product_type  operator ()() const;//@}

private:
    friend class adler32_computer;

    // Implementation types and meta-constants
    typedef fast_integral<sized_integral< bits_per_checksum::value + (CHAR_BIT
     >= bits_per_checksum::value), unsigned >::type>::type  sum_type;

    typedef mpl::integral_c<sum_type, 65521u>       primer;
    typedef mpl::integral_c<sum_type, (1UL << 16)>  shift_multiplier;

#if ( CHAR_BIT == 8 ) && BOOST_CODING_ADLER32_CONTROL_USES_THRESHOLD
    typedef mpl::size_t<5803u>  initial_threshold;  // by Daryle Walker
    typedef mpl::size_t<5552u>    reset_threshold;  // by Mark Adler
    // These both assume the minimum size of sum_type, 32-bits.  Recalculating
    // the thresholds for longer registers can't readily be done because it
    // involves quadratic equations where the constant term is around -2 times
    // the maximum value of the register's type.  Finding a square-root at
    // compile-time (for the discriminant) is hard enough without having to go
    // bignum.  BTW, I did "manually" do the calculations for 64-bit registers,
    // and the initial threshold goes up to 380,368,696 with the reset threshold
    // being 380,368,439.
#else
    typedef mpl::size_t<0u>  initial_threshold, reset_threshold;  // no queuing
#endif

    // Member data
    sum_type     s1, s2;
    std::size_t  threshold;

    bool  test_invariant() const;

    /*! \name Persistence */ //@{
    // Serialization
    friend class boost::serialization::access;

    //! Write serialization
    template < class Archive >
    void  save( Archive &ar, const unsigned int version ) const;
    //! Read serialization
    template < class Archive >
    void  load( Archive &ar, const unsigned int version );
    //! Enables persistence with Boost.Serialization-compatible archives
    template < class Archive >
    void  serialize( Archive &ar, const unsigned int version );//@}

};  // adler32_context


//  Adler-32 checksum core computation constructor definitions  --------------//

/** Constructs an \c adler32_context set to initial conditions.  That is, with
    the sums initialized as in RFC 1950, section 2.2, last paragraph.

    \post  Sorry, there is no externally-accessible state.  (However,
           <code>std::cout &lt;&lt; this-&gt;operator ()()</code> should write
           &quot;1&quot; to the console.)
 */
inline  adler32_context::adler32_context()  : s1( 1u ), s2( 0u ), threshold(
 initial_threshold::value )  { BOOST_ASSERT( this->test_invariant() ); }

/** Constructs an \c adler32_context set to conditions based on the checksum of
    a previous run.  This works because the mapping from the state conditions
    (that are dependent on the byte sequence) to the checksum is reversible.

    \pre  <code>(<var>checksum</var> &amp; 0xFFFF) &lt; 65,521 &amp;&amp;
          (<var>checksum</var> &gt;&gt; 16) &lt; 65,521</code>.  (This implies
          that \p checksum is less than 2<sup>32</sup>.)

    \param checksum  The checksum of the conditions to restart from.

    \post  Sorry, there is no externally-accessible state.  (However,
           <code>std::cout &lt;&lt; this-&gt;operator ()()</code> should write
           \p checksum to the console.)
 */
inline  adler32_context::adler32_context( product_type checksum )
  : s1( checksum % shift_multiplier::value )
  , s2( checksum / shift_multiplier::value )
  , threshold( reset_threshold::value )
{
    BOOST_ASSERT( (checksum == ( checksum & 0xFFFFFFFFul )) && (this->s1 <
     primer::value) && (this->s2 < primer::value) );
    BOOST_ASSERT( this->test_invariant() );
}


//  Adler-32 checksum core computation member operator func. definitions  ----//

/** Submits a byte for processing.

    \param byte  The byte value to be submitted.

    \post  Sorry, there is no externally-accessible state.  (However, the byte's
           value is used to update the internal hash.)
 */
inline void  adler32_context::operator ()( consumed_type byte )
{
    if ( !this->threshold-- )
    {
        this->s1 %= primer::value;
        this->s2 %= primer::value;
        this->threshold = reset_threshold::value;
    }
    this->s1 += byte;
    this->s2 += this->s1;
    BOOST_ASSERT( this->test_invariant() );
}

/** Submits another context-run for a merged context.

    \pre  \p SizeType is a numeric-integer type.
    \pre  \p o_length is non-negative.

    \tparam SizeType  The type of the length attribute, which should accept
                      conversions to and from the built-in integer types, and
                      hopefully accept 16-bit values without mangling.

    \param o         The context whose checksum will be assimilated.
    \param o_length  The number of bytes submitted into \p o.

    \post  Sorry, there is no externally-accessible state.  (However,
           <var>o</var>'s internal hash is merged with this object's hash, as if
           <var>o</var>'s byte sequence came after this object's original byte
           sequence.)
 */
template < typename SizeType >
void  adler32_context::operator ()( self_type const &o, SizeType o_length )
{
    // Parameter checks
    BOOST_CONCEPT_ASSERT( (boost::Integer<SizeType>) );
    BOOST_ASSERT( o_length >= SizeType(0) );

    // Modulo arithmetic -> an operand is just as good after reduction
    sum_type const  l = o_length % SizeType( primer::value );

    this->s1 %= primer::value;
    this->s2 %= primer::value;
    this->threshold = reset_threshold::value;

    // Adapted from zlib's "adler32_combine" function and the Internet:
    // Let sequence 1: a, b, c; sequence 2: d, e, f
    // first_s1 = a + b + c + 1, second_s1 = d + e + f + 1
    // first_s2 = 3a + 2b + c + 3, second_s2 = 3d + 2e + f + 3
    // Combined sequence: a, b, c, d, e, f
    // combined_s1 = a + b + c + d + e + f + 1
    //             = first_s1 + second_s1 - 1
    //             = (first_s1 - 1) + second_s1
    // combined_s2 = 6a + 5b + 4c + 3d + 2e + f + 6
    //             = (6a + 5b + 4c + 3) + second_s2
    //             = (first_s2 + 3a + 3b + 3c) + second_s2
    //             = (first_s2 + 3(a + b + c)) + second_s2
    //             = (first_s2 + length_second * (first_s1 - 1)) + second_s2
    // In modulo arithmetic, if the register doesn't support negative values,
    // then add (Modulus - X) to 0 instead of subtracting X from it.
    this->s1 += this->s1 ? -static_cast<sum_type>( 1 ) : primer::value - 1u;

    this->s2 += this->s1 * l % primer::value;
    this->s2 += o.s2 % primer::value;
    this->s2 %= primer::value;

    this->s1 += o.s1 % primer::value;
    this->s1 %= primer::value;
    BOOST_ASSERT( this->test_invariant() );
}

/** Compares computation contexts for equivalence.  Such contexts are equal if
    their internal states are equal.  (This means that they should both return
    the same checksum, and continue to do so as long as the same byte sequence
    is submitted to both contexts.)

    \param o  The right-side operand to be compared.

    \retval true   \c *this and \p o are equivalent.
    \retval false  \c *this and \p o are not equivalent.
 */
inline bool  adler32_context::operator ==( self_type const &o ) const
{
    return ( (this->s1 % primer::value) == (o.s1 % primer::value) ) &&
           ( (this->s2 % primer::value) == (o.s2 % primer::value) );
}

/** Compares computation contexts for non-equivalence.  Such engines are unequal
    if their internal states are unequal.  (Note that it is possible for two
    contexts with differing input histories to end up with the same internal
    state, and therefore output checksum.  It may be possible to deliberately
    create such a collision.)

    \param o  The right-side operand to be compared.

    \retval true   \c *this and \p o are not equivalent.
    \retval false  \c *this and \p o are equivalent.
 */
inline bool  adler32_context::operator !=( self_type const &o ) const
{ return !this->operator ==( o ); }

/** Provides the computed check-sum of all the submitted bytes, through a
    standard generator interface.

    \return  The check-sum.
 */
inline adler32_context::product_type  adler32_context::operator ()() const
{
    return ( this->s2 % primer::value ) * shift_multiplier::value + ( this->s1 %
     primer::value );
}


//  Adler-32 checksum core computation private member function definitions  --//

// Make sure that a new entry doesn't blow the registers
inline bool  adler32_context::test_invariant() const
{
    sum_type const  sum_limit = boost::integer_traits<sum_type>::const_max -
     primer::value;

    return !this->threshold || this->s1 < sum_limit && this->s2 < sum_limit;
}

/** Perserves a core computer to a serialization deflated to an archive using
    the Boost.Serialization protocols.  This member function is meant to be
    called only by the Boost.Serialization system, as needed.

    \tparam Archive  The type of \p ar.  It must conform to the requirements
                     Boost.Serialization expects of archiving classes.

    \param ar       The archiving object that this object's representation will
                    be streamed to.
    \param version  The version of the persistence format for this object.  (It
                    should be zero, since this type just got created.)
 */
template < class Archive >
void  adler32_context::save( Archive &ar, const unsigned int version ) const
{
    using boost::serialization::make_nvp;

    sum_type const  temp_s1 = this->s1 % primer::value, temp_s2 = this->s2 %
     primer::value;

    switch ( version )
    {
    default:
    case 0u:
        ar << make_nvp( "s1", temp_s1 ) << make_nvp( "s2", temp_s2 );
        break;
    }
}

/** Resets a core computer to a serialization inflated from an archive using the
    Boost.Serialization protocols.  This member function is meant to be called
    only by the Boost.Serialization system, as needed.

    \tparam Archive  The type of \p ar.  It must conform to the requirements
                     Boost.Serialization expects of archiving classes.

    \param ar       The archiving object that this object's representation will
                    be streamed from.
    \param version  The version of the persistence format for this object.  (It
                    should be zero, since this type just got created.)
 */
template < class Archive >
void  adler32_context::load( Archive &ar, const unsigned int version )
{
    using boost::serialization::make_nvp;

    switch ( version )
    {
    default:
    case 0u:
        this->threshold = 0;  // The order used protects against mishaps!
        ar >> make_nvp( "s1", this->s1 ) >> make_nvp( "s2", this->s2 );
        break;
    }
    BOOST_ASSERT( this->test_invariant() );
}

/** Streams a core computer to/from an archive using the Boost.Serialization
    protocols.  This member function is meant to be called only by the
    Boost.Serialization system, as needed.

    \tparam Archive  The type of \p ar.  It must conform to the requirements
                     Boost.Serialization expects of archiving classes.

    \param ar       The archiving object that this object's representation will
                    be streamed to/from.
    \param version  The version of the persistence format for this object.  (It
                    should be zero, since this type just got created.)
 */
template < class Archive >
inline void  adler32_context::serialize(Archive &ar, const unsigned int version)
{ boost::serialization::split_member( ar, *this, version ); }


//  Adler-32 checksum computation class declaration  -------------------------//

/** \brief  A class for generating an Adler-32 checksum from submitted data.

    This class can accept data in several runs and produce a hash based on that
    data from the Adler-32 checksum algorithm described in RFC 1950.  It should
    have a similar interface to Boost.CRC, plus specialized function object
    interfaces for byte-level processing (inspired by Boost.Bimap).  Comparisons
    are supported for check-summing purposes, but not ordering.  Persistence is
    supported though Boost.Serialization.

    \see  boost::coding::adler32_context
    \see  boost::coding::adler32(void const*,std::size_t)
 */
class adler32_computer
    : public byte_coding_shell<adler32_context>
{
    typedef byte_coding_shell<adler32_context>  base_type;
    typedef adler32_computer                    self_type;

public:
    // Types
    /** \brief  Number of bits for checksums, as an integral meta-constant

        Represents the number of significant (low-order) bits kept for the
        checksum, as given in RFC 1950, section 2.2, last two paragraphs.
     */
    typedef adler32_context::bits_per_checksum  significant_bits_per_checksum;
    /** \brief  Type of the checksum values

        Represents the type used to return a checksum or any intermediate sums.
     */
    typedef adler32_context::product_type  sum_type;

    // Lifetime management (use automatic copy constructor and destructor)
    //! Default construction
              adler32_computer();
    //! Previous-run construction
    explicit  adler32_computer( sum_type checksum );

    // Inspectors
    //! Returns (modulated) sum of bytes read so far, plus 1
    sum_type  augmented_byte_sum() const;
    //! Returns (modulated) sum of each updated byte-sum
    sum_type  sum_of_byte_sums() const;

    // Assignment
    //! Sets state back to initial conditions
    void  reset();
    //! Sets state to restart from a given checksum
    void  configure( sum_type checksum );
    //! Changes the current state to a copy of another object's
    void  assign( self_type const &c );

    //! Exchanges state with another object
    void  swap( self_type &other );

    // Input processing
    //! Combines two computation runs
    void  append_computation( self_type const &other,std::size_t other_length );

};  // adler32_computer


//  Adler-32 checksum computation constructor definitions  -------------------//

/** Initializes this computer with a default context.  This constructor is
    explicitly provided due to the existence of the other constructor.

    \post  <code>#augmented_byte_sum() == 1</code>.
    \post  <code>#sum_of_byte_sums() == 0</code>.
 */
inline  adler32_computer::adler32_computer()  : base_type()  {}

/** Initializes this computer with a context that starts off with the given
    checksum from a previous run.  This works because the mapping from the state
    conditions (that are dependent on the byte sequence) to the checksum is
    reversible.

    \pre  <code>(<var>checksum</var> &amp; 0xFFFF) &lt; 65,521 &amp;&amp;
          (<var>checksum</var> &gt;&gt; 16) &lt; 65,521</code>.  (This implies
          that \p checksum is less than 2<sup>32</sup>.)

    \param checksum  The checksum of the conditions to restart from.

    \post  <code>#augmented_byte_sum() == <var>checksum</var> &amp; FFFF</code>.
    \post  <code>#sum_of_byte_sums() == <var>checksum</var> &gt;&gt; 16</code>.
 */
inline  adler32_computer::adler32_computer( sum_type checksum )  : base_type(
 adler32_context(checksum) )  {}


//  Adler-32 checksum computation inspector member function definitions  -----//

/** Returns one more than the sum of all the bytes that have been submitted,
    reduced by the checksum-modulus.

    \return  The sum of bytes plus 1.
 */
inline adler32_computer::sum_type  adler32_computer::augmented_byte_sum() const
{ return this->context().s1 % adler32_context::primer::value; }

/** Returns the sum of every value of \c #augmented_byte_sum() after each
    byte-submission, reduced by the checksum-modulus.

    \return  The sum of every (augmented by 1) sum of bytes.
 */
inline adler32_computer::sum_type  adler32_computer::sum_of_byte_sums() const
{ return this->context().s2 % adler32_context::primer::value; }


//  Adler-32 checksum computation assignment member function definitions  ----//

/** Changes an object to be like it was default-constructed.

    \post  <code>#augmented_byte_sum() == 1</code>.
    \post  <code>#sum_of_byte_sums() == 0</code>.
 */
inline void  adler32_computer::reset()  { this->context() = adler32_context(); }

/** Changes an object to be like it was (re)started from a given checksum.

    \pre  <code>(<var>checksum</var> &amp; 0xFFFF) &lt; 65,521 &amp;&amp;
          (<var>checksum</var> &gt;&gt; 16) &lt; 65,521</code>.  (This implies
          that \p checksum is less than 2<sup>32</sup>.)

    \param checksum  The checksum of the conditions to restart from.

    \post  <code>#augmented_byte_sum() == <var>checksum</var> &amp; FFFF</code>.
    \post  <code>#sum_of_byte_sums() == <var>checksum</var> &gt;&gt; 16</code>.
 */
inline void  adler32_computer::configure( sum_type checksum )
{ this->context() = adler32_context( checksum ); }

/** Changes an object to be like the given object.  Only the computation
    elements are copied; no function object proxies are reseated.

    \param c  The object with the new state to be copied.

    \post  <code>#augmented_byte_sum() ==
           <var>c</var>.augmented_byte_sum()</code>.
    \post  <code>#sum_of_byte_sums() == <var>c</var>.sum_of_byte_sums()</code>.
    \post  \c #bytes \e still points to \c *this.
 */
inline void  adler32_computer::assign( self_type const &c )  { *this = c; }

/** Swaps the content of this object with another.  Only the computation
    elements are changed; no function object proxies are reseated.

    \param other  The other object to trade state with this object.

    \post  <code>*this == <var>old_other</var> &amp;&amp; <var>old_this</var> ==
           <var>other</var></code>.
 */
inline void  adler32_computer::swap( self_type &other )
{ std::swap( this->context(), other.context() ); }


//  Adler-32 checksum computation input member function definitions  ---------//

/** Submits the results of another computation into this one.  The merge is as
    if this computer's original byte sequence was followed by <var>other</var>'s
    byte sequence.

    \param other         The computer that will be assimilated.
    \param other_length  The number of bytes submitted into \p other.

    \post  <code>#augmented_byte_sum() ==
           <var>old_this</var>.augmented_byte_sum() +
           <var>other</var>.augmented_byte_sum() - 1</code> (mod 65,521).
    \post  <code>#sum_of_byte_sums() == <var>old_this</var>.sum_of_byte_sums() +
           <var>other</var>.sum_of_byte_sums() + <var>other_length</var> *
           (<var>old_this</var>.augmented_byte_sum() - 1)</code> (mod
           65,521).

    \note  The length information is not kept within this class type because
           that will give a space penalty to any user that does not need to
           combine results (and a time penalty to update said length count).
 */
inline void  adler32_computer::append_computation( self_type const &other,
 std::size_t other_length )
{ this->context()( other.context(), other_length ); }


//  Adler-32 checksum computation miscellaneous function definitions  --------//

/** \brief  Non-member swapping function for \c adler32_computer

    Exchanges the states of two \c adler32_computer objects.  This
    specialization of the algorithm can be called by generic code that uses
    free-function (template) swap assisted with argument dependent lookup.

    \param a  The first object involved in the swap.
    \param b  The second object involved in the swap.

    \post  <code><var>a</var> == <var>old_b</var> &amp;&amp; <var>old_a</var> ==
           <var>b</var></code>

    \see  boost::coding::adler32_computer::swap

    \relates  boost::coding::adler32_computer
 */
inline void  swap( adler32_computer &a, adler32_computer &b )  { a.swap( b ); }


//  Adler-32 checksum computation function definition  -----------------------//

/** \brief  Immediate Adler-32 checksum computation

    Determines the Adler-32 checksum of a given block of data, without requiring
    the setup of a computation object.

    \pre  \p buffer must point to a valid region of memory that contains at
          least \p byte_count bytes past the given pointer.

    \param buffer      Points to the beginning of the data block to be
                       processed.
    \param byte_count  The length of the data block to be processed, in bytes.

    \return  The Adler-32 checksum of the data block.

    \see  boost::coding::adler32_context
 */
inline uint_least32_t  adler32( void const *buffer, std::size_t byte_count )
{
    adler32_computer  c;

    c.process_bytes( buffer, byte_count );
    return c.checksum();
}


}  // namespace coding

namespace serialization
{


//  Adler-32 checksum structure serialization template function definition  --//

/** \brief  Enables persistence with Boost.Serialization-compatible archives for
            \c boost::coding::adler32_computer, non-member

    Streams a computer to/from an archive using the Boost.Serialization
    protocols.  This function is meant to be called only by the
    Boost.Serialization system, as needed.

    \tparam Archive  The type of \p ar.  It must conform to the requirements
                     Boost.Serialization expects of archiving classes.

    \param ar       The archiving object that this object's representation will
                    be streamed to/from.
    \param c        The \c adler32_computer object to be serialized or
                    deserialized.
    \param version  The version of the persistence format for this object.  (It
                    should be zero, since this type just got created.)

    \relates  boost::coding::adler32_computer
 */
template < class Archive >
inline void
serialize(Archive &ar, coding::adler32_computer &c, const unsigned int version)
{
    switch ( version )
    {
    default:
    case 0u:
        ar & make_nvp( "context", c.context() );
        break;
    }
}


}  // namespace serialization
}  // namespace boost


#endif  // BOOST_CODING_ADLER32_HPP
