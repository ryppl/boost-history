// (C) Copyright Daniel Wallin 2004.
// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_MOVE_SUPPORT_HPP_INCLUDED
#define BOOST_IDL_DETAIL_MOVE_SUPPORT_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#define BOOST_IDL_MOVE_SUPPORT(name) \
    private: \
        template<typename Ptr> struct cant_move_from_const; \
        template<typename T> \
        struct cant_move_from_const< const name<T> > \
        { typedef typename name<T>::error type; }; \
        template<typename Ptr> \
        name(Ptr&, typename cant_move_from_const<Ptr>::type = 0); \
        name(name&); \
        template<typename Subinterface> \
        name(name<Subinterface>&); \
    public: \
    /**/

#endif // #ifndef BOOST_IDL_DETAIL_MOVE_SUPPORT_HPP_INCLUDED
