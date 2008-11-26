/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__VIVID__FACTORY_WINDOW_HPP
#define BOOST__DATAFLOW__VIVID__FACTORY_WINDOW_HPP


#include <boost/dataflow/blueprint/factory.hpp>

#include <boost/guigl/window.hpp>
#include <boost/guigl/widget/labeled_button.hpp>
#include <boost/guigl/widget/label.hpp>
#include <boost/guigl/layout/grid.hpp>

#include <memory>


namespace boost { namespace dataflow { namespace vivid {

template<typename BlueprintFramework>
class network_window;

template<typename BlueprintFramework>
class factory_window
{
    typedef blueprint::factory<BlueprintFramework> factory_type;
    typedef blueprint::framework_entity<BlueprintFramework> framework_entity_type;
public:
    factory_window()
        : m_window(( guigl::_label="Factory", guigl::_size=guigl::size_type(60, 600) ))
    {};

    void set_factory(const factory_type &factory)
    {
        m_factory = factory;
        rearrange();
    }
    void set_network_window(network_window<BlueprintFramework> &network)
    {
        m_network = &network;
    }
    
    guigl::window &window()
    {   return m_window; }

private:
    void rearrange()
    {
        guigl::layout::grid layout((guigl::_grid_size=m_window.size(), guigl::_vertical=10));
        for(typename factory_type::iterator it=m_factory.begin(); it!=m_factory.end(); it++)
        {
            guigl::widget::labeled_button *button;
            m_window << (button = layout.create<guigl::widget::labeled_button>((guigl::_label=it->c_str())));
            button->on_click.connect(boost::bind(&factory_window::select, this, *it));
        }
    }
    void select(const std::string &key)
    {
        if(m_network)
        {
            std::auto_ptr<framework_entity_type> entity(
                m_factory[key](m_network->framework_context()));
            m_network->add_entity(entity);
        }
    }
    network_window<BlueprintFramework> * m_network;
    factory_type m_factory;
    guigl::window m_window;
	
//    component_buttons_type m_component_buttons;
//    friend class component_button;
};

} } }

#endif // BOOST__DATAFLOW__VIVID__FACTORY_WINDOW_HPP
