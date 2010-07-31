#ifndef BOOST_UNICODE_PROPRIETIES_HPP
#define BOOST_UNICODE_PROPRIETIES_HPP

#include <boost/unicode/ucd/detail/unicode_decl.hpp>

#include <boost/cuchar.hpp>
#include <boost/unicode/ucd/properties_types.hpp>
#include <boost/unicode/ucd/block_types.hpp>

#include <boost/range/iterator_range.hpp>

#include <boost/config/abi_prefix.hpp> // must be the last #include

/** BOOST_UNICODE_UCD_VERSION / 1000 is the major version
 * 
 * BOOST_UNICODE_UCD_VERSION % 1000 is the minor version */
#define BOOST_UNICODE_UCD_VERSION 5001
#define BOOST_UNICODE_UCD_VERSION_MAJOR 5
#define BOOST_UNICODE_UCD_VERSION_MINOR 1

namespace boost
{
namespace unicode
{

/** INTERNAL ONLY **/
#define BOOST_UNICODE_GET_PROPERTY_DEF(Name)                           \
BOOST_UNICODE_DECL Name::type get_ ## Name(char32 ch);

namespace ucd
{

BOOST_UNICODE_GET_PROPERTY_DEF(category)
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

#undef BOOST_UNICODE_GET_PROPERTY_DEF

/** Returns the Canonical Combining Class associated with \c ch,
 * useful for canonical ordering of combining sequences. */
BOOST_UNICODE_DECL int get_combining_class(char32 ch);

/** Returns the decomposition associated with \c ch as a range of code
 * points; an empty range is returned if there is none.
 * See the \c decomposition_type property to know what kind of decomposition it is. */
BOOST_UNICODE_DECL iterator_range<const char32*> get_decomposition(char32 ch);

/** Returns the block the code point designated by \c ch is in, or
 * \c block::none if the code point does not lie in any block. */
BOOST_UNICODE_DECL block::type get_block(char32 ch);

} // namespace ucd
} // namespace unicode
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif
