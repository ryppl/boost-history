// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Boost.SpatialIndex - rtree leaf implementation
//
// Copyright 2008 Federico J. Fernandez.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_RTREE_LEAF_HPP
#define BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_RTREE_LEAF_HPP

#include <deque>
#include <iostream> // TODO: Remove if print() is removed
#include <stdexcept>
#include <utility>
#include <vector>

#include <boost/shared_ptr.hpp>

#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry/algorithms/assign.hpp>
#include <boost/geometry/algorithms/combine.hpp>
#include <boost/geometry/algorithms/intersects.hpp>

#include <boost/geometry/extensions/index/rtree/helpers.hpp>
#include <boost/geometry/extensions/index/rtree/rtree_node.hpp>

namespace boost { namespace geometry { namespace index
{

template <typename Value, typename Translator, typename Box>
class rtree_leaf : public rtree_node<Value, Translator, Box>
{
public:

    // awulkiew - typedef added
    typedef rtree_node<Value, Translator, Box> rtree_node;

    typedef boost::shared_ptr<rtree_node> node_pointer;

    /// container type for the leaves
    typedef std::vector<Value> leaf_map;

    /**
     * \brief Creates an empty leaf
     */
    inline rtree_leaf()
    {
    }

    /**
     * \brief Creates a new leaf, with 'parent' as parent
     */
    inline rtree_leaf(node_pointer const& parent)
        : rtree_node(parent, 0)
    {
    }

    // awulkiew - leaf methods

    /**
     * \brief Clear the node
     */
    // awulkiew - name changed from empty_nodes to clear_values
    void clear_values()
    {
        m_nodes.clear();
    }

    // awulkiew - internal node and leaf virtual methods

    /**
     * \brief True if we are a leaf
     */
    virtual bool is_leaf() const
    {
        return true;
    }

    /**
     * \brief Number of elements in the tree
     */
    virtual size_t elements() const
    {
        return m_nodes.size();
    }

    /**
     * \brief Search for elements in 'box' in the Rtree. Add them to 'result'.
     *        If exact_match is true only return the elements having as
     *        key the 'box'. Otherwise return everything inside 'box'.
     */
    // awulkiew - exact match case removed
    virtual void find(Box const& box, std::deque<Value>& result, Translator const& tr)
    {
        for (typename leaf_map::const_iterator it = m_nodes.begin();
             it != m_nodes.end(); ++it)
        {
            // awulkiew - commented
            //if (exact_match)
            //{
            //    if (geometry::equals(it->first, box))
            //    {
            //        result.push_back(it->second);
            //    }
            //}
            //else
            //{

            // awulkiew - is_overlapping changed to geometry::intersects
            if (geometry::intersects(tr(*it), box))
            {
                result.push_back(*it);
            }

            //}
        }
    }

    /**
     * \brief Compute bounding box for this leaf
     */
    virtual Box compute_box(Translator const& tr) const
    {
        if (m_nodes.empty())
        {
            return Box ();
        }

        Box r;
        geometry::assign_inverse(r);
        for(typename leaf_map::const_iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
        {
            geometry::combine(r, tr(*it));
        }
        return r;
    }

    /**
     * \brief Proyect leaves of this node.
     */
    virtual std::vector<Value> get_leaves() const
    {
        return m_nodes;
    }

    /**
     * \brief Remove value with key 'box' in this leaf
     */
    // awulkiew - name changed to avoid ambiguity (Value may be of type Box)
    virtual void remove_in(Box const& box, Translator const& tr)
    {

        for (typename leaf_map::iterator it = m_nodes.begin();
             it != m_nodes.end(); ++it)
        {
            // TODO - awulkiew - implement object specific equals() function
            Box b;
            detail::convert_to_box(tr(*it), b);
            if (geometry::equals(b, box))
            {
                m_nodes.erase(it);
                return;
            }
        }

        // TODO: mloskot - use GGL exception
        throw std::logic_error("Node not found.");
    }

    /**
    * \brief Proyect boxes from this node
    */
    virtual std::vector<Box> get_boxes(Translator const& tr) const
    {
        std::vector<Box> result;
        for (typename leaf_map::const_iterator it = m_nodes.begin();
             it != m_nodes.end(); ++it)
        {
            // TODO: awulkiew - implement object specific behaviour - get_bounding_objects(get boxes or points)
            Box box;
            detail::convert_to_box(tr(*it), box);

            result.push_back(box);
        }

        return result;
    }

    /**
    * \brief Print leaf (mainly for debug)
    */
    virtual void print(Translator const& tr) const
    {
        std::cerr << "\t" << " --> Leaf --------" << std::endl;
        std::cerr << "\t" << "  Size: " << m_nodes.size() << std::endl;
        for (typename leaf_map::const_iterator it = m_nodes.begin();
             it != m_nodes.end(); ++it)
        {
            // TODO: awulkiew - implement object specific behaviour - display boxes or points
            Box box;
            detail::convert_to_box(tr(*it), box);

            std::cerr << "\t" << "  | ";
            std::cerr << "( " << geometry::get<min_corner, 0>
                (box) << " , " << geometry::get<min_corner, 1>
                (box) << " ) x ";
            std::cerr << "( " << geometry::get<max_corner, 0>
                (box) << " , " << geometry::get<max_corner, 1>
                (box) << " )";
            // awulkiew - commented
            //std::cerr << " -> ";
            //std::cerr << it->second;
            std::cerr << " | " << std::endl;;
        }
        std::cerr << "\t" << " --< Leaf --------" << std::endl;
    }

    // awulkiew - leaf only virtual methods

    /**
     * \brief Insert a new element, with key 'box' and value 'v'
     */
    virtual void insert(Value const& v)
    {
        m_nodes.push_back(v);
    }

    /**
     * \brief Add a new leaf to this node
     */
    virtual void add_value(Value const& v)
    {
        m_nodes.push_back(v);
    }

    /**
     * \brief Proyect value in position 'index' in the nodes container
     */
    virtual Value get_value(unsigned int index) const
    {
        return m_nodes[index];
    }

    /**
     * \brief Remove value in this leaf
     */
    virtual void remove(Value const& v, Translator const& tr)
    {
        for (typename leaf_map::iterator it = m_nodes.begin();
             it != m_nodes.end(); ++it)
        {
            // awulkiew - use of translator
            if ( tr.equals(*it, v) )
            {
                m_nodes.erase(it);
                return;
            }
        }

        // TODO: mloskot - use GGL exception
        throw std::logic_error("Node not found.");
    }

    // awulkiew - unclassified methods

    /**
     * \brief Box projector for leaf
     */
    // awulkiew - commented, not used
    //virtual Box get_box(unsigned int index, Translator const& tr) const
    //{
    //    // TODO: awulkiew - get_bounding_object - add object specific behaviour
    //    // or just base on get_value
    //    Box box;
    //    detail::convert_to_box(tr(m_nodes[index]), box);
    //    return box;
    //}

private:

    /// leaves of this node
    leaf_map m_nodes;
};

}}} // namespace boost::geometry::index

#endif // BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_RTREE_LEAF_HPP

