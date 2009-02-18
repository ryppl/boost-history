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

#include <boost/move/move.hpp>
#include <boost/test/minimal.hpp>
#include <boost/utility.hpp> // for boost::noncopyable

#include <cstdlib> // for size_t
#include <cstring> // for strlen, strncpy, strncmp

#ifdef BOOST_NO_STDC_NAMESPACE
namespace std { using ::size_t; using ::strlen; using ::strncpy; using ::strncmp; }
#endif

#include <algorithm> // for swap

//////////////////////////////////////////////////////////////////////////
// class moveable_string
//
// Moveable std::string-like type. But look, no implicit copying allowed!
//
class moveable_string
{
public: // constants
    BOOST_STATIC_CONSTANT(
          std::size_t
        , npos = static_cast<std::size_t>(-1)
        );

private: // representation
    std::size_t len_;
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
    
    moveable_string(moveable_string const& x)
      : len_(x.len_),
        str_(len_ != 0 ? new char[len_ + 1] : 0)
    {
        if (str_ != 0)
            std::strncpy(str_, x.str_, len_);
        
        BOOST_ERROR("The moveable_string should not be copied in this test.");
    }
    
    moveable_string(boost::move_from<moveable_string> x)
      : len_(x.source.len_)
      , str_(x.source.str_)
    {
        x.source.len_ = 0;
        x.source.str_ = 0;
    }

    moveable_string(const char* str, std::size_t len = npos)
        : len_(len == npos ? std::strlen(str) : len)
        , str_(len_ != 0 ? new char[len_ + 1] : 0)
    {
        if (str_ != 0)
            std::strncpy(str_, str, len_);
    }

public: // modifiers
    moveable_string& operator=(moveable_string rhs)
    {
        clear();

        len_ = rhs.len_;
        rhs.len_ = 0;

        str_ = rhs.str_;
        rhs.str_ = 0;
        return *this;
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
            std::size_t len = length();
            char* newstr = new char[len + rhs.length() + 1];

            std::strncpy(newstr,            str_,     len);
            std::strncpy(newstr + len, rhs.str_, rhs.length());

            clear();
            str_ = newstr;
            len_ = len + rhs.length();
        }
        return *this;
    }
};

moveable_string operator+(const moveable_string& lhs, const moveable_string& rhs)
{
    // we *need* to do a copy here -- the only place though!
    moveable_string lhs_copy(lhs.c_str(), lhs.length());

    return boost::move(lhs_copy += rhs);
}

bool operator==(const moveable_string& lhs, const moveable_string& rhs)
{
    return lhs.length() == rhs.length() && std::strncmp(lhs.c_str(), rhs.c_str(), lhs.length()) == 0;
}

//////////////////////////////////////////////////////////////////////////
// function source
// 
// Returns a moveable_string via move (i.e. without deep copy).
//
moveable_string source()
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

    return 0;
}
