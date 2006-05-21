//  Boost bit reference library header file  ---------------------------------//

//  (C) Copyright Daryle Walker 2002.  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears 
//  in all copies.  This software is provided "as is" without express or
//  implied warranty, and with no claim as to its suitability for any purpose. 

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_BIT_REFERENCE_HPP
#define BOOST_BIT_REFERENCE_HPP

#include <boost/limits.hpp>         // for std::numeric_limits
#include <boost/static_assert.hpp>  // for BOOST_STATIC_ASSERT

#include <cstddef>   // for std::size_t



namespace boost
{


//  Forward declarations  ----------------------------------------------------//

template < typename BitBucket >
    class const_bit_reference;

template < typename BitBucket >
    class bit_reference;


//  Constant bit reference class declaration  --------------------------------//

template < typename BitBucket >
class const_bit_reference
{
    typedef std::numeric_limits<BitBucket>  limits_type;

    BOOST_STATIC_ASSERT( limits_type::is_specialized && !limits_type::is_signed
     && limits_type::is_integer && (limits_type::radix == 2) );

public:
    typedef BitBucket  block_type;

    const_bit_reference( block_type const &ref, std::size_t bit_pos );

    operator bool() const;

    bool  operator !() const;

private:
    const_bit_reference( block_type const &ref, block_type const &mask );

    block_type const &  ref_;
    block_type const    mask_;

    friend class bit_reference<BitBucket>;

};  // boost::const_bit_reference


//  Bit reference class declaration  -----------------------------------------//

template < typename BitBucket >
class bit_reference
{
    typedef bit_reference<BitBucket>        self_type;
    typedef const_bit_reference<BitBucket>  const_reference_type;

public:
    typedef BitBucket  block_type;

    bit_reference( block_type &ref, std::size_t bit_pos );

    operator bool() const;
    operator const_reference_type() const;

    bool  operator !() const;

    self_type &  operator =( const_reference_type const &ref );
    self_type &  operator =( self_type const &ref );
    self_type &  operator =( bool val );

    void  flip();

private:
    block_type &      ref_;
    block_type const  mask_;

};  // boost::bit_reference


//  Constant bit reference class function definitions  -----------------------//

template < typename BitBucket >
const_bit_reference<BitBucket>::const_bit_reference
(
    block_type const &  ref,
    block_type const &  mask
)
    : ref_( ref ), mask_( mask )
{
}

template < typename BitBucket >
const_bit_reference<BitBucket>::const_bit_reference
(
    block_type const &  ref,
    std::size_t         bit_pos
)
    : ref_( ref ), mask_( static_cast<block_type>(1u) << bit_pos )
{
}

template < typename BitBucket >
const_bit_reference<BitBucket>::operator bool
(
) const
{
    return ref_ & mask_;
}

template < typename BitBucket >
bool
const_bit_reference<BitBucket>::operator !
(
) const
{
    return !this->operator bool();
}


//  Bit reference class function definitions  --------------------------------//

template < typename BitBucket >
bit_reference<BitBucket>::bit_reference
(
    block_type &  ref,
    std::size_t   bit_pos
)
    : ref_( ref ), mask_( static_cast<block_type>(1u) << bit_pos )
{
}

template < typename BitBucket >
bit_reference<BitBucket>::operator bool
(
) const
{
    return ref_ & mask_;
}

template < typename BitBucket >
bit_reference<BitBucket>::operator const_bit_reference<BitBucket>
(
) const
{
    return const_reference_type( ref_, mask_ );
}

template < typename BitBucket >
bool
bit_reference<BitBucket>::operator !
(
) const
{
    return !this->operator bool();
}

template < typename BitBucket >
bit_reference<BitBucket> &
bit_reference<BitBucket>::operator =
(
    const_bit_reference<BitBucket> const &  ref
)
{
    if ( !ref != !(*this) )
        this->flip();
    return *this;
}

template < typename BitBucket >
bit_reference<BitBucket> &
bit_reference<BitBucket>::operator =
(
    bit_reference<BitBucket> const &  ref
)
{
    if ( !ref != !(*this) )
        this->flip();
    return *this;
}

template < typename BitBucket >
bit_reference<BitBucket> &
bit_reference<BitBucket>::operator =
(
    bool  val
)
{
    if ( !val != !(*this) )
        this->flip();
    return *this;
}

template < typename BitBucket >
void
bit_reference<BitBucket>::flip
(
)
{
    ref_ ^= mask_;
}


}  // namespace boost


#endif  // BOOST_BIT_REFERENCE_HPP
