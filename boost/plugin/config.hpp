// Copyright Hartmut Kaiser 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_PLUGIN_CONFIG_HPP_HK_2005_11_07)
#define BOOST_PLUGIN_CONFIG_HPP_HK_2005_11_07

#include <boost/config.hpp>

///////////////////////////////////////////////////////////////////////////////
#ifdef BOOST_NO_STRINGSTREAM
#include <strstream>
inline std::string BOOST_PLUGIN_OSSTREAM_GETSTRING(std::ostrstream& ss)
{
    ss << std::ends;
    std::string rval = ss.str();
    ss.freeze(false);
    return rval;
}
#define BOOST_PLUGIN_OSSTREAM std::ostrstream
#else
#include <sstream>
#define BOOST_PLUGIN_OSSTREAM_GETSTRING(ss) ss.str()
#define BOOST_PLUGIN_OSSTREAM std::ostringstream
#endif

///////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_WINDOWS)
#define BOOST_PLUGIN_EXPORT_API __declspec(dllexport)
#define BOOST_PLUGIN_API        __cdecl
#else
#define BOOST_PLUGIN_EXPORT_API /**/
#define BOOST_PLUGIN_API        /**/
#endif

///////////////////////////////////////////////////////////////////////////////
//
//  The PLUGIN_ARGUMENT_LIMIT defines the upper limit of possible arguments to
//  the virtual constructors.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined(PLUGIN_ARGUMENT_LIMIT)
#define PLUGIN_ARGUMENT_LIMIT 10
#endif

#endif // BOOST_PLUGIN_CONFIG_HPP_HK_2005_11_07
