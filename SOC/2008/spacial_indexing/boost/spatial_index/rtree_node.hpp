//
//	Spatial Index - rTree Node
//
//
// Copyright 2008 Federico J. Fernandez.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//


#ifndef BOOST_SPATIAL_INDEX_RTREE_NODE_HPP
#define BOOST_SPATIAL_INDEX_RTREE_NODE_HPP

#include <geometry/area.hpp>

namespace boost {
namespace spatial_index {

  template<typename Point, typename Value>
  class rtree_leaf;

  template<typename Point, typename Value>
  class rtree_node
  {
  public:
    /// type for the node map
    typedef std::vector< std::pair<geometry::box<Point>, boost::shared_ptr<rtree_node<Point, Value> > > > node_map;

  public:
    /// default constructor (needed for the containers)
    rtree_node(void) : root_(false) {}

    /// normal constructor
    rtree_node(const boost::shared_ptr<rtree_node<Point, Value> > &parent, const unsigned int &level) 
      : parent_(parent), level_(level), root_(false) {}

    /// level projector
    virtual unsigned int get_level(void) const { return level_; }

    /// element count
    virtual unsigned int elements(void) const
    {
      return nodes_.size();
    }

    /// first element, to replace root in case of condensing
    boost::shared_ptr<rtree_node<Point, Value> > first_element(void) const
    {
      if(nodes_.size() == 0) {
	throw std::logic_error("first_element in empty node");
      }
      return nodes_.begin()->second;
    }

    /// true if it is a leaf node
    virtual bool is_leaf(void) const { return false; }

    /// true if it is the root
    bool is_root(void) const { return root_; }

    /// define this node as the root
    void set_root(void) { root_ = true; }

    /// get a node
    boost::shared_ptr< rtree_node<Point, Value> > get_node(const unsigned int i) { return nodes_[i].second; }

    /// query method
    virtual void  find(const geometry::box<Point> &e, std::deque<Value> &r, const bool exact_match)
    {
      for(typename node_map::const_iterator it = nodes_.begin(); it != nodes_.end(); ++it) {
	if(overlaps(it->first, e)) {
	  it->second->find(e, r, exact_match);
	}
      }
    }


    /// get leaves
    virtual std::vector< std::pair< geometry::box<Point>, Value > > get_leaves(void) const
    {
      throw std::logic_error("No leaves in a non-leaf node.");
    }

    /// compute bounding box for this leaf
    virtual geometry::box<Point> compute_box(void) const
    {
      if(nodes_.empty()) {
	throw std::logic_error("Compute box in an empty node.");
      }
      
      typename node_map::const_iterator it = nodes_.begin();
      geometry::box<Point> r = it->first;
      it++;
      for(; it != nodes_.end(); ++it) {
	r = enlarge_box(r, it->first);
      }
      return r;
    }

    /// insert a value (not allowed for a node)
    virtual void insert(const geometry::box<Point> &e, const Value &v) 
    {
      std::cerr << "Insert in node!" << std::endl;
    }

    /// get the envelopes of a node
    virtual std::vector< geometry::box<Point> > get_boxes(void) const
    {
      std::vector< geometry::box<Point> > r;
      for(typename node_map::const_iterator it = nodes_.begin(); it != nodes_.end(); ++it) {
	r.push_back(it->first);
      }
      return r;
    }

    /// recompute the box
    void adjust_box(const boost::shared_ptr<rtree_node<Point, Value> > &n)
    {
      unsigned int index = 0;
      for(typename node_map::iterator it = nodes_.begin(); it != nodes_.end(); ++it, index++) {
	if(it->second.get() == n.get()) {
 	  std::cerr << "---------------------------------------->" << std::endl;
  	  std::cerr << "Node found!" << std::endl;
 	  std::cerr << "Box: " << geometry::get<0>(n->compute_box().min()) << " , " <<geometry::get<1>(n->compute_box().min()) << std::endl;
 	  std::cerr << "Box: " << geometry::get<0>(n->compute_box().max()) << " , " <<geometry::get<1>(n->compute_box().max()) << std::endl;
	  if(it->second->get_parent().get() != n->get_parent().get()) {
	    std::cerr << "ERR" << std::endl;
	  }
	  nodes_[index] = std::make_pair(n->compute_box(), n);
 	  std::cerr << "---------------------------------------->" << std::endl;
	  return;
	}
      }
//       std::cerr << "adjust_node: node not found." << std::endl;
    }

    virtual void remove(const geometry::box<Point> &k)
    {
      for(typename node_map::iterator it = nodes_.begin(); it != nodes_.end(); ++it) {
	if(it->first.min() == k.min() && it->first.max() == k.max()) {
	  std::cerr << "Erasing node." << std::endl;
	  nodes_.erase(it);
	  return;
	}
      }
    }

    /// replace the node in the nodes_ vector and recompute the box
    void replace_node(const boost::shared_ptr<rtree_node<Point, Value> > &l, boost::shared_ptr<rtree_node<Point, Value> > &new_l)
    {
      unsigned int index = 0;
      for(typename node_map::iterator it = nodes_.begin(); it != nodes_.end(); ++it, index++) {
	if(it->second.get() == l.get()) {
// 	  std::cerr << "Node found!" << std::endl;
	  nodes_[index] = std::make_pair(new_l->compute_box(), new_l);
	  new_l->update_parent(new_l);
	  return;
	}
      }
      std::cerr << "replace_node: node not found." << std::endl;
    }

