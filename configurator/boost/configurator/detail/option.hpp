// Configurator (C++ library for configuration file parsing)
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_OPTION_HPP
#define BOOST_CONFIGURATOR_OPTION_HPP

#include <boost/configurator/detail/misc.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

/// \class option
/// \brief One option.
///
/// Presents functionality of one configuration option.
class option {
public:    
    explicit option( const std::string& _name, const std::string& _sections_separator ) :
            sections_separator( _sections_separator )
            , name( _name.begin(), _name.end() )
            , value()
            , semantic( no_semantic )
            , is_necessary( false )
            , multi_values_allowed( false ) {}
private:
    const std::string& sections_separator;
public:
    std::string name;
    std::string value;
    value_semantic semantic;
    bool is_necessary;
    bool multi_values_allowed;
public:
    bool operator==( const std::string& full_name ) const {
        return full_name == name;
    }
private:
    std::string default_value_reserve;
public:
    void use_default_value_reserve() {
        if ( !default_value_reserve.empty() ) {
            value.assign( default_value_reserve.begin(), default_value_reserve.end() );
        } else {}
    }
public:
    template< typename ValueType >
    option& default_value( const ValueType& _value ) {
        check_option_necessity();
        try {
            value = boost::lexical_cast< std::string >( _value );
            default_value_reserve.assign( value.begin(), value.end() );
        } catch ( const std::exception& /* exc */ ) {
            notify( "Default value for option '" + name + "' cannot be stringified!" );
        }
        return *this;
    }
private:
    void check_option_necessity() const {
        if ( is_necessary ) {
            notify( "Option '"
                    + prepare_full_name_for_log( name, sections_separator )
                    + "' registered as necessary, so it cannot have default_value!" );
        } else {}
    }
public:
    option& default_value( const std::string& _value ) {
        value.assign( _value.begin(), _value.end() );
        return *this;
    }
public:
    option& necessary() {
        check_default_value_existence();
        is_necessary = true;
        return *this;
    }
private:
    void check_default_value_existence() const {
        if ( !value.empty() ) {
            notify( "Option '"
                    + prepare_full_name_for_log( name, sections_separator )
                    + "' already have default value '" + value + "', "
                    + "so it cannot be necessary!" );
        } else {}
    }
public:
    option& check_semantic( const value_semantic& _semantic ) {
        check_correctness_of( _semantic );
        semantic = _semantic;
        return *this;
    }
private:
    void check_correctness_of( const value_semantic& semantic ) const {
        if ( semantic < no_semantic || semantic > exp_record ) {
            o_stream what_happened;
            what_happened << "invalid semantic value '" << semantic
                          << "', use supported semantic only (see documentation)!"
                          ;
            notify( what_happened.str() );
        } else {}
    }
public:
    bool semantic_defined() const {
        return no_semantic != semantic;
    }
public:
    option& allow_multi_values() {
        multi_values_allowed = true;
        return *this;
    }
public:
    bool already_has_default_value() const  { return !value.empty(); }
    bool it_is_necessary() const            { return is_necessary; }
    bool empty() const                      { return value.empty(); }
};

/// \struct pure_option
/// \brief 
///
/// Presents pure option obtained from configuration file.
struct pure_option {
    pure_option() {}
    pure_option( const std::string& _name, const std::string& _value ) :
            name( _name.begin(), _name.end() )
            , value( _value.begin(), _value.end() ) {}
public:
    std::string name;
    std::string value;
public:
    bool empty() const {
        return value.empty();
    }

    bool operator==( const std::string& _name ) const {
        return _name == name;
    }

    bool operator==( const pure_option& another ) const {
        return another.name == name;
    }
};

inline bool operator<( const pure_option& left, const pure_option& right ) {
    return left.name < right.name;
}

} // namespace detail

typedef detail::option              option;

typedef boost::ptr_vector< option > options;
typedef options::iterator           option_it;
typedef options::const_iterator     option_const_it;

typedef detail::pure_option         pure_option;
typedef std::vector< pure_option >  pure_options;
typedef pure_options::iterator      pure_option_it;

} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_OPTION_HPP
