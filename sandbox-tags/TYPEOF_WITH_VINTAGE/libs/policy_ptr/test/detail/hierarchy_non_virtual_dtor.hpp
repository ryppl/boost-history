// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

#ifndef BOOST_POLICY_PTR_TEST_HIERARCHY_NON_VIRTUAL_DTOR_HPP_INCLUDED
#define BOOST_POLICY_PTR_TEST_HIERARCHY_NON_VIRTUAL_DTOR_HPP_INCLUDED 

#include "./counter.hpp"

struct base : counted<base> { };

struct derived : base, counted<derived> { };

#endif // #ifndef BOOST_POLICY_PTR_TEST_HIERARCHY_NON_VIRTUAL_DTOR_HPP_INCLUDED
