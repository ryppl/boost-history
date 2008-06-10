// Copyright 2008 Federico J. Fernandez.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_SPATIAL_INDEX_RTREE_HPP
#define BOOST_SPATIAL_INDEX_RTREE_HPP

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
    rtree_node(const unsigned int &level, const unsigned int &m, const unsigned int &M) 
      : level_(level), m_(m), M_(M) {}

    /// true if it is a leaf node
    virtual bool is_leaf(void) { return false; }

    void add_leaf_node(const geometry::box<Point> &b, const boost::shared_ptr<rtree_leaf<Point, Value> > &l)
    {
      
      if(nodes_.size() < M_) {
	nodes_[b] = l;
      } else {
	// split
      }

    }

    virtual ~rtree_node(void) {}




    // level of this node
    unsigned int level_;

    // minimum number of elements per node
    unsigned int m_;
    // maximum number of elements per node
    unsigned int M_;

    std::map<geometry::box<Point>, boost::shared_ptr<rtree_node> > nodes_;

  };


  template<typename Point, typename Value>
  class rtree_leaf : public rtree_node<Point, Value>
  {
  public:
    rtree_leaf(void) : L_(8), level_(0) {}
    rtree_leaf(const unsigned int &L) : L_(L), level_(0) {}

    /// yes, we are a leaf
    virtual bool is_leaf(void) { return true; }

  private:

    // maximum number of elements per leaf
    unsigned int L_;

    // level of this node
    unsigned int level_;


    std::map<geometry::box<Point>, Value > nodes_;
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
	root_(new rtree_node<Point, Value>(1, m, M))
    {
      boost::shared_ptr< rtree_leaf<Point, Value> > new_leaf(new rtree_leaf<Point, Value>(L));
      root_->add_leaf_node(initial_box, new_leaf);
    }
      
    virtual void insert(const Point &k, const Value &v)
    {
      element_count++;
    }

    // TODO: do private


    void insert(const geometry::box<Point> &e, const Value &v)
    {
      boost::shared_ptr<rtree_node<Point, Value> > l = choose_leaf(e);

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

    boost::shared_ptr<rtree_node<Point, Value> > choose_leaf(const geometry::box<Point> e)
    {
      boost::shared_ptr< rtree_node<Point, Value> > N = root_;

      std::cerr << "Choosing." << std::endl;

//       while() {
	if(N->is_leaf()) {
	  return N;
	} else {
	  /// traverse N's map to see which node we should select
	  std::cerr << "Not a leaf." << std::endl;

	  
	}
//       }
	// FIXME
	return root_;
    
    }


  };


} // namespace spatial_index
} // namespace boost

#endif // BOOST_SPATIAL_INDEX_RTREE_HPP

