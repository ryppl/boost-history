// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Boost.Index - R-tree boxes checking visitor
//
// Copyright 2011 Adam Wulkiewicz.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_VISITORS_ARE_BOXES_OK_HPP
#define BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_VISITORS_ARE_BOXES_OK_HPP

#include <boost/geometry/algorithms/equals.hpp>
#include <boost/geometry/extensions/index/rtree/node.hpp>

namespace boost { namespace geometry { namespace index {

namespace detail { namespace rtree { namespace visitors {

template <typename Value, typename Translator, typename Box, typename Tag>
class are_boxes_ok : public boost::static_visitor<bool>
{
    typedef typename rtree::internal_node<Value, Box, Tag>::type internal_node;
    typedef typename rtree::leaf<Value, Box, Tag>::type leaf;

public:
    inline are_boxes_ok(Translator const& tr)
        : m_tr(tr), m_is_root(true)
    {}

    inline bool operator()(internal_node const& n)
    {
        typedef typename rtree::elements_type<internal_node>::type elements_type;
        elements_type const& elements = rtree::elements_get(n);

        if (elements.empty())
            return false;

        Box box_bckup = m_box;
        bool is_root_bckup = m_is_root;

        m_is_root = false;

        for ( typename elements_type::const_iterator it = elements.begin();
              it != elements.end() ; ++it)
        {
            m_box = it->first;

            if ( false == boost::apply_visitor(*this, *it->second) )
                return false;
        }

        m_box = box_bckup;
        m_is_root = is_root_bckup;

        Box result;
        geometry::convert(elements.front().first, result);
        for( typename elements_type::const_iterator it = elements.begin() + 1;
            it != elements.end() ; ++it)
        {
            geometry::expand(result, it->first);
        }
        
        return m_is_root || geometry::equals(result, m_box);
    }

    inline bool operator()(leaf const& n)
    {
        typedef typename rtree::elements_type<leaf>::type elements_type;
        elements_type const& elements = rtree::elements_get(n);

        if (elements.empty())
            return false;

        Box result;
        geometry::convert(m_tr(elements.front()), result);
        for(typename elements_type::const_iterator it = elements.begin() + 1;
            it != elements.end() ; ++it)
        {
            geometry::expand(result, m_tr(*it));
        }

        return m_is_root || geometry::equals(result, m_box);
    }

private:
    Translator const& m_tr;
    Box m_box;
    bool m_is_root;
};

}}} // namespace detail::rtree::visitors

template <typename Value, typename Translator, typename Tag>
bool are_boxes_ok(rtree<Value, Translator, Tag> const& tree)
{
    typedef rtree<Value, Translator, Tag> rt;
    detail::rtree::visitors::are_boxes_ok<
        typename rt::value_type,
        typename rt::translator_type,
        typename rt::box_type,
        typename rt::tag_type> v(tree.get_translator());
    
    return tree.apply_visitor(v);
}

}}} // namespace boost::geometry::index

#endif // BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_VISITORS_ARE_BOXES_OK_HPP
