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

#include "boost/move.hpp"
#include "boost/test/minimal.hpp"

#include <cstdlib> // for size_t
#include <cstring> // for strlen, str(n)cpy
#include <algorithm> // for swap
#include "boost/utility.hpp" // for boost::noncopyable

//////////////////////////////////////////////////////////////////////////
// class moveable_string
//
// Moveable std::string-like type. But look, no implicit copying allowed!
//
class moveable_string
    : public boost::moveable< moveable_string >
    , boost::noncopyable
{
public: // constants
    BOOST_STATIC_CONSTANT(
          std::size_t
        , npos = static_cast<std::size_t>(-1)
        );

private: // representation
    size_t len_;
    char* str_;

public: // structors
    ~moveable_string()
    {
        clear();
    }

    moveable_string()
        : len_(0)
        , str_(0)
    {
    }

    moveable_string(boost::move_source<moveable_string> source)
        : len_(source.get().len_)
        , str_(source.get().str_)
    {
        source.get().len_ = 0;
        source.get().str_ = 0;
    }

    moveable_string(const char* str, std::size_t len = npos)
        : len_(len == npos ? std::strlen(str) : len)
        , str_(len_ != 0 ? new char[len_ + 1] : 0)
    {
        if (str_ != 0)
            std::strncpy(str_, str, len_);
    }

public: // modifiers
    moveable_string& operator=(boost::move_source<moveable_string> source)
    {
        moveable_string& rhs = source.get();

        clear();

        len_ = rhs.len_;
        rhs.len_ = 0;

        str_ = rhs.str_;
        rhs.str_ = 0;        
    }

    void swap(moveable_string& rhs)
    {
        std::swap(len_, rhs.len_);
        std::swap(str_, rhs.str_);
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
    std::size_t length() const
    {
        return len_;
    }

    const char* c_str() const
    {
        return str_ == 0 ? "" : str_;
    }

public: // operators
    moveable_string& operator+=(const moveable_string& rhs)
    {
        if (rhs.length() > 0)
        {
            char* newstr = new char[length() + rhs.length() + 1];

            std::strncpy(newstr,            str_,     length());
            std::strncpy(newstr + length(), rhs.str_, rhs.length());

            clear();
            str_ = newstr;
            len_ = length() + rhs.length();
        }
        return *this;
    }
};

  boost::return_t<moveable_string>::type
operator+(const moveable_string& lhs, const moveable_string& rhs)
{
    // we *need* to do a copy here -- the only place though!
    moveable_string lhs_copy(lhs.c_str(), lhs.length());

    return lhs_copy += rhs;
}

bool operator==(const moveable_string& lhs, const moveable_string& rhs)
{
    return std::strcmp(lhs.c_str(), rhs.c_str()) == 0;
}

//////////////////////////////////////////////////////////////////////////
// function source
// 
// Returns a moveable_string via move (i.e. without deep copy).
//
  boost::return_t<moveable_string>::type
source()
{
    moveable_string str("efg");
    return "abcd" + str;
}

//////////////////////////////////////////////////////////////////////////
// function test_main
//
int test_main( int, char *[] )
{
    moveable_string sink(source());
    BOOST_CHECK( sink == "abcdefg" );

    return boost::exit_success;
}
