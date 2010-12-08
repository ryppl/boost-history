// configurator.hpp
// ~~~~~~~~~~~~~~~~
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_HPP
#define BOOST_CONFIGURATOR_HPP

#include <boost/configurator/detail/macro.hpp>
#include <boost/configurator/detail/validators.hpp>
#include <boost/configurator/detail/comments_remover.hpp>
#include <boost/configurator/detail/pure_strings_obtainer.hpp>
#include <boost/configurator/detail/pure_options_obtainer.hpp>
#include <boost/configurator/detail/incorrect_options_checker.hpp>
#include <boost/configurator/detail/necessary_options_checker.hpp>
#include <boost/configurator/detail/semantics_checker.hpp>
#include <boost/configurator/detail/options_repetition_handler.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#ifdef WITH_SINGLETON
#include <boost/configurator/detail/singleton.hpp>
#endif

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

struct configurator_settings {
    configurator_settings() :
            option_name_value_separator( '=' )
            , option_name_value_separator_str( "=" )
            , one_line_comment_sign( "//" ) 
            , case_sensitivity( false ) {}
public:
    char        option_name_value_separator;
    std::string one_line_comment_sign;
    bool        case_sensitivity;
    std::string option_name_value_separator_str;
    //
public:
    configurator_settings& set_case_sensitivity_for_names() {
        case_sensitivity = true;
        return *this;
    }

    configurator_settings& set_name_value_separator( char separator ) {
        option_name_value_separator = separator;
        check_separator_validity();
        return *this;
    }
private:
    void check_separator_validity() const {
        const int ascii_code = option_name_value_separator;
        if ( ascii_code < 0x20 ) {
            detail::o_stream what_happened;
            what_happened << "Symbol (ASCII-code is " << ascii_code
                          << ") is not suitable for name-value separator!"
                          ;
            notify( what_happened.str() );
        } else {}
    }
public:
    //
};

/// \class configurator
/// \brief Configurator.
///
/// Core class.
class configurator : boost::noncopyable {
public:
    configurator() :
            sections_separator( "::" )
            , settings_of_configurator()
            , options_obtainer( settings_of_configurator.option_name_value_separator_str
                                , sections_separator
                                , registered_options
                                , settings_of_configurator.case_sensitivity )
            , check_incorrect_options( registered_options, sections_separator )
            , check_necessary_options( registered_options, sections_separator )
            , check_options_values_semantics( registered_options, sections_separator )
            , handle_options_repetition( registered_options, sections_separator ) {}
private:
    const std::string sections_separator;
private:
    configurator_settings settings_of_configurator;
public:
    configurator_settings& settings() {
        return settings_of_configurator;
    }
public:
    template< typename Option >
    option& add() {
        Option* option = new Option();
        registered_options.push_back( option );
        return *option;
    }
public:
    template< typename Option >
    std::string get_value_of() {
        registered_option_const_it it = std::find( registered_options.begin()
                                                   , registered_options.end()
                                                   , typeid( Option ) );
        if ( registered_options.end() == it ) {
            notify( "Option with type '" + name_of< Option >() + "' is not registered!" );
        } else {}
        const std::string& value = it->value;
        check_factual_existence_of_value< Option >( value );
        return value;
    }
private:
    template< typename Option >
    void check_factual_existence_of_value( const std::string& value ) const {
        if ( value.empty() ) {
            notify( "You have requested a value of option '"
                    + name_of< Option >() + "' and such option was registered, "
                    + "but it missed in configuration file and have not default value!" );
        } else {}
    }
public:
    template
    <
        typename Option
        , typename Value
    >
    Value get_value_of() {
        const std::string value_as_string = get_value_of< Option >();
        Value value();
        try {
            value = boost::lexical_cast< Value >( value_as_string );
        } catch ( const std::exception& /* exc */ ) {
            notify_about_incorrect_type_of_value< Option, Value >( value_as_string );
        }
        return value;
    }

    template
    <
        typename Option
        , typename Value
    >
    configurator& get_value_of( Value& value ) {
        const std::string value_as_string = get_value_of< Option >();
        try {
            value = boost::lexical_cast< Value >( value_as_string );
        } catch ( const std::exception& /* exc */ ) {
            notify_about_incorrect_type_of_value< Option, Value >( value_as_string );
        }
        return *this;
    }
private:
    template
    <
        typename Option
        , typename Value
    >
    void notify_about_incorrect_type_of_value( const std::string& value_as_string ) const {
        detail::type_name type;
        notify( "Value '" + value_as_string + "' of option '" + name_of< Option >()
                + "' cannot be cast to type " + type.name_of< Value >() + "!" );
    }

