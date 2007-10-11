// forward_constructor.hpp

// Boost Logging library
//
// Author: John Torjo, www.torjo.com
//
// Copyright (C) 2007 John Torjo (see www.torjo.com for email)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.
 
// See http://www.boost.org for updates, documentation, and revision history.
// See http://www.torjo.com/log2/ for more details

#ifndef JT28092007_forward_constructor_HPP_DEFINED
#define JT28092007_forward_constructor_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>
#include <boost/config.hpp>

//#if BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1400))

#ifdef BOOST_MSVC
// because of copy-constructor bug 
#include <boost/type_traits/is_base_of.hpp>
#endif

namespace boost { namespace logging {

#define BOOST_LOGGING_FORWARD_CONSTRUCTOR(class_name,forward_to) \
        template<class p1> class_name(const p1 & a1 ) : forward_to(a1) {} \
        template<class p1, class p2> class_name(const p1 & a1 , const p2 & a2) : forward_to(a1,a2) {} \
        template<class p1, class p2, class p3> class_name(const p1 & a1 , const p2 & a2, const p3 & a3) : forward_to(a1,a2,a3) {} \
        template<class p1, class p2, class p3, class p4> class_name(const p1 & a1 , const p2 & a2, const p3 & a3, const p4 & a4) : forward_to(a1,a2,a3,a4) {} \
        template<class p1, class p2, class p3, class p4, class p5> class_name(const p1 & a1 , const p2 & a2, const p3 & a3, const p4 & a4, const p5 & a5) : forward_to(a1,a2,a3,a4,a5) {}



#ifdef BOOST_MSVC
// workaround for VS - problem with copy constructor

#define BOOST_LOGGING_FORWARD_CONSTRUCTOR_WITH_NEW(class_name,forward_to,type) \
        template<class p1> class_name(const p1 & a1 ) { \
            see_if_copy_constructor( a1, forward_to, boost::is_base_of<class_name,p1>() ); \
        } \
        template<class p1, class forward_type> void see_if_copy_constructor(const p1 & a1, forward_type&, const boost::true_type& ) { \
            forward_to = a1.forward_to; \
        } \
        template<class p1, class forward_type> void see_if_copy_constructor(const p1 & a1, forward_type&, const boost::false_type& ) { \
            forward_to = forward_type(new type(a1)); \
        } \
        template<class p1, class p2> class_name(const p1 & a1 , const p2 & a2) : forward_to(new type(a1,a2)) {} \
        template<class p1, class p2, class p3> class_name(const p1 & a1 , const p2 & a2, const p3 & a3) : forward_to(new type(a1,a2,a3)) {} \
        template<class p1, class p2, class p3, class p4> class_name(const p1 & a1 , const p2 & a2, const p3 & a3, const p4 & a4) : forward_to(new type(a1,a2,a3,a4)) {} \
        template<class p1, class p2, class p3, class p4, class p5> class_name(const p1 & a1 , const p2 & a2, const p3 & a3, const p4 & a4, const p5 & a5) : forward_to(new type(a1,a2,a3,a4,a5)) {}

#else
#define BOOST_LOGGING_FORWARD_CONSTRUCTOR_WITH_NEW(class_name,forward_to,type) \
        template<class p1> class_name(const p1 & a1 ) : forward_to(new type(a1)) {} \
        template<class p1, class p2> class_name(const p1 & a1 , const p2 & a2) : forward_to(new type(a1,a2)) {} \
        template<class p1, class p2, class p3> class_name(const p1 & a1 , const p2 & a2, const p3 & a3) : forward_to(new type(a1,a2,a3)) {} \
        template<class p1, class p2, class p3, class p4> class_name(const p1 & a1 , const p2 & a2, const p3 & a3, const p4 & a4) : forward_to(new type(a1,a2,a3,a4)) {} \
        template<class p1, class p2, class p3, class p4, class p5> class_name(const p1 & a1 , const p2 & a2, const p3 & a3, const p4 & a4, const p5 & a5) : forward_to(new type(a1,a2,a3,a4,a5)) {}

#endif

}}

#endif

