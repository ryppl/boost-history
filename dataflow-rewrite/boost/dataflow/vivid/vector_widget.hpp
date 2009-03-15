/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__VIVID__VECTOR_WIDGET_HPP
#define BOOST__DATAFLOW__VIVID__VECTOR_WIDGET_HPP


#include <boost/dataflow/vivid/entity_widget.hpp>
#include <boost/dataflow/blueprint/vector.hpp>

#include <boost/guigl/widget/compound.hpp>
#include <boost/guigl/layout/grid.hpp>

namespace boost { namespace dataflow { namespace vivid {

template<typename BlueprintFramework>
class vector_widget
    : public guigl::view::draggable<vector_widget<BlueprintFramework>, guigl::button::right_type,
        guigl::widget::compound>
{
    typedef blueprint::port<BlueprintFramework> port_type;
    typedef blueprint::vector<BlueprintFramework> vector_type;
    typedef entity_widget<BlueprintFramework> entity_widget_type;
    
public:
    typedef guigl::view::draggable<vector_widget<BlueprintFramework>, guigl::button::right_type,
        guigl::widget::compound> base_type;

    template<typename Args>
    vector_widget(const Args &args)
        : base_type(args)
        , m_entity(dynamic_cast<vector_type *>(args[_entity]))
    {
        guigl::layout::grid layout((guigl::_vertical=3, guigl::_grid_size=this->size()));
        guigl::widget::compound *consumer_ports = layout.create<guigl::widget::compound>((guigl::_background=guigl::color_type(0.5,0,0)));
        layout.next();
        guigl::widget::compound *producer_ports = layout.create<guigl::widget::compound>((guigl::_background=guigl::color_type(0,0,0.5)));

        guigl::layout::grid consumer_layout((guigl::_horizontal=3, guigl::_grid_size=consumer_ports->size()));
        guigl::layout::grid producer_layout((guigl::_horizontal=3, guigl::_grid_size=producer_ports->size()));
        blueprint::port_iterator<BlueprintFramework> it = m_entity->begin();
        blueprint::port_iterator<BlueprintFramework> end = m_entity->end();
        for(;it!=end;++it)
        {
            port_type *port;
            if((port = dynamic_cast<port_type *>(&*it)))
            {
                bool consumer = port->category_type_info() == typeid(ports::consumer);
                *(consumer ? consumer_ports : producer_ports)
                    << (consumer ? consumer_layout : producer_layout).create<entity_widget_type>((_entity=port, guigl::_label=(consumer ? "<" : ">"), _click_callback=args[_click_callback]));
            }
        }

        *this << consumer_ports;
        layout.next();
        *this << producer_ports;
    }
    void draggable_on_drag(const guigl::position_type &position)
    {
        guigl::position_type difference(position - base_type::drag_origin());
        base_type::set_position(base_type::position() + difference);
        guigl::window::redraw(*this);
    }
    vector_type &entity()
    {   return *m_entity; }
private:
    boost::scoped_ptr<vector_type> m_entity;
};

} } }

#endif // BOOST__DATAFLOW__VIVID__VECTOR_WIDGET_HPP
