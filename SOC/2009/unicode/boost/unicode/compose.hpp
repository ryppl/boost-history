#ifndef BOOST_UNICODE_COMPOSE_HPP
#define BOOST_UNICODE_COMPOSE_HPP

#include <boost/unicode/ucd/properties.hpp>
#include <boost/unicode/hangul.hpp>

#include <boost/integer/static_pow.hpp>

namespace boost
{
namespace unicode
{

/** Model of \c OneManyPipe that decomposes a code point, i.e. it
 * converts a code point into a sequence of code points.
 * It applies UCD decomposition that match \c mask as well as the Hangul decompositions. */
template<int mask = static_pow<2, ucd::decomposition_type::canonical>::value>
struct decomposer
{
    template<typename Out>
    Out operator()(char32 ch, Out out)
    {
        const char32* p = ucd::get_decomposition(ch);
        if(p && ((1 << ucd::get_decomposition_type(ch)) & mask))
        {
            for(; *p; ++p)
                *out++ = *p;
        }
        else
        {
            out = hangul_decomposer()(ch, out);
        }
        return out;
    }
};

} // namespace unicode
} // namespace boost

#endif
