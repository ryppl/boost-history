#ifndef BOOST_UNICODE_STRING_CP_HPP
#define BOOST_UNICODE_STRING_CP_HPP

#include <boost/config.hpp>
#include <boost/unicode/static_utf_codecs.hpp>

#include <boost/mpl/int.hpp>
#include <boost/mpl/bitwise.hpp>

#if defined(BOOST_LITTLE_ENDIAN) && defined(__SUNPRO_CC)
#include <boost/mpl/reverse_fold.hpp>
#else
#include <boost/mpl/fold.hpp>
#endif

namespace boost
{
namespace unicode
{

namespace detail
{
    
template<typename Sequence>
struct combine_bytes
#if defined(BOOST_LITTLE_ENDIAN) && defined(__SUNPRO_CC)
  : mpl::reverse_fold<
#else
  : mpl::fold<
#endif
        Sequence,
        mpl::int_<0>,
        mpl::bitor_<
            mpl::shift_left< mpl::_1, mpl::int_<8> >,
            mpl::bitand_< mpl::_2, mpl::int_<0xff> >
        >
    >::type
{
};
} // namespace detail

/** Meta-function that takes a Unicode code point and returns it encoded
 * in UTF-8 as an \c int usable as a parameter to
 * \c boost::mpl::string.
 * Note this only works if \c int is big enough to hold the set of
 * encoded UTF-8 code units. */
template<char32 I>
struct string_cp : detail::combine_bytes<typename static_u8_encoder<I>::type>
{
};

} // namespace unicode
} // namespace boost

#endif
