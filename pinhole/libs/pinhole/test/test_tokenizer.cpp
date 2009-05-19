// Boost.Pinhole library

// Copyright Jared McIntyre 2009. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PinholeLib

#include <boost/pinhole.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>

using namespace std;
using namespace boost;
using namespace boost::pinhole;
using namespace boost::pinhole::detail;

BOOST_AUTO_TEST_CASE( TestTokenizer_ValidPaths )
{
    boost::pinhole::detail::token_path path;

    // empty path

    BOOST_CHECK_EQUAL( process_path("", path), 0);
    BOOST_CHECK_EQUAL( path.relative_path, false );
    BOOST_CHECK_EQUAL( path.tokens.size(), 0 );

    // Simple relative path

    BOOST_CHECK_EQUAL( process_path("relative/", path), 1);
    BOOST_CHECK_EQUAL( path.relative_path, true );
    BOOST_CHECK_EQUAL( path.tokens.size(), 1 );
    BOOST_CHECK_EQUAL( (*path.tokens.begin()).name, "relative" );
    BOOST_CHECK( (*path.tokens.begin()).property.empty() );
    BOOST_CHECK( (*path.tokens.begin()).value.empty() );

    BOOST_CHECK_EQUAL( process_path("relative", path), 1);
    BOOST_CHECK_EQUAL( path.relative_path, true );
    BOOST_CHECK_EQUAL( path.tokens.size(), 1 );
    BOOST_CHECK_EQUAL( (*path.tokens.begin()).name, "relative" );
    BOOST_CHECK( (*path.tokens.begin()).property.empty() );
    BOOST_CHECK( (*path.tokens.begin()).value.empty() );

    // Simple fixed path

    BOOST_CHECK_EQUAL( process_path("/non-relative", path), 1);
    BOOST_CHECK_EQUAL( path.tokens.size(), 1 );
    BOOST_CHECK_EQUAL( (*path.tokens.begin()).name, "non-relative" );
    BOOST_CHECK( (*path.tokens.begin()).property.empty() );
    BOOST_CHECK( (*path.tokens.begin()).value.empty() );

    BOOST_CHECK_EQUAL( process_path("/non-relative/", path), 1);
    BOOST_CHECK_EQUAL( path.tokens.size(), 1 );
    BOOST_CHECK_EQUAL( (*path.tokens.begin()).name, "non-relative" );
    BOOST_CHECK( (*path.tokens.begin()).property.empty() );
    BOOST_CHECK( (*path.tokens.begin()).value.empty() );

    // multiple tokens

    BOOST_CHECK_EQUAL( process_path("first/second/third/fourth/fifth/", path), 5);
    BOOST_CHECK_EQUAL( path.relative_path, true );
    BOOST_CHECK_EQUAL( path.tokens.size(), 5 );
    int ct=0;
    BOOST_FOREACH( const boost::pinhole::detail::token_path::token& token, path.tokens )
    {
        if( ct == 0 ) BOOST_CHECK_EQUAL( token.name, "first" );
        if( ct == 1 ) BOOST_CHECK_EQUAL( token.name, "second" );
        if( ct == 2 ) BOOST_CHECK_EQUAL( token.name, "third" );
        if( ct == 3 ) BOOST_CHECK_EQUAL( token.name, "fourth" );
        if( ct == 4 ) BOOST_CHECK_EQUAL( token.name, "fifth" );

        BOOST_CHECK( token.property.empty() );
        BOOST_CHECK( token.value.empty() );

        ++ct;
    }

    // simple property values

    BOOST_CHECK_EQUAL( process_path("name.property=value", path), 1);
    BOOST_CHECK_EQUAL( path.relative_path, true );
    BOOST_CHECK_EQUAL( path.tokens.size(), 1 );
    BOOST_CHECK_EQUAL( (*path.tokens.begin()).name,     "name" );
    BOOST_CHECK_EQUAL( (*path.tokens.begin()).property, "property" );
    BOOST_CHECK_EQUAL( (*path.tokens.begin()).value,    "value" );

    // multiple property values

    BOOST_CHECK_EQUAL( process_path("/first.a1=b2/second.c=d/third.f3=g4/fourth.h=/fifth.5=6", path), 5);
    BOOST_CHECK_EQUAL( path.relative_path, false );
    BOOST_CHECK_EQUAL( path.tokens.size(), 5 );
    ct=0;
    BOOST_FOREACH( const boost::pinhole::detail::token_path::token& token, path.tokens )
    {
        if( ct == 0 )
        {
            BOOST_CHECK_EQUAL( token.name,     "first" );
            BOOST_CHECK_EQUAL( token.property, "a1" );
            BOOST_CHECK_EQUAL( token.value,    "b2" );
        }
        if( ct == 1 )
        {
            BOOST_CHECK_EQUAL( token.name,     "second" );
            BOOST_CHECK_EQUAL( token.property, "c" );
            BOOST_CHECK_EQUAL( token.value,    "d" );
        }
        if( ct == 2 )
        {
            BOOST_CHECK_EQUAL( token.name,     "third" );
            BOOST_CHECK_EQUAL( token.property, "f3" );
            BOOST_CHECK_EQUAL( token.value,    "g4" );
        }
        if( ct == 3 )
        {
            BOOST_CHECK_EQUAL( token.name,     "fourth" );
            BOOST_CHECK_EQUAL( token.property, "h" );
            BOOST_CHECK_EQUAL( token.value,    "" );
        }
        if( ct == 4 )
        {
            BOOST_CHECK_EQUAL( token.name,     "fifth" );
            BOOST_CHECK_EQUAL( token.property, "5" );
            BOOST_CHECK_EQUAL( token.value,    "6" );
        }

        ++ct;
    }

    // spaces

    BOOST_CHECK_EQUAL( process_path(" relat ive ", path), 1);
    BOOST_CHECK_EQUAL( (*path.tokens.begin()).name, " relat ive " );
    BOOST_CHECK( (*path.tokens.begin()).property.empty() );
    BOOST_CHECK( (*path.tokens.begin()).value.empty() );

    BOOST_CHECK_EQUAL( process_path(" relat ive / more spaces ", path), 2);
    ct=0;
    BOOST_FOREACH( const boost::pinhole::detail::token_path::token& token, path.tokens )
    {
        if( ct == 0 )
        {
            BOOST_CHECK_EQUAL( token.name, " relat ive " );
        }
        if( ct == 1 )
        {
            BOOST_CHECK_EQUAL( token.name, " more spaces " );
        }

        BOOST_CHECK( token.property.empty() );
        BOOST_CHECK( token.value.empty() );

        ++ct;
    }

    BOOST_CHECK_EQUAL( process_path(" relat ive . som eSpaces = a value / more spaces . mor eSpaces = another value ", path), 2);
    ct=0;
    BOOST_FOREACH( const boost::pinhole::detail::token_path::token& token, path.tokens )
    {
        if( ct == 0 )
        {
            BOOST_CHECK_EQUAL( token.name,     " relat ive " );
            BOOST_CHECK_EQUAL( token.property, " som eSpaces " );
            BOOST_CHECK_EQUAL( token.value,    " a value " );
        }
        if( ct == 1 )
        {
            BOOST_CHECK_EQUAL( token.name,     " more spaces " );
            BOOST_CHECK_EQUAL( token.property, " mor eSpaces " );
            BOOST_CHECK_EQUAL( token.value,    " another value " );
        }

        ++ct;
    }
}

BOOST_AUTO_TEST_CASE( TestTokenizer_InvalidPaths )
{
    boost::pinhole::detail::token_path path;

    // Simple relative path

    BOOST_CHECK_THROW( process_path("first/second//fourth/fifth", path), invalid_path);
    BOOST_CHECK_THROW( process_path("first.=value", path), invalid_path);
    BOOST_CHECK_THROW( process_path("first=value", path), invalid_path);
    BOOST_CHECK_THROW( process_path("first.value", path), invalid_path);
    BOOST_CHECK_THROW( process_path("first/second.=value", path), invalid_path);
    BOOST_CHECK_THROW( process_path("first/second=value", path), invalid_path);
    BOOST_CHECK_THROW( process_path("first/second.value", path), invalid_path);
}