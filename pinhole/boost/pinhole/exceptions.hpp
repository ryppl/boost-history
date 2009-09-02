// Pinhole Exceptions.hpp file
//
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PINHOLE_EXCEPTIONS
#define BOOST_PINHOLE_EXCEPTIONS

#include <exception>
#include <stdexcept>
#include <string>
#include <boost/exception.hpp>

namespace boost { namespace pinhole
{
    class multiple_property_groups : public std::runtime_error, public ::boost::exception
    {
        public: multiple_property_groups(const std::string& strErrMsg) : std::runtime_error(strErrMsg) {;}
    };

    class invalid_path : public std::runtime_error, public ::boost::exception
    {
        public: invalid_path(const std::string& strErrMsg) : std::runtime_error(strErrMsg) {;}
    };

    class failed_to_find_group : public std::runtime_error, public ::boost::exception
    {
        public: failed_to_find_group(const std::string& strErrMsg) : std::runtime_error(strErrMsg) {;}
    };
    
    class no_metadata_defined : public std::runtime_error, public ::boost::exception
    {
        public: no_metadata_defined() : runtime_error( "No metadata defined for property" ) {;}
    };

    typedef ::boost::error_info< struct tag_additional_info, const std::string > exception_additional_info;
    typedef ::boost::error_info< struct tag_requested_type,  const std::string > exception_requested_type;
    typedef ::boost::error_info< struct tag_property_name,   const std::string > exception_property_name;
    typedef ::boost::error_info< struct tag_property_type,   const std::string > exception_property_type;
    typedef ::boost::error_info< struct tag_actual_type,     const std::string > exception_actual_type;
    typedef ::boost::error_info< struct tag_action_name,     const std::string > exception_action_name;
    typedef ::boost::error_info< struct tag_signal_name,     const std::string > exception_signal_name;
    typedef ::boost::error_info< struct tag_signal_type,     const std::string > exception_signal_type;
    typedef ::boost::error_info< struct tag_path,            const std::string > exception_path;
    typedef ::boost::error_info< struct tag_path_section,    const std::string > exception_path_section;
}}

#endif // include guard