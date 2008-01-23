// named_writer.hpp

// Boost Logging library
//
// Author: John Torjo, www.torjo.com
//
// Copyright (C) 2007 John Torjo (see www.torjo.com for email)
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.
// See http://www.torjo.com/log2/ for more details


#ifndef JT28092007_named_writer_HPP_DEFINED
#define JT28092007_named_writer_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/format_ts.hpp>

// all destinations
#include <boost/logging/format/destination/file.hpp>
#include <boost/logging/format/destination/named.hpp>
#include <boost/logging/format/destination/rolling_file.hpp>

// all formats 
#include <boost/logging/format/formatter/named_spacer.hpp>
#include <boost/logging/format/formatter/thread_id.hpp>

#ifndef __GNUC__
// Boost 1.33.1 - GCC has error compiling microsec_clock
#include <boost/logging/format/formatter/high_precision_time.hpp>
namespace boost { namespace logging { namespace detail { 
    typedef formatter::high_precision_time formatter_time_type ; 
    typedef formatter::high_precision_time_t<formatter::do_convert_format::append> formatter_time_type_append ; 
}}}
#else
#include <boost/logging/format/formatter/time.hpp>
namespace boost { namespace logging { namespace detail { 
    typedef formatter::time formatter_time_type ; 
    typedef formatter::time_t<formatter::do_convert_format::append> formatter_time_type_append ; 
}}}
#endif


