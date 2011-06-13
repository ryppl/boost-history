// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Boost.Index - R-tree printing visitor
//
// Copyright 2008 Federico J. Fernandez.
// Copyright 2011 Adam Wulkiewicz.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_VISITORS_PRINT_HPP
#define BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_VISITORS_PRINT_HPP

#include <iostream>

#include <boost/geometry/extensions/index/rtree/node.hpp>

namespace boost { namespace geometry { namespace index {

namespace detail { namespace rtree { namespace visitors {

namespace dispatch {

template <typename Point, size_t Dimension>
struct print_point
{
    BOOST_STATIC_ASSERT(0 < Dimension);

    static inline void apply(std::ostream & os, Point const& p)
    {
        print_point<Point, Dimension - 1>::apply(os, p);

        os << ", " << geometry::get<Dimension - 1>(p);
    }
};

template <typename Point>
struct print_point<Point, 1>
{
    static inline void apply(std::ostream & os, Point const& p)
    {
        os << geometry::get<0>(p);
    }
};

template <typename Box, size_t Corner, size_t Dimension>
struct print_corner
{
    BOOST_STATIC_ASSERT(0 < Dimension);

    static inline void apply(std::ostream & os, Box const& b)
    {
        print_corner<Box, Corner, Dimension - 1>::apply(os, b);

        os << ", " << geometry::get<Corner, Dimension - 1>(b);
    }
};

template <typename Box, size_t Corner>
struct print_corner<Box, Corner, 1>
{
    static inline void apply(std::ostream & os, Box const& b)
    {
        os << geometry::get<Corner, 0>(b);
    }
};

template <typename Indexable, typename Tag>
struct print_indexable
{
};

template <typename Indexable>
struct print_indexable<Indexable, box_tag>
{
    static const size_t dimension = index::traits::dimension<Indexable>::value;

    static inline void apply(std::ostream &os, Indexable const& i)
    {
        os << '(';
        print_corner<Indexable, min_corner, dimension>::apply(os, i);
        os << ")x(";
        print_corner<Indexable, max_corner, dimension>::apply(os, i);
        os << ')';
    }
};

template <typename Indexable>
struct print_indexable<Indexable, point_tag>
{
    static const size_t dimension = index::traits::dimension<Indexable>::value;

    static inline void apply(std::ostream &os, Indexable const& i)
    {
        os << '(';
        print_point<Indexable, dimension>::apply(os, i);
        os << ')';
    }
};

} // namespace dispatch

namespace detail {

template <typename Indexable>
inline void print_indexable(std::ostream & os, Indexable const& i)
{
    dispatch::print_indexable<
        Indexable,
        typename geometry::traits::tag<Indexable>::type
    >::apply(os, i);
}

} // namespace detail

template <typename Value, typename Options, typename Translator, typename Box>
struct print : public rtree::visitor<Value, Box, typename Options::node_tag, true>::type
{
    typedef typename rtree::internal_node<Value, Box, typename Options::node_tag>::type internal_node;
    typedef typename rtree::leaf<Value, Box, typename Options::node_tag>::type leaf;

    inline print(std::ostream & o, Translator const& t)
        : os(o), tr(t), level(0)
    {}

    inline void operator()(internal_node const& n)
    {
        typedef typename rtree::elements_type<internal_node>::type elements_type;
        elements_type const& elements = rtree::elements(n);

        spaces(level) << "INTERNAL NODE - L:" << level << " Ch:" << elements.size() << " @:" << &n << '\n';
        
        for (typename elements_type::const_iterator it = elements.begin();
            it != elements.end(); ++it)
        {
            spaces(level);
            detail::print_indexable(os, it->first);
            os << " ->" << it->second << '\n';
        }

        size_t level_backup = level;
        ++level;

        for (typename elements_type::const_iterator it = elements.begin();
            it != elements.end(); ++it)
        {
            rtree::apply_visitor(*this, *it->second);
        }

        level = level_backup;
    }

    inline void operator()(leaf const& n)
    {
        typedef typename rtree::elements_type<leaf>::type elements_type;
        elements_type const& elements = rtree::elements(n);

        spaces(level) << "LEAF - L:" << level << " V:" << elements.size() << " @:" << &n << '\n';
        for (typename elements_type::const_iterator it = elements.begin();
            it != elements.end(); ++it)
        {
            spaces(level);
            detail::print_indexable(os, tr(*it));
            os << '\n';
        }
    }

    inline std::ostream & spaces(size_t level)
    {
        for ( size_t i = 0 ; i < 2 * level ; ++i )
            os << ' ';
        return os;
    }

    std::ostream & os;
    Translator const& tr;

    size_t level;
};

}}} // namespace detail::rtree::visitors

template <typename Value, typename Options, typename Translator>
std::ostream & operator<<(std::ostream & os, rtree<Value, Options, Translator> const& tree)
{
    typedef typename rtree<Value, Options, Translator>::value_type value_type;
	typedef typename rtree<Value, Options, Translator>::options_type options_type;
    typedef typename rtree<Value, Options, Translator>::translator_type translator_type;
    typedef typename rtree<Value, Options, Translator>::box_type box_type;
    detail::rtree::visitors::print<value_type, options_type, translator_type, box_type> print_v(os, tree.get_translator());
    tree.apply_visitor(print_v);
    return os;
}

}}} // namespace boost::geometry::index

#endif // BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_VISITORS_PRINT_HPP
