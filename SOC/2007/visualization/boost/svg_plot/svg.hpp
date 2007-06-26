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

#include "detail/svg_tag.hpp"
#include "detail/svg_style.hpp"

namespace boost {
namespace svg {

class svg
{
private:
    void _write_header(std::ostream&);
    void _write_document(std::ostream&);

protected:
    unsigned int x_size;
    unsigned int y_size;
    
    g_element document;

public:
    svg();
    
    virtual ~svg();

    svg(const svg&);
    svg& operator=(const svg&);

    svg& image_size(unsigned int, unsigned int);

    svg& write(std::string);
    svg& write(std::ostream&);

    svg& point(double, double);
    svg& point(double, double, g_element&);
    
    svg& line(double, double, double, double);
    svg& line(double, double, double, double, g_element&); 
    
    svg& text(double, double, std::string);

    svg& rect(double, double, double, double);
    svg& rect(double, double, double, double, g_element&);

    g_element& add_g_element();
    g_element& add_g_element(g_element&);

    g_element& get_g_element(int i);
    g_element& svg::get_g_element(int, g_element&);

    friend std::ostream& operator<<(std::ostream&, const svg&);

    unsigned int get_x_size();
    unsigned int get_y_size();
};

// -----------------------------------------------------------------
// -----------------------------------------------------------------
svg::svg():x_size(200), y_size(200)
{

}

svg::~svg()
{

}

svg& svg::write(std::string _file)
{
    std::ofstream fout(_file.c_str());
    
    if(fout.fail())
    {
        throw "Unable to open file "+_file;
    }

    _write_header(fout);

    //begin svg tag
    fout<<"<svg width=\""<<x_size<<"\" height =\"" 
                    <<y_size<<"\" version=\"1.1\""
                    <<" xmlns=\"http://www.w3.org/2000/svg\">"<<std::endl;

    _write_document(fout);

    //close off svg tag
    fout<<"</svg>";

    return *this;
}

svg& svg::write(std::ostream& s_out)
{
     _write_header(s_out);

    //begin svg tag
    s_out<<"<svg width=\""<<x_size<<"\" height =\"" 
                    <<y_size<<"\" version=\"1.1\""
                    <<" xmlns=\"http://www.w3.org/2000/svg\">"<<std::endl;

    _write_document(s_out);

    //close off svg tag
    s_out<<"</svg>";

    return *this;
}

// -----------------------------------------------------------------
// Internal function to write all of the data to the svg document
// -----------------------------------------------------------------
void svg::_write_document(std::ostream& s_out)
{
    //Write color information

    //Write all visual elements
    for(size_t i=0; i<document.size(); ++i)
    {
        document[ (unsigned int)i ].write(s_out);
    }

    //end g tag
}

// -----------------------------------------------------------------
// This prints the svg 1.1 header into the document
// -----------------------------------------------------------------
void svg::_write_header(std::ostream& s_out)
{
    s_out << "<?xml version=\"1.0\" standalone=\"no\"?>"
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
    location.push_back(new point_element(x, y));

    return *this;
}

svg& svg::point(double x, double y)
{
    document.push_back(new point_element(x, y));

    return *this;
}

// -----------------------------------------------------------------
// Writes the information about lines to the document
// TODO: Allow other line width
// TODO: Allow other line colors
// -----------------------------------------------------------------
svg& svg::line(double x1, double y1, double x2, double y2)
{
    document.push_back(new line_element(x1, y1, x2, y2));

    return *this;
}

svg& svg::line(double x1, double y1, double x2, double y2, 
                g_element& location)
{
    location.push_back(new line_element(x1, y1, x2, y2));

    return *this;
}

// -----------------------------------------------------------------
// Writes the information about text to the document
// TODO: allow different fonts and font sizes
// -----------------------------------------------------------------
svg& svg::text(double x, double y, std::string text)
{
    document.push_back(new text_element(x, y, text));

    return *this;
}

svg& svg::rect(double x1, double y1, double x2, double y2)
{
    document.push_back(new rect_element(x1, y1, x2, y2));

    return *this;
}

svg& svg::rect(double x1, double y1, double x2, double y2, 
                g_element& location)
{
    location.push_back(new rect_element(x1, y1, x2, y2));

    return *this;
}

g_element& svg::add_g_element()
{
    return document.add_g_element();
}

g_element& svg::add_g_element(g_element& _g)
{
    return _g.add_g_element();
}

g_element& svg::get_g_element(int i)
{
    return document.g_tag(i);
}

g_element& svg::get_g_element(int i, g_element& _g)
{
    return _g.g_tag(i);
}

unsigned int svg::get_x_size()
{
    return x_size;
}

unsigned int svg::get_y_size()
{
    return y_size;
}

}
}

#endif
