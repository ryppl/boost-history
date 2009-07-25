#ifndef BOOST_UNICODE_SURROGATES_HPP
#define BOOST_UNICODE_SURROGATES_HPP

#include <boost/cuchar.hpp>

namespace boost
{
namespace unicode
{

namespace detail
{
    static const char16 high_surrogate_base = 0xD7C0u;
    static const char16 low_surrogate_base = 0xDC00u;
    static const char32 ten_bit_mask = 0x3FFu;
}

inline bool is_high_surrogate(char32 v)
{
   return (v & 0xFC00u) == 0xd800u;
}

inline bool is_low_surrogate(char32 v)
{
   return (v & 0xFC00u) == 0xdc00u;
}

inline bool is_surrogate(char32 v)
{
   return (v & 0xF800u) == 0xd800;
}

} // namespace unicode
} // namespace boost

#endif
