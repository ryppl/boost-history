#ifndef BOOST_UNICODE_STATIC_UTF_CODECS_HPP
#define BOOST_UNICODE_STATIC_UTF_CODECS_HPP

#include <boost/cuchar.hpp>
#include <boost/unicode/surrogates.hpp>

#include <boost/mpl/string.hpp>
#include <boost/mpl/ustring.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/static_assert.hpp>

namespace boost
{
namespace unicode
{

namespace detail
{
    
    template<char32 I>
    struct static_u8_encoder1
    {
        typedef mpl::string<
            static_cast<unsigned char>(I)
        > type;
    };
    
    template<char32 I>
    struct static_u8_encoder2
    {
        typedef mpl::string<
            static_cast<unsigned char>(0xC0u + (I >> 6)),
			static_cast<unsigned char>(0x80u + (I & 0x3Fu))
        > type;
    };
    
    template<char32 I>
    struct static_u8_encoder3
    {   
        typedef mpl::string<
            static_cast<unsigned char>(0xE0u + (I >> 12)),
			static_cast<unsigned char>(0x80u + ((I >> 6) & 0x3Fu)),
            static_cast<unsigned char>(0x80u + (I & 0x3Fu))
        > type;
    };
    
    template<char32 I>
    struct static_u8_encoder4
    {
        typedef mpl::string<
            static_cast<unsigned char>(0xF0u + (I >> 18)),
			static_cast<unsigned char>(0x80u + ((I >> 12) & 0x3Fu)),
			static_cast<unsigned char>(0x80u + ((I >> 6) & 0x3Fu)),
			static_cast<unsigned char>(0x80u + (I & 0x3Fu))
        > type;
    };
    
    template<char32 I>
    struct static_u16_encoder1
    {
        typedef mpl::u16string<
            static_cast<char16>(I)
        > type;
    };
    
    template<char32 I>
    struct static_u16_encoder2
    {
        typedef mpl::u16string<
            static_cast<char16>((I >> 10) + high_surrogate_base),
            static_cast<char16>((I & ten_bit_mask) + low_surrogate_base)
        > type;
    };
    
} // namespace detail

/** Meta-function that takes a Unicode code point and returns it encoded
 * in UTF-8 as a \c boost::mpl::string */
template<char32 I>
struct static_u8_encoder
{
    BOOST_STATIC_ASSERT(I <= 0x10FFFFu);
    
    typedef typename mpl::eval_if_c<
            I < 0x80u,
            detail::static_u8_encoder1<I>,
            mpl::eval_if_c<
                I < 0x800u,
                detail::static_u8_encoder2<I>,
                    mpl::eval_if_c<
                        I < 0x10000u,
                        detail::static_u8_encoder3<I>,
                        detail::static_u8_encoder4<I>
                    >
            >
    >::type type;
};

/** Meta-function that takes a Unicode code point and returns it encoded
 * in UTF-16 as a \c boost::mpl::u16string */
template<char32 I>
struct static_u16_encoder
{
    BOOST_STATIC_ASSERT(I <= 0x10FFFFu);
    
    typedef typename mpl::eval_if_c<
            I < 0x10000u,
            detail::static_u16_encoder1<I>,
            detail::static_u16_encoder2<I>
    >::type type;
};

} // namespace unicode
} // namespace boost

#endif
