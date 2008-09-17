/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__EXAMPLE__TWO_SPHERES_HPP
#define BOOST__GUIGL__EXAMPLE__TWO_SPHERES_HPP

#include <boost/guigl/view/colored.hpp>
#include <boost/guigl/view/positioned.hpp>
#include <boost/guigl/view/periodic.hpp>
#include <boost/guigl/view/solid_background.hpp>
#include <boost/guigl/view/navigable.hpp>
#include <boost/guigl/view/three_dimensional.hpp>

class two_spheres;

typedef
    boost::guigl::view::colored<
        boost::guigl::view::navigable<
            boost::guigl::view::three_dimensional<
                boost::guigl::view::solid_background<
                    boost::guigl::view::periodic<two_spheres,
                        boost::guigl::view::positioned<>
    >   >   >   >   >   two_spheres_base_type;

class two_spheres : public two_spheres_base_type
{
    typedef two_spheres_base_type base_type;
public:
    template<typename Args>
    two_spheres(const Args &args)
        : base_type(args)
        , m_closer(true)
    {}
protected:
    void draw();
    void draw_prologue();
    static void *sphere();
    static void *s_sphere;
    
    friend class boost::guigl::view::periodic<two_spheres,
        boost::guigl::view::positioned<> >;
    
    void periodic_callback();
    bool m_closer;
};

#endif // BOOST__GUIGL__EXAMPLE__TWO_SPHERES_HPP