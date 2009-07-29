#ifndef BOOST_UNICODE_STATIC_UTF_HPP
#define BOOST_UNICODE_STATIC_UTF_HPP

#include <boost/unicode/static_utf_codecs.hpp>
#include <boost/mpl/fold.hpp>

namespace boost
{
namespace unicode
{

namespace detail
{
    
template<typename I>
struct static_u8_encoder_type
  : static_u8_encoder<I::value>
{
};

template<typename I>
struct static_u16_encoder_type
  : static_u16_encoder<I::value>
{
};

template<typename S1, typename S2>
struct concat
  : mpl::fold<
    S2,
    S1,
    mpl::push_back<
        mpl::_1,
        mpl::_2
    >
  >::type
{
};

} // namespace detail

/** Meta-function that takes an MPL sequence in UTF-32 and returns it
 * encoded in UTF-8 as a \c boost::mpl::string */
template<typename Sequence>
struct static_u8_encode
  : mpl::fold<
    Sequence,
    mpl::string<>,
    detail::concat<
        mpl::_1,
        detail::static_u8_encoder_type<mpl::_2>
    >
  >::type
{
};

/** Meta-function that takes an MPL sequence in UTF-32 and returns it
 * encoded in UTF-16 as a \c boost::mpl::u16string */
template<typename Sequence>
struct static_u16_encode
  : mpl::fold<
    Sequence,
    mpl::u16string<>,
    detail::concat<
        mpl::_1,
        detail::static_u16_encoder_type<mpl::_2>
    >
  >::type
{
};

} // namespace unicode
} // namespace boost

#endif
