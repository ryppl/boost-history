// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_IDL_BEGIN_HPP_INCLUDED
#define BOOST_IDL_DETAIL_IDL_BEGIN_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/interfaces/access.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/preprocessor/cat.hpp>

#define BOOST_IDL_BEGIN_TEMPLATE(name, arity) \
    struct BOOST_PP_CAT(name, _interface_impl_) { \
        BOOST_STATIC_CONSTANT(int, start_line_idl_ = __LINE__); \
        template<typename Base_> \
        struct inner : Base_ { \
            typedef typename \
                    ::boost::interfaces::advice_category< \
                        Base_ \
                    >::type category; \
        BOOST_IDL_PRIVATE: \
            friend class ::boost::interfaces::access; \
            template<typename NNN_, typename Dummy_ = int> \
            struct tracker_idl_ { typedef void type; }; \
        public: \
            struct supers_idl_ \
                : ::boost::mpl::identity< ::boost::mpl::vector< \
    /**/

#define BOOST_IDL_BEGIN(name) BOOST_IDL_BEGIN_TEMPLATE(name, 0)
    /**/

#define BOOST_IDL_BEGIN1(name) BOOST_IDL_BEGIN_TEMPLATE(name, 1)
#define BOOST_IDL_BEGIN2(name) BOOST_IDL_BEGIN_TEMPLATE(name, 2)
#define BOOST_IDL_BEGIN3(name) BOOST_IDL_BEGIN_TEMPLATE(name, 3)
#define BOOST_IDL_BEGIN4(name) BOOST_IDL_BEGIN_TEMPLATE(name, 4)
#define BOOST_IDL_BEGIN5(name) BOOST_IDL_BEGIN_TEMPLATE(name, 5)
#define BOOST_IDL_BEGIN6(name) BOOST_IDL_BEGIN_TEMPLATE(name, 6)
#define BOOST_IDL_BEGIN7(name) BOOST_IDL_BEGIN_TEMPLATE(name, 7)
#define BOOST_IDL_BEGIN8(name) BOOST_IDL_BEGIN_TEMPLATE(name, 8)
#define BOOST_IDL_BEGIN9(name) BOOST_IDL_BEGIN_TEMPLATE(name, 9)
#define BOOST_IDL_BEGIN10(name) BOOST_IDL_BEGIN_TEMPLATE(name, 10)

#endif // #ifndef BOOST_IDL_DETAIL_IDL_BEGIN_HPP_INCLUDED
