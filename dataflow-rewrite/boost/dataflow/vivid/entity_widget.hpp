/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__VIVID__ENTITY_WIDGET_HPP
#define BOOST__DATAFLOW__VIVID__ENTITY_WIDGET_HPP


#include <boost/dataflow/blueprint/framework_entity.hpp>

#include <boost/guigl/widget/labeled_button.hpp>
#include <boost/guigl/view/impl/draggable.hpp>
#include <boost/scoped_ptr.hpp>

namespace boost { namespace dataflow { namespace vivid {

BOOST_PARAMETER_UNTYPED_NAME(entity);

template<typename BlueprintFramework>
class entity_widget
    : public guigl::view::draggable<entity_widget<BlueprintFramework>, guigl::button::right_type,
        guigl::widget::labeled_button>
{
    typedef blueprint::framework_entity<BlueprintFramework> framework_entity_type;
public:
    typedef guigl::view::draggable<entity_widget<BlueprintFramework>, guigl::button::right_type,
        guigl::widget::labeled_button> base_type;

    template<typename Args>
    entity_widget(const Args &args)
        : base_type(args)
        , m_entity(args[_entity])
    {}
    void draggable_on_drag(const guigl::position_type &position)
    {
        guigl::position_type difference(position - base_type::drag_origin());
        base_type::set_position(base_type::position() + difference);
        guigl::window::redraw(*this);
    }
    framework_entity_type &entity()
    {   return *m_entity; }
private:
    boost::scoped_ptr<framework_entity_type> m_entity;
};

} } }

#endif // BOOST__DATAFLOW__VIVID__FACTORY_WINDOW_HPP
