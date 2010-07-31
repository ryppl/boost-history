#define BOOST_UNICODE_SOURCE
#include <boost/unicode/ucd/properties.hpp>
#include "unichar_data.hpp"

namespace boost
{
namespace unicode
{

#define BOOST_UNICODE_GET_PROPERTY_DEF(Name)                           \
BOOST_UNICODE_DECL Name::type get_ ## Name(char32 ch)                  \
{                                                                      \
    return (Name::type)ucd::get_data_internal(ch).Name;                \
}

namespace ucd
{

BOOST_UNICODE_GET_PROPERTY_DEF(category)
BOOST_UNICODE_GET_PROPERTY_DEF(bidi_class)
BOOST_UNICODE_GET_PROPERTY_DEF(line_break)
BOOST_UNICODE_GET_PROPERTY_DEF(grapheme_cluster_break)
BOOST_UNICODE_GET_PROPERTY_DEF(word_break)
BOOST_UNICODE_GET_PROPERTY_DEF(sentence_break)
BOOST_UNICODE_GET_PROPERTY_DEF(decomposition_type)

BOOST_UNICODE_DECL int get_combining_class(char32 ch)
{
    return ucd::get_data_internal(ch).combining;
}

BOOST_UNICODE_DECL iterator_range<const char32*> get_decomposition(char32 ch)
{
    const char32* p = ucd::get_data_internal(ch).decomp;
    return p ? make_iterator_range(p+1, p+1+p[0]) : make_iterator_range(p, p);
}

} // namespace ucd
} // namespace unicode
} // namespace boost
