// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_ASPECT_HPP_INCLUDED
#define BOOST_IDL_ASPECT_HPP_INCLUDED

namespace boost { namespace interfaces {          

template<typename Pointcut, typename Advice>
struct aspect {
    typedef Pointcut  pointcut_type;
    typedef Advice    advice_type;
};

} } // End namespaces interfaces, boost.

#endif // #ifndef BOOST_IDL_ASPECT_HPP_INCLUDED
