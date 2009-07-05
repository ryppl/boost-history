#ifndef BOOST_UNICODE_PROPRIETIES_HPP
#define BOOST_UNICODE_PROPRIETIES_HPP

#include <boost/cuchar.hpp>
#include <boost/unicode/ucd/properties_types.hpp>
#include <boost/unicode/ucd/block_types.hpp>

#include <boost/unicode/ucd/detail/unichar_data.hpp>

#include <algorithm>
#include <iostream>

#define BOOST_UNICODE_UCD_VERSION_MAJOR 5
#define BOOST_UNICODE_UCD_VERSION_MINOR 1

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

#ifdef BOOST_UNICODE_UCD_BIG
BOOST_UNICODE_GET_PROPERTY_DEF(join_type)
#endif

BOOST_UNICODE_GET_PROPERTY_DEF(bidi_class)
BOOST_UNICODE_GET_PROPERTY_DEF(line_break)
BOOST_UNICODE_GET_PROPERTY_DEF(grapheme_cluster_break)
BOOST_UNICODE_GET_PROPERTY_DEF(word_break)
BOOST_UNICODE_GET_PROPERTY_DEF(sentence_break)

/** Hangul syllables have a canonical decomposition, but it is not present
 * in the UCD as it can be procedurally generated. This function will thus
 * return \c decomposition_type::none for those code points.
 * See also \c boost::unicode::hangul_decomposer. */
BOOST_UNICODE_GET_PROPERTY_DEF(decomposition_type)

inline bool is_unknown(char32 ch)
{
    return ucd::get_data_internal(ch).category != ucd::category::unknown;
}

#ifdef BOOST_UNICODE_UCD_BIG
/** Returns the name of the code point \c ch as a zero-terminated string
 * in ASCII. */
inline const char* get_name(char32 ch)
{
    return ucd::get_data_internal(ch).name;
}
#endif

/** Returns the Canonical Combining Class associated with \c ch,
 * useful for canonical ordering of combining sequences. */
inline int get_combining_class(char32 ch)
{
    return ucd::get_data_internal(ch).combining;
}

/** Returns the block the code point designated by \c ch is in, or
 * \c block::none if the code point does not lie in any block. */
block::type get_block(char32 ch);

/** Returns the decomposition associated with \c ch as a zero-terminated
 * sequence of code points.
 * See the \c decomposition_type property to know what kind of decomposition it is. */
inline const char32* get_decomposition(char32 ch)
{
    return ucd::get_data_internal(ch).decomp;
}

} // namespace ucd
} // namespace unicode
} // namespace boost

#endif
