#ifndef INCLUDE_BASE_OPERATIONS_H
#define INCLUDE_BASE_OPERATIONS_H

#include "operations.h"

struct TokenStreamOp : public Operation{
    std::vector<token_t>  m_tokens;
    void add (const token_t& t) { m_tokens.push_back (t); }
    
    virtual void operator () (OutputDelegate *);
    virtual ~TokenStreamOp () {}
};

struct TokenOp : public Operation {
    token_t m_tok;
    TokenOp (const token_t& t) : m_tok(t) {}
    virtual void operator() (OutputDelegate *);
    virtual ~TokenOp () {}
};

struct StringOp : public Operation {
    std::string m_str;
    StringOp (const std::string& s) : m_str(s) {}
    virtual void operator () (OutputDelegate *);
    virtual ~StringOp () {}
};

bool whitespace (const token_t& tok);

#endif
