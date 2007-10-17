// svg.hpp

// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// -----------------------------------------------------------------

#ifndef _BOOST_SVG_SVG_HPP
#define _BOOST_SVG_SVG_HPP

#include <string>
#include <ostream>
#include <fstream>
#include <exception>
#include <vector>

#include "stylesheet.hpp" // TODO better to be called svg_stylesheet.hpp?
#include "detail/svg_tag.hpp"
#include "svg_style.hpp"

namespace boost {
namespace svg {

class svg
{
protected:
    unsigned int x_size;
    unsigned int y_size;

    g_element document;

	std::vector<clip_path_element> clip_paths;

    std::string css;

private:

    // -----------------------------------------------------------------
    // Internal function to write all of the data to the svg document
    // -----------------------------------------------------------------
    void _write_document(std::ostream& s_out)
    {
        //Write clip paths
		for(size_t i=0; i<clip_paths.size(); ++i)
        {
            clip_paths[ (unsigned int)i ].write(s_out);
        }

		//Write all visual elements
        for(size_t i=0; i<document.size(); ++i)
        {
            document[ (unsigned int)i ].write(s_out);
        }
    }

    // -----------------------------------------------------------------
    // This prints the svg 1.1 header into the document
    // -----------------------------------------------------------------
    void _write_header(std::ostream& s_out)
    {
        s_out  << "<?xml version=\"1.0\" standalone=\"no\"?>"
               << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" "
               << "\"http://www.w3.org/graphics/svg/1.1/dtd/svg11.dtd\">"<<std::endl;
    }

    void _write_css(std::ostream& s_out)
    {
        s_out << "<defs><style type=\"text/css\"><![CDATA[" << css
            << "]]></style></defs>" << std::endl;
    }

public:

    svg():x_size(400), y_size(400)
    {
    }

    svg(const svg& rhs):x_size(rhs.x_size), y_size(rhs.y_size)
    {
    }

    svg& write(const std::string& _file)
    {
        std::ofstream f_out(_file.c_str());

        if(f_out.fail())
        {
            throw std::runtime_error("Unable to open file "+_file);
        }

        write(f_out);

        return *this;
    }

    svg& write(std::ostream& _s_out)
    {

        _write_header(_s_out);

        //begin svg tag
        _s_out<<"<svg width=\""<<x_size<<"\" height =\""
                        <<y_size<<"\" version=\"1.1\""
                        <<" xmlns=\"http://www.w3.org/2000/svg\">"<<std::endl;

        _write_css(_s_out);

        _write_document(_s_out);

        //close off svg tag
        _s_out<<"</svg>";

        return *this;
    }

    // -----------------------------------------------------------------
    // Writes the information about the size of the file to the document
    // TODO: allow other unit identifiers
    // -----------------------------------------------------------------
    svg& image_size(unsigned int x, unsigned int y)
    {
        x_size = x;
        y_size = y;

        return *this;
    }

    svg& circle(double x, double y, unsigned int radius = 5)
    {
        document.push_back(new circle_element(x, y, radius));
        return *this;
    }

    // -----------------------------------------------------------------
    // Writes the information about lines to the document
    // -----------------------------------------------------------------
    svg& line(double x1, double y1, double x2, double y2)
    {
        document.push_back(new line_element(x1, y1, x2, y2));

        return *this;
    }

    // -----------------------------------------------------------------
    // Writes the information about text to the document
    // -----------------------------------------------------------------
    svg& text(double x, double y, const std::string& text,
        int text_size = 12, text_style align = center_align,
        int rotation = 0)
    {
        document.push_back(new text_element(x, y, text, text_size, align,
                                            rotation));

        return *this;
    }

    svg& rect(double x1, double y1, double x2, double y2)
    {
        document.push_back(new rect_element(x1, y1, x2, y2));

        return *this;
    }

    path_element& path()
    {
        document.push_back(new path_element());
        return *(static_cast<path_element*>(&(document[(unsigned int)(document.size()-1)])));
    }

    svg& clip_path(const rect_element& _rect, const std::string& _id)
    {
        clip_paths.push_back(clip_path_element(_id,_rect));

        return *this;
    }

    g_element& add_g_element()
    {
        return document.add_g_element();
    }

    g_element& get_g_element(int i)
    {
        return document.g_tag(i);
    }

    unsigned int get_x_size()
    {
        return x_size;
    }

    unsigned int get_y_size()
    {
        return y_size;
    }

    svg& load_stylesheet(const std::string& input)
    {
        std::ifstream if_str(input.c_str());

        if(if_str.fail())
        {
            throw std::runtime_error("Error opening file " + input);
        }

        if(!validate_stylesheet(if_str))
        {
            throw std::runtime_error("Error loading stylesheet");
        }

        if_str.clear();
        if_str.seekg(0);

        std::string tmp;

        css = "";
        while(std::getline(if_str, tmp))
        {
            css += tmp;
        }

        return *this;
    }
}; // end class svg

}
}

#endif
