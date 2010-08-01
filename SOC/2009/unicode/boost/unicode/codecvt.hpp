#ifndef BOOST_UNICODE_CODECVT_HPP
#define BOOST_UNICODE_CODECVT_HPP

#include <boost/unicode/utf_codecs.hpp>
#include <boost/unicode/compose_fwd.hpp>
#include <boost/iterator/converter_codecvt.hpp>

namespace boost
{
    
namespace unicode
{

/** Codecvt facet that converts between UTF-X in-memory and UTF-8 in file,
 * 'X' depending on the size of \c wchar_t.
 * Also normalizes to NFC when reading the data from the file. */
typedef converter_codecvt<
    wchar_t,
    utf_boundary,
    utf_transcoder<char>,
    utf_combine_boundary,
    multi_converter<
        converted_converter<utf_decoder, normalizer>,
        utf_encoder<wchar_t>
    >
> utf_u8_normalize_codecvt;

/** Codecvt facet that converts between UTF-X in-memory and UTF-8 in file,
 * 'X' depending on the size of \c wchar_t. */
typedef converter_codecvt<
    wchar_t,
    utf_boundary,
    utf_transcoder<char>,
    utf_boundary,
    utf_transcoder<wchar_t>
> utf_u8_codecvt;

} // namespace unicode

} // namespace boost

#endif
