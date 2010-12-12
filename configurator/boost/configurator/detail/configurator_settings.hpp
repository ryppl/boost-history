// detail/configurator_settings.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_SETTINGS_HPP
#define BOOST_CONFIGURATOR_SETTINGS_HPP

#include <boost/configurator/detail/types.hpp>
#include <boost/configurator/detail/misc.hpp>

#include <string>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

///
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

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_SETTINGS_HPP
