// svg.hpp

// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef _SVG_HPP
#define _SVG_HPP

#include <vector>
#include <ostream>
#include <sstream>
#include <iostream>
#include <fstream>

#include "svg_tag.hpp"
#include "svg_style.hpp"

namespace boost {
namespace svg {

class svg
{
private:
    std::ostream *s_out;
    
    void _write_header();
    void _write_document();

    //Don't let people initialize this class
    //without specifying the stream. I can't think
    //of a reasonable default.
    svg();

protected:
    unsigned int x_size;
    unsigned int y_size;
    
    g_element document;

public:
    svg(const std::string&);
    
    virtual ~svg();

    svg(const svg&);
    svg& operator=(const svg&);

    svg& image_size(unsigned int, unsigned int);
    svg& write();

    svg& point(double, double);
    svg& point(double, double, g_element&);
    
    svg& line(double, double, double, double);
    svg& line(double, double, double, double, g_element&); 
    
    svg& text(double, double, std::string);
    svg& line_color(svg_color);

    svg& rect(double, double, double, double);
    svg& rect(double, double, double, double, g_element&); 

    friend std::ostream& operator<<(std::ostream&, const svg&);
};

// -----------------------------------------------------------------
// Constructors will be added so that the user can specify
// a stream instead of a filename
// -----------------------------------------------------------------
svg::svg(const std::string& fname)
            :s_out(new std::ofstream(fname.c_str())) 
{ 
}

svg::~svg()
{ 
    delete s_out;
}

//I still need to put more thought into whether I want this class
//copyable or not
svg::svg(const svg& rhs)
{
    x_size = rhs.x_size;
    y_size = rhs.y_size;
    //Todo: put something that will copy the elements from the
    //ptr_vector in rhs to this. ptr_vectors are noncopyable

    //document = rhs.document;
}

// -----------------------------------------------------------------
// -----------------------------------------------------------------
svg& svg::write()
{
    _write_header();

    //begin svg tag
    *s_out<<"<svg width=\""<<x_size<<"\" height =\"" 
                    <<y_size<<"\" version=\"1.1\""
                    <<" xmlns=\"http://www.w3.org/2000/svg\">"<<std::endl;

    _write_document();

    //close off svg tag
    *s_out<<"</svg>";

    return *this;
}

// -----------------------------------------------------------------
// Internal function to write all of the data to the svg document
// -----------------------------------------------------------------
void svg::_write_document()
{
    //Write color information

    //Write all visual elements
    for(size_t i=0; i<document.size(); ++i)
    {
        document[ (unsigned int)i ].write(*s_out);
    }

    //end g tag
}

// -----------------------------------------------------------------
// This prints the svg 1.1 header into the document
// -----------------------------------------------------------------
void svg::_write_header()
{
    *s_out << "<?xml version=\"1.0\" standalone=\"no\"?>"
           << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" "
           << "\"http://www.w3.org/graphics/svg/1.1/dtd/svg11.dtd\">"<<std::endl;
}

// -----------------------------------------------------------------
// Writes the information about the size of the file to the document
// TODO: allow other unit identifiers
// -----------------------------------------------------------------
svg& svg::image_size(unsigned int x, unsigned int y)
{
    x_size = x;
    y_size = y;

    return *this;
}

// -----------------------------------------------------------------
// Writes the information about points to the document
// Since a point is not visible, we are actually drawing small
// circles
// TODO: allow other unit identifiers, shapes
// -----------------------------------------------------------------

// this overload has a pointer to a node in the document tree
svg& svg::point(double x, double y, g_element& location)
{
    location.children.push_back(new point_element(x, y));

    return *this;
}

svg& svg::point(double x, double y)
{
    document.children.push_back(new point_element(x, y));

    return *this;
}

// -----------------------------------------------------------------
// Writes the information about lines to the document
// TODO: Allow other line thicknesses
// TODO: Allow other line colors
// -----------------------------------------------------------------
svg& svg::line(double x1, double y1, double x2, double y2)
{
    document.children.push_back(new line_element(x1, y1, x2, y2));

    return *this;
}

svg& svg::line(double x1, double y1, double x2, double y2, 
                g_element& location)
{
    location.children.push_back(new line_element(x1, y1, x2, y2));

    return *this;
}

// -----------------------------------------------------------------
// Writes the information about text to the document
// TODO: allow different fonts and font sizes
// -----------------------------------------------------------------
svg& svg::text(double x, double y, std::string text)
{
    document.children.push_back(new text_element(x, y, text));

    return *this;
}

// -----------------------------------------------------------------
// Hopefully this one will be filld out next week
// -----------------------------------------------------------------
svg& svg::line_color(svg_color col)
{
    return *this;
}

svg& svg::rect(double x1, double y1, double x2, double y2)
{
    document.children.push_back(new rect_element(x1, y1, x2, y2));

    return *this;
}

svg& svg::rect(double x1, double y1, double x2, double y2, 
                g_element& location)
{
    location.children.push_back(new rect_element(x1, y1, x2, y2));

    return *this;
}


}
}

#endif
