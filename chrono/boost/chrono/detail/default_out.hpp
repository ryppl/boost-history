//  boost/chrono/stopwatch_formatter.hpp  ------------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_DETAIL_DEFAULT_OUT__HPP
#define BOOST_CHRONO_DETAIL_DEFAULT_OUT__HPP

#include <boost/chrono/config.hpp>
#include <iostream>

namespace boost { namespace chrono  {


namespace detail {
    template <typename CharT,typename Traits>
    struct default_out;
    template <typename Traits>
    struct default_out<char,Traits> {
        static std::basic_ostream<char,Traits>& apply() {
            return std::cout;
        }
    };
#ifndef BOOST_NO_STD_WSTRING
    template <typename Traits>
    struct default_out<wchar_t,Traits> {
        static std::basic_ostream<wchar_t,Traits>& apply() {
            return std::wcout;
        }
    };
#endif    
}
    
  } // namespace chrono
} // namespace boost


#endif
