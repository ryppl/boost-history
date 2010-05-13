#ifndef BOOST_BIT_MASK_IMPL_HPP
#define BOOST_BIT_MASK_IMPL_HPP

#include <boost/mpl/integral_c.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/bitwise.hpp>
#include <boost/static_assert.hpp>

namespace boost { namespace details {



/** Type which is used during specialization to infer that a template patameter
 *  has not been used in the initilization of bit_mask_impl.
 */
struct unused_type { };

/** Forward Declaration for the bit_mask_impl.
 *  This is being used for specifying default parameters so they can be used in 
 *  specializations to see how may parameters bit_mask_impl was called with.
 *  If I had variadic templates this would be less of an issue.
 */
template <  typename MaskedType,
            typename MaskOffsetInBits = unused_type ,
            typename MaskWidth = unused_type >
struct bit_mask_impl;



/** Houses the structure for creating simple masks as well as 
 *  implements the preconditions for a simple_bit_mask.
 *
 *  Pre-Conditions
 *
 *  1. The width of the masked section must not be longer then the
 *  mask itself.
 *
 *  2. Valid range for offset is >= 0.
 *
 *  3. Valid range for mask width is > 0.
 *  
 *  4. The type which is being masked must satisfy the is_integral type trait.
 */
template <  typename MaskedType,
            typename MaskOffsetInBits,
            typename MaskWidth >
struct bit_mask_impl {


    // precondition 1.
    BOOST_STATIC_ASSERT(( (MaskOffsetInBits::value + MaskWidth::value)
                            < (sizeof(MaskedType) * 8) ));
    // precondition 2.
    BOOST_STATIC_ASSERT(( MaskOffsetInBits::value >= 0 ));

    // precondition 3.
    BOOST_STATIC_ASSERT(( MaskWidth::value > 0 ));
    
    // precondition 4.
    BOOST_STATIC_ASSERT(( is_integral<MaskedType>::value ));


    // this is supposed to make decalring a static constant easy, I think...
    // BOOST_STATIC_CONSTANT( MaskedType, value = ( ) );

    // type typedef
    typedef bit_mask_impl<   MaskedType,
                             MaskOffsetInBits,
                             MaskWidth
                         >   type;

    typedef MaskedType       mask_type;
    typedef MaskOffsetInBits bit_offset;
    typedef MaskWidth        mask_width;
};


/** bit_mask_impl specialization over an mpl type. */
template < typename MaskedType, typename IntegralType, IntegralType IntegralValue>
struct bit_mask_impl< MaskedType, mpl::integral_c<IntegralType, IntegralValue>, unused_type>{

};

} // end of details namespace.
} // end of boost namespace.

#endif
