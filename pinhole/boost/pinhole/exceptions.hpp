// Pinhole Exceptions.hpp file
//
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DETAIL_EXCEPTIONS
#define BOOST_DETAIL_EXCEPTIONS

#include <exception>
#include <stdexcept>
#include <string>

namespace boost { namespace pinhole
{
    class multiple_property_groups : public std::runtime_error
    {
        public: multiple_property_groups(const std::string& strErrMsg) : std::runtime_error(strErrMsg) {;}
    };

    class invalid_path : public std::runtime_error
    {
        public: invalid_path(const std::string& strErrMsg) : std::runtime_error(strErrMsg) {;}
    };

    class failed_to_find_group : public std::exception{};
    
    class no_metadata_defined_error : public std::runtime_error
    {
        public:
        no_metadata_defined_error() : runtime_error( "No Property Editor Defined" ) {;}
    };
}}

#endif // include guard