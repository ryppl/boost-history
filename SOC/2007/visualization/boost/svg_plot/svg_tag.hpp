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

class svg_element : boost::noncopyable
{
public:
    virtual std::string to_string()=0;
};

// -----------------------------------------------------------------
// The node element of our document tree
// -----------------------------------------------------------------

class g_element
{
private: 
    svg_g_style styleInfo;

public:
    //public allows me to skip dozens of add_point, add_line, etc methods
    //or polymorphic checks. Also allows the root element of the node
    //to act as a makeshift DOM
    boost::ptr_vector<svg_element> children;    
    
    svg_element& operator[](unsigned int);
    size_t size();
    
    void set_line_color(const svg_color&);
};

svg_element& g_element::operator[](unsigned int i)
{
    return children[i];
}

size_t g_element::size()
{
    return children.size();
}

void set_line_color(const svg_color& rhs)
{

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
    std::string to_string();
};

point_element::point_element(double _x, double _y):x(_x), y(_y)
{
    
}

std::string point_element::to_string()
{
    std::stringstream fmt;

    fmt<<"<circle cx=\""<<x<<"\" cy=\""<<y<<"\" r=\"5\" fill=\"red\" />";
    
    std::string to_ret;
    std::getline(fmt, to_ret);

    return to_ret;
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
    std::string to_string();
};

line_element::line_element(double _x1, double _y1, double _x2,
                             double _y2):x1(_x1), y1(_y1),
                             x2(_x2), y2(_y2)
{
    
}

std::string line_element::to_string()
{
    std::stringstream fmt;

    fmt<<"<line stroke=\"black\" x1=\""<<x1<<"\" y1=\""<<y1<<"\" x2=\""<<x2<<"\" y2=\""
        <<y2<<"\"/>";
    
    std::string to_ret;
    std::getline(fmt, to_ret);

    return to_ret;
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
    std::string to_string();
};

text_element::text_element(double _x, double _y, std::string _text)
            :x(_x), y(_y), text(_text)
{
    
}

std::string text_element::to_string()
{
    std::stringstream fmt;
    std::string to_write;

    fmt<<"<text x=\""<<x<<"\""
                <<" y=\""<<y<<"\" "
                <<" font-family=\"Veranda\" font-size=\"12\" fill=\"black\">"
                << text
                <<" </text>";

    getline(fmt, to_write);

    return to_write;
}


}
}

#endif