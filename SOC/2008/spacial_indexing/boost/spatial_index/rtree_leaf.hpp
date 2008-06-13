// Copyright 2008 Federico J. Fernandez.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//
//	Spatial Index - rTree Leaf
//


#ifndef BOOST_SPATIAL_INDEX_RTREE_LEAF_HPP
#define BOOST_SPATIAL_INDEX_RTREE_LEAF_HPP

#include <geometry/area.hpp>

namespace boost {
namespace spatial_index {

  template<typename Point, typename Value>
  class rtree_leaf : public rtree_node<Point, Value>
  {
  public:
    rtree_leaf(void) : L_(8), level_(0) {}
    rtree_leaf(const boost::shared_ptr< rtree_node<Point,Value> > &parent, const unsigned int &L) 
      : rtree_node<Point,Value>(parent, 0, 0, 0), L_(L), level_(0) {}

    /// yes, we are a leaf
    virtual bool is_leaf(void) const { return true; }

    virtual bool is_full(void) const { return nodes_.size() >= L_; }

    virtual void insert(const geometry::box<Point> &e, const Value &v) 
    {
      nodes_.push_back(std::make_pair(e, v));
//       std::cerr << "Node size: " << nodes_.size() << std::endl;
    }

    virtual void add_node(const geometry::box<Point> &b, const boost::shared_ptr<rtree_node<Point, Value> > &n)
    {
      throw std::logic_error("Can't add node to leaf node."); 
    }

    virtual void add_value(const geometry::box<Point> &b, const Value &v)
    {
      nodes_.push_back(std::make_pair(b, v));
    }

    virtual void empty_nodes(void) {
      nodes_.clear();
    }

    virtual unsigned int get_capacity(void) const
    {
      return L_;
    }

    virtual Value get_value(const unsigned int i) const { return nodes_[i].second; }

    virtual std::vector< geometry::box<Point> > get_boxes(void) const
    {
      std::vector< geometry::box<Point> > r;
      for(typename leaves_map::const_iterator it = nodes_.begin(); it != nodes_.end(); ++it) {
	r.push_back(it->first);
      }
      return r;
    }

    virtual void print(void) const
    {
      std::cerr << " --> Leaf --------" << std::endl;
      std::cerr << "  Size / Capacity: " << nodes_.size() << " / " << L_ << std::endl;
      for(typename leaves_map::const_iterator it = nodes_.begin(); it != nodes_.end(); ++it) {
	std::cerr << "  | ";
	std::cerr << "( " << geometry::get<0>(it->first.min()) << " , " << geometry::get<1>(it->first.min()) << " ) x " ;
	std::cerr << "( " << geometry::get<0>(it->first.max()) << " , " << geometry::get<1>(it->first.max()) << " )" ;
	std::cerr << " -> ";
	std::cerr << it->second;
	std::cerr << " | " << std::endl;;


      }
      std::cerr << std::endl;
      std::cerr << " --< Leaf --------" << std::endl;
    }

  private:

    // maximum number of elements per leaf
    unsigned int L_;

    // level of this node
    unsigned int level_;

    typedef std::vector< std::pair< geometry::box<Point>, Value > > leaves_map;
    leaves_map nodes_;
  };



} // namespace spatial_index
} // namespace boost

#endif // BOOST_SPATIAL_INDEX_RTREE_LEAF_HPP
