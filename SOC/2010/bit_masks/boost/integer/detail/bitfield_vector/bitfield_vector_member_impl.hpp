//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_MEMBER_IMPL_HPP
#define BOOST_BITFIELD_MEMBER_IMPL_HPP
#include "bitfield_vector_base.hpp"
#include "mask_creator.hpp"
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/assert.hpp>
#include <cstring>
#include <boost/integer/high_low_bits.hpp>
#include <boost/integer/bit_width.hpp>
#include <climits>
#include <limits>
#include <boost/mpl/less.hpp>
#include <iterator>
#include "bitfield_vector_iterator_fwd.hpp"

namespace boost { namespace detail {

template <std::size_t Width, bool = bool((Width%CHAR_BIT) > 0)>
struct next_allocation_size;


// NOT divible by 8
template <std::size_t Width>
struct next_allocation_size<Width, true> {
    std::size_t compute_8_bit_boundry(std::size_t bits) {
        if((bits%CHAR_BIT) == 0) {
            return bits;
        }else{
            return bits + std::size_t(CHAR_BIT-(bits%8));
        }
    }

    typedef typename mpl::divides<
        typename mpl::plus<
            mpl::size_t<Width>,
            typename mpl::minus<
                mpl::size_t<CHAR_BIT>,
                typename mpl::modulus<
                    mpl::size_t<Width>,
                    mpl::size_t<CHAR_BIT>
                >::type
            >::type
        >::type,
        mpl::size_t<CHAR_BIT>            
    >::type                 first_allocm_size;
        
        
    std::size_t operator()(std::ptrdiff_t currentm_size, std::size_t bits_used) {
        if(currentm_size == 0) {
            return std::size_t(first_allocm_size::value);
        }
        return compute_8_bit_boundry(bits_used*2)/8;
    }
};

// IS divible by 8
template <std::size_t Width>
struct next_allocation_size<Width,false> {
    typedef typename mpl::divides<
        mpl::size_t<Width>,
        mpl::size_t<CHAR_BIT>
    >::type             first_return_value;

    std::size_t operator()(std::ptrdiff_t currentm_size, std::size_t) {
        if(currentm_size == 0) {
            return first_return_value::value;
        }else{
            return currentm_size * 2;
        }
    }
};


/** This is the proxy reference type used by the iterator and by the 
 *  bitfield_tuple for accessing the storage and correctly returning
 *  and setting the values within the bitfield_vector.
 */

template <  typename RetType,
            std::size_t Width,
            bool = is_signed<RetType>::type::value >
class proxy_reference_type;

template <typename RetType,std::size_t Width>
class proxy_reference_type<RetType,Width,true> {
    typedef proxy_reference_type<RetType,Width,true> _self;
    proxy_reference_type();
public:
    /** Typedefs and integral static constant values. */
    //@{
    typedef unsigned char       storage_type;
    typedef RetType             value_type;
    typedef std::size_t         offset_type;

    typedef typename make_unsigned<value_type>::type unsigned_value_type;
    BOOST_STATIC_CONSTANT( std::size_t, width = Width );
    BOOST_STATIC_CONSTANT(value_type, sign_bit =
        (~value_type(0) <<(bit_width<value_type>::value-1) ));

    BOOST_STATIC_CONSTANT(value_type, stored_sign_bit =
        ( value_type(1) << (width -1)) );

    /** constructors and destructor for the proxy_reference_type type. */
    //@{

    /** Copy Constructor. */
    proxy_reference_type(_self const& x)
        :m_ptr(x.m_ptr), m_mask(x.m_mask )
    { }

    /** pointer, offset constructor. */
    proxy_reference_type(storage_type* ptr, offset_type offset)
        :m_ptr(ptr), m_mask(get_mask_detail<Width>(offset) )
    { }
    //@}

