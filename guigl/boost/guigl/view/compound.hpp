/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__COMPOUND_HPP
#define BOOST__GUIGL__VIEW__COMPOUND_HPP


#include <boost/guigl/view/static_compound.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace boost { namespace guigl { namespace view {

namespace detail
{
    template<typename BaseView>
    struct compound_event_visitor;
}

template<typename BaseView=base>
class compound : public static_compound<ptr_vector<positioned<> >, BaseView>
{    
public:
    typedef static_compound<ptr_vector<positioned<> >, BaseView> base_type;

    template<typename ArgumentPack>
    compound(const ArgumentPack &args)
        : base_type(( args, _children=0 ))
    {}
    compound(const compound &rhs)
        : base_type(static_cast<const base_type &>(rhs))
    {}
    
    compound & operator << (positioned<> *v)
    {
        base_type::m_children.push_back(v);
        access::set_parent(*v, *this);
        return *this;
    }

};

}}}

#endif // BOOST__GUIGL__VIEW_HPP