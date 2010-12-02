// Configurator (C++ library for configuration file parsing)
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_HPP
#define BOOST_CONFIGURATOR_HPP

#include <boost/configurator/detail/option.hpp>
#include <boost/configurator/detail/validators.hpp>
#include <boost/configurator/detail/comments_remover.hpp>
#include <boost/configurator/detail/pure_strings_obtainer.hpp>
#include <boost/configurator/detail/pure_options_obtainer.hpp>
#include <boost/configurator/detail/incorrect_options_checker.hpp>
#include <boost/configurator/detail/necessary_options_checker.hpp>
#include <boost/configurator/detail/semantics_checker.hpp>
#include <boost/configurator/detail/options_repetition_handler.hpp>

#ifdef WITH_SINGLETON
#include <boost/configurator/detail/singleton.hpp>
#endif

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \class configurator
/// \brief Configurator.
///
/// Core class.
class configurator : boost::noncopyable {
public:
    configurator() :
            option_name_value_separator( "=" )
            , sections_separator( "::" )
            , case_sensitivity_for_names( false )
            , options_obtainer( option_name_value_separator
                                , sections_separator
                                , unique_names_of_sections
                                , case_sensitivity_for_names )
            , check_incorrect_options( registered_options, sections_separator )
            , check_necessary_options( registered_options, sections_separator )
            , check_options_values_semantics( registered_options, sections_separator )
            , handle_options_repetition( registered_options, sections_separator )
            , reparsing( false ) {}
private:
    std::string         option_name_value_separator;
    const std::string   sections_separator;
public:
    void set_name_value_separator( char new_separator ) {
        check_separator_validity( new_separator );
        option_name_value_separator = new_separator;
    }
private:
    void check_separator_validity( char new_separator ) const {
        const int ascii_code = new_separator;
        if ( ascii_code < 0x20 ) {
            detail::o_stream what_happened;
            what_happened << "Symbol (ASCII-code is " << ascii_code
                          << ") is not suitable for name-value separator!"
                          ;
            notify( what_happened.str() );
        } else {}
    }
private:
    bool case_sensitivity_for_names;
public:
    configurator& set_case_sensitivity_for_names() {
        case_sensitivity_for_names = true;
        return *this;
    }
public:
    void use_canonical_one_line_comments() {
        remove_comments_from.use_canonical_one_line_comments();
    }
private:
    options registered_options;
    detail::str_set unique_names_of_sections;
public:
    configurator& in_section( const std::string& section_name ) {
        store_unique_section_name( section_name );
        if ( this_is_begin_call() ) {
            current_section_path.clear();
        } else {}
        current_section_path += section_name + sections_separator;
        return *this;
    }
private:
    void store_unique_section_name( const std::string& section_name ) {
        std::string name( section_name.begin(), section_name.end() );
        convert_name_depending_on_case_sensitivity( name );
        unique_names_of_sections.insert( name );
    }

    bool this_is_begin_call() const {
        return !std::equal( sections_separator.begin()
                            , sections_separator.end()
                            , current_section_path.rbegin() );
    }
public:
    configurator& in( const std::string& section_name ) {
        return in_section( section_name );
    }

    configurator& from_section( const std::string& section_name ) {
        return in_section( section_name );
    }
    
    configurator& from( const std::string& section_name ) {
        return from_section( section_name );
    }
public:
    option& add_option( const std::string& option_name ) {
        std::string full_name_of_option( option_name.begin(), option_name.end() );
        convert_name_depending_on_case_sensitivity( full_name_of_option );
        registered_options.push_back( new option( full_name_of_option, sections_separator ) );
        return registered_options.back();
    }
    
    option& add( const std::string& option_name ) {
        return add_option( option_name );
    }

    option& add_option_here( const std::string& option_name ) {
        cut_current_section_path_if_necessary();
        std::string full_name_of_option = current_section_path + option_name;
        convert_name_depending_on_case_sensitivity( full_name_of_option );
        current_section_path += option_name;
        registered_options.push_back( new option( full_name_of_option, sections_separator ) );
        return registered_options.back();
    }
    
    option& add_here( const std::string& option_name ) {
        return add_option_here( option_name );
    }
private:
    void cut_current_section_path_if_necessary() {
        detail::string_it previous_option_name_begin =
                boost::find_last( current_section_path, sections_separator ).end();
        detail::string_it previous_option_name_end = current_section_path.end();
        current_section_path.erase( previous_option_name_begin, previous_option_name_end );
    } 