    /** Copy assignment. */
    _self& operator=(_self const& x) {
        m_ptr = x.m_ptr;
        m_mask = x.m_mask;
        return *this;
    }
    /** Implicit conversion operator.*/
    operator value_type() const {

        value_type ret = 0;
        // value_type value_bits = 0;
        if(m_mask.m_size == 1) {
            ret = (value_type( m_mask.m_first_byte & *m_ptr ) >>
                m_mask.m_last_shift);

            if((stored_sign_bit & ret) == stored_sign_bit) {
                ret = (~value_type(0) << (width - 1)) |
                    ((~stored_sign_bit) & ret);
            }
            return ret;
        }

        storage_ptr_t byte_ptr = m_ptr;

        if(m_mask.m_size == 2) {
            ++byte_ptr;
            byte_ptr = m_ptr;
            // getting first bits.
            ret = value_type(m_mask.m_first_byte & *byte_ptr)
                << ( CHAR_BIT - m_mask.m_last_shift );
            ++byte_ptr;
            unsigned_value_type retrieved_value;
            if( m_mask.m_last_byte == 0xFF) {
                ret <<= CHAR_BIT;
                ret += 0xFF;
            }else{
                 retrieved_value = (unsigned_value_type( m_mask.m_last_byte)
                    & unsigned_value_type(*byte_ptr));
                retrieved_value >>= m_mask.m_last_shift;
                ret += retrieved_value;
            }
            if(stored_sign_bit & ret) {
                ret = (~value_type(0) << (width - 1)) |
                    (~stored_sign_bit & ret);
            }
            return ret;
        }
        
        const storage_t all_bits = 0xFF;
        // gettting first byte.

        ret = value_type( m_mask.m_first_byte & *byte_ptr);
        ++byte_ptr;
        
        // getting middle bytes
        for(std::size_t index = 0; index < m_mask.m_size - 2; ++index) {
            ret <<= CHAR_BIT;
            ret += *byte_ptr & all_bits;
            ++byte_ptr;
        }
        // shifting bits
        if(m_mask.m_last_byte == 0xFF) {
            ret <<= CHAR_BIT;
            ret += value_type(*byte_ptr & m_mask.m_last_byte);
        }else{
            ret <<= CHAR_BIT - m_mask.m_last_shift;
            ret += unsigned_value_type( *byte_ptr & m_mask.m_last_byte )
                >> ( m_mask.m_last_shift);
        }
        if(stored_sign_bit & ret) {
            ret = (~value_type(0) << (width - 1)) ^
                (~stored_sign_bit & ret);
        }

        return ret;
    }

    /** value_type storage assignement operator.*/
    _self& operator=(value_type x) {

        unsigned_value_type x_adjusted = ((unsigned_value_type(x & sign_bit) >>
                (bit_width<value_type>::value - width))
            |
        (low_bits_mask<value_type, std::size_t(width-1)>::value & x) );
 
        if(m_mask.m_size == 1) {
            storage_t previous_values = *m_ptr & ~m_mask.m_first_byte;
            storage_t new_value = low_bits_mask<value_type, width>::value &
                x_adjusted;
            new_value <<= m_mask.m_last_shift;
            previous_values |= new_value;
            *m_ptr = previous_values;
            return *this;
        }
        
        storage_t       to_be_stored = 0;
        std::size_t     bits_in_mask = 0;
        value_type      mask         = 0;
        storage_ptr_t   byte_ptr     = m_ptr;

        if(m_mask.m_size == 2) {
            bits_in_mask = CHAR_BIT - m_mask.m_offset;
            mask = (~(~value_type(0) << bits_in_mask))
                << (width - bits_in_mask);
            to_be_stored = storage_t((mask & 
                x_adjusted)>>(width - bits_in_mask));
            *byte_ptr = ((*byte_ptr) & (~m_mask.m_first_byte)) | to_be_stored;
            
            ++byte_ptr;
            bits_in_mask = width - bits_in_mask;
            mask = ~(~value_type(0) << bits_in_mask);
            to_be_stored = storage_t((mask & x_adjusted) << m_mask.m_last_shift);
            *byte_ptr = (*byte_ptr & ~m_mask.m_last_byte) | to_be_stored;
            return *this;
        }
        // calculate the offset of the first bit within x
        // and creating a mask to extract the fist bits from within x
        bits_in_mask = CHAR_BIT - m_mask.m_offset;
        mask = m_mask.m_first_byte;
        mask <<= width - bits_in_mask;

        *byte_ptr = (*byte_ptr & ~m_mask.m_first_byte) | ((x_adjusted & mask )
            >> (width - bits_in_mask));
        ++byte_ptr;
        mask = 0xFF;
        mask <<= width - bits_in_mask - CHAR_BIT;
        for(std::size_t index = 0; index < m_mask.m_size - 2;++index) {
            *byte_ptr = (mask & x_adjusted) >> (width - (bits_in_mask +
                (CHAR_BIT * index))- CHAR_BIT);
            mask >>= CHAR_BIT;
            ++byte_ptr;
        }
        // now calculating the last bytes information, retrieving it and then
        // storing the data within the array.
        if(m_mask.m_last_byte == 0xFF) {
            *byte_ptr = m_mask.m_last_byte & x_adjusted;
        }else{
            mask = m_mask.m_last_byte >> m_mask.m_last_shift;
            *byte_ptr = (*byte_ptr & ~m_mask.m_last_byte) |
                    ((mask & x_adjusted) << (m_mask.m_last_shift));
        }
        return *this;
    }

