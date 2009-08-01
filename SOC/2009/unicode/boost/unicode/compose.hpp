#ifndef BOOST_UNICODE_COMPOSE_HPP
#define BOOST_UNICODE_COMPOSE_HPP

#include <boost/unicode/ucd/properties.hpp>
#include <boost/unicode/hangul.hpp>

#include <boost/integer/static_pow.hpp>
#include <climits>

namespace boost
{
namespace unicode
{
    
#define BOOST_UNICODE_OPTION(o) (boost::static_pow<2, (o)>::value)
#ifdef BOOST_UNICODE_DOXYGEN_INVOKED
#undef BOOST_UNICODE_OPTION
#endif

/** Model of \c \xmlonly<conceptname>OneManyPipe</conceptname>\endxmlonly
 * that decomposes a code point, i.e. it converts a code point into a
 * sequence of code points.
 * It applies UCD decompositions that match \c mask as well as the Hangul decompositions. */
template<unsigned mask = BOOST_UNICODE_OPTION(ucd::decomposition_type::canonical)>
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
