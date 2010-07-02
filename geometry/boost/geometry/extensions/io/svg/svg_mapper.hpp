// Boost.Geometry (aka GGL, Generic Geometry Library) test file
//
// Copyright Barend Gehrels 2009-2010, Geodan, Amsterdam, the Netherlands
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_IO_SVG_MAPPER_HPP
#define BOOST_GEOMETRY_IO_SVG_MAPPER_HPP

#include <cstdio>

#include <vector>


#include <boost/scoped_ptr.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/noncopyable.hpp>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <boost/geometry/algorithms/envelope.hpp>
#include <boost/geometry/algorithms/transform.hpp>
#include <boost/geometry/algorithms/num_points.hpp>
#include <boost/geometry/strategies/transform.hpp>

#include <boost/geometry/strategies/transform/map_transformer.hpp>

#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/linear_ring.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>



#include <boost/geometry/multi/core/tags.hpp>

#include <boost/geometry/extensions/io/svg/write_svg.hpp>


namespace boost { namespace geometry
{


#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template <typename GeometryTag, bool IsMulti, typename Geometry>
struct svg_map
{
};


template <typename Point>
struct svg_map<boost::geometry::point_tag, false, Point>
{
    template <typename TransformStrategy>
    static inline void apply(std::ostream& stream,
                    std::string const& style, int size,
                    Point const& point, TransformStrategy const& strategy)
    {
        boost::geometry::point_xy<int> ipoint;
        boost::geometry::transform(point, ipoint, strategy);
        stream << boost::geometry::svg(ipoint, style, size) << std::endl;
    }
};

template <typename Box>
struct svg_map<boost::geometry::box_tag, false, Box>
{
    template <typename TransformStrategy>
    static inline void apply(std::ostream& stream,
                    std::string const& style, int size,
                    Box const& box, TransformStrategy const& strategy)
    {
        boost::geometry::box<boost::geometry::point_xy<int> > ibox;
        boost::geometry::transform(box, ibox, strategy);

        stream << boost::geometry::svg(ibox, style, size) << std::endl;
    }
};


template <typename Range1, typename Range2>
struct svg_map_range
{
    template <typename TransformStrategy>
    static inline void apply(std::ostream& stream,
                std::string const& style, int size,
                Range1 const& range, TransformStrategy const& strategy)
    {
        Range2 irange;
        boost::geometry::transform(range, irange, strategy);
        stream << boost::geometry::svg(irange, style, size) << std::endl;
    }
};



template <typename Ring>
struct svg_map<boost::geometry::ring_tag, false, Ring>
    : svg_map_range<Ring, boost::geometry::linear_ring<boost::geometry::point_xy<int> > >
{};


template <typename Linestring>
struct svg_map<boost::geometry::linestring_tag, false, Linestring>
    : svg_map_range<Linestring, boost::geometry::linestring<boost::geometry::point_xy<int> > >
{};


template <typename Polygon>
struct svg_map<boost::geometry::polygon_tag, false, Polygon>
{
    template <typename TransformStrategy>
    static inline void apply(std::ostream& stream,
                    std::string const& style, int size,
                    Polygon const& polygon, TransformStrategy const& strategy)
    {
        boost::geometry::polygon<boost::geometry::point_xy<int> > ipoly;
        boost::geometry::transform(polygon, ipoly, strategy);
        stream << boost::geometry::svg(ipoly, style, size) << std::endl;
    }
};


template <typename Tag, typename Multi>
struct svg_map<Tag, true, Multi>
{
    template <typename TransformStrategy>
    static inline void apply(std::ostream& stream,
                    std::string const& style, int size,
                    Multi const& multi, TransformStrategy const& strategy)
    {
        for (typename boost::range_iterator<Multi const>::type it
            = boost::begin(multi);
            it != boost::end(multi);
            ++it)
        {
            svg_map
                <
                    typename boost::geometry::single_tag<Tag>::type,
                    false,
                    typename boost::range_value<Multi>::type
                >::apply(stream, style, size, *it, strategy);
        }
    }
};


} // namespace dispatch
#endif


template <typename Geometry, typename TransformStrategy>
inline void svg_map(std::ostream& stream,
            std::string const& style, int size,
            Geometry const& geometry, TransformStrategy const& strategy)
{
    dispatch::svg_map
        <
            typename boost::geometry::tag<Geometry>::type,
            boost::geometry::is_multi<Geometry>::type::value,
            typename boost::remove_const<Geometry>::type
        >::apply(stream, style, size, geometry, strategy);
}


template <typename Point, bool SameScale = true>
class svg_mapper : boost::noncopyable
{
    typedef boost::geometry::point_xy<int> map_point_type;
    typedef boost::geometry::strategy::transform::map_transformer
        <
            Point,
            map_point_type,
            true,
            SameScale
        > transformer_type;

    boost::geometry::box<Point> m_bounding_box;
    boost::scoped_ptr<transformer_type> m_matrix;
    std::ostream& m_stream;
    int m_width, m_height;

    void init_matrix()
    {
        if (! m_matrix)
        {
            m_matrix.reset(new transformer_type(m_bounding_box,
                            m_width, m_height));

            m_stream << "<?xml version=\"1.0\" standalone=\"no\"?>"
                << std::endl
                << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\""
                << std::endl
                << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">"
                << std::endl
                << "<svg width=\"100%\" height=\"100%\" version=\"1.1\""
                << std::endl
                << "xmlns=\"http://www.w3.org/2000/svg\">"
                << std::endl;
        }
    }

public :
    svg_mapper(std::ostream& s, int w, int h)
        : m_stream(s)
        , m_width(w)
        , m_height(h)
    {
        boost::geometry::assign_inverse(m_bounding_box);
    }

    virtual ~svg_mapper()
    {
        m_stream << "</svg>" << std::endl;
    }

    template <typename Geometry>
    void add(Geometry const& geometry)
    {
        if (boost::geometry::num_points(geometry) > 0)
        {
            boost::geometry::combine(m_bounding_box,
                boost::geometry::make_envelope
                    <
                        boost::geometry::box<Point>
                    >(geometry));
        }
    }

    template <typename Geometry>
    void map(Geometry const& geometry, std::string const& style,
                int size = -1)
    {
        init_matrix();
        svg_map(m_stream, style, size, geometry, *m_matrix);
    }

    template <typename TextPoint>
    void text(TextPoint const& point, std::string const& s,
                std::string const& style,
                int offset_x = 0, int offset_y = 0, int lineheight = 10)
    {
        init_matrix();
        map_point_type map_point;
        boost::geometry::transform(point, map_point, *m_matrix);
        m_stream
            << "<text style=\"" << style << "\""
            << " x=\"" << boost::geometry::get<0>(map_point) + offset_x << "\""
            << " y=\"" << boost::geometry::get<1>(map_point) + offset_y << "\""
            << ">";
        if (s.find("\n") == std::string::npos)
        {
             m_stream  << s;
        }
        else
        {
            // Multi-line modus

            std::vector<std::string> splitted;
            boost::split(splitted, s, boost::is_any_of("\n"));
            for (std::vector<std::string>::const_iterator it
                = splitted.begin();
                it != splitted.end();
                ++it, offset_y += lineheight)
            {
                 m_stream
                    << "<tspan x=\"" << boost::geometry::get<0>(map_point) + offset_x
                    << "\""
                    << " y=\"" << boost::geometry::get<1>(map_point) + offset_y
                    << "\""
                    << ">" << *it << "</tspan>";
            }
        }
        m_stream << "</text>" << std::endl;
    }
};


}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_IO_SVG_MAPPER_HPP