    /** Member variables. */
    storage_type*    m_ptr;
    mask_detail     m_mask;
};

/** Proxy reference type for unsigned types. */
template <typename RetType, std::size_t Width>
class proxy_reference_type<RetType,Width,false> {
    typedef proxy_reference_type<RetType,Width,false> _self;
    proxy_reference_type();
public:
    /** Typedefs and integral static constant values. */
    //@{
    typedef unsigned char       storage_type;
    typedef RetType             value_type;
    typedef std::size_t         offset_type;
    BOOST_STATIC_CONSTANT( std::size_t, width = Width );
    //@}
    
    /** constructors and destructor for the proxy_reference_type type. */
    //@{

    /** Copy Constructor. */
    proxy_reference_type(_self const& x)
        :m_ptr(x.m_ptr), m_mask(x.m_mask)
    { }

    /** pointer, offset constructor. */
    proxy_reference_type(storage_type* ptr, offset_type offset)
        :m_ptr(ptr), m_mask(get_mask_detail<Width>(offset))
    { }
    //@}

    /** Copy assignment. */
    _self& operator=(_self const& x) {
        m_ptr = x.m_ptr;
        m_mask = x.m_mask;
        return *this;
    }

    /** Implicit Conversion Operator*/
    operator value_type() const {
        if(m_mask.m_size == 1) {
            return (value_type( m_mask.m_first_byte & *m_ptr ) >>
                m_mask.m_last_shift);
        }

        value_type ret = 0;
        storage_ptr_t byte_ptr = m_ptr;

        if(m_mask.m_size == 2) {
            ++byte_ptr;
            byte_ptr = m_ptr;
            // getting first bits.
            ret = value_type(m_mask.m_first_byte & *byte_ptr)
                << ( CHAR_BIT - m_mask.m_last_shift );
            ++byte_ptr;
            value_type retrieved_value;
            if( m_mask.m_last_byte == 0xFF) {
                ret <<= CHAR_BIT;
                ret += value_type(*byte_ptr);
            }else{
                retrieved_value = (value_type( m_mask.m_last_byte)
                    & value_type(*byte_ptr));
                retrieved_value >>= m_mask.m_last_shift;
                ret += retrieved_value;
            }
            return ret;
        }
        
        const storage_t all_bits = 0xFF;
        // gettting first byte.

        ret = value_type( m_mask.m_first_byte & *byte_ptr);
        ++byte_ptr;
        
        // getting middle bytes
        for(std::size_t index = 0; index < m_mask.m_size - 2; ++index) {
            ret <<= CHAR_BIT;
            ret += *byte_ptr & all_bits;
            ++byte_ptr;
        }
        // shifting bits
        if(m_mask.m_last_byte == 0xFF) {
            ret <<= CHAR_BIT;
            ret += value_type(*byte_ptr & m_mask.m_last_byte);
        }else{
            ret <<= CHAR_BIT - m_mask.m_last_shift;
            ret += value_type( *byte_ptr & m_mask.m_last_byte )
                >> ( m_mask.m_last_shift);
        }
        return ret;
    }

    /** value_type storage assignement operator.*/
    _self& operator=(value_type x) {
        if(m_mask.m_size == 1) {
            storage_t previous_values = *m_ptr & ~m_mask.m_first_byte;
            storage_t new_value = low_bits_mask<value_type, width>::value & x;
            new_value <<= m_mask.m_last_shift;
            previous_values |= new_value;
            *m_ptr = previous_values;
            return *this;
        }
        
        storage_t       to_be_stored = 0;
        std::size_t     bits_in_mask = 0;
        value_type      mask         = 0;
        storage_ptr_t   byte_ptr     = m_ptr;

        if(m_mask.m_size == 2) {
            bits_in_mask = CHAR_BIT - m_mask.m_offset;
            mask = (~(~value_type(0) << bits_in_mask))
                << (width - bits_in_mask);
            to_be_stored = storage_t((mask&x)>>(width - bits_in_mask));
            *byte_ptr = ((*byte_ptr) & (~m_mask.m_first_byte)) | to_be_stored;
            
            ++byte_ptr;
            bits_in_mask = width - bits_in_mask;
            mask = ~(~value_type(0) << bits_in_mask);
            to_be_stored = storage_t((mask & x) << m_mask.m_last_shift);
            *byte_ptr = (*byte_ptr & ~m_mask.m_last_byte) | to_be_stored;
            return *this;
        }
        // calculate the offset of the first bit within x
        // and creating a mask to extract the fist bits from within x
        bits_in_mask = CHAR_BIT - m_mask.m_offset;
        mask = m_mask.m_first_byte;
        mask <<= width - bits_in_mask;

        *byte_ptr = (*byte_ptr & ~m_mask.m_first_byte) | ((x & mask )
            >> (width - bits_in_mask));
        ++byte_ptr;
        mask = 0xFF;
        mask <<= width - bits_in_mask - CHAR_BIT;
        for(std::size_t index = 0; index < m_mask.m_size - 2;++index) {
            *byte_ptr = (mask & x) >> (width - (bits_in_mask +
                (CHAR_BIT * index))- CHAR_BIT);
            mask >>= CHAR_BIT;
            ++byte_ptr;
        }
        // now calculating the last bytes information, retrieving it and then
        // storing the data within the array.
        if(m_mask.m_last_byte == 0xFF) {
            *byte_ptr = m_mask.m_last_byte & x;
        }else{
            mask = m_mask.m_last_byte >> m_mask.m_last_shift;
            *byte_ptr = (*byte_ptr & ~m_mask.m_last_byte) |
                    ((mask & x) << (m_mask.m_last_shift));
        }
        return *this;
    }

