// svg_tag.hpp

// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef _SVG_TAG_HPP
#define _SVG_TAG_HPP

#if defined (BOOST_MSVC)
#  pragma warning(push)
#  pragma warning(disable: 4127) // "conditional expression is constant."
#  pragma warning(disable: 4512) // "assignment operator could not be generated."
#  pragma warning(disable: 4100) // "'boost_parameter_enabler_argument' : unreferenced formal parameter"
#endif

#include <boost/ptr_container/ptr_container.hpp>
#include "../svg_style.hpp"

#include <sstream>
#include <string>

#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

namespace boost {
namespace svg {


// -----------------------------------------------------------------
// This file defines all classes that can occur in the SVG parse
// tree
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// The base class for all leaf elements
// -----------------------------------------------------------------
    
class svg_element
{
protected:
    svg_style style_info;
    std::string id_name;
	std::string class_name;
	std::string clip_name;

public:
    virtual void write(std::ostream& rhs) = 0;

    virtual ~svg_element()
    {

    }

	void write_elements(std::ostream& rhs)
	{
		if(id_name.size())
		{
			rhs << " id=\"" << id_name << "\"";
		}

		if(class_name.size())
		{
			rhs << " class=\"" << class_name << "\"";
		}

		if(clip_name.size())
		{
			rhs << " clip-path=\"" << id_name << "\" ";
		}
	}

   svg_style& style(){ return style_info; }
   const svg_style& style() const{ return style_info; }

   void id(const std::string& _id) { id_name = _id; }
   std::string id( ) { return id_name; }

   void clip(const std::string& _name){ clip_name = _name; }
   std::string clip( ) { return clip_name; }

   void xml_class(const std::string& _class) { class_name = _class; }
   std::string xml_class() { return class_name; }
};

// -----------------------------------------------------------------
// Represents a single block of text
// -----------------------------------------------------------------
class rect_element: public svg_element
{
private:
    double x, y, height, width;

public:

    rect_element(double _x, double _y, double _w, double _h)
                :x(_x), y(_y), width(_w), height(_h)
    {
        
    }

    void write(std::ostream& rhs)
    {
        rhs<<"<rect";
		write_elements(rhs);
		rhs<<"x=\""<<x<<"\""
                    <<" y=\""<<y<<"\" "
                    <<" width=\""<<width<<"\" "
                    <<" height=\""<<height<<"\"/>"
                    ;
    }
};

// -----------------------------------------------------------------
// Represents a single point
// -----------------------------------------------------------------
class circle_element: public svg_element
{
private:
    double x, y, radius;

public:
    circle_element(double _x, double _y, double _radius = 5):x(_x), y(_y), 
                                                          radius(_radius)
    {
        
    }

    void write(std::ostream& rhs)
    {
        rhs<<"<circle";
		write_elements(rhs);
		rhs<<"cx=\""
           <<x<<"\" cy=\""
           <<y<<"\" r=\""
           <<radius<<"\"/>";
    }
};

// -----------------------------------------------------------------
// Represents a line
// -----------------------------------------------------------------
class line_element: public svg_element
{
private:
    double x1, x2, y1, y2, y;

public:
    line_element(double _x1, double _y1, double _x2,
                                 double _y2):x1(_x1), y1(_y1),
                                 x2(_x2), y2(_y2)
    {
        
    }

    void write(std::ostream& rhs)
    {
        rhs<<"<line ";
		write_elements(rhs);
		rhs<<"x1=\""<<x1<<"\" y1=\""<<y1<<"\" x2=\""<<x2<<"\" y2=\""
            <<y2<<"\"/>";
    }
};

// -----------------------------------------------------------------
// Represents a single block of text
// -----------------------------------------------------------------
enum text_style{left_align, right_align, center_align};

class text_element: public svg_element
{
private:
    double x_coord, y_coord;
    int size;
    std::string txt;
    text_style align;

public:
    void alignment(text_style _a)
    {
        align = _a;
    }

    int font_size()
    {
        return size;
    }

    double x(double _x)
    {
        x_coord = _x;
    }

    double y(double _y)
    {
        y_coord = _y;
    }

    text_element(double _x, double _y, std::string _text, int _size = 12, 
                 text_style _align = center_align)
                 :x_coord(_x), y_coord(_y), txt(_text), size(_size), align(_align)
    {
        
    }

    void write(std::ostream& rhs)
    {
        std::string output;

        switch(align)
        {
        case left_align:
            output = "start";
            break;

        case right_align:
            output = "end";
            break;

        case center_align:
            output = "middle";
            break;

        default:
            output = "";
            break;
        }

        rhs << "<text ";
		write_elements(rhs);
		rhs<<"x=\"" << x_coord << "\""
            <<" y=\"" << y_coord << "\" ";
        
        if(output != "")
        {
            rhs << "text-anchor=\"" << output << "\" ";
        }

        rhs << " font-family=\"verdana\"";

        if(size == 0)
        {
            rhs << " font-size=\"12\">";
        }

        else
        {
            rhs << " font-size=\"" << size << "\">";
        }

        rhs << txt
            <<" </text>";
    }

