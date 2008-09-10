/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/guigl/window.hpp>
#include <boost/config.hpp>

#include <map>

#include <boost/guigl/platform/glut.hpp>
#include <iostream>

namespace boost { namespace guigl {

class glut
{
public:
    glut()
    {
        int argc = 0;
        glutInit(&argc,0);
    }
    
    int create_window(const size_type &size, const std::string &label, bool depth, window::impl *impl)
    {
        glutInitWindowSize(size.x, size.y);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | (depth?GLUT_DEPTH:0));
        int id = glutCreateWindow(label.c_str());
        if(depth)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
        s_windows[id] = impl;
        
       	glutDisplayFunc(display);
        glutMouseFunc(mouse);
        glutPassiveMotionFunc(movement);
        glutMotionFunc(movement);
        glutEntryFunc(entry);
                
        return id;
    }
    
    void destroy_window(int id)
    {
        glutDestroyWindow(id);
    }
private:
    static void display();
    static void mouse(int button, int state, int x, int y);
    static void movement(int x, int y);
    static void entry(int state);
    static std::map<int, window::impl *> s_windows;
};

std::map<int, window::impl *> glut::s_windows;


class window::impl
{
public:
    impl(const window::argument_pack_type &map, window *ptr)
        : m_window(ptr)
    {
        if(!s_glut)
            s_glut = new glut();
        m_label = map[_label];
        m_id = s_glut->create_window(map[_size], m_label, map[_depth], this);
    }
    void display()
    {
        m_window->draw();
    }
    void mouse(int button, int state, int x, int y)
    {
        if (!m_window->mouse_state())
            return;

        button_event event_info;
        switch(button)
        {
            case GLUT_LEFT_BUTTON:
                event_info.button = button::left; break;
            case GLUT_MIDDLE_BUTTON:
                event_info.button = button::middle; break;
            case GLUT_RIGHT_BUTTON:
                event_info.button = button::right;
        }
        switch(state)
        {
            case GLUT_UP:
                event_info.direction = direction::up; break;
            case GLUT_DOWN:
                event_info.direction = direction::down; break;            
        }
        event_info.position = position_type(x, y);
        m_window->on_event(event_info);
    }
    void movement(int x, int y)
    {
        if (!m_window->mouse_state())
            return;
        movement_event event_info;
        event_info.position = position_type(x, y);
        m_window->on_event(event_info);
    }
    void entry_exit(int state)
    {
        entry_exit_event event_info;
        event_info.region = state == GLUT_ENTERED ? region::entry : region::exit;
        m_window->on_event(event_info);
    }
    ~impl()
    {
        s_glut->destroy_window(m_id);
    }
    int id() const
    {   return m_id; }
private:
    int m_id;
    static glut *s_glut;
    window *m_window;
    std::string m_label;
};

glut *window::impl::s_glut = 0;

window::window(const argument_pack_type &args)
    : widget::window(args)
    , m_impl(new impl(args, this))
{}

window::~window()
{}

void window::redraw(const view::positioned<> &v)
{
    const window *window_ptr = dynamic_cast<const window *>(v.root());
    if(window_ptr)
        glutPostWindowRedisplay(window_ptr->m_impl->id());
}

void glut::display()
{
    int id = glutGetWindow();
    s_windows[id]->display();
    glutSwapBuffers();
}

void glut::mouse(int button, int state, int x, int y)
{
    int id = glutGetWindow();
    s_windows[id]->mouse(button, state, x, y);
}

void glut::movement(int x, int y)
{
    int id = glutGetWindow();
    s_windows[id]->movement(x, y);
}

void glut::entry(int state)
{
    int id = glutGetWindow();
    s_windows[id]->entry_exit(state);
}


} }