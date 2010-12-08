// Boost.Configurator
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_PURE_OPTIONS_OBTAINER_HPP
#define BOOST_CONFIGURATOR_PURE_OPTIONS_OBTAINER_HPP

#include <boost/configurator/detail/misc.hpp>
#include <boost/configurator/detail/option.hpp>
#include <boost/algorithm/string/regex.hpp>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

/// \class pure_options_obtainer
/// \brief Pure options obtainer.
/// 
/// Obtains pure options from pure strings.
/// Checks sections correctness.
class pure_options_obtainer {
public:
    pure_options_obtainer( std::string&         _option_name_value_separator
                           , const std::string& _sections_separator
                           , const options&     _registered_options
                           , const bool&        _case_sensitivity_for_names ) :
            option_name_value_separator( _option_name_value_separator )
            , sections_separator( _sections_separator )
            , registered_options( _registered_options )
            , case_sensitivity_for_names( _case_sensitivity_for_names )
            , open_section_tag_begin_sign( "<" )
            , open_section_tag_end_sign( ">" )
            , close_section_tag_begin_sign( "</" )
            , close_section_tag_end_sign( ">" )
            , regex_open_section_tag( "[" 
                                      + open_section_tag_begin_sign
                                      + "]{1}[a-zA-Z0-9_.-]{1,}["
                                      + open_section_tag_end_sign
                                      + "]{1}" )
            , regex_close_section_tag( "[" 
                                       + open_section_tag_begin_sign
                                       + "]{1}[/]{1}[a-zA-Z0-9_.-]{1,}["
                                       + open_section_tag_end_sign
                                       + "]{1}" ) {}
private:
    std::string&        option_name_value_separator;
    const std::string&  sections_separator;
    const options&      registered_options;
    const bool&         case_sensitivity_for_names;
private:
    const std::string   open_section_tag_begin_sign;
    const std::string   open_section_tag_end_sign;
    const std::string   close_section_tag_begin_sign;
    const std::string   close_section_tag_end_sign;
    const boost::regex  regex_open_section_tag;
    const boost::regex  regex_close_section_tag;
private:
    std::string         current_section_path;
    str_set             nonmulti_sections_uniqueness_checker;
public:
    pure_options operator()( const str_storage& obtained_strings ) {
        pure_options factual_obtained_options;

        BOOST_FOREACH ( const std::string& s, obtained_strings ) {
            if ( option_exists_in_this( s ) ) {
                obtain_option( s, factual_obtained_options );
            } else if ( open_section_tag_exists_in_this( s ) ) {
                handle_opening_of_section( s );
            } else if ( close_section_tag_exists_in_this( s ) ) {
                handle_closing_of_section( s );
            } else {
                notify_about_meaningless_string( s );
            }
        }

        check_last_section_closing();
        nonmulti_sections_uniqueness_checker.clear();
        return factual_obtained_options;
    }
private: 
    bool option_exists_in_this( const std::string& s ) const {
        const char separator = *option_name_value_separator.begin();
        boost::regex regex_valid_option; 
        if ( ' ' == separator ) {
            regex_valid_option = boost::regex( "[ \t]{0,}[^ \t]{1,}[ \t]{1,}[^ \t]{1,}[ \t]{0,}" );
        } else {
            regex_valid_option = boost::regex( std::string( "[^" )
                                               + separator
                                               + "]{1,}["
                                               + separator
                                               + "]{1}[^"
                                               + separator
                                               + "]{1,}" );
        }
        return regex_match( s, regex_valid_option );
    } 

    bool open_section_tag_exists_in_this( const std::string& s ) const {
        return regex_match( s, regex_open_section_tag );
    }

    bool close_section_tag_exists_in_this( const std::string& s ) const {
        return regex_match( s, regex_close_section_tag );
    }
private:
    void obtain_option( const std::string& s, pure_options& factual_obtained_options ) const {
        str_storage option_parts = get_option_parts_from( s );
        std::string name  = option_parts.front();
        std::string value = option_parts.back();
        boost::trim( name );
        boost::trim( value );
        const std::string full_name_of_option = current_section_path + name;
        factual_obtained_options += pure_option( full_name_of_option, value );
    }
    
    str_storage get_option_parts_from( const std::string& s ) const {
        str_storage option_parts;
        const char separator = *option_name_value_separator.begin();
        if ( ' ' == separator ) {
            boost::find_all_regex( option_parts, s, boost::regex( "[^ \t]{1,}" ) );
        } else {
            boost::find_all_regex( option_parts, s, boost::regex( std::string( "[^" ) + separator + "]{1,}" ) );
        } 
        return option_parts;
    } 
private:
    void handle_opening_of_section( const std::string& s ) {
        std::string opening_section_name = retrieve_section_name_from_open_tag( s );
        convert_name_depending_on_case_sensitivity( opening_section_name );
        check_duplication_of( opening_section_name );
        current_section_path += opening_section_name + sections_separator;
        check_nonmulti_sections_uniqueness();
    }

