// Copyright (C) 2002 Hugo Duncan

// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Hugo Duncan makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.

#ifdef _MSC_VER
#pragma once
#endif

/// include guard
#ifndef URL_HPP
#define URL_HPP 1

#include "boost/spirit/core.hpp"
#include "boost/spirit/symbols/symbols.hpp"
#include "boost/spirit/utility/chset.hpp"
#include "boost/spirit/utility/escape_char.hpp"
#include "boost/spirit/utility/confix.hpp"
#include "boost/spirit/error_handling.hpp"

#include <string>

using namespace spirit;
  std::string schemename;
  std::string servername;
  std::string pathname;

//! Grammer to read model files
struct UrlGrammar : public spirit::grammar<UrlGrammar>
{

  template <typename ScannerT>
  struct definition
  {
    definition(UrlGrammar const& self)
        : SPACE(' '),SEMICOLON(';'),SLASH('/'),QUESTION('?'),COLON(':'),
          AT('@'),AMPERSAND('&'),EQUAL('='),PLUS('+'),DOLLAR('$'), COMMA(','),
          MINUS('-'), UNDERSCORE('_'), DOT('.'), BANG('!'),TILDE('~'),
          STAR('*'), APOSTROPHE('\''), LEFT_PAREN('('), RIGHT_PAREN(')'),
          PERCENT('%'), LESS('<'), GREATER('>'), HASH('#'),LEFT_BRACE('{'),
          RIGHT_BRACE('}'),OR('|'),BACKSLASH('\\'), HAT('^'),
          LEFT_BRACKET('['), RIGHT_BRACKET(']'),
          BACK_APOSTROPHE('`')
    {
      unreserved  = alnum_p | mark;
      mark        =
        MINUS | UNDERSCORE | DOT | BANG | TILDE |
        STAR | APOSTROPHE | LEFT_PAREN | RIGHT_PAREN;
      escaped     = PERCENT >> xdigit_p >> xdigit_p ;

      absoluteURI   = scheme[assign(schemename)] >> COLON >> hier_part ;
      hier_part     = net_path >> abs_path[assign(pathname)]
                               >> !( QUESTION >> +anychar_p );
      net_path      = SLASH >> SLASH >> server[assign(servername)] ;
      abs_path      = SLASH  >> path_segments ;
      scheme        = alpha_p >> *( alpha_p | digit_p | PLUS | MINUS | DOT ) ;
      server        = !( !( userinfo >> AT ) >> hostport ) ;
      userinfo      = *( unreserved | escaped |
                         SEMICOLON | COLON | AMPERSAND | EQUAL | PLUS
                         | DOLLAR | COMMA );
      hostport      = host >> !( COLON >> port );
      host          = hostname | IPv4address;
      hostname      = label >> hostname_helper ;
      hostname_helper  =  ( DOT >> label >> hostname_helper ) | epsilon_p ;
      label         = alpha_p >> label_helper  ;
      label_helper  = ( ( alnum_p | MINUS ) >> label_helper ) | epsilon_p ;

      IPv4address   = +digit_p >> DOT >> +digit_p >> DOT
                               >> +digit_p >> DOT >> +digit_p ;
      port          = +digit_p ;
      path          = ( abs_path | opaque_part );
      path_segments = segment >> *( SLASH >> segment );
      segment       = *pchar >> *( SEMICOLON >> param );
      param         = *pchar ;
      pchar         = unreserved | escaped |
        COLON | AT | AMPERSAND | EQUAL | PLUS | DOLLAR | COMMA ;

      SPIRIT_DEBUG_RULE(alnum_p);
      SPIRIT_DEBUG_RULE(alpha_p);
      SPIRIT_DEBUG_RULE(unreserved);
      SPIRIT_DEBUG_RULE(mark);
      SPIRIT_DEBUG_RULE(escaped);
      SPIRIT_DEBUG_RULE(SPACE);
      SPIRIT_DEBUG_RULE(absoluteURI);
      SPIRIT_DEBUG_RULE(hier_part);
      SPIRIT_DEBUG_RULE(net_path);
      SPIRIT_DEBUG_RULE(abs_path);
      SPIRIT_DEBUG_RULE(opaque_part);
      SPIRIT_DEBUG_RULE(scheme);
      SPIRIT_DEBUG_RULE(authority);
      SPIRIT_DEBUG_RULE(reg_name);
      SPIRIT_DEBUG_RULE(server);
      SPIRIT_DEBUG_RULE(userinfo);
      SPIRIT_DEBUG_RULE(hostport);
      SPIRIT_DEBUG_RULE(host);
      SPIRIT_DEBUG_RULE(hostname);
      SPIRIT_DEBUG_RULE(hostname_helper);
      SPIRIT_DEBUG_RULE(label);
      SPIRIT_DEBUG_RULE(label_helper);
      SPIRIT_DEBUG_RULE(IPv4address);
      SPIRIT_DEBUG_RULE(port);
      SPIRIT_DEBUG_RULE(path);
      SPIRIT_DEBUG_RULE(path_segments);
      SPIRIT_DEBUG_RULE(segment);
      SPIRIT_DEBUG_RULE(param);
      SPIRIT_DEBUG_RULE(pchar);
      SPIRIT_DEBUG_RULE(query);
    }

    rule<ScannerT> url ;
    rule<ScannerT> unreserved;
    rule<ScannerT> mark;
    rule<ScannerT> escaped;
    chlit<> SPACE,SEMICOLON,SLASH,QUESTION,COLON,AT,AMPERSAND,
            EQUAL,PLUS,DOLLAR, COMMA,
            MINUS, UNDERSCORE, DOT, BANG,TILDE,
            STAR, APOSTROPHE, LEFT_PAREN, RIGHT_PAREN,
            PERCENT, LESS, GREATER, HASH,
      LEFT_BRACE, RIGHT_BRACE, OR, BACKSLASH, HAT,
      LEFT_BRACKET, RIGHT_BRACKET,
      BACK_APOSTROPHE
      ;
    rule<ScannerT> absoluteURI;
    rule<ScannerT> hier_part;
    rule<ScannerT> net_path;
    rule<ScannerT> abs_path;
    rule<ScannerT> opaque_part;
    rule<ScannerT> scheme;
    rule<ScannerT> authority;
    rule<ScannerT> reg_name;
    rule<ScannerT> server;
    rule<ScannerT> userinfo;
    rule<ScannerT> hostport;
    rule<ScannerT> host;
    rule<ScannerT> hostname;
    rule<ScannerT> hostname_helper;
    rule<ScannerT> label;
    rule<ScannerT> label_helper;
    rule<ScannerT> IPv4address;
    rule<ScannerT> port;
    rule<ScannerT> path;
    rule<ScannerT> path_segments;
    rule<ScannerT> segment;
    rule<ScannerT> param;
    rule<ScannerT> pchar;
    rule<ScannerT> query;

    rule<ScannerT> const& start() const { return absoluteURI; }
  };

};



#endif
