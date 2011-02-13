//  Boost endian_view.hpp header file -------------------------------------------------------//

//  (C) Copyright VicenteJ Botet Escriba 2010

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/endian

//--------------------------------------------------------------------------------------//


#ifndef BOOST_ENDIAN_ENDIANNESS__HPP
#define BOOST_ENDIAN_ENDIANNESS__HPP

//~ #include <boost/config.hpp>
#include <boost/detail/endian.hpp>
#ifdef BOOST_INTEGER_ENDIAN_USES_ENDIANNESS
#include <boost/detail/scoped_enum_emulation.hpp>
#endif

namespace boost {
namespace integer {

#   ifdef BOOST_INTEGER_ENDIAN_USES_ENDIANNESS
  //  endian class template and specializations  ---------------------------------------//
#   ifdef BOOST_BIG_ENDIAN
    BOOST_SCOPED_ENUM_START(endianness) { big, little, middle, mixed, native=big }; BOOST_SCOPED_ENUM_END
#   else
    BOOST_SCOPED_ENUM_START(endianness) { big, little, middle, mixed, native=little }; BOOST_SCOPED_ENUM_END
#   endif
    const BOOST_SCOPED_ENUM_START(endianness) big_endian    = endianness::big;   
    const BOOST_SCOPED_ENUM_START(endianness) little_endian = endianness::little;   
    const BOOST_SCOPED_ENUM_START(endianness) middle_endian = endianness::middle;   
    const BOOST_SCOPED_ENUM_START(endianness) mixed_endian = endianness::mixed;   
    const BOOST_SCOPED_ENUM_START(endianness) native_endian = endianness::native;   
#else
    
namespace endianness {
    
    
    struct big {
        //~ static const BOOST_SCOPED_ENUM(endianness) value= endianness::big;
    };
    struct little {
        //~ static const BOOST_SCOPED_ENUM(endianness) value= endianness::little;
    };
    struct middle {
        //~ static const BOOST_SCOPED_ENUM(endianness) value= endianness::middle;
    };
    struct mixed {
        //~ static const BOOST_SCOPED_ENUM(endianness) value= endianness::mixed;
    };
#   ifdef BOOST_BIG_ENDIAN
    typedef big native ;
#   else
    typedef little native ;
#   endif

}

    typedef endianness::big     big_endian;
    typedef endianness::little  little_endian;
    typedef endianness::middle  middle_endian;
    typedef endianness::mixed   mixed_endian;
    typedef endianness::native  native_endian;
#   endif

} // namespace integer
} // namespace boost


#endif // BOOST_ENDIAN_ENDIANNESS__HPP
