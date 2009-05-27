#ifndef BOOST_UNICODE_PROPRIETIES
#define BOOST_UNICODE_PROPRIETIES

#include <boost/cuchar.hpp>

namespace boost
{
namespace unicode
{

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