    void convert_name_depending_on_case_sensitivity( std::string& full_name_of_option ) const {
        if ( !case_sensitivity_for_names ) {
            boost::to_lower( full_name_of_option );
        } else {}
    }
private:
    // detail::str_storage multiply_sections_names;
public:
    configurator& allow_section_multiplicity() {
        // Not implemented yet, planning...
        //
        // std::string section_name( current_section_path.begin(), current_section_path.end() );
        // convert_name_depending_on_case_sensitivity( section_name );
        // multiply_sections_names.push_back( section_name );
        return *this;
    }
private:
    std::string                         current_section_path;
    detail::comments_remover            remove_comments_from;
    detail::pure_strings_obtainer       obtain_pure_strings;
    detail::pure_options_obtainer       options_obtainer;
    detail::incorrect_options_checker   check_incorrect_options;
    detail::necessary_options_checker   check_necessary_options;
    detail::semantics_checker            check_options_values_semantics;
    detail::options_repetition_handler  handle_options_repetition;
public:
    void parse( const std::string& path_to_configuration_file ) {
        check_registered_options_existence();
        check_configuration_file_existence( path_to_configuration_file );
        detail::str_storage obtained_strings = obtain_pure_strings( path_to_configuration_file );
        check_actual_data_existence( obtained_strings );
        remove_comments_from( obtained_strings );
        pure_options factual_obtained_options = options_obtainer( obtained_strings );
        prepare_names_depending_on_case_sensitivity( factual_obtained_options );
        check_incorrect_options( factual_obtained_options );
        check_necessary_options( factual_obtained_options );
        handle_options_repetition( factual_obtained_options );
        store_obtained_values( factual_obtained_options );
        check_options_values_semantics();
        current_section_path.clear();
    }

    void load( const std::string& path_to_configuration_file ) {
        parse( path_to_configuration_file );
    }
private:
    bool reparsing;

    struct reparsing_flag_switch {
        explicit reparsing_flag_switch( bool& _reparsing ) :
                reparsing( _reparsing ) {
            reparsing = true;
        }
        ~reparsing_flag_switch() {
            reparsing = false;
        }
    private:
        bool& reparsing;
    };
public:
    void reparse( const std::string& new_path_to_configuration_file ) {
        reparsing_flag_switch s( reparsing );
        parse( new_path_to_configuration_file );
    }

    void reload( const std::string& new_path_to_configuration_file ) {
        reparse( new_path_to_configuration_file );
    }

    void reparse() {
        reparsing_flag_switch s( reparsing );
        const std::string previous_path_to_configuration_file = obtain_pure_strings.stored_path();
        parse( previous_path_to_configuration_file );
    }

    void reload() {
        reparse();
    }
private:
    void check_registered_options_existence() const {
        if ( registered_options.empty() ) {
            notify( "Cannot parse configuration file, because no one option registered!" );
        } else {}
    }

    void check_configuration_file_existence( const std::string& path_to_configuration_file ) const {
        try {
            detail::pure_check_path_existence( path_to_configuration_file );
        } catch ( const std::exception& exc ) {
            notify( std::string( "Invalid path to configuration file: " ) + exc.what() );
        }
    }
    
    void check_actual_data_existence( detail::str_storage& obtained_strings ) const {
        if ( at_least_one_option_has_default_value() ) {
            return;
        } else {} 
        
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
        if ( !case_sensitivity_for_names ) {
            BOOST_FOREACH ( option& option, registered_options ) {
                boost::to_lower( option.name );
            }

            BOOST_FOREACH ( pure_option& option, factual_obtained_options ) {
                boost::to_lower( option.name );
            }
        } else {}
    }

    void store_obtained_values( const pure_options& factual_obtained_options ) {
        BOOST_FOREACH ( const pure_option& option, factual_obtained_options ) {
            option_it it = std::find( registered_options.begin()
                                      , registered_options.end()
                                      , option.name );
            if ( registered_options.end() != it ) {
                store_if_need( it->value, option.value );
            } else {}
        }

        if ( reparsing ) {
            use_default_value_reserves( factual_obtained_options );
        } else {}
    }
    
    void store_if_need( std::string& value_of_registered_option
                        , const std::string& value_of_obtained_option ) {
        if ( value_of_obtained_option != value_of_registered_option ) {
            value_of_registered_option.assign( value_of_obtained_option.begin()
                                               , value_of_obtained_option.end() );
        } else {}
    }

    void use_default_value_reserves( const pure_options& factual_obtained_options ) {
        // Not implemented yet.
    }
public:
    std::string get_value( const std::string& name_of_option ) {
        std::string full_name_of_option( name_of_option.begin(), name_of_option.end() );
        return get_value_by_name( full_name_of_option );
    }

    std::string get( const std::string& name_of_option ) {
        return get_value( name_of_option );
    } 
public:
    template< typename ValueType >
    ValueType get_value( const std::string& name_of_option ) {
        std::string value_as_string = get_value( name_of_option );
        ValueType value;
        try {
            value = boost::lexical_cast< ValueType >( value_as_string );
        } catch ( const std::exception& /* exc */ ) {
            notify( "Value '" + value_as_string + "' of option '" + name_of_option
                    + "' cannot be cast to <" + get_type_identifier< ValueType >() + ">!" );
        }
        return value;
    }
    
    template< typename ValueType >
    ValueType get( const std::string& name_of_option ) {
        return get_value< ValueType >( name_of_option );
    }

    template< typename ValueType >
    configurator& get_value( const std::string& name_of_option, ValueType& value ) {
        value = get_value< ValueType >( name_of_option );
        return *this;
    }

