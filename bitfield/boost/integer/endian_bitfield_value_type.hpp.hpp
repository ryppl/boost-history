//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Emile Cormier 2006. 
// (C) Copyright Vicente J. Botet Escriba 2008-2009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/integer for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTEGER_BITFIELD__HPP
#define BOOST_INTEGER_BITFIELD__HPP

#include <cstddef>
#include <boost/integer/endian.hpp>
#include <boost/bitfield.hpp>

namespace boost { namespace integer {
    
    template <endianness E, typename T, std::size_t n_bits, alignment A>
    struct bitfield_value_type<endian<E,T,n_bits,A> > {
        typedef typename endian<E,T,n_bits,A>::value_type type;
    };

}}
#endif

