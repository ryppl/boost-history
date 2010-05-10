#ifndef BOOST_BIT_MASK_IMPL_HPP
#define BOOST_BIT_MASK_IMPL_HPP
#include <boost/mpl/bitwise.hpp>
#include <boost/static_assert.hpp>

namespace boost { namespace details {




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
 * 
 */
template <  typename MaskedType,
            typename MaskOffsetInBits,
            typename MaskWidth >
struct simple_bit_mask_impl {

    // precondition 1.
    BOOST_STATIC_ASSERT(( (MaskOffsetInBits::value + MaskWidth::value)
                            < (sizeof(MaskedType) * 8) ));
    // precondition 2.
    BOOST_STATIC_ASSERT(( MaskOffsetInBits::value >= 0 ));

    // precondition 3.
    BOOST_STATIC_ASSERT(( MaskWidth::value > 0 ));

    // this is supposed to make decalring a static constant easy.
//    BOOST_STATIC_CONSTANT( MaskedType, value = ( ) );

    // type typedef
    typedef simple_bit_mask_impl<   MaskedType,
                                    MaskOffsetInBits,
                                    MaskWidth
                                >   type;

    typedef MaskedType              mask_type;
    typedef MaskOffsetInBits        bit_offset;
    typedef MaskWidth               mask_width;
};


} // end of details namespace.
} // end of boost namespace.

#endif
