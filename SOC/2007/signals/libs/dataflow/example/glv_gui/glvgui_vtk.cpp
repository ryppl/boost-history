// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <glv.h>
#include <glv_pimpl_binding.h>

#include "blueprint_bank.hpp"
#include "blueprint_window.hpp"

#include <boost/dataflow/vtk/support.hpp>
#include "vtkPolyDataMapper.h"
#include "vtkConeSource.h"
#include <boost/dataflow/blueprint/component_bank.hpp>

DATAFLOW_RUNTIME_PROPERTY(vtk::tag, const char *, "Dataflow.Graph")
// 40 us supposed to be a UUID :-)
DATAFLOW_RUNTIME_PROPERTY(vtk::tag, int, 40)

namespace df=boost::dataflow;
namespace blueprint=boost::dataflow::blueprint;

template<typename T>
struct vtk_smart_ptr
{
    typedef T element_type;
    
    vtk_smart_ptr()
    {
        m_ptr = T::New();
    }
    vtk_smart_ptr(const vtk_smart_ptr &other)
    {
        m_ptr = other.m_ptr;
        other.m_ptr = NULL;
    }
    ~vtk_smart_ptr()
    {
        if(m_ptr)
            m_ptr->Delete();
    }
    T &operator*()
    {
        return *m_ptr;
    }
private:
    mutable T *m_ptr;
};

// A component_bank with some components
class example_bank : public blueprint::tag_component_bank<df::vtk::tag>
{
public:
    example_bank()
    {
        add_component<vtk_smart_ptr<vtkRenderWindow> >("Render\nWindow");
        add_component<vtk_smart_ptr<vtkRenderer> >("Renderer");
        add_component<vtk_smart_ptr<vtkActor> >("Actor");
        add_component<vtk_smart_ptr<vtkPolyDataMapper> >("PolyData\nMapper");
        add_component<vtk_smart_ptr<vtkConeSource> >("Cone\nSource");
    }
private:
};

int main()
{
    glv::GLV top;

	glv::Window win(640, 640, "GLV Blueprint GUI", &top);
    glv::Placer placer(top, glv::Direction::E, glv::Place::TL, 0, 0, 0);
    
    using namespace boost::dataflow::glv_gui;

    // create the blueprint and component windows
    blueprint_bank bank;
    blueprint_window window;

    // initialize the bank
    bank.set_bank(example_bank());
    bank.set_blueprint(window);
    
    placer << bank.view() << window;
    glv::Application::run();
}

//]

