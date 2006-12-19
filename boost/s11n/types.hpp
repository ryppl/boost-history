#ifndef BOOST_S11N_TYPES_HPP_INCLUDED
#define BOOST_S11N_TYPES_HPP_INCLUDED

#include <boost/strong_typedef.hpp>
#include <string>
#include <cstddef>

namespace boost { namespace s11n
{
    
    BOOST_STRONG_TYPEDEF(std::string, class_name_t)
    BOOST_STRONG_TYPEDEF(std::size_t, container_size_t)

    // Derive from this to cause the deriving class to be always serialized nonpolymorphically
    struct force_nonpolymorphic_serialization { };
    
    // Use this struct to serialize raw bytes of data
    struct raw_bytes
    {
        raw_bytes(char *ptr, std::size_t size):
            ptr(ptr), size(size)
        {
        }
        char *ptr;
        std::size_t size;
    };

} }

#endif