    void font_size(unsigned int _size){ size = _size; }
    void text(const std::string& _txt) { txt = _txt; }

    std::string text()
    {
        return txt;
    }
};

class clip_path_element: public svg_element
{
private:
    std::string element_id;
    rect_element rect;

public:

    clip_path_element(const std::string& _id, const rect_element& _rect):
            element_id(_id), rect(_rect)
    {
        
    }

    void write(std::ostream& rhs)
    {
        rhs << "<clip-path id=\"" << element_id << "\">" <<std::endl;

        rect.write(rhs);

        rhs<<std::endl<<"</clip-path>";
    }
};


struct path_point
{
    bool relative;
    
    virtual void write(std::ostream& rhs) = 0;

    virtual ~path_point()
    {

    }

    path_point(bool _rel): relative(_rel)
    {

    }
};

struct m_path: public path_point
{
    double x, y;
    
    void write(std::ostream& o_str)
    {
        if(relative)
        {
            o_str<<"m";
        }

        else
        {
            o_str<<"M";
        }

        o_str<<x<<" "<<y<<" ";
    }

    m_path(double _x, double _y, bool _rel = false):
            x(_x), y(_y), path_point(_rel)
    {   
    }
};

struct z_path: public path_point
{
    void write(std::ostream& o_str)
    {
        o_str<<"Z ";
    }

    z_path():path_point(false){}
};

struct l_path: public path_point
{
    double x, y;
    
    void write(std::ostream& o_str)
    {
        if(relative)
        {
            o_str<<"l";
        }

        else
        {
            o_str<<"L";
        }

        o_str<<x<<" "<<y<<" ";
    }

    l_path(double _x, double _y, bool _rel = false):
            x(_x), y(_y), path_point(_rel)
    {   
    }
};

struct h_path: public path_point
{
    double x;
    
    void write(std::ostream& o_str)
    {
        if(relative)
        {
            o_str<<"h";
        }

        else
        {
            o_str<<"H";
        }

        o_str<<x<<" ";
    }

    h_path(double _x, bool _rel = false):
            x(_x), path_point(_rel)
    {
    }
};

struct v_path: public path_point
{
    double y;
    
    void write(std::ostream& o_str)
    {
        if(relative)
        {
            o_str<<"v";
        }

        else
        {
            o_str<<"V";
        }

        o_str<<y<<" ";
    }

    v_path(double _y, bool _rel = false):
            y(_y), path_point(_rel)
    {       
    }
};

struct c_path: public path_point
{
    double x1, y1, x2, y2, x, y;
    
    void write(std::ostream& o_str)
    {
        if(relative)
        {
            o_str<<"c";
        }

        else
        {
            o_str<<"C";
        }

        o_str<<x1<<" "<<y1<<" "
             <<x2<<" "<<y2<<" "
             <<x<<" "<<y<<" ";
    }

    c_path(double _x1, double _y1, double _x2, double _y2, 
           double _x, double _y, bool _rel = false):
           x1(_x1), y1(_y1), x2(_x2), y2(_y2), x(_x), y(_y), path_point(_rel)
    {
        
    }
};

struct q_path: public path_point
{
    double x1, y1, x, y;
    
    void write(std::ostream& o_str)
    {
        if(relative)
        {
            o_str<<"q";
        }

        else
        {
            o_str<<"Q";
        }

        o_str<<x1<<" "<<y1<<" "
             <<x<<" "<<y<<" ";
    }

    q_path(double _x1, double _y1,
           double _x, double _y, bool _rel = false):
           x1(_x1), y1(_y1), x(_x), y(_y), path_point(_rel)
    {
        
    }
};

struct s_path: public path_point
{
    double x1, y1, x, y;
    
    void write(std::ostream& o_str)
    {
        if(relative)
        {
            o_str<<"s";
        }

        else
        {
            o_str<<"S";
        }

        o_str<<x1<<" "<<y1<<" "
             <<x<<" "<<y<<" ";
    }

    s_path(double _x1, double _y1,
           double _x, double _y, bool _rel = false):
           x1(_x1), y1(_y1), x(_x), y(_y), path_point(_rel)
    {
        
    }
};

struct t_path: public path_point
{
    double x, y;
    
    void write(std::ostream& o_str)
    {
        if(relative)
        {
            o_str<<"t";
        }

        else
        {
            o_str<<"T";
        }

        o_str<<x<<" "<<y<<" ";
    }

    t_path(double _x, double _y, bool _rel = false):
           x(_x), y(_y), path_point(_rel)
    {
        
    }
};

class path_element: public svg_element
{
private:
    ptr_vector<path_point> path;
public:
    bool _fill;

