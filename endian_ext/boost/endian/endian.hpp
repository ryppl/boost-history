//  Boost endian_view.hpp header file -------------------------------------------------------//

//  (C) Copyright VicenteJ Botet Escriba 2010

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/endian

//--------------------------------------------------------------------------------------//


#ifndef BOOST_ENDIAN_ENDIAN__HPP
#define BOOST_ENDIAN_ENDIAN__HPP

//~ #include <boost/config.hpp>
#include <boost/detail/endian.hpp>
#include <boost/detail/scoped_enum_emulation.hpp>

namespace boost {

#   ifdef BOOST_BIG_ENDIAN
    BOOST_SCOPED_ENUM_START(endianness) { big, little, middle, mixed, native=big }; BOOST_SCOPED_ENUM_END
#   else
    BOOST_SCOPED_ENUM_START(endianness) { big, little, middle, mixed, native=little }; BOOST_SCOPED_ENUM_END
#   endif
    
namespace endian {
    struct big {
        static const BOOST_SCOPED_ENUM(endianness) value= endianness::big;
    };
    struct little {
        static const BOOST_SCOPED_ENUM(endianness) value= endianness::little;
    };
    struct middle {
        static const BOOST_SCOPED_ENUM(endianness) value= endianness::middle;
    };
    struct mixed {
        static const BOOST_SCOPED_ENUM(endianness) value= endianness::mixed;
    };
#   ifdef BOOST_BIG_ENDIAN
    typedef big native ;
#   else
    typedef little native ;
#   endif

}

} // namespace boost


#endif // BOOST_ENDIAN_ENDIAN__HPP
