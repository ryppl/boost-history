#ifndef BOOST_ARCHIVE_BASIC_TEXT_OPRIMITIVE_HPP
#define BOOST_ARCHIVE_BASIC_TEXT_OPRIMITIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// basic_text_oprimitive.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

// archives stored as text - note these ar templated on the basic
// stream templates to accommodate wide (and other?) kind of characters
//
// note the fact that on libraries without wide characters, ostream is
// is not a specialization of basic_ostream which in fact is not defined
// in such cases.   So we can't use basic_ostream<OStream::char_type> but rather
// use two template parameters

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>
#if BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)
#include <boost/archive/dinkumware.hpp>
#endif

#include <iomanip>
#include <locale>
#include <cstddef> // size_t

#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std{ 
    using ::size_t;
    #if ! defined(BOOST_DINKUMWARE_STDLIB) && ! defined(__SGI_STL_PORT)
        using ::locale;
    #endif
} // namespace std
#endif

#include <boost/limits.hpp>
#include <boost/io/ios_state.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/throw_exception.hpp>
#include <boost/archive/archive_exception.hpp>

#include <boost/archive/detail/abi_prefix.hpp> // must be the last header

namespace boost {
namespace archive {

class save_access;

/////////////////////////////////////////////////////////////////////////
// class basic_text_oprimitive - output of prmitives to stream
template<class OStream>
class basic_text_oprimitive
{
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
protected:
#else
public:
#endif
    OStream &os;
    io::ios_flags_saver flags_saver;
    io::ios_precision_saver precision_saver;
    boost::scoped_ptr<std::locale> archive_locale;
    io::basic_ios_locale_saver<
        BOOST_DEDUCED_TYPENAME OStream::char_type, BOOST_DEDUCED_TYPENAME OStream::traits_type
    > locale_saver;

    // default saving of primitives.
    template<class T>
    void save(const T &t){
        if(os.fail())
            boost::throw_exception(archive_exception(archive_exception::stream_error));
        os << t;
    }

    /////////////////////////////////////////////////////////
    // fundamental types that need special treatment
    void save(const signed char t)
    {
        if(os.fail())
            boost::throw_exception(archive_exception(archive_exception::stream_error));
        os << static_cast<short int>(t);
    }
    void save(const unsigned char t)
    {
        if(os.fail())
            boost::throw_exception(archive_exception(archive_exception::stream_error));
        os << static_cast<short unsigned int>(t);
    }
    void save(const char t)
    {
        if(os.fail())
            boost::throw_exception(archive_exception(archive_exception::stream_error));
        os << static_cast<short int>(t);
    }
    #ifndef BOOST_NO_INTRINSIC_WCHAR_T
    void save(const wchar_t t)
    {
        if(os.fail())
            boost::throw_exception(archive_exception(archive_exception::stream_error));
        os << static_cast<int>(t);
    }
    #endif
    void save(const float t)
    {
        if(os.fail())
            boost::throw_exception(archive_exception(archive_exception::stream_error));
        os << std::setprecision(std::numeric_limits<float>::digits10 + 2);
        os << t;
    }
    void save(const double t)
    {
        if(os.fail())
            boost::throw_exception(archive_exception(archive_exception::stream_error));
        os << std::setprecision(std::numeric_limits<double>::digits10 + 2);
        os << t;
    }
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(BOOST_PP_EMPTY())
    basic_text_oprimitive(OStream & os, bool no_codecvt);
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(BOOST_PP_EMPTY()) 
    ~basic_text_oprimitive();
public:
    // unformatted append of one character
    void put(int c){
        if(os.fail())
            boost::throw_exception(archive_exception(archive_exception::stream_error));
        os.put(c);
    }
    // unformatted append of null terminated string
    void put(const char * s){
        if(os.fail())
            boost::throw_exception(archive_exception(archive_exception::stream_error));
        while('\0' != *s)
            os.put(*s++);
    }
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void) 
    save_binary(const void *address, std::size_t count);
};

} //namespace boost 
} //namespace archive 

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#endif // BOOST_ARCHIVE_BASIC_TEXT_OPRIMITIVE_HPP
