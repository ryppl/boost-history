// svg_tag.hpp

// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef _SVG_TAG_HPP
#define _SVG_TAG_HPP

#include <sstream>
#include <string>
#include <boost/ptr_container/ptr_container.hpp>
#include <boost/noncopyable.hpp>

#include "svg_style.hpp"

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
public:
    virtual void write(std::ostream&) = 0;
};

// -----------------------------------------------------------------
// The node element of our document tree
// -----------------------------------------------------------------

class g_element: public svg_element
{
private: 
    svg_g_style style_info;

public:
    boost::ptr_vector<svg_element> children;
    
    svg_element& operator[](unsigned int);
    size_t size();
    
    void set_stroke_color(const svg_color&);
    void set_fill_color(const svg_color&);

    void write(std::ostream&);

    g_element& g_tag(int);
};

svg_element& g_element::operator[](unsigned int i)
{
    return children[i];
}

size_t g_element::size()
{
    return children.size();
}

void g_element::write(std::ostream& rhs)
{
    rhs << "<g ";
    style_info.write(rhs);
    rhs<< " >" << std::endl;
    
    for(unsigned int i=0; i<children.size(); ++i)
    {
        children[i].write(rhs);
    }

    rhs << "</g>" << std::endl;

}

g_element& g_element::g_tag(int i)
{
    return *(static_cast<g_element*>(&children[i]));
}

void g_element::set_stroke_color(const svg_color& _col)
{
    style_info.set_stroke_color(_col);
}

void g_element::set_fill_color(const svg_color& _col)
{
    style_info.set_fill_color(_col);
}


// -----------------------------------------------------------------
// Represents a single point
// -----------------------------------------------------------------
class point_element: public svg_element
{
private:
    double x, y;

public:
    point_element(double, double);
    void write(std::ostream&);
};

point_element::point_element(double _x, double _y):x(_x), y(_y)
{
    
}

void point_element::write(std::ostream& rhs)
{
    rhs<<"<circle cx=\""<<x<<"\" cy=\""<<y<<"\" r=\"5\"/>";

}

// -----------------------------------------------------------------
// Represents a line
// -----------------------------------------------------------------
class line_element: public svg_element
{
private:
    double x1, x2, y1, y2, y;

public:
    line_element(double, double, double, double);
    void write(std::ostream&);
};

line_element::line_element(double _x1, double _y1, double _x2,
                             double _y2):x1(_x1), y1(_y1),
                             x2(_x2), y2(_y2)
{
    
}

void line_element::write(std::ostream& rhs)
{
    rhs<<"<line x1=\""<<x1<<"\" y1=\""<<y1<<"\" x2=\""<<x2<<"\" y2=\""
        <<y2<<"\"/>";
}

// -----------------------------------------------------------------
// Represents a single block of text
// -----------------------------------------------------------------
class text_element: public svg_element
{
private:
    double x, y;
    std::string text;

public:
    text_element(double, double, std::string);
    void write(std::ostream&);
};

text_element::text_element(double _x, double _y, std::string _text)
            :x(_x), y(_y), text(_text)
{
    
}

void text_element::write(std::ostream& rhs)
{
    rhs<<"<text x=\""<<x<<"\""
                <<" y=\""<<y<<"\" "
                <<" font-family=\"Veranda\" font-size=\"12\" stroke=\"black\">"
                << text
                <<" </text>";
}


}
}

#endif