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
        BOOST_FOREACH ( std::string& s, obtained_strings ) {
            if ( one_line_comment_in_this( s ) ) {
                remove_one_line_comment_from_this( s );
            } else {}
        }
    }
    
    bool one_line_comment_in_this( const std::string& s ) const {
        return boost::contains( s, one_line_comment_sign );
    }

    void remove_one_line_comment_from_this( std::string& s ) const {
        s.erase( boost::find_first( s, one_line_comment_sign ).begin()
                 , s.end() );
    }
private:
    void remove_multi_line_comments_from( str_storage& obtained_strings ) const {
        align_multi_line_comments_in( obtained_strings ); 
        remove_aligned_multi_line_comments_from( obtained_strings );
    } 
    
    void align_multi_line_comments_in( str_storage& obtained_strings ) const {
        bool waiting_for_comment_closing = false; 
        str_it last_string;
        
        for ( str_it it = obtained_strings.begin(); obtained_strings.end() != it; ++it ) {
            std::string& s = *it;

            if ( waiting_for_comment_closing ) {
                if ( comment_closing_in_this( s ) ) {
                    waiting_for_comment_closing = false;
                } else {}
                *last_string += s;
                s.clear();
            } else {
                check_unopened_multi_line_comment( s );
                string_it begin_of_comment = boost::find_first( s, multi_line_comment_begin_sign ).begin();
                if ( s.end() != begin_of_comment ) {
                    if ( comment_closing_in_this( s ) ) {
                        remove_multi_line_comment_from_this( s, begin_of_comment );
                    } else {
                        waiting_for_comment_closing = true;
                        last_string = it;
                    }
                } else {}
            }
        }

        check_unclosed_multi_line_comment( waiting_for_comment_closing );
    }

    bool comment_closing_in_this( const std::string& s ) const {
        return boost::contains( s, multi_line_comment_end_sign );
    }

    void remove_aligned_multi_line_comments_from( str_storage& obtained_strings ) const {
        BOOST_FOREACH ( std::string& s, obtained_strings ) {
            string_it begin_of_comment = boost::find_first( s, multi_line_comment_begin_sign ).begin();
            if ( s.end() != begin_of_comment ) {
                remove_multi_line_comment_from_this( s, begin_of_comment ); 
            } else {}
        }
    }

    void remove_multi_line_comment_from_this( std::string& s, string_it begin_of_comment ) const {
        s.erase( begin_of_comment
                 , boost::find_first( s, multi_line_comment_end_sign ).end() );
    }
    
    void check_unopened_multi_line_comment( const std::string& s ) const {
        if ( !boost::contains( s, multi_line_comment_begin_sign ) 
             && boost::contains( s, multi_line_comment_end_sign ) ) {
            notify( "Unopened multi-line comment detected in string '" + s + "'!" );
        } else {}
    }
    
    void check_unclosed_multi_line_comment( bool waiting_for_comment_closing ) const {
        if ( waiting_for_comment_closing ) {
            notify( "Unclosed multi-line comment detected!" );
        } else {}
    } 
};

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_COMMENTS_REMOVER_HPP
