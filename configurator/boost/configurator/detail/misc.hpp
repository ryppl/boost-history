// Configurator (C++ library for configuration file parsing)
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_MISC_HPP
#define BOOST_CONFIGURATOR_MISC_HPP

#include <boost/configurator/detail/types.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>

#include <iostream>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

inline void notify( const std::string& notification ) {
    const std::string what_happened = "[Configurator] " + notification;
    throw std::runtime_error( what_happened );
}

inline void warning( const std::string& warning_message ) {
    std::cout << "[Configurator] " << warning_message << std::endl;
}

/// \enum value_semantic 
/// \brief Value semantic types (for check).
enum value_semantic {
    no_semantic			/*!< Default value. */
	, path				/*!< Path semantics check. */
	, optional_path	    /*!< Optional path semantics check. */
	, ipv4				/*!< IPv4 semantics check. */
    , ipv6              /*!< IPv6 semantics check. */
    , ip                /*!< IP semantics check. */
    , email             /*!< E-mail semantics check. */
    , size              /*!< Size semantics check. */
    , time_period       /*!< Time period semantics check. */
    , exp_record        /*!< Exponential record semantics check. */
};

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

inline std::string sections_separator_for_log() { return " > "; }

template
<
    typename STLContainer
    , typename Element
>
inline void delete_element( STLContainer& cont, const Element& element ) {
    cont.erase( std::remove( cont.begin(), cont.end(), element )
				, cont.end() );
}

inline void notify_without_prefix( const std::string& notification ) {
    throw std::runtime_error( notification );
}

inline void pure_check_path_existence( const std::string& _path ) {
    boost::filesystem::path path( _path );
    
    const std::string full_path_name = path.string();
    const std::string path_name      = path.parent_path().string();
    const std::string file_name      = path.filename();

    if ( boost::filesystem::exists( path_name ) ) {
        if ( !boost::filesystem::exists( full_path_name ) ) {
            notify_without_prefix( "directory '" + path_name + "' exists, but there is no file '" + file_name + "'!" );
        } else {}
    } else {
        notify_without_prefix( "directory '" + path_name + "' not exists!" );
    }	
}

inline std::string prepare_full_name_for_log( const std::string& full_name_of_option
                                              , const std::string& sections_separator ) {
    std::string s( full_name_of_option.begin(), full_name_of_option.end() );
    boost::replace_all( s, sections_separator, sections_separator_for_log() );
    string_const_it last_separator_end = boost::find_last( s, sections_separator_for_log() ).end();
    if ( s.end() == last_separator_end ) {
        boost::erase_last( s, sections_separator_for_log() );
    } else {}
    return s;
}

inline void remove_empty_strings_from( str_storage& strings ) {
    strings.erase( std::remove_if( strings.begin()
                                   , strings.end()
                                   , boost::mem_fn( &std::string::empty ) )
                   , strings.end() );
}

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_MISC_HPP
