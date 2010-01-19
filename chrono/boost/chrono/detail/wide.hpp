//  boost/chrono/stopwatch_formatter.hpp  ------------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_DETAIL_ADAPTIVE_STRING__HPP
#define BOOST_CHRONO_DETAIL_ADAPTIVE_STRING__HPP

#include <string>

namespace boost { namespace chrono  {

namespace detail {
    
    /**
     * this class is a shim between std::string type and wchar_t type.
     * it accepts a std::string type and returns a std::string or 
     * std::wstring according to the context(e.g. type on the left side
     * of '=' operator):
     * - in case of a string type, it forwards the passed-in std::string
     * - in case of a wstring type, it widens it passed-in std::string 
     * before forwarding
     *
     * typical usage:
     *    std::string s   = adaptive_string("hello"); // s  = "hello"
     *    std::wstring ws = adaptive_string("hello"); // ws = L"hello"
     * N.B. it doe not do any code conversion like: MBCS <--> UNICODE
     */
        
    struct adaptive_string
    {
        adaptive_string(const std::string& s):str_(s)
        {}
        
        // implicit convert to string         
        template <
            typename Traits, 
            class Alloc
        >
        operator std::basic_string<char, Traits, Alloc>() const
        {
            //return str_;
            std::basic_string<char, Traits, Alloc> s;
            s.assign(str_.begin(), str_.end());
            return s;
        }
        // implicit convert to wstring 
        template <
            typename Traits, 
            class Alloc
        >
        operator std::basic_string<wchar_t, Traits, Alloc>() const
        {
            std::basic_string<wchar_t, Traits, Alloc> ws;
            ws.assign(str_.begin(), str_.end());
            return ws;
        }
      
      private:
        const std::string& str_;
    };
    
    
} // namespace detail  
} // namespace chrono
} // namespace boost


#endif