    path_element(const path_element& rhs)//:path(rhs.path.release())
    {
        path = (const_cast<path_element&>(rhs)).path.release();
    }

    path_element(): _fill(true) { }

    path_element& m(double x, double y)
    {
        path.push_back(new m_path(x, y, true));
        return *this;
    }

    path_element& M(double x, double y)
    {
        path.push_back(new m_path(x, y, false));
        return *this;
    }

    path_element& z()
    {
        path.push_back(new z_path());
        return *this;
    }

    path_element& l(double x, double y)
    {
        path.push_back(new l_path(x, y, true));
        return *this;
    }

    path_element& L(double x, double y)
    {
        path.push_back(new l_path(x, y, false));
        return *this;
    }

    path_element& h(double x)
    {
        path.push_back(new h_path(x, true));
        return *this;
    }

    path_element& H(double x)
    {
        path.push_back(new h_path(x, false));
        return *this;
    }

    path_element& v(double y)
    {
        path.push_back(new v_path(y, true));
        return *this;
    }

    path_element& V(double y)
    {
        path.push_back(new v_path(y, false));
        return *this;
    }

    path_element& c(double x1, double y1, double x2, double y2, double x, double y)
    {
        path.push_back(new c_path(x1, y1, x2, y2, x, y, true));
        return *this;
    }

    path_element& C(double x1, double y1, double x2, double y2, double x, double y)
    {
        path.push_back(new c_path(x1, y1, x2, y2, x, y, false));
        return *this;
    }

    path_element& q(double x1, double y1, double x, double y)
    {
        path.push_back(new q_path(x1, y1, x, y, true));
        return *this;
    }
    
    path_element& Q(double x1, double y1, double x, double y)
    {
        path.push_back(new q_path(x1, y1, x, y, false));
        return *this;
    }

    path_element& s(double x1, double y1, double x, double y)
    {
        path.push_back(new s_path(x1, y1, x, y, true));
        return *this;
    }
    
    path_element& S(double x1, double y1, double x, double y)
    {
        path.push_back(new s_path(x1, y1, x, y, false));
        return *this;
    }

    path_element& t(double x, double y)
    {
        path.push_back(new t_path(x, y, true));
        return *this;
    }

    path_element& T(double x, double y)
    {
        path.push_back(new t_path(x, y, false));
        return *this;
    }

    void write(std::ostream& o_str)
    {
        o_str<<"<path ";
		write_elements(o_str);
		o_str<<"d=\"";
           
        for(ptr_vector<path_point>::iterator i = path.begin();
            i!=path.end();
            ++i)
        {
            (*i).write(o_str);
        }
        o_str<<"\" ";
        
        style_info.write(o_str);

        if(!_fill)
        {
            o_str << "fill = \"none\"";
        }

        o_str<<"/>";
    }
};


// -----------------------------------------------------------------
// The node element of our document tree
// -----------------------------------------------------------------
class g_element: public svg_element
{
private: 
    ptr_vector<svg_element> children;
    
public:

    svg_element& operator[](unsigned int i)
    {
        return children[i];
    }

    size_t size()
    {
        return children.size();
    }

    void write(std::ostream& rhs)
    {
        rhs << "<g ";

		write_elements(rhs);
        style_info.write(rhs);

        rhs<< " >" << std::endl;
        
        for(unsigned int i=0; i<children.size(); ++i)
        {
            children[i].write(rhs);
        }

        rhs << "</g>" << std::endl;

    }

    g_element& g_tag(int i)
    {
        return *(static_cast<g_element*>(&children[i]));
    }

    //returns a reference to the node created
    g_element& add_g_element()
    {
        children.push_back(new g_element);
        
        return *(static_cast<g_element*>(&children[children.size()-1]));
    }

    void push_back(svg_element* _g)
    {
        children.push_back(_g);
    }

    void clear()
    {
        children.clear();
    }

    g_element& circle(double x, double y, double radius = 5.)
    {
        children.push_back(new circle_element(x, y, radius));
        return *this;
    }

    g_element& text(double x, double y, std::string text)
    {
        children.push_back(new text_element(x, y, text));
        return *this;
    }

    g_element& get_g_element(int i)
    {
        return *(static_cast<g_element*>(&children[i]));
    }

    g_element& rect(double x1, double y1, double x2, double y2)
    {
        children.push_back(new rect_element(x1, y1, x2, y2));

        return *this;
    }

    g_element& line(double x1, double y1, double x2, double y2)
    {
        children.push_back(new line_element(x1, y1, x2, y2));

        return *this;
    }

    path_element& path()
    {
        children.push_back(new path_element());
        return *(static_cast<path_element*>(&(children[(unsigned int)(children.size()-1)])));
    }
};

}
}

#endif