    /// add a node
    virtual void add_node(const geometry::box<Point> &b, const boost::shared_ptr<rtree_node<Point, Value> > &n)
    {
      nodes_.push_back(std::make_pair(b, n));
      n->update_parent(n);
    }

    /// add a value (not allowed in nodes)
    virtual void add_value(const geometry::box<Point> &b, const Value &v)
    {
      throw std::logic_error("Can't add value to non-leaf node."); 
    }

    /// 
    void add_leaf_node(const geometry::box<Point> &b, const boost::shared_ptr<rtree_leaf<Point, Value> > &l)
    {
      nodes_.push_back(std::make_pair(b, l));
    }

    /// insertion algorithm choose node
    boost::shared_ptr<rtree_node<Point, Value> > choose_node(const geometry::box<Point> e)
    {
//       std::cerr << "Choose node" << std::endl;

      if(nodes_.size() == 0) {
	throw std::logic_error("Empty node trying to choose the least enlargment node.");
      }
      bool first = true;
      double min_area;
      double min_diff_area;
      boost::shared_ptr<rtree_node<Point,Value> > chosen_node;

      for(typename node_map::const_iterator it = nodes_.begin(); it != nodes_.end(); ++it) {

	double diff_area = compute_union_area(e, it->first) - geometry::area(it->first);

	if(first) {
	  // it's the first time, we keep the first
	  min_diff_area = diff_area;
	  min_area = geometry::area(it->first);
	  chosen_node = it->second;

	  first = false;
	} else {
	  if(diff_area < min_diff_area) {
	    min_diff_area = diff_area;
	    min_area = geometry::area(it->first);
	    chosen_node = it->second;	    
	  } else {
	    if(diff_area == min_diff_area) {
	      if(geometry::area(it->first) < min_area) {
		min_diff_area = diff_area;
		min_area = geometry::area(it->first);
		chosen_node = it->second;	    
	      }
	    }
	  }

	}
      }
//       std::cerr << "We have a node." << std::endl;
      return chosen_node;
    }

    /// empty the node
    virtual void empty_nodes(void) {
      nodes_.clear();
    }

    /// projector for parent
    boost::shared_ptr<rtree_node<Point,Value> > get_parent(void) const {
      return parent_;
    }

    // update the parent of all the childs
    void update_parent(const boost::shared_ptr<rtree_node<Point,Value> > &p)
    {
      for(typename node_map::iterator it = nodes_.begin(); it != nodes_.end(); ++it) {
	it->second->set_parent(p);
      }
    }

    // setter for parent
    void set_parent(const boost::shared_ptr<rtree_node<Point,Value> > &p) 
    {
       parent_ = p;
    }

    /// value projector for leaf_node (not allowed here)
    virtual Value get_value(const unsigned int i) const { throw std::logic_error("No values in a non-leaf node."); }

    /// box projector for node
    virtual geometry::box<Point> get_box(const unsigned int i) const { return nodes_[i].first; }

    /// box projector for node
    virtual geometry::box<Point> get_box(const boost::shared_ptr<rtree_node<Point, Value> > &l) const
    {
      for(typename node_map::const_iterator it = nodes_.begin(); it != nodes_.end(); ++it) {
	if(it->second.get() == l.get()) {
	  return it->first;
	}
      }
      throw std::logic_error("Node not found");
    }


    /// value projector for the nodes
    node_map get_nodes(void) const { return nodes_; }

    /// print node
    virtual void print(void) const
    {
      std::cerr << " --> Node --------" << std::endl;
      std::cerr << "  Address: " << this << std::endl;
      std::cerr << "  Is Root: " << is_root() << std::endl;
      std::cerr << "  Level: " << level_ << std::endl;
      std::cerr << "  Size: " << nodes_.size() << std::endl;
      std::cerr << "  | ";
      for(typename node_map::const_iterator it = nodes_.begin(); it != nodes_.end(); ++it) {

	if(it->second->get_parent().get() != this) {
	  std::cerr << "ERROR - " << this  << " is not  " <<it->second->get_parent().get() << " ";
	}

	std::cerr << "( " << geometry::get<0>(it->first.min()) << " , " << geometry::get<1>(it->first.min()) << " ) x " ;
	std::cerr << "( " << geometry::get<0>(it->first.max()) << " , " << geometry::get<1>(it->first.max()) << " )" ;
	std::cerr << " | ";
      }
      std::cerr << std::endl;
      std::cerr << " --< Node --------" << std::endl;

      // print child nodes
      std::cerr << " Children: " << std::endl;
      for(typename node_map::const_iterator it = nodes_.begin(); it != nodes_.end(); ++it) {
	it->second->print();
      }
    }

    /// destructor (virtual because we have virtual functions)
    virtual ~rtree_node(void) {}

  private:


    // parent node
    boost::shared_ptr< rtree_node<Point, Value> > parent_;

    // level of this node
    unsigned int level_;

    // true if it is the root
    bool root_;

    /// child nodes
    node_map nodes_;

  };



} // namespace spatial_index
} // namespace boost

#endif // BOOST_SPATIAL_INDEX_RTREE_NODE_HPP

