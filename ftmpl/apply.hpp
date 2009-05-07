#ifndef BOOST_FTMPL_APPLY_HPP
#define BOOST_FTMPL_APPLY_HPP

#include "type.hpp"

namespace boost { namespace ftmpl {

// ToDo: Possibly remove this file and definition
template< typename Metafun, typename ... Args >
auto apply( type_t< Metafun >&, Args& ... args )
-> decltype( Metafun()( args... ) );

} }

#endif // BOOST_FTMPL_APPLY_HPP
