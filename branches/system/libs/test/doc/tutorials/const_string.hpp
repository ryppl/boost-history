//  (C) Copyright Gennadiy Rozental 2001-2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : simple string class definition
// ***************************************************************************

#ifndef  CONST_STRING_HPP
#define  CONST_STRING_HPP

// STL
#include <string>
using std::string;

namespace common_layer {

// ************************************************************************** //
// **************                 const_string                 ************** //
// ************************************************************************** //

class const_string {
public:
    // Subtypes
    typedef     char const*                                 iterator;
    typedef     char const*                                 const_iterator;
    typedef     std::reverse_iterator<iterator,char, char const&> reverse_iterator;
    typedef     reverse_iterator                            const_reverse_iterator;

    // Constructor
    const_string()
    : m_begin( "" ), m_end( m_begin )                      {}

    // Copy constructor is generated by compiler

    const_string( const std::string& s )
    : m_begin( s.c_str() ),
      m_end( m_begin + s.length() )                        {}

    const_string( char const* s );

    const_string( char const* s, size_t length )
    : m_begin( s ), m_end( m_begin + length )              { if( length == 0 ) erase(); }

    const_string( char const* first, char const* last )
    : m_begin( first ), m_end( last )                      {}

    // data access methods
    char            operator[]( size_t index ) const        { return m_begin[index]; }
    char            at( size_t index ) const;

    char const*     data() const                            { return m_begin; }

    // length operators
    size_t          length() const                          { return m_end - m_begin; }
    bool            is_empty() const                        { return m_end == m_begin; }

    void            erase()                                 { m_begin = m_end = ""; }
    void            resize( size_t new_len )                { if( m_begin + new_len < m_end ) m_end = m_begin + new_len; }
    void            rshorten( size_t shift = 1 )            { m_end  -= shift; if( m_end <= m_begin ) erase(); }
    void            lshorten( size_t shift = 1 )            { m_begin += shift; if( m_end <= m_begin ) erase(); }

    // Assignment operators
    const_string&   operator=( const_string const& s );
    const_string&   operator=( string const& s )            { return *this = const_string( s ); }
    const_string&   operator=( char const* s )              { return *this = const_string( s ); }

    const_string&   assign( const_string const& s )         { return *this = s; }
    const_string&   assign( string const& s, size_t len )   { return *this = const_string( s.data(), len ); }
    const_string&   assign( string const& s )               { return *this = const_string( s ); }
    const_string&   assign( char const* s )                 { return *this = const_string( s ); }
    const_string&   assign( char const* s, size_t len )     { return *this = const_string( s, len ); }
    const_string&   assign( char const* f, char const* l )  { return *this = const_string( f, l ); }

    void            swap( const_string& s )                 {
       // do not want to include alogrithm
        char const* tmp1 = m_begin;
        char const* tmp2 = m_end;

        m_begin     = s.m_begin;
        m_end       = s.m_end;

        s.m_begin   = tmp1;
        s.m_end     = tmp2;
    }

    // Comparison operators
    friend bool     operator==( const_string const& s1, const_string const& s2 );
    friend bool     operator==( const_string const& s1, char const* s2 )            { return s1 == const_string( s2 ); }
    friend bool     operator==( const_string const& s1, const string& s2 )          { return s1 == const_string( s2 ); }

    friend bool     operator!=( const_string const& s1, const_string const& s2 )    { return !(s1 == s2); }
    friend bool     operator!=( const_string const& s1, char const* s2 )            { return !(s1 == s2); }
    friend bool     operator!=( const_string const& s1, const string& s2 )          { return !(s1 == s2); }

    friend bool     operator==( char const* s2,         const_string const& s1 )    { return s1 == s2; }
    friend bool     operator==( const string& s2,       const_string const& s1 )    { return s1 == s2; }

    friend bool     operator!=( char const* s2,         const_string const& s1 )    { return !(s1 == s2); }
    friend bool     operator!=( const string& s2,       const_string const& s1 )    { return !(s1 == s2); }

    // Iterators
    iterator        begin() const                           { return m_begin; }
    iterator        end() const                             { return m_end;   }
    reverse_iterator rbegin() const                         { return m_end;   }
    reverse_iterator rend() const                           { return m_begin; }

    // search operation
    iterator        find_first_of( char c );
    iterator        find_first_of( const_string cs );
    iterator        find_last_of( char c );
    iterator        find_last_of( const_string cs );

private:

    // Data members
    char const*     m_begin;
    char const*     m_end;
};

//____________________________________________________________________________//

// first character
class first_char {
public:
    char operator()( const_string source, char default_char = '\0' ) const {
        return source.is_empty() ? default_char : *source.data();
    }
};

//____________________________________________________________________________//

// last character
class last_char {
public:
    char operator()( const_string source, char default_char = '\0' ) const {
        return source.is_empty() ? default_char : *source.rbegin();
    }
};

//____________________________________________________________________________//

inline const_string&
const_string::operator=( const_string const& s ) {
    if( &s != this ) {
        m_begin = s.m_begin;
        m_end  = s.m_end;
    }

    return *this;
}

//____________________________________________________________________________//

typedef const_string const literal;

}; // namespace common_layer

#endif // CONST_STRING_HPP
