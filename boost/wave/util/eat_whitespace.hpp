/*=============================================================================
    Wave: A Standard compliant C++ preprocessor library
    Whitespace eater
    
    http://spirit.sourceforge.net/

    Copyright (c) 2003 Paul Mensonides
    Copyright (c) 2001-2004 Hartmut Kaiser. 
    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(EAT_WHITESPACE_HPP_4CE9AD17_F82D_4AB2_A117_555DF0DCC801_INCLUDED)
#define EAT_WHITESPACE_HPP_4CE9AD17_F82D_4AB2_A117_555DF0DCC801_INCLUDED

#include <boost/wave/wave_config.hpp>   
#include <boost/wave/token_ids.hpp>   

///////////////////////////////////////////////////////////////////////////////
namespace boost {
namespace wave {
namespace util {

template <typename TokenT>
class eat_whitespace {

public:
    eat_whitespace();
    
    bool may_skip (TokenT &token, bool &skipped_newline);
    
private:
    typedef bool state_t(TokenT &token, bool &skipped_newline);
    state_t eat_whitespace::* state;
    state_t general, newline, newline_2nd, whitespace;
};

template <typename TokenT>
inline 
eat_whitespace<TokenT>::eat_whitespace()
:   state(&eat_whitespace::newline) 
{
}

template <typename TokenT>
inline bool 
eat_whitespace<TokenT>::may_skip(TokenT &token, bool &skipped_newline) 
{
    return (this->*state)(token, skipped_newline);
}

template <typename TokenT>
inline bool 
eat_whitespace<TokenT>::general(TokenT &token, bool &skipped_newline) 
{
    using boost::wave::token_id;

    token_id id = token_id(token);
    if (T_NEWLINE == id || T_CPPCOMMENT == id) {
        state = &eat_whitespace::newline;
    }
    else if (T_SPACE == id || T_SPACE2 == id || T_CCOMMENT == id) {
        state = &eat_whitespace::whitespace;

        if (T_CCOMMENT == id) {
            if (TokenT::string_type::npos != 
                token.get_value().find_first_of("\n"))
            {
                skipped_newline = true;
            }
        }

        if (token.get_value().size() > 1)
            token.set_value(" ");   // replace with a single space
    }
    else {
        state = &eat_whitespace::general;
    }
    return false;
}

template <typename TokenT>
inline bool 
eat_whitespace<TokenT>::newline(TokenT &token, bool &skipped_newline) 
{
    using namespace boost::wave;
    
    token_id id = token_id(token);
    if (T_NEWLINE == id || T_CPPCOMMENT == id) {
        skipped_newline = true;
        state = &eat_whitespace::newline_2nd;
        return true;
    }
    else if (T_SPACE != id && T_SPACE2 != id && T_CCOMMENT != id) {
        return general(token, skipped_newline);
    }

    if (T_CCOMMENT == id) {
        if (TokenT::string_type::npos != 
            token.get_value().find_first_of("\n"))
        {
            skipped_newline = true;
        }
    }
    return true;
}

template <typename TokenT>
inline bool 
eat_whitespace<TokenT>::newline_2nd(TokenT &token, bool &skipped_newline) 
{
    using namespace boost::wave;
    
    token_id id = token_id(token);
    if (T_SPACE == id || T_SPACE2 == id)
        return true;
    if (T_CCOMMENT == id) {
        if (TokenT::string_type::npos != 
            token.get_value().find_first_of("\n"))
        {
            skipped_newline = true;
        }
        return true;
    }
    if (T_NEWLINE != id && T_CPPCOMMENT != id) 
        return general(token, skipped_newline);

    skipped_newline = true;
    return true;
}

template <typename TokenT>
inline bool 
eat_whitespace<TokenT>::whitespace(TokenT &token, bool &skipped_newline) 
{
    using namespace boost::wave;
    
    token_id id = token_id(token);
    if (T_SPACE != id && T_SPACE2 != id && T_CCOMMENT != id) 
        return general(token, skipped_newline);

    if (T_CCOMMENT == id) {
        if (TokenT::string_type::npos != 
            token.get_value().find_first_of("\n"))
        {
            skipped_newline = true;
        }
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
}   // namespace util
}   // namespace wave
}   // namespace boost

#endif // !defined(EAT_WHITESPACE_HPP_4CE9AD17_F82D_4AB2_A117_555DF0DCC801_INCLUDED)