    /** Member variables. */
    storage_type*   m_ptr;
    mask_detail     m_mask;
};


template <typename RetType, std::size_t Width>
class const_proxy_reference_type
    :proxy_reference_type<RetType,Width>
{
    typedef proxy_reference_type<RetType,Width>         _base;
    typedef const_proxy_reference_type<RetType,Width>   _self;
    const_proxy_reference_type();
public:

    typedef typename _base::storage_type                storage_type;
    typedef typename _base::value_type                  value_type;
    typedef typename _base::offset_type                 offset_type;
    BOOST_STATIC_CONSTANT( std::size_t, width = Width );


    const_proxy_reference_type(_self const& x)
        :_base( static_cast<_base>(x) )
    { }

    explicit const_proxy_reference_type(_base const& x)
        :_base(x)
    { }

    const_proxy_reference_type(storage_type* ptr, offset_type offset)
        :_base(ptr, offset)
    { }

    _self& operator=(_self const& rhs) {
        static_cast<_base>(*this) = static_cast<_base>(rhs);
        return *this;
    }
    
    operator value_type() const {
        return static_cast<_base>(*this);
    }

};


/** Tags used for dispatching to the correct function and then allowing
 *  the dispatch work in a much easier fashion.
 */
struct constant_time_distance;
struct other_iterator_type;
#if 0
/** Tag dispatch for iterator types. */
template <typename Iter, typename BitfieldIter, typename Visitor, typename>
struct bitfield_vector_insert_dispatch;

/** random_access Dispatch. */
template <typename Iter, typename BitfieldIter, typename Visitor>
struct bitfield_vector_insert_dispatch <
    Iter,
    BitfieldIter,
    Visitor,
    std::random_access_iterator_tag
> {
};


/** This is an implementation for insert dispatch for bitfield_vector.
 *  This is directly used for nothing but dispatching, overloading and using 
 *  a visitor to do exactly what needs to be for a particular algorithm. This
 *  will hopfully work for all of the different scenarios which occur
 *  within bitfield_vector.
 */
template <typename T, typename BitfieldIter, typename Visitor>
struct btifield_vector_insert_impl {
};

/** Partical specializations for iterators from the bitfield_vector class.
 *  because they arn't actually bidirectional iterators, but they are classiffied
 *  as such, although they have a constant time size.
 */
/** const_bf_vector_iterator<T,Width> */
template <typename T, std::size_t Width, typename BitfieldIter, typename Visitor>
struct btifield_vector_insert_impl <
    bf_vector_iterator<T,Width>,
    BitfieldIter,
    Visitor
> {
};

/** const_bf_vector_iterator<T,Width> */
template <typename T, std::size_t Width, typename BitfieldIter, typename Visitor>
struct btifield_vector_insert_impl <
    const_bf_vector_iterator<T,Width>,
    BitfieldIter,
    Visitor
> {
};

/** bf_vector_reverse_iterator<T,Width> */
template <typename T, std::size_t Width, typename BitfieldIter, typename Visitor>
struct btifield_vector_insert_impl <
    bf_vector_reverse_iterator<T,Width>,
    BitfieldIter,
    Visitor
> {
};

/** const_bf_vector_reverse_iterator<T,Width> */
template <typename T, std::size_t Width, typename BitfieldIter, typename Visitor>
struct btifield_vector_insert_impl <
    const_bf_vector_reverse_iterator<T,Width>,
    BitfieldIter,
    Visitor
> {
};

/** pointer overload */
template <typename T, typename BitfieldIter, typename Visitor>
struct btifield_vector_insert_impl <
    T*,
    BitfieldIter,
    Visitor
> {
};
#endif

}} // end boost::detail


#endif