    void handle_closing_of_section( const std::string& s ) {
        std::string closing_section_name = retrieve_section_name_from_close_tag( s );
        convert_name_depending_on_case_sensitivity( closing_section_name );
        check_is_any_section_was_opened( closing_section_name );
        check_section_symmetry( closing_section_name );
        cut_current_section_name_by( closing_section_name );
    }
    
    void check_duplication_of( const std::string& opening_section_name ) {
        const std::string name_of_last_opening_section = retrieve_name_of_last_opening_section();
        if ( opening_section_name == name_of_last_opening_section ) {
            notify( "Duplication of open tag for section '"
                    + prepare_full_name_for_log( current_section_path, sections_separator )
                    + "' detected!" );
        } else {}
    }
private:
    void check_section_symmetry( const std::string& closing_section_name ) const {
        const std::string name_of_last_opening_section = retrieve_name_of_last_opening_section();
        if ( closing_section_name != name_of_last_opening_section ) {
            notify( "Dissymmetry of sections detected: last opening section '"
                    + name_of_last_opening_section + "', but closing is '" + closing_section_name + "'!" );
        } else {}
    }

    std::string retrieve_name_of_last_opening_section() const {
        string_const_it name_end = boost::find_last( current_section_path, sections_separator ).begin();
        string_const_it name_begin;
        string_const_it checker = std::search( current_section_path.begin()
                                               , name_end
                                               , sections_separator.begin()
                                               , sections_separator.end() );
        if ( name_end != checker ) {
            name_begin = boost::find_nth( current_section_path, sections_separator, -2 ).end();
        } else {
            name_begin = current_section_path.begin();
        }
        return std::string( name_begin, name_end );
    }

    void check_nonmulti_sections_uniqueness() {
        const size_t current_nonmulti_sections_quantity = nonmulti_sections_uniqueness_checker.size();
        nonmulti_sections_uniqueness_checker.insert( current_section_path );
        const size_t new_nonmulti_sections_quantity = nonmulti_sections_uniqueness_checker.size();
        if ( current_nonmulti_sections_quantity == new_nonmulti_sections_quantity ) {
            notify( "Section '"
                    + prepare_full_name_for_log( current_section_path, sections_separator )
                    + "' repeats, but it no allowed repeating!" );
        } else {}
    }
private:
    std::string retrieve_section_name_from_open_tag( const std::string& s ) const {
        string_const_it section_name_begin = boost::find_first( s, open_section_tag_begin_sign ).end();
        string_const_it section_name_end   = boost::find_first( s, open_section_tag_end_sign ).begin();
        std::string section_name( section_name_begin, section_name_end );
        boost::trim( section_name );
        check_section_existence( section_name );
        return section_name;
    }

    std::string retrieve_section_name_from_close_tag( const std::string& s ) const { 
        string_const_it section_name_begin = boost::find_first( s, close_section_tag_begin_sign ).end();
        string_const_it section_name_end   = boost::find_first( s, close_section_tag_end_sign ).begin();
        std::string section_name( section_name_begin, section_name_end );
        boost::trim( section_name );
        check_section_existence( section_name );
        return section_name;
    }
private:
    void cut_current_section_name_by( const std::string& closing_section_name ) {
        boost::erase_last( current_section_path, closing_section_name + sections_separator );
    }
private: 
    void check_is_any_section_was_opened( const std::string& closing_section_name ) const {
        if ( current_section_path.empty() ) {
            notify( "Closing of section '" + closing_section_name + "' detected, "
                    + "but this section was not opened!" );
        } else {}
    }

    void check_last_section_closing() {
        if ( !current_section_path.empty() ) {
            boost::erase_last( current_section_path, sections_separator );
            notify( "Section '" + current_section_path + "' unclosed!" );
        } else {}
    }

    void check_section_existence( std::string& section_name ) const {
        registered_option_const_it it = std::find_if( registered_options.begin()
                                                      , registered_options.end()
                                                      , boost::bind( &option::corresponds_by_section
                                                                     , _1
                                                                     , section_name ) );
        if ( registered_options.end() == it ) {
            notify_about_incorrect_section_name( section_name );
        } else {}
    }
    
    void notify_about_incorrect_section_name( const std::string& incorrect_section_name ) const {
        std::string what_happened = "Incorrect section name '"
                                    + incorrect_section_name
                                    + "' detected!";
        if ( !current_section_path.empty() ) {
            boost::erase_last( what_happened, "!" );
            what_happened += " in section '"
                             + prepare_full_name_for_log( current_section_path, sections_separator )
                             + "'!"
                             ;
        } else {}
        notify( what_happened );
    }

    void convert_name_depending_on_case_sensitivity( std::string& section_name ) const {
        if ( !case_sensitivity_for_names ) {
            boost::to_lower( section_name );
        } else {}
    }
private:
    void notify_about_meaningless_string( const std::string& s ) const {
        std::string what_happened = "in global scope";
        if ( !current_section_path.empty() ) {
            what_happened = "in section '"
                          + prepare_full_name_for_log( current_section_path, sections_separator )
                          + "'"
                          ;
        } else {}
        what_happened = "Meaningless string '" + s + "' detected " + what_happened + "!";
        notify( what_happened );
    }
};

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_PURE_OPTIONS_OBTAINER_HPP
