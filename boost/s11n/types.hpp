#ifndef BOOST_S11N_TYPES_HPP_INCLUDED
#define BOOST_S11N_TYPES_HPP_INCLUDED

#include <boost/strong_typedef.hpp>
#include <string>
#include <cstddef>

namespace boost { namespace s11n
{
    BOOST_STRONG_TYPEDEF(std::string, class_name_t);
    BOOST_STRONG_TYPEDEF(std::size_t, container_size_t);
} }

#endif