    template< typename Option >
    std::string name_of() const {
        using boost::spirit::qi::int_;
        using boost::spirit::qi::char_;
        using boost::spirit::qi::parse;
        using boost::spirit::qi::_1;
        using boost::phoenix::ref;
        using boost::phoenix::push_back;
        
        std::string pure_name = typeid( Option ).name();
        std::string name;
        parse( pure_name.begin()
               , pure_name.end()
               , int_ >> +( char_[ push_back( ref(name), _1 ) ] ) );
        return name; 
    }
private:
    options registered_options;
private:
    detail::comments_remover            remove_comments_from;
    detail::pure_strings_obtainer       obtain_pure_strings;
    detail::pure_options_obtainer       options_obtainer;
    detail::incorrect_options_checker   check_incorrect_options;
    detail::necessary_options_checker   check_necessary_options;
    detail::semantics_checker           check_options_values_semantics;
    detail::options_repetition_handler  handle_options_repetition;
public:
    void load( const std::string& path_to_configuration_file ) {
        primary_check( path_to_configuration_file ); 
        detail::str_storage obtained_strings = obtain_pure_strings( path_to_configuration_file );
        check_actual_data_existence_in( obtained_strings );
        remove_comments_from( obtained_strings );
        pure_options factual_obtained_options = options_obtainer( obtained_strings );
        prepare_names_depending_on_case_sensitivity( factual_obtained_options );
        check_incorrect_options( factual_obtained_options );
        check_necessary_options( factual_obtained_options );
        handle_options_repetition( factual_obtained_options );
        store_obtained_values( factual_obtained_options );
        check_options_values_semantics();
    }
private:
    void primary_check( const std::string& path_to_configuration_file ) const {
        check_registered_options_existence();
        check_configuration_file_existence( path_to_configuration_file );
    } 
    
    void check_registered_options_existence() const {
        if ( registered_options.empty() ) {
            notify( "Cannot continue, because no one option has not been registered!" );
        } else {}
    }

    void check_configuration_file_existence( const std::string& path_to_configuration_file ) const {
        try {
            detail::pure_check_path_existence( path_to_configuration_file );
            detail::check_is_it_file( path_to_configuration_file );
        } catch ( const std::exception& exc ) {
            notify( std::string( "Invalid path to configuration file: " ) + exc.what() );
        }
    } 
private: 
    void check_actual_data_existence_in( detail::str_storage& obtained_strings ) const {
        if ( at_least_one_option_has_default_value() ) {
            return;
        } else {} 
        
        BOOST_FOREACH ( std::string& s, obtained_strings ) {
            boost::trim( s );
        }

        if ( obtained_strings.empty() ) {
            notify( "No actual data in configuration file, is file empty?" );
        } else {} 
    }

    bool at_least_one_option_has_default_value() const {
        option_const_it it = std::find_if( registered_options.begin()
                                           , registered_options.end()
                                           , boost::mem_fn( &option::already_has_default_value ) );
        return registered_options.end() != it;
    }
    
    void prepare_names_depending_on_case_sensitivity( pure_options& factual_obtained_options ) {
        if ( !settings_of_configurator.case_sensitivity ) {
            BOOST_FOREACH ( option& option, registered_options ) {
                boost::to_lower( option.location );
            }

            BOOST_FOREACH ( pure_option& option, factual_obtained_options ) {
                boost::to_lower( option.location );
            }
        } else {}
    }

    void store_obtained_values( const pure_options& factual_obtained_options ) {
        BOOST_FOREACH ( const pure_option& option, factual_obtained_options ) {
            option_it it = std::find( registered_options.begin()
                                      , registered_options.end()
                                      , option.location );
            if ( registered_options.end() != it ) {
                store_if_need( it->value, option.value );
            } else {}
        } 
    }
    
    void store_if_need( std::string& value_of_registered_option
                        , const std::string& value_of_obtained_option ) {
        if ( value_of_obtained_option != value_of_registered_option ) {
            value_of_registered_option.assign( value_of_obtained_option.begin()
                                               , value_of_obtained_option.end() );
        } else {}
    }
public:
    void reload( const std::string& new_path_to_configuration_file ) {
        load( new_path_to_configuration_file );
    }

    void reload() {
        const std::string previous_path_to_configuration_file = obtain_pure_strings.stored_path();
        reload( previous_path_to_configuration_file );
    }
};

#ifdef WITH_SINGLETON
/// \class single_configurator
/// \brief Singleton-variant of configurator class.
class single_configurator : public configurator
                            , public detail::singleton< single_configurator > {
private:
    single_configurator() {}
    ~single_configurator() {}
    friend class detail::singleton< single_configurator >;
};
#endif

} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_HPP
