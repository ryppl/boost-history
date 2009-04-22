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

