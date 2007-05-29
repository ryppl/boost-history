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

namespace boost {
namespace svg {

/*The lines in the beginning of document store the xml document in 
the following format:

<?xml version=...
<!Doctype svg...
<svg width="...
<g
	Style line.. optional
>

*/

enum {SVG_XML_HEADER, SVG_XML_DOCTYPE, SVG_OPEN, SVG_G_OPEN, SVG_G_STROKE,
	  SVG_G_FILL, SVG_G_CLOSE};

class svg
{
private:
	std::vector<std::string> document;

	std::ostream *s_out;
	
	void _write_header();

	//Don't let people initialize this class
	//without specifying the stream. I can't think
	//of a reasonable default.
	svg();

protected:
	unsigned int x_size;
	unsigned int y_size;
	void _check_size();
	void _size(unsigned int, unsigned int);
	
	void _write();
	void _point(double, double);
	void _line(double, double, double, double);
	void _text(double, double, std::string);
	void _line_color(svg_color);

public:
	svg(const std::string&);
	
	virtual ~svg();
	svg(const svg&);
	svg& operator=(const svg&);

	void operator<<(const svg_instruction&);

	void operator<<(const svg_point&);

	void operator<<(const svg_line&);
	void operator<<(const svg_text&);

	void operator<<(const svg_line_color&);

	friend std::ostream& operator<<(std::ostream&, const svg&);
};

// -----------------------------------------------------------------
// Constructors will be added so that the user can specify
// a stream instead of a filename
// -----------------------------------------------------------------
svg::svg(const std::string& fname)
			:s_out(new std::ofstream(fname.c_str())) 
{ 
	_write_header();
}

svg::~svg()
{ 
	delete s_out;
}

//specify a new stream after the copy
svg::svg(const svg& rhs)
{
	x_size = rhs.x_size;
	y_size = rhs.y_size;
	document = rhs.document;

}

// -----------------------------------------------------------------
// Processes the svg_instructions
// This allows the user to easily enter information:
//
// my_image<<color(RED)<<line(x1,y1,x2,y2)<<write(); 
// 
// is the eventual goal for the interface to the svg object 
// -----------------------------------------------------------------
void svg::operator<<(const svg_instruction& rhs)
{
	switch(rhs.i_type)
	{
	case SVG_WRITE:
		_check_size();
		_write();
		break;
	}
}

// -----------------------------------------------------------------
// Stream operator to eventually stick data together in a stream-
// like interface
// -----------------------------------------------------------------
void svg::operator<<(const svg_point &rhs)
{
	switch(rhs.i_type)
	{
	case SVG_SIZE:
		_size((unsigned int)rhs.x, (unsigned int)rhs.y);
		break;
	case SVG_POINT:
		_point(rhs.x, rhs.y);
	}
}

void svg::operator<<(const svg_line &rhs)
{
	_line(rhs.x1, rhs.y1, rhs.x2, rhs.y2);
}

void svg::operator<<(const svg_text &rhs)
{
	_text(rhs.x, rhs.y, rhs.text);
}

void svg::operator<<(const svg_line_color &rhs)
{
	_line_color(rhs.col);
}

// -----------------------------------------------------------------
// Internal function to write the data to a specified stream
// TODO: allow other streams than a file stream
// -----------------------------------------------------------------
void svg::_write()
{
	document.push_back("</g>");
	document.push_back("</svg>");

	std::copy(document.begin(), document.end(), 
		std::ostream_iterator<std::string>(*s_out, "\n"));
}

// -----------------------------------------------------------------
// This prints the svg header into the document
// -----------------------------------------------------------------
void svg::_write_header()
{
	document.push_back("<?xml version=\"1.0\" standalone=\"no\"?>");
	document.push_back((std::string)"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" "+
					   "\"http://www.w3.org/graphics/svg/1.1/dtd/svg11.dtd\">");
}

// -----------------------------------------------------------------
// Writes the information about the size of the file to the document
// TODO: allow other unit identifiers
// -----------------------------------------------------------------
void svg::_size(unsigned int x, unsigned int y)
{
	x_size = x;
	y_size = y;

	std::stringstream fmt;
	std::string to_write;
	
	fmt<<"<svg width=\""<<x<<"\" height =\"" 
					<<y<<"\" version=\"1.1\""
					<<" xmlns=\"http://www.w3.org/2000/svg\">";
	
	std::getline(fmt, to_write);

	//if nothing has been written to the document yet, append
	if(document.size() == 2)
	{
		document.push_back(to_write);

		document.push_back("<g ");
		document.push_back("");
		document.push_back("");
		document.push_back(">");
	}

	//otherwise, we are changing existing information
	else
		document[2] = to_write;
}

// -----------------------------------------------------------------
// This makes sure that the size of the svg image has been written
// before we do any other kind of work building the image
// -----------------------------------------------------------------
void svg::_check_size()
{
	if(document.size() < 3)
	{
		std::string to_write = "<svg width=\"100\" height =\"100\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">";

		document.push_back(to_write);

		document.push_back("<g ");

		//optional style line
		document.push_back("");
		document.push_back("");
		document.push_back(">");
	}
}

// -----------------------------------------------------------------
// Writes the information about points to the document
// Since a point is not visible, we are actually drawing small
// circles
// TODO: allow other unit identifiers
// -----------------------------------------------------------------
void svg::_point(double x, double y)
{
	std::stringstream fmt;
	std::string to_write;
	
	fmt<<"<circle cx=\""<<x<<"\" cy =\"" 
					<<y<<"\" r=\"5\" "
					<<"fill=\"red\"/>";
	
	std::getline(fmt, to_write);

	document.push_back(to_write);
}

// -----------------------------------------------------------------
// Writes the information about lines to the document
// TODO: allow other unit identifiers
// TODO: Allow other line thicknesses
// TODO: Allow other line colors
// -----------------------------------------------------------------
void svg::_line(double x1, double y1, double x2, double y2)
{
	std::stringstream fmt;
	std::string to_write;
	//<line x1="000" y1="000" x2="000" y2="000" stroke="black"/>	
	fmt<< "<line x1=\""<<x1<<"\""
					<<" y1 =\""<<y1<<"\" "
					<<" x2 =\""<<x2<<"\" "
					<<" y2 =\""<<y2<<"\"/>";

	getline(fmt, to_write);

	document.push_back(to_write);
}

// -----------------------------------------------------------------
// Writes the information about lines to the document
// TODO: allow different fonts and font sizes
// -----------------------------------------------------------------
void svg::_text(double x, double y, std::string text)
{
	std::stringstream fmt;
	std::string to_write;

	fmt<<"<text x=\""<<x<<"\""
				<<" y=\""<<y<<"\" "
				<<" font-family=\"Veranda\" font-size=\"12\" fill=\"black\">"
				<< text
				<<" </text>";

	getline(fmt, to_write);

	document.push_back(to_write);
}

// -----------------------------------------------------------------
// TODO: Add all of the colors that are supported by the SVG format
// -----------------------------------------------------------------
void svg::_line_color(svg_color col)
{
	switch(col)
	{
	case BLACK:
		document[SVG_G_STROKE] = "stroke = \"black\"";
		break;
	case GRAY:
		document[SVG_G_STROKE] = "stroke = \"gray\"";
		break;
	case RED:
		document[SVG_G_STROKE] = "stroke = \"red\"";
		break;
	}
}


}
}

#endif
