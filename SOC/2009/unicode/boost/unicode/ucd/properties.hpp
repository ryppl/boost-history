#ifndef BOOST_UNICODE_PROPRIETIES_HPP
#define BOOST_UNICODE_PROPRIETIES_HPP

#include <boost/cuchar.hpp>
#include <boost/unicode/ucd/properties_types.hpp>
#include <boost/unicode/ucd/block_types.hpp>

#include <boost/unicode/ucd/detail/unichar_data.hpp>

#include <algorithm>
#include <iostream>

namespace boost
{
namespace unicode
{

/** INTERNAL ONLY **/
#define BOOST_UNICODE_GET_PROPERTY_DEF(Name)                           \
inline Name::type get_ ## Name(char32 ch)                              \
{                                                                      \
    return (Name::type)ucd::get_data_internal(ch).Name;                \
}

namespace ucd
{

BOOST_UNICODE_GET_PROPERTY_DEF(category)
BOOST_UNICODE_GET_PROPERTY_DEF(join_type)
BOOST_UNICODE_GET_PROPERTY_DEF(bidi_class)
BOOST_UNICODE_GET_PROPERTY_DEF(decomposition_type)
BOOST_UNICODE_GET_PROPERTY_DEF(break_class)
BOOST_UNICODE_GET_PROPERTY_DEF(grapheme_cluster_break)
BOOST_UNICODE_GET_PROPERTY_DEF(word_break)
BOOST_UNICODE_GET_PROPERTY_DEF(sentence_break)

inline bool is_unknown(char32 ch)
{
    return ucd::get_data_internal(ch).unknown_char;
}

inline const char* get_name(char32 ch)
{
    return ucd::get_data_internal(ch).name;
}

inline int get_combining_class(char32 ch)
{
    return ucd::get_data_internal(ch).combining;
}

/** Returns the block the code point designated by \c ch is in, or
 * \c block::none if the code point does not lie in any block. */
block::type get_block(char32 ch);

} // namespace ucd
} // namespace unicode
} // namespace boost

#endif
