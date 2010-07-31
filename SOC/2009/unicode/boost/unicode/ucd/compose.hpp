#ifndef BOOST_UNICODE_UCD_COMPOSE_HPP
#define BOOST_UNICODE_UCD_COMPOSE_HPP

#include <boost/unicode/ucd/detail/unicode_decl.hpp>
#include <boost/cuchar.hpp>

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost
{
namespace unicode
{
namespace ucd
{

struct unichar_compose_data_entry
{
    const char32* decomp;
    char32 ch;
};

BOOST_UNICODE_DECL extern const unichar_compose_data_entry __uni_compose_entry[];
BOOST_UNICODE_DECL extern const size_t __uni_compose_entry_size;
    
} // namespace ucd
} // namespace unicode
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif
