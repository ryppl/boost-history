// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

#ifndef BOOST_POLICY_PTR_TEST_HIERARCHY_NON_VIRTUAL_DTOR_HPP_INCLUDED
#define BOOST_POLICY_PTR_TEST_HIERARCHY_NON_VIRTUAL_DTOR_HPP_INCLUDED 

#include "./counter.hpp"

template<typename T>
struct destroyer {
    void operator()(T* t) const { t->destroy(); }
};

struct base : semi_counted<base> {
    void destroy() 
    { 
        counted<base>::dec();  
        delete this; 
    }
};

struct derived : base, semi_counted<derived> { 
    void destroy() 
    { 
        counted<derived>::dec(); 
        base::destroy();
    }
};

#endif // #ifndef BOOST_POLICY_PTR_TEST_HIERARCHY_NON_VIRTUAL_DTOR_HPP_INCLUDED
