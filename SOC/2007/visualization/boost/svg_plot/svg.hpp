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

#include "svg_instruction.hpp"
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

    void _size(unsigned int, unsigned int);

    void _write();

    void _point(double, double);
    void _point(double, double, g_element&);
    
    void _line(double, double, double, double);
    void _line(double, double, double, double, g_element&); 
    
    void _text(double, double, std::string);
    void _line_color(svg_color);


public:
    svg(const std::string&);
    
    virtual ~svg();
    svg(const svg&);
    svg& operator=(const svg&);

    svg& operator<<(const svg_instruction&);

    svg& operator<<(const svg_point&);

    svg& operator<<(const svg_line&);
    svg& operator<<(const svg_text&);

    svg& operator<<(const svg_stroke_color&);

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
// Processes the svg_instructions
// This allows the user to easily enter information than ordinary
// method calls, and is more clear that the method supports chaining
// -----------------------------------------------------------------
svg& svg::operator<<(const svg_instruction& rhs)
{
    switch(rhs.i_type)
    {
    case SVG_WRITE:
        _write();
        break;
    }
    return *this;
}

// -----------------------------------------------------------------
// Chained stream operators. Each operator overload below will
// represent a different instruction type, after I rewrite svg's
// instructions to be more generic.
// -----------------------------------------------------------------
svg& svg::operator<<(const svg_point &rhs)
{
    switch(rhs.i_type)
    {
    case SVG_SIZE:
        _size((unsigned int)rhs.x, (unsigned int)rhs.y);
        break;
    case SVG_POINT:
        _point(rhs.x, rhs.y, document);
    }

    return *this;
}

svg& svg::operator<<(const svg_line &rhs)
{
    _line(rhs.x1, rhs.y1, rhs.x2, rhs.y2);

    return *this;
}

svg& svg::operator<<(const svg_text &rhs)
{
    _text(rhs.x, rhs.y, rhs.text);

    return *this;
}

svg& svg::operator<<(const svg_stroke_color &rhs)
{
    _line_color(rhs.col);

    return *this;
}

// -----------------------------------------------------------------
// Internal function to write the data to a specified stream
// TODO: allow other streams than a file stream
// -----------------------------------------------------------------
void svg::_write()
{
    _write_header();

    //begin svg tag
    *s_out<<"<svg width=\""<<x_size<<"\" height =\"" 
                    <<y_size<<"\" version=\"1.1\""
                    <<" xmlns=\"http://www.w3.org/2000/svg\">"<<std::endl;

    _write_document();

    //close off svg tag
    *s_out<<"</svg>";
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
void svg::_size(unsigned int x, unsigned int y)
{
    x_size = x;
    y_size = y;
}

// -----------------------------------------------------------------
// Writes the information about points to the document
// Since a point is not visible, we are actually drawing small
// circles
// TODO: allow other unit identifiers, shapes
// -----------------------------------------------------------------

// this overload has a pointer to a node in the document tree
void svg::_point(double x, double y, g_element& location)
{
    location.children.push_back(new point_element(x, y));
}

void svg::_point(double x, double y)
{
    document.children.push_back(new point_element(x, y));
}

// -----------------------------------------------------------------
// Writes the information about lines to the document
// TODO: Allow other line thicknesses
// TODO: Allow other line colors
// -----------------------------------------------------------------
void svg::_line(double x1, double y1, double x2, double y2)
{
    document.children.push_back(new line_element(x1, y1, x2, y2));
}

void svg::_line(double x1, double y1, double x2, double y2, 
                g_element& location)
{
    location.children.push_back(new line_element(x1, y1, x2, y2));
}

// -----------------------------------------------------------------
// Writes the information about text to the document
// TODO: allow different fonts and font sizes
// -----------------------------------------------------------------
void svg::_text(double x, double y, std::string text)
{
    document.children.push_back(new text_element(x, y, text));
}

// -----------------------------------------------------------------
// Hopefully this one will be filld out next week
// -----------------------------------------------------------------
void svg::_line_color(svg_color col)
{

}


}
}

#endif
