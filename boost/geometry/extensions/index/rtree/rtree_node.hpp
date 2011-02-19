// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Boost.SpatialIndex - rtree node, base class to leafs and internal nodes
//
// Copyright 2011 Adam Wulkiewicz.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_RTREE_NODE_HPP
#define BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_RTREE_NODE_HPP

namespace boost { namespace geometry { namespace index {

template <typename Value, typename Translator, typename Box>
class rtree_leaf;

template <typename Value, typename Translator, typename Box>
class rtree_internal_node;

template <typename Value, typename Translator, typename Box>
class rtree_node
{
public:

    typedef boost::shared_ptr<rtree_node<Value, Translator, Box> > node_pointer;
    typedef boost::shared_ptr<rtree_leaf<Value, Translator, Box> > leaf_pointer;

    typedef std::vector<std::pair<Box, node_pointer > > node_map;

    /**
     * \brief destructor (virtual because we have virtual functions)
     */
    virtual ~rtree_node()
    {
    }

    // awulkiew - node methods

    /**
     * \brief Projector for parent
     */
    inline node_pointer get_parent() const
    {
        return m_parent;
    }

    /**
     * \brief Set parent
     */
    void set_parent(node_pointer const& node)
    {
        m_parent = node;
    }

    /**
     * \brief Level projector
     */
    size_t get_level() const
    {
        return m_level;
    }

    // awulkiew - internal node and leaf virtual methods

    /**
     * \brief True if it is a leaf node
     */
    virtual bool is_leaf() const
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("shouldn't be here");
        return false;
    }

    /**
     * \brief Number of elements in the subtree
     */
    virtual size_t elements() const
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("shouldn't be here");
        return 0;
    }

    /**
     * \brief Search for elements in 'box' in the Rtree. Add them to 'result'.
     */
    virtual void find(Box const& box, std::deque<Value>& result, Translator const& tr)
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("shouldn't be here");
    }

    /**
    * \brief Compute bounding box for this node
    */
    virtual Box compute_box(Translator const&) const
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("shouldn't be here");

        return Box();
    }

    /**
    * \brief Get leaves for a node
    */
    virtual std::vector<Value> get_leaves() const
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("shouldn't be here");

        return std::vector<Value>();
    }

    /**
     * \brief Remove elements inside the 'box'
     */
    // awulkiew - name changed to avoid ambiguity (Value may be of type Box)
    virtual void remove_in(Box const& box, Translator const&)
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("shouldn't be here");
    }

    /**
     * \brief Get the envelopes of a node
     */
    virtual std::vector<Box> get_boxes(Translator const&) const
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("shouldn't be here");

        return std::vector<Box>();
    }

    /**
     * \brief Print Rtree subtree (mainly for debug)
     */
    virtual void print(Translator const& tr) const
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("shouldn't be here");
    }

    // awulkiew - leaf only virtual methods

    /**
     * \brief Insert a value (not allowed for a node, only on leaves)
     */
    virtual void insert(Value const&)
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("Insert in node!");
    }

    /**
     * \brief add a value (not allowed in nodes, only on leaves)
     */
    virtual void add_value(Value const&)
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("Can't add value to non-leaf node.");
    }

    /**
     * \brief Value projector for leaf_node (not allowed for non-leaf nodes)
     */
    virtual Value get_value(unsigned int) const
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("No values in a non-leaf node.");
    }

    /**
     * \brief Remove value in this leaf
     */
    virtual void remove(Value const&, Translator const&)
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("Can't remove a non-leaf node by value.");
    }

    // awulkiew - internal node only virtual methods

    /**
     * \brief Add a new child (node) to this node
     */
    virtual void add_node(Box const&, node_pointer const&)
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("Can't add node to leaf node.");
    }

    virtual node_pointer first_element() const
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("shouldn't be here.");
        return node_pointer();
    }

    /**
     * \brief Proyector for the 'i' node
     */
    virtual node_pointer get_node(size_t index)
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("shouldn't be here.");
        return node_pointer();
    }

    /**
     * \brief Return in 'result' all the leaves inside 'box'
     */
    virtual void find_leaves(Box const& box, typename std::vector<node_pointer>& result) const
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("shouldn't be here.");
    }

    /**
     * \brief Recompute the bounding box
     */
    virtual void adjust_box(node_pointer const& node, Translator const& tr)
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("shouldn't be here.");
    }

    /**
     * \brief Replace the node in the m_nodes vector and recompute the box
     */
    virtual void replace_node(node_pointer const& leaf, node_pointer& new_leaf, Translator const& tr)
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("shouldn't be here.");
    }

    /**
     * \brief Add a child leaf to this node
     */
    virtual void add_leaf_node(Box const& box, leaf_pointer const& leaf)
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("shouldn't be here.");
    }

    /**
     * \brief Choose a node suitable for adding 'box'
     */
    virtual node_pointer choose_node(Box const& box)
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("shouldn't be here.");
    }

    /**
     * \brief Update the parent of all the childs
     */
    virtual void update_parent(node_pointer const& node)
    {
        // In case of leaf do nothing
    }

    /**
     * \brief Box projector for node pointed by 'leaf'
     */
    virtual Box get_box(node_pointer const& leaf) const
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("shouldn't be here.");
        return Box();
    }

    /**
     * \brief Children projector
     */
    virtual node_map get_nodes() const
    {
        // TODO: mloskot - define & use GGL exception
        throw std::logic_error("shouldn't be here.");
        return node_map();
    }

protected:

    /**
     * \brief Creates a default node
     */
    rtree_node()
    {
    }

    /**
     * \brief Creates a node with 'parent' as parent and 'level' as its level
     */
    rtree_node(node_pointer const& parent, size_t const& level)
        : m_parent(parent), m_level(level)
    {
    }

private:

    /// parent node
    node_pointer m_parent;

    /// level of this node
    // TODO: mloskot - Why not std::size_t or node_map::size_type, same with member functions?
    // awulkiew - size_t used instead of unsigned int
    size_t m_level;
};

}}} // namespace boost::geometry::index

#endif // BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_RTREE_NODE_HPP
