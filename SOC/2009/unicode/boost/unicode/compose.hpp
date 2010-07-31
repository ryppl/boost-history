#ifndef BOOST_UNICODE_COMPOSE_HPP
#define BOOST_UNICODE_COMPOSE_HPP

#include <boost/unicode/compose_fwd.hpp>

namespace boost
{
namespace unicode
{

BOOST_CONVERTER_DEF(boost::unicode::composer, compose)
BOOST_CONVERTER_DEF(boost::unicode::decomposer, decompose)
BOOST_CONVERTER_DEF(boost::unicode::normalizer, normalize)

} // namespace unicode
} // namespace boost

#endif
