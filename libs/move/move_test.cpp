//-----------------------------------------------------------------------------
// boost libs/move/auto_mover_test.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#include "boost/auto_mover.hpp"
#include "boost/test/minimal.hpp"

#include <cstdlib> // for size_t
#include <cstring> // for strlen, str(n)cpy

#include "boost/swap.hpp"

//////////////////////////////////////////////////////////////////////////
// class simple_string
//
// Moveable std::string-like type.
//
class simple_string
    : public boost::moveable< simple_string >
{
private: // representation
    size_t len_;
    char* str_;

public: // structors
    ~simple_string()
    {
        clear();
    }

    simple_string()
        : len_(0)
        , str_(0)
    {
    }

    simple_string(const simple_string& operand)
        : len_(operand.len_)
        , str_(len_ == 0 ? 0 : new char[len_ + 1])
    {
        if (len_)
            std::strncpy(str_, operand.str_, len_);
    }

    simple_string(boost::move_source<simple_string> source)
        : len_(source.get().len_)
        , str_(source.get().str_)
    {
        source.get().len_ = 0;
        source.get().str_ = 0;
    }

    simple_string(const char* str)
        : len_(std::strlen(str))
        , str_(new char[len_ + 1])
    {
        std::strcpy(str_, str);
    }

public: // modifiers
    simple_string& operator=(simple_string rhs)
    {
        swap(rhs);
        return *this;
    }

    simple_string& operator=(boost::move_source<simple_string> source)
    {
        simple_string& rhs = source.get();

        clear();

        len_ = rhs.len_;
        rhs.len_ = 0;

        str_ = rhs.str_;
        rhs.str_ = 0;        
    }

    void swap(simple_string& rhs)
    {
        boost::swap(len_, rhs.len_);
        boost::swap(str_, rhs.str_);
    }

    void clear()
    {
        if (str_)
        {
            delete[] str_;
            str_ = 0;
            len_ = 0;
        }
    }

public: // queries
    bool empty() const
    {
        return str_ == 0;
    }

    std::size_t size() const
    {
        return len_;
    }

    const char* c_str() const
    {
        return str_ == 0 ? "" : str_;
    }

public: // operators
    simple_string& operator+=(const simple_string& rhs)
    {
        if (rhs.size() > 0)
        {
            char* newstr = new char[size() + rhs.size() + 1];

            std::strncpy(newstr,          str_,     size());
            std::strncpy(newstr + size(), rhs.str_, rhs.size());

            clear();
            str_ = newstr;
            len_ = size() + rhs.size();
        }
        return *this;
    }
};

  boost::move_return<simple_string>
operator+(simple_string lhs, const simple_string& rhs)
{
    return lhs += rhs;
}

bool operator==(const simple_string& lhs, const simple_string& rhs)
{
    return std::strcmp(lhs.c_str(), rhs.c_str()) == 0;
}

//////////////////////////////////////////////////////////////////////////
// function source
// 

// Returns a simple_string via move (i.e. without deep copy).
//
  boost::move_return<simple_string>
source()
{
    simple_string str("efg");
    return "abcd" + str;
}

//////////////////////////////////////////////////////////////////////////
// function test_main
//
int test_main( int, char *[] )
{
    simple_string sink(source());
    BOOST_CHECK(sink == "abcdefg");

    return boost::exit_success;
}