    template< typename ValueType >
    configurator& get( const std::string& name_of_option, ValueType& value ) {
        return get_value( name_of_option, value );
    }
public:
    std::string get_value_from_here( const std::string& name_of_option ) {
        cut_current_section_path_if_necessary();
        std::string full_name_of_option = current_section_path + name_of_option;
        current_section_path += name_of_option;
        return get_value_by_name( full_name_of_option ); 
    }
    
    std::string get_from_here( const std::string& name_of_option ) {
        return get_value_from_here( name_of_option );
    }

    template< typename ValueType >
    ValueType get_value_from_here( const std::string& name_of_option ) {
        std::string value_as_string = get_value_from_here( name_of_option );
        ValueType value;
        try {
            value = boost::lexical_cast< ValueType >( value_as_string );
        } catch ( const std::exception& /* exc */ ) {
            const std::string full_name_of_option = current_section_path;
            notify( "Value '" + value_as_string + "' of option '"
                    + detail::prepare_full_name_for_log( full_name_of_option, sections_separator )
                    + "' cannot be cast to <" + get_type_identifier< ValueType >() + ">!" );
        }
        return value;
    }
    
    template< typename ValueType >
    ValueType get_from_here( const std::string& name_of_option ) {
        return get_value_from_here< ValueType >( name_of_option );
    }

    template< typename ValueType >
    configurator& get_value_from_here( const std::string& name_of_option, ValueType& value ) {
        value = get_value_from_here< ValueType >( name_of_option );
        return *this;
    }

    template< typename ValueType >
    configurator& get_from_here( const std::string& name_of_option, ValueType& value ) {
        return get_value_from_here( name_of_option, value );
    }
private:
    std::string get_value_by_name( std::string& full_name_of_option ) const {
        convert_name_depending_on_case_sensitivity( full_name_of_option );
        option_const_it it = std::find( registered_options.begin()
                                        , registered_options.end()
                                        , full_name_of_option );
        std::string value;
        if ( registered_options.end() != it ) {
            const std::string& value_of_registered_option = it->value;
            check_factual_obtaining( value_of_registered_option, full_name_of_option );
            value.assign( value_of_registered_option.begin(), value_of_registered_option.end() );
        } else {
            notify( "You request a value of option '"
                    + detail::prepare_full_name_for_log( full_name_of_option, sections_separator )
                    + "', but such option not registered!" );
        }
        return value;
    }

    void check_factual_obtaining( const std::string& value_of_registered_option
                                  , const std::string& full_name_of_option ) const {
        if ( value_of_registered_option.empty() ) {
            notify( "You request a value of option '"
                    + detail::prepare_full_name_for_log( full_name_of_option, sections_separator )
                    + "' and such option was registered, but it missed in configuration file!" );
        } else {}
    }
private:
    template< typename ArgType >
    std::string get_type_identifier() const {
        std::string identifier;
        
        if      ( typeid( ArgType ) == typeid( bool ) )                { identifier = "bool"; }
        else if ( typeid( ArgType ) == typeid( char ) )                { identifier = "char"; }
        else if ( typeid( ArgType ) == typeid( signed char ) )         { identifier = "signed char"; }
        else if ( typeid( ArgType ) == typeid( unsigned char ) )       { identifier = "unsigned char"; }
        else if ( typeid( ArgType ) == typeid( int ) )                 { identifier = "int"; }
        else if ( typeid( ArgType ) == typeid( signed int ) )          { identifier = "signed int"; }
        else if ( typeid( ArgType ) == typeid( unsigned int ) )        { identifier = "unsigned int"; }
        else if ( typeid( ArgType ) == typeid( short int ) )           { identifier = "short int"; }
        else if ( typeid( ArgType ) == typeid( long int ) )            { identifier = "long int"; }
        else if ( typeid( ArgType ) == typeid( unsigned long int ) )   { identifier = "unsigned long int"; }
        else if ( typeid( ArgType ) == typeid( unsigned short int ) )  { identifier = "unsigned short int"; }
        else if ( typeid( ArgType ) == typeid( signed long int ) )     { identifier = "signed long int"; }
        else if ( typeid( ArgType ) == typeid( signed short int ) )    { identifier = "signed short int"; }
        else if ( typeid( ArgType ) == typeid( float ) )               { identifier = "float"; }
        else if ( typeid( ArgType ) == typeid( double ) )              { identifier = "double"; }
        else if ( typeid( ArgType ) == typeid( long double ) )         { identifier = "long double"; }
        else if ( typeid( ArgType ) == typeid( std::string ) )         { identifier = "std::string"; }
        else                                                           { identifier = "unknown"; }

        return identifier;
    }
};

#ifdef WITH_SINGLETON
/// \class single_configurator
/// \brief Singleton-variant of configurator class.
class single_configurator :
        public configurator
        , public detail::singleton< single_configurator > {
private:
    single_configurator() {}
    single_configurator( const single_configurator& another ) {}
    ~single_configurator() {}
    single_configurator& operator=( const single_configurator& another ) {
        return *this; 
    }
    friend class detail::singleton< single_configurator >;
};
#endif

} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_HPP
