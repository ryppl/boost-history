// Configurator (C++ library for configuration file parsing)
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_COMMENTS_REMOVER_HPP
#define BOOST_CONFIGURATOR_COMMENTS_REMOVER_HPP

#include <boost/configurator/detail/misc.hpp>
#include <boost/foreach.hpp>
#include <boost/mem_fn.hpp>
#include <boost/assign.hpp>

#include <iostream>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

using namespace boost::assign;

/// \class comments_remover
/// \brief Comments remover.
///
/// Removes one-line comments and multi-line comments.
class comments_remover {
public:
    comments_remover() :
            one_line_comment_sign( "//" )
            , multi_line_comment_begin_sign( "/*" )
            , multi_line_comment_end_sign( "*/" ) {}
private:
    std::string       one_line_comment_sign;
    const std::string multi_line_comment_begin_sign;
    const std::string multi_line_comment_end_sign;
public:
    void use_canonical_one_line_comments() {
        one_line_comment_sign = "#";
    }
public:
    void operator()( str_storage& obtained_strings ) const {
        remove_one_line_comments_from( obtained_strings );
        remove_multi_line_comments_from( obtained_strings );
        remove_empty_strings_from( obtained_strings );
    }
private:
    void remove_one_line_comments_from( str_storage& obtained_strings ) const {
        std::for_each( obtained_strings.begin()
                       , obtained_strings.end()
                       , boost::bind( &comments_remover::remove_one_line_comment
                                      , this
                                      , _1 ) );
    }
    
    void remove_one_line_comment( std::string& s ) const {
        if ( boost::contains( s, one_line_comment_sign ) ) {
            s.erase( boost::find_first( s, one_line_comment_sign ).begin()
                     , s.end() );
        } else {}
    }
private:
    void remove_multi_line_comments_from( str_storage& obtained_strings ) const {
        if ( at_least_one_multi_line_comment_exists_in( obtained_strings ) ) {
            std::string s = concatenate( obtained_strings );
            std::string uncomment_s = extract_uncomment_strings_from( s );
            resplit( uncomment_s, obtained_strings );
        } else {}
    }
    
    bool at_least_one_multi_line_comment_exists_in( const str_storage& obtained_strings ) const {
        BOOST_FOREACH ( const std::string& s, obtained_strings ) {
            if ( multi_line_comment_exists_in( s ) ) {
                return true;
            } else {}
        }
        return false;
    }

    bool multi_line_comment_exists_in( const std::string& s ) const {
        return boost::contains( s, multi_line_comment_begin_sign )
               && boost::contains( s, multi_line_comment_end_sign );
    }

    std::string concatenate( const str_storage& obtained_strings ) const {
        std::string conc;
        BOOST_FOREACH ( const std::string& s, obtained_strings ) {
            conc += s + '\n';
        }
        return conc;
    }
    
    std::string extract_uncomment_strings_from( const std::string& s ) const {
        string_const_it first_it = s.begin();
        string_const_it end_it   = s.end();
        const size_t multi_line_comment_sign_size = 2;

        std::string uncomment_strings;
        while ( true ) {
            string_const_it begin_of_comment = std::search( first_it
                                                            , end_it
                                                            , multi_line_comment_begin_sign.begin()
                                                            , multi_line_comment_begin_sign.end() );
            string_const_it end_of_comment = std::search( first_it
                                                          , end_it
                                                          , multi_line_comment_end_sign.begin()
                                                          , multi_line_comment_end_sign.end() );
            if ( end_it == begin_of_comment ) {
                uncomment_strings.append( first_it, end_it );
                break;
            } else if ( end_it == end_of_comment ) {
                notify( "Unclosed multi-line comment detected!" );
            } else if ( begin_of_comment > end_of_comment ) {
                notify( "Unopened multi-line comment detected!" );
            }

            uncomment_strings.append( first_it, begin_of_comment );
            first_it = end_of_comment + multi_line_comment_sign_size;
        }
        return uncomment_strings;
    }

    void resplit( const std::string& s, str_storage& obtained_strings ) const {
        obtained_strings.clear();
        boost::split( obtained_strings, s, boost::is_any_of( "\n" ) );
    } 
};

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_COMMENTS_REMOVER_HPP
