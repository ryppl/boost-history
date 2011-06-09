// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Boost.Index - R-tree levels checking visitor
//
// Copyright 2011 Adam Wulkiewicz.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_VISITORS_ARE_LEVELS_OK_HPP
#define BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_VISITORS_ARE_LEVELS_OK_HPP

#include <boost/geometry/extensions/index/rtree/node.hpp>

namespace boost { namespace geometry { namespace index {

namespace detail { namespace rtree { namespace visitors {

template <typename Value, typename Translator, typename Box, typename Tag>
class are_levels_ok : public rtree::visitor<Value, Box, Tag, true>::type
{
    typedef typename rtree::internal_node<Value, Box, Tag>::type internal_node;
    typedef typename rtree::leaf<Value, Box, Tag>::type leaf;

public:
    inline are_levels_ok(Translator const& tr)
        : result(true), m_tr(tr), m_leafs_level(std::numeric_limits<size_t>::max()), m_current_level(0)
    {}

    inline void operator()(internal_node const& n)
    {
        typedef typename rtree::elements_type<internal_node>::type elements_type;
        elements_type const& elements = rtree::elements(n);

        if (elements.empty())
        {
            result = false;
            return;
        }

		size_t current_level_backup = m_current_level;
		++m_current_level;

		for ( typename elements_type::const_iterator it = elements.begin();
              it != elements.end() ; ++it)
        {
            rtree::apply_visitor(*this, *it->second);

            if ( result == false )
                return;
        }

		m_current_level = current_level_backup;
    }

    inline void operator()(leaf const& n)
    {
        typedef typename rtree::elements_type<leaf>::type elements_type;
        elements_type const& elements = rtree::elements(n);

		// empty leaf in non-root node
        if (0 < m_current_level && elements.empty())
        {
            result = false;
            return;
        }

        if ( m_leafs_level == std::numeric_limits<size_t>::max() )
		{
			m_leafs_level = m_current_level;
		}
		else if ( m_leafs_level != m_current_level )
		{
			result = false;
		}
    }

    bool result;

private:
    Translator const& m_tr;
    size_t m_leafs_level;
	size_t m_current_level;
};

}}} // namespace detail::rtree::visitors

template <typename Value, typename Translator, typename Tag>
bool are_levels_ok(rtree<Value, Translator, Tag> const& tree)
{
    typedef rtree<Value, Translator, Tag> rt;
    detail::rtree::visitors::are_levels_ok<
        typename rt::value_type,
        typename rt::translator_type,
        typename rt::box_type,
        typename rt::tag_type> v(tree.get_translator());
    
    tree.apply_visitor(v);

    return v.result;
}

}}} // namespace boost::geometry::index

#endif // BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_VISITORS_ARE_LEVELS_OK_HPP
