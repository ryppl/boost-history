#ifndef BOOST_FTMPL_QUOTE_HPP
#define BOOST_FTMPL_QUOTE_HPP

namespace boost { namespace ftmpl {
#if 0
template< template< typename ... > class Metafun >
struct quote_t;

template< template< typename ... > class Metafun >
quote_t< Metafun >& quote();

template< template< typename ... > class Metafun, typename ... Arg >
auto apply_t( quote_t< Metafun >& quo, Arg arg ... )
-> decltype( Metafun< Arg... >() );
#endif

} }

#endif // BOOST_FTMPL_QUOTE_HPP