namespace boost { namespace logging { namespace writer {

/** 
@brief Composed of a named formatter and a named destinations. Thus, you can specify the formatting and destinations as strings

@code
#include <boost/logging/format/named_writer.hpp>
@endcode


Contains a very easy interface for using formatters and destinations:
- at construction, specify 2 params: the formatter string and the destinations string

Setting the formatters and destinations to write to is extremely simple:

@code
// first param - the formatter(s) , second param : the destination(s)
g_l()->writer().write("%time%($hh:$mm.$ss.$mili) [%idx%] |\n", "cout file(out.txt) debug");

// set the formatter(s)
g_l()->writer().format("%time%($hh:$mm.$ss.$mili) [%idx%] |\n");

g_l()->writer().destination("cout file(out.txt) debug");
@endcode

The syntax of the format string:

The syntax of the destination string:


FIXME - need to allow tags by default, here!

@param format_write_ the underlying format writer
*/
template<class format_write_ /* = default_ */ > struct named {
    typedef typename use_default< format_write_, format_write< formatter::base<> , destination::base<> > >::type format_write_type;

    typedef typename format_write_type::formatter_base_type formatter_base_type;
    typedef typename format_write_type::destination_base_type destination_base_type;
    typedef typename format_write_type::lock_resource_type lock_resource_type;

    typedef hold_string_type string_type;

    named() {
        m_writer.add_formatter( m_format_before);
        m_writer.add_formatter( m_format_after);
        m_writer.add_destination( m_destination);

        init();
    }

    /** @brief sets the format string: what should be before, and what after the original message, separated by "|"

    Example: \n
    "[%idx%] |\n" - this writes "[%idx%] " before the message, and "\n" after the message

    If "|" is not present, the whole message is prepended to the message
    */
    void format(const string_type & format_str) {
        m_format_str = format_str;

        typename string_type::size_type idx = format_str.find('|');
        string_type before, after;
        if ( idx != string_type::npos) {
            before = format_str.substr(0, idx);
            after = format_str.substr(idx + 1);
        }
        else
            before = format_str;

        format( before, after);
    };

    /** @brief sets the format strings (what should be before, and what after the original message)    
    */
    void format(const string_type & format_before_str, const string_type & format_after_str) {
        m_format_before_str = format_before_str;
        m_format_after_str = format_after_str;

        set_and_configure( m_format_before, format_before_str, parse_formatter() );
        set_and_configure( m_format_after, format_after_str, parse_formatter() );
    };

    /** @brief sets the destinations string - where should logged messages be outputted
    */
    void destination(const string_type & destination_str) {
        m_destination_str = destination_str;
        set_and_configure( m_destination, destination_str, parse_destination() );
    }

    /** @brief Specifies the formats and destinations in one step
    */
    void write(const string_type & format_str, const string_type & destination_str) {
        format( format_str);
        destination( destination_str);
    }

    const string_type & format() const              { return m_format_str; }
    const string_type & destination() const         { return m_destination_str; }

    template<class msg_type> void operator()(msg_type & msg) const {
        m_writer(msg);
    }

    /** @brief Replaces a destination from the named destination.

    You can use this, for instance, when you want to share a destination between multiple named writers.        
    */
    template<class destination> void replace_destination(const string_type & name, destination d) {
        m_destination.del(name);
        m_destination.add(name, d);
    }

    /** @brief Replaces a formatter from the named formatter.

    You can use this, for instance, when you want to share a formatter between multiple named writers.        
    */
    template<class formatter> void replace_formatter(const string_type & name, formatter d) {
        if ( m_format_before_str.find(name) != string_type::npos) {
            m_format_before.del(name);
            m_format_before.add(name, d);
        }

        if ( m_format_after_str.find(name) != string_type::npos) {
            m_format_after.del(name);
            m_format_after.add(name, d);
        }
    }

private:
    struct parse_destination {
        bool has_manipulator_name() const { return !m_manipulator.empty(); }
        string_type get_manipulator_name() const { 
            BOOST_ASSERT( has_manipulator_name() );
            if ( m_manipulator[0] == '-' || m_manipulator[0] == '+')
                // + or - -> turning on or off a destination
                return m_manipulator.substr(1);
            else
                return m_manipulator; 
        }
        void clear() { m_manipulator.clear(); }

        void add(char_type c) {
            // destination always follows ' '
            if ( c == ' ')
                clear();
            else
                m_manipulator += c;
        }
    private:
        string_type m_manipulator;
    };

    struct parse_formatter {
        // formatter starts and ends with %
        bool has_manipulator_name() const { 
            if ( m_manipulator.empty() ) 
                return false;
            if ( m_manipulator.size() > 1)
                if ( m_manipulator[0] == '%' && (*m_manipulator.rbegin() == '%') )
                    return true;

            return false;
        }

        string_type get_manipulator_name() const { 
            BOOST_ASSERT( has_manipulator_name() );
            // ignore starting and ending %
            return m_manipulator.substr( 1, m_manipulator.size() - 2); 
        }
        void clear() { m_manipulator.clear(); }

        void add(char_type c) {
            if ( has_manipulator_name() )
                // was a manipulator until now
                clear();

            if ( c == '%') {
                m_manipulator += c;
                if ( !has_manipulator_name() )
                    // it could be the start of a formatter
                    m_manipulator = '%';
            }
            else if ( m_manipulator.empty())
                ; // ignore this char - not from a manipulator
            else if ( m_manipulator[0] == '%')
                m_manipulator += c;
            else
                // manipulator should always start with %
                BOOST_ASSERT(false);
        }
    private:
        string_type m_manipulator;
    };


    template<class manipulator, class parser_type> void set_and_configure(manipulator & manip, const string_type & name, parser_type parser) {
        // need to parse string
        string_type prev ;
        bool parsing_params = false;
        string_type params;
        string_type stripped_str;
        for ( typename string_type::const_iterator b = name.begin(), e = name.end(); b != e; ++b) {
            if ( (*b == '(') && !parsing_params) {
                if ( parser.has_manipulator_name() ) {
                    parsing_params = true;
                    params.clear();
                }
            }
            else if ( (*b == ')') && parsing_params) {
                BOOST_ASSERT ( parser.has_manipulator_name() );
                manip.configure_inner( parser.get_manipulator_name(), params);
                parser.clear();
                parsing_params = false;
            }
            else {
                if ( parsing_params)
                    params += *b;
                else {
                    stripped_str += *b;
                    parser.add( *b);
                }
            }  
        }
        manip.string( stripped_str);
    }

private:
    void init() {
        m_format_before
            .add( BOOST_LOG_STR("idx"), formatter::idx() )
            .add( BOOST_LOG_STR("time"), ::boost::logging::detail::formatter_time_type( BOOST_LOG_STR("$hh:$mm:$ss") ) )
            .add( BOOST_LOG_STR("thread_id"), formatter::thread_id() );

        m_format_after
            .add( BOOST_LOG_STR("idx"), formatter::idx_t<formatter::do_convert_format::append>() )
            .add( BOOST_LOG_STR("time"), ::boost::logging::detail::formatter_time_type_append( BOOST_LOG_STR("$hh:$mm:$ss") ) )
            .add( BOOST_LOG_STR("thread_id"), formatter::thread_id_t<formatter::do_convert_format::append>() );

        m_destination
            .add( BOOST_LOG_STR("file"), destination::file("out.txt") )
            .add( BOOST_LOG_STR("file2"), destination::file("out.txt") )
            .add( BOOST_LOG_STR("rol_file"), destination::rolling_file("out.txt") )
            .add( BOOST_LOG_STR("rol_file2"), destination::rolling_file("out.txt") )
            .add( BOOST_LOG_STR("cout"), destination::cout() )
            .add( BOOST_LOG_STR("debug"), destination::dbg_window() );
    }

private:
    formatter::named_spacer_t< formatter::do_convert_format::prepend, formatter_base_type, lock_resource_type > m_format_before;
    formatter::named_spacer_t< formatter::do_convert_format::append,  formatter_base_type, lock_resource_type > m_format_after;
    destination::named_t< destination_base_type, lock_resource_type > m_destination;
    format_write_type m_writer;

    string_type m_format_str;
    string_type m_format_before_str, m_format_after_str;
    string_type m_destination_str;
};

}}}


#endif


