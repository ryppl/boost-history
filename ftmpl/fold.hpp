#ifndef BOOST_FTMPL_FOLD_HPP
#define BOOST_FTMPL_FOLD_HPP

#include "vector.hpp"
#include "type.hpp"
#include "is_same.hpp"

namespace boost { namespace ftmpl {

template< typename Fn, typename State >
State& fold( Fn&, State&, type_t< vector_t<> >& );

template< typename Fn, typename State, typename Head, typename ... Tail >
auto fold( type_t< Fn >&, State&, type_t< vector_t< Head, Tail... > >& )
-> decltype( fold( type< Fn >(), Fn()( type< State >(), type< Head >() ), vector< Tail... >() ) );

} }

#endif // BOOST_FTMPL_FOLD_HPP
