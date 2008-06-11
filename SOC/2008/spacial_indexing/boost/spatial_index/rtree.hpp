// Copyright 2008 Federico J. Fernandez.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_SPATIAL_INDEX_RTREE_HPP
#define BOOST_SPATIAL_INDEX_RTREE_HPP

#include <geometry/area.hpp>

// #include "quadtree_node.hpp"

// #include <boost/thread/xtime.hpp>

namespace boost {
namespace spatial_index {

  template<typename Point, typename Value>
  class rtree_leaf;

  template<typename Point, typename Value>
  class rtree_node
  {
  public:
    rtree_node(void) : m_(4), M_(8) {}
    rtree_node(const boost::shared_ptr<rtree_node<Point, Value> > &parent, const unsigned int &level, const unsigned int &m, const unsigned int &M) 
      : parent_(parent), level_(level), m_(m), M_(M) {}

    virtual bool is_full(void) const { return nodes_.size() >= M_; }

    /// true if it is a leaf node
    virtual bool is_leaf(void) const { return false; }

    /// true if it is the root
    bool is_root(void) const { return root_; }
    void set_root(void) { root_ = true; }

    virtual void insert(const geometry::box<Point> &e, const Value &v) 
    {
      std::cerr << "Insert in node!" << std::endl;
    }


    void add_leaf_node(const geometry::box<Point> &b, const boost::shared_ptr<rtree_leaf<Point, Value> > &l)
    {
      
      if(nodes_.size() < M_) {
	nodes_.push_back(std::make_pair(b, l));
      } else {
	// split
      }

    }

    boost::shared_ptr<rtree_node<Point, Value> > choose_node(const geometry::box<Point> e)
    {
      std::cerr << "Choose node" << std::endl;

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
      std::cerr << "We have a node." << std::endl;
      return chosen_node;
    }

    virtual void print(void) const
    {
      std::cerr << " --> Node --------" << std::endl;
      std::cerr << "  Is Root: " << is_root() << std::endl;
      std::cerr << "  Level: " << level_ << std::endl;
      std::cerr << "  Size / Min / Max: " << nodes_.size() << " / " << m_ << " / " << M_ << std::endl;
      std::cerr << "  | ";
      for(typename node_map::const_iterator it = nodes_.begin(); it != nodes_.end(); ++it) {
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



    virtual ~rtree_node(void) {}

  private:

    // true if it is the root
    bool root_;

    // parent node
    boost::shared_ptr< rtree_node<Point, Value> > parent_;

    // level of this node
    unsigned int level_;

    // minimum number of elements per node
    unsigned int m_;
    // maximum number of elements per node
    unsigned int M_;

    typedef std::vector< std::pair<geometry::box<Point>, boost::shared_ptr<rtree_node> > > node_map;
    node_map nodes_;

  private:


    geometry::box<Point> enlarge_box(const geometry::box<Point> &b1, const geometry::box<Point> &b2) const
    {
      Point min(
		geometry::get<0>(b1.min()) < geometry::get<0>(b2.min()) ? geometry::get<0>(b1.min()) : geometry::get<0>(b2.min()),
		geometry::get<1>(b1.min()) < geometry::get<1>(b2.min()) ? geometry::get<1>(b1.min()) : geometry::get<1>(b2.min())
		);

      Point max(
		geometry::get<0>(b1.max()) > geometry::get<0>(b2.max()) ? geometry::get<0>(b1.max()) : geometry::get<0>(b2.max()),
		geometry::get<1>(b1.max()) > geometry::get<1>(b2.max()) ? geometry::get<1>(b1.max()) : geometry::get<1>(b2.max())
		);

      return geometry::box<Point>(min, max);
    }

    double compute_union_area(const geometry::box<Point> &b1, const geometry::box<Point> &b2) const
    {
      geometry::box<Point> enlarged_box = enlarge_box(b1, b2);
      return geometry::area(enlarged_box);
    }


  };


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

    virtual void print(void) const
    {
      std::cerr << " --> Leaf --------" << std::endl;
      std::cerr << "  Size / Capacity: " << nodes_.size() << " / " << L_ << std::endl;
      std::cerr << "  | ";
      for(typename leaves_map::const_iterator it = nodes_.begin(); it != nodes_.end(); ++it) {
	std::cerr << "( " << geometry::get<0>(it->first.min()) << " , " << geometry::get<1>(it->first.min()) << " ) x " ;
	std::cerr << "( " << geometry::get<0>(it->first.max()) << " , " << geometry::get<1>(it->first.max()) << " )" ;
	std::cerr << " -> ";
	std::cerr << it->second;
	std::cerr << " | ";


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


  template<typename Point, typename Value>
  class rtree : public spatial_index<Point, Value>
  {
  private:
    unsigned int element_count;

    // maximum number of elements per leaf
    unsigned int L_;
    // minimum number of elements per node
    unsigned int m_;
    // maximum number of elements per node
    unsigned int M_;

    boost::shared_ptr< rtree_node<Point,Value> > root_;

  public:
    rtree(const geometry::box<Point> &initial_box, const unsigned int &L, const unsigned int &m, const unsigned int &M) 
      : element_count(0), L_(L), m_(m), M_(M),
	root_(new rtree_node<Point, Value>(boost::shared_ptr< rtree_node<Point,Value> >(), 1, m, M))
    {
      root_->set_root();
      boost::shared_ptr< rtree_leaf<Point, Value> > new_leaf(new rtree_leaf<Point, Value>(root_, L));
      root_->add_leaf_node(initial_box, new_leaf);
    }

    virtual void insert(const Point &k, const Value &v)
    {
      element_count++;
    }

    virtual void print(void) const
    {
      root_->print();
    }

    void insert(const geometry::box<Point> &e, const Value &v)
    {
      boost::shared_ptr<rtree_node<Point, Value> > l(choose_leaf(e));

      if(l->is_full()) {
	std::cerr << "Node full. Split." << std::endl;
	
	// split!
      }

      l->insert(e, v);

      adjust_tree(l);

      element_count++;
    }

    virtual void bulk_insert(Value &v_begin, Value &v_end, std::vector<Point> &v)
    {
    }

    virtual Value find(const Point &k)
    {
      return Value();
    }

    virtual std::deque<Value> find(const geometry::box<Point> &r)
    {
	std::deque<Value> result;
	return result;
    }

    virtual unsigned int elements(void) { return element_count; }
    

    virtual ~rtree() {}


  private:

    void adjust_tree(boost::shared_ptr<rtree_node<Point, Value> > &l)
    {
      boost::shared_ptr<rtree_node<Point,Value> > N = l;
      if(N->is_root()) {
	return;
      }

    }

    boost::shared_ptr<rtree_node<Point, Value> > choose_leaf(const geometry::box<Point> e)
    {
      boost::shared_ptr< rtree_node<Point, Value> > N = root_;

      std::cerr << "Choosing." << std::endl;

      while(!N->is_leaf()) {
	/// traverse N's map to see which node we should select
	std::cerr << "Not a leaf." << std::endl;
	N = N->choose_node(e);
      }
      return N;
    }


  };


} // namespace spatial_index
} // namespace boost

#endif // BOOST_SPATIAL_INDEX_RTREE_HPP

