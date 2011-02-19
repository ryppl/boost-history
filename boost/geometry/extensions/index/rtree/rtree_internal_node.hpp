// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Boost.SpatialIndex - rtree node implementation
//
// Copyright 2008 Federico J. Fernandez.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_RTREE_INTERNAL_NODE_HPP
#define BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_RTREE_INTERNAL_NODE_HPP

#include <deque>
#include <iostream> // TODO: Remove if print() is removed
#include <stdexcept>
#include <utility>
#include <vector>

#include <boost/shared_ptr.hpp>

#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry/algorithms/assign.hpp>
#include <boost/geometry/algorithms/combine.hpp>
#include <boost/geometry/algorithms/equals.hpp>

#include <boost/geometry/extensions/index/rtree/helpers.hpp>
#include <boost/geometry/extensions/index/rtree/rtree_node.hpp>

namespace boost { namespace geometry { namespace index {

template <typename Value, typename Translator, typename Box>
class rtree_internal_node : public rtree_node<Value, Translator, Box>
{
public:

    typedef rtree_node<Value, Translator, Box> rtree_node;
    typedef rtree_leaf<Value, Translator, Box> rtree_leaf;

    typedef boost::shared_ptr<rtree_node> node_pointer;
    typedef boost::shared_ptr<rtree_leaf> leaf_pointer;

    /// type for the node map
    typedef std::vector<std::pair<Box, node_pointer > > node_map;

    /**
     * \brief Creates a default node (needed for the containers)
     */
    rtree_internal_node()
    {
    }

    /**
     * \brief Creates a node with 'parent' as parent and 'level' as its level
     */
    rtree_internal_node(node_pointer const& parent, size_t const& level)
        : rtree_node(parent, level)
    {
    }

    // awulkiew - internal node methods

    /**
     * \brief Clear the node
     */
    // awulkiew - name changed from empty_nodes to clear_nodes
    void clear_nodes()
    {
        m_nodes.clear();
    }

    // awulkiew - internal node and leaf virtual methods

    /**
     * \brief True if it is a leaf node
     */
    virtual bool is_leaf() const
    {
        return false;
    }

    /**
     * \brief Number of elements in the subtree
     */
    virtual size_t elements() const
    {
        return m_nodes.size();
    }

    /**
     * \brief Search for elements in 'box' in the Rtree. Add them to 'result'.
     *        If exact_match is true only return the elements having as
     *        key the box 'box'. Otherwise return everything inside 'box'.
     */
    // awulkiew - exact match case removed
    virtual void find(Box const& box, std::deque<Value>& result, Translator const& tr)
    {
        for (typename node_map::const_iterator it = m_nodes.begin();
             it != m_nodes.end(); ++it)
        {
            // awulkiew - is_overlapping changed to geometry::intersects
            if (geometry::intersects(it->first, box))
            {
                it->second->find(box, result, tr);
            }
        }
    }

