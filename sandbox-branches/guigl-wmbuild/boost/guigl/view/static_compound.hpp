/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__STATIC_COMPOUND_HPP
#define BOOST__GUIGL__VIEW__STATIC_COMPOUND_HPP


#include <boost/guigl/view/positioned.hpp>
#include <boost/guigl/view/impl/detail/algorithm.hpp>

namespace boost { namespace guigl { namespace view {

namespace detail
{
    template<typename StaticCompound>
    struct draw;

    template<typename ChildrenSequence, typename BaseView>
    struct static_compound_event_visitor;
    
    template<typename StaticCompound>
    struct set_parent
    {
        set_parent(StaticCompound &compound)
            : compound(compound)
        {}
        
        void operator()(view::positioned<> &view) const
        {
            compound.set_parent_of_child(view);
        }
        
        StaticCompound &compound;
    };
}

template<typename ChildrenSequence, typename BaseView=base>
class static_compound : public BaseView
{    
public:
    typedef BaseView base_type;

    template<typename ArgumentPack>
    static_compound(const ArgumentPack &args)
        : base_type(args)
        , m_children(args[_children])
        , m_mouse_focus_child(0)
        , m_button_focus_child(0)
    {
        detail::for_each(m_children, detail::set_parent<static_compound>(*this));
    }
    static_compound(const static_compound &rhs)
        : base_type(static_cast<const base_type &>(rhs))
        , m_children(rhs.m_children)
        , m_mouse_focus_child(0)
        , m_button_focus_child(0)
    {}
    
    bool on_event(const event_type &event_info);

    const ChildrenSequence &children() const
    {   return m_children; }

protected:
    void draw_prologue();

    ChildrenSequence &children()
    {   return m_children; }

    ChildrenSequence m_children;
private:
    positioned<> *m_mouse_focus_child, *m_button_focus_child;

    template<typename View>
    static void draw(View &view)
    {   access::draw(view); }
    
    void set_parent_of_child(positioned<> &view)
    {
        access::set_parent(view, *this);
    }
    
    friend struct detail::static_compound_event_visitor<ChildrenSequence, BaseView>;
    friend struct detail::draw<static_compound>;
    friend struct detail::set_parent<static_compound>;
};

}}}

#endif // BOOST__GUIGL__VIEW__STATIC_COMPOUND_HPP
