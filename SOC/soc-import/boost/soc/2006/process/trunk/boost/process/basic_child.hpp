//
// Boost.Process
// Implementation of the Child concept.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//
 
#if !defined(BOOST_PROCESS_BASIC_CHILD_HPP)
#define BOOST_PROCESS_BASIC_CHILD_HPP

#include <boost/process/basic_process.hpp>

namespace boost {
    namespace process {

        template< class Attributes >
        class basic_child : 
            public basic_process< Attributes >
        {

        public:
        };

    } // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_BASIC_CHILD_HPP)