    /**
    * \brief Compute bounding box for this node
    */
    virtual Box compute_box(Translator const&) const
    {
        if (m_nodes.empty())
        {
            return Box();
        }

        Box result;
        geometry::assign_inverse(result);
        for(typename node_map::const_iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
        {
            geometry::combine(result, it->first);
        }

        return result;
    }

    /**
    * \brief Get leaves for a node
    */
    virtual std::vector<Value> get_leaves() const
    {
        typedef std::vector<Value> leaf_type;
        leaf_type leaf;

        for (typename node_map::const_iterator it = m_nodes.begin();
             it != m_nodes.end(); ++it)
        {
            leaf_type this_leaves = it->second->get_leaves();

            for (typename leaf_type::iterator it_leaf = this_leaves.begin();
                it_leaf != this_leaves.end(); ++it_leaf)
            {
                leaf.push_back(*it_leaf);
            }
        }

        return leaf;
    }

    /**
     * \brief Remove elements inside the 'box'
     */
    // awulkiew - name changed to avoid ambiguity (Value may be of type Box)
    virtual void remove_in(Box const& box, Translator const&)
    {
        for (typename node_map::iterator it = m_nodes.begin();
             it != m_nodes.end(); ++it)
        {
            if (geometry::equals(it->first, box))
            {
                m_nodes.erase(it);
                return;
            }
        }
    }

    /**
     * \brief Get the envelopes of a node
     */
    virtual std::vector<Box> get_boxes(Translator const&) const
    {
        std::vector<Box> result;
        for(typename node_map::const_iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
        {
            result.push_back(it->first);
        }
        return result;
    }

    /**
     * \brief Print Rtree subtree (mainly for debug)
     */
    virtual void print(Translator const& tr) const
    {
        std::cerr << " --> Node --------" << std::endl;
        std::cerr << "  Address: " << this << std::endl;
        std::cerr << "  Level: " << this->get_level() << std::endl;
        std::cerr << "  Size: " << m_nodes.size() << std::endl;
        std::cerr << "  | ";
        for(typename node_map::const_iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
        {
            if (this != it->second->get_parent().get())
            {
                std::cerr << "ERROR - " << this << " is not  " << it->second->get_parent().get() << " ";
            }

            std::cerr << "( " << geometry::get<min_corner, 0>(it->first) << " , "
                << geometry::get<min_corner, 1>(it->first) << " ) x ";
            std::cerr << "( " << geometry::get<max_corner, 0>(it->first) << " , "
                << geometry::get<max_corner, 1>(it->first) << " )";
            std::cerr << " | ";
        }
        std::cerr << std::endl;
        std::cerr << " --< Node --------" << std::endl;

        // print child nodes
        std::cerr << " Children: " << std::endl;
        for (typename node_map::const_iterator it = m_nodes.begin();
             it != m_nodes.end(); ++it)
        {
            it->second->print(tr);
        }
    }

    // awulkiew - internal node only virtual methods

    /**
     * \brief Add a child to this node
     */
    virtual void add_node(Box const& box, node_pointer const& node)
    {
        m_nodes.push_back(std::make_pair(box, node));
        node->update_parent(node);
    }

    /**
     * \brief Project first element, to replace root in case of condensing
     */
    virtual node_pointer first_element() const
    {
        if (0 == m_nodes.size())
        {
            // TODO: mloskot - define & use GGL exception
            throw std::logic_error("first_element in empty node");
        }
        return m_nodes.begin()->second;
    }

    /**
     * \brief Proyector for the 'i' node
     */
    virtual node_pointer get_node(size_t index)
    {
        return m_nodes[index].second;
    }

    /**
     * \brief Return in 'result' all the leaves inside 'box'
     */
    virtual void find_leaves(Box const& box, typename std::vector<node_pointer>& result) const
    {
        for (typename node_map::const_iterator it = m_nodes.begin();
             it != m_nodes.end(); ++it)
        {
            // awulkiew - is_overlapping changed to geometry::intersects
            if (geometry::intersects(it->first, box))
            {
                if (it->second->is_leaf())
                {
                    result.push_back(it->second);
                }
                else
                {
                    it->second->find_leaves(box, result);
                }
            }
        }
    }

    /**
     * \brief Recompute the bounding box
     */
    virtual void adjust_box(node_pointer const& node, Translator const& tr)
    {
        unsigned int index = 0;
        for (typename node_map::iterator it = m_nodes.begin();
             it != m_nodes.end(); ++it, index++)
        {
            if (it->second.get() == node.get())
            {
                m_nodes[index] = std::make_pair(node->compute_box(tr), node);
                return;
            }
        }
    }

    /**
     * \brief Replace the node in the m_nodes vector and recompute the box
     */
    virtual void replace_node(node_pointer const& leaf, node_pointer& new_leaf, Translator const& tr)
    {
        unsigned int index = 0;
        for(typename node_map::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it, index++)
        {
            if (it->second.get() == leaf.get())
            {
                m_nodes[index] = std::make_pair(new_leaf->compute_box(tr), new_leaf);
                new_leaf->update_parent(new_leaf);
                return;
            }
        }

        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("Node not found.");
    }

    /**
     * \brief Add a child leaf to this node
     */
    virtual void add_leaf_node(Box const& box, leaf_pointer const& leaf)
    {
        m_nodes.push_back(std::make_pair(box, leaf));
    }

    /**
     * \brief Choose a node suitable for adding 'box'
     */
    virtual node_pointer choose_node(Box const& box)
    {
        if (m_nodes.size() == 0)
        {
            // TODO: mloskot - define & use GGL exception
            throw std::logic_error("Empty node trying to choose the least enlargement node.");
        }

        // awulkiew - areas types changed
        typedef typename area_result<Box>::type area_type;

        bool first = true;
        area_type min_area = 0;
        area_type min_diff_area = 0;
        node_pointer chosen_node;

        // check for the least enlargement
        for (typename node_map::const_iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
        {
            // awulkiew - areas types changed
            area_type const diff_area = compute_union_area(box, it->first) - geometry::area(it->first);

            if (first)
            {
                // it's the first time, we keep the first
                min_diff_area = diff_area;
                min_area = geometry::area(it->first);
                chosen_node = it->second;

                first = false;
            }
            else
            {
                if (diff_area < min_diff_area)
                {
                    min_diff_area = diff_area;
                    min_area = geometry::area(it->first);
                    chosen_node = it->second;
                }
                else
                {
                    if (diff_area == min_diff_area)
                    {
                        if (geometry::area(it->first) < min_area)
                        {
                            min_diff_area = diff_area;
                            min_area = geometry::area(it->first);
                            chosen_node = it->second;
                        }
                    }
                }
            }
        }

        return chosen_node;
    }

    /**
     * \brief Update the parent of all the childs
     */
    virtual void update_parent(node_pointer const& node)
    {
        for (typename node_map::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
        {
            it->second->set_parent(node);
        }
    }

    /**
     * \brief Box projector for node pointed by 'leaf'
     */
    virtual Box get_box(node_pointer const& leaf) const
    {
        for (typename node_map::const_iterator it = m_nodes.begin();
             it != m_nodes.end(); ++it)
        {
            if (it->second.get() == leaf.get())
            {
                return it->first;
            }
        }

        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("Node not found");
    }

    /**
     * \brief Children projector
     */
    virtual node_map get_nodes() const
    {
        return m_nodes;
    }

    // awulkiew - unclassified methods

    /**
     * \brief Box projector for node 'index'
     */
    // awulkiew - commented, not used
    /*virtual Box get_box(unsigned int index, Translator const& tr) const
    {
        return m_nodes[index].first;
    }*/

private:

    /// child nodes
    node_map m_nodes;
};

}}} // namespace boost::geometry::index

#endif // BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_RTREE_INTERNAL_NODE_HPP
