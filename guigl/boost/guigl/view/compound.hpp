/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__COMPOUND_HPP
#define BOOST__GUIGL__VIEW__COMPOUND_HPP


#include <boost/guigl/view/positioned.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace boost { namespace guigl { namespace view {

namespace detail
{
    template<typename BaseView>
    struct compound_event_visitor;
}

template<typename BaseView=base>
class compound : public BaseView
{    
public:
    typedef BaseView base_type;

    template<typename ArgumentPack>
    compound(const ArgumentPack &args)
        : base_type(args)
        , m_mouse_focus_child(0) 
    {}
    
    compound & operator << (positioned<> *v)
    {
        m_children.push_back(v);
        access::set_parent(*v, *this);
        return *this;
    }
    
    bool on_event(const event_type &event_info);

    const ptr_vector<positioned<> > &children() const
    {   return m_children; }
    
protected:
    void draw_prologue();

    ptr_vector<positioned<> > &children()
    {   return m_children; }

private:
    ptr_vector<positioned<> > m_children;
    positioned<> * m_mouse_focus_child;

    friend class detail::compound_event_visitor<BaseView>;
};

}}}

#endif // BOOST__GUIGL__VIEW_HPP