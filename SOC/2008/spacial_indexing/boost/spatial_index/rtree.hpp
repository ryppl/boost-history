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

    virtual unsigned int get_capacity(void) const
    {
      return M_;
    }

    virtual std::vector< geometry::box<Point> > get_boxes(void) const
    {
      std::vector< geometry::box<Point> > r;
      for(typename node_map::const_iterator it = nodes_.begin(); it != nodes_.end(); ++it) {
	r.push_back(it->first);
      }
      return r;
    }

    void add_leaf_node(const geometry::box<Point> &b, const boost::shared_ptr<rtree_leaf<Point, Value> > &l)
    {
      
//       if(nodes_.size() < M_) {
	nodes_.push_back(std::make_pair(b, l));
//       } else {
// 	// split
// 	boost::shared_ptr< rtree_node<Point,Value> > n1, n2;
// 	split_node(l, n1, n2);
//       }

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

    virtual void empty_nodes(void) {
      nodes_.clear();
    }

    boost::shared_ptr<rtree_node<Point,Value> > get_parent(void) const {
      return parent_;
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

    virtual void empty_nodes(void) {
      nodes_.clear();
    }

    virtual unsigned int get_capacity(void) const
    {
      return L_;
    }

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

	l->insert(e, v);
	
	// split!
	boost::shared_ptr< rtree_node<Point, Value> > n1(new rtree_leaf<Point,Value>(l->get_parent(), l->get_capacity()));
	boost::shared_ptr< rtree_node<Point, Value> > n2(new rtree_leaf<Point,Value>(l->get_parent(), l->get_capacity()));

	split_node(l, n1, n2);
      } else {
	l->insert(e, v);
      }



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

    void split_node(const boost::shared_ptr<rtree_node<Point, Value> > &n, boost::shared_ptr<rtree_node<Point, Value> > &n1
		    , boost::shared_ptr<rtree_node<Point, Value> > &n2) const
    {
      std::cerr << "Split Node." << std::endl;

      boost::shared_ptr< rtree_node<Point,Value> > seed1, seed2;

      linear_pick_seeds(n, seed1, seed2);
    }


    void linear_pick_seeds(const boost::shared_ptr< rtree_node<Point,Value> > &n, 
			   boost::shared_ptr< rtree_node<Point,Value> > &seed1,
			   boost::shared_ptr< rtree_node<Point,Value> > &seed2) const
    {
      std::cerr << "Linear Pick Seeds." << std::endl;

      // get boxes from the node
      std::vector< geometry::box<Point> > boxes = n->get_boxes();
      if(boxes.size() == 0) {
	throw std::logic_error("Empty Node trying to Pick Seeds");
      }

      // only two dim for now
      // unsigned int dimensions = geometry::point_traits<Point>::coordinate_count;

      // find the first two elements
      double separation_x, separation_y;
      unsigned int first_x, second_x;
      unsigned int first_y, second_y;
      find_normalized_separations<0u>(boxes, separation_x, first_x, second_x);
      find_normalized_separations<1u>(boxes, separation_y, first_y, second_y);

      std::cout << "Separation X: " << separation_x << " within " << first_x << " and " << second_x << std::endl;
      std::cout << "Separation Y: " << separation_y << " within " << first_y << " and " << second_y << std::endl;
    }

    template<unsigned int Dimension>
    void find_normalized_separations(const std::vector< geometry::box<Point> > &boxes, double &separation, unsigned int &first, unsigned int &second) const
    {

      std::cout << "Boxes: " << boxes.size() << std::endl;

      // find the lowest high
      typename std::vector< geometry::box<Point> >::const_iterator it = boxes.begin();
      double lowest_high = geometry::get<Dimension>(it->max());
      unsigned int lowest_high_index = 0;
      unsigned int index;
      ++it;
      index = 1;
      for(; it != boxes.end(); ++it) {
	if(geometry::get<Dimension>(it->max()) < lowest_high) {
	  lowest_high = geometry::get<Dimension>(it->max());
	  lowest_high_index = index;
	}
	index++;
      }
      std::cerr << "Lowest High: " << lowest_high << " Index: " << lowest_high_index << std::endl;

      // find the lowest low
      it = boxes.begin();
      double lowest_low = geometry::get<Dimension>(it->min());
      ++it;
      for(; it != boxes.end(); ++it) {
	if(geometry::get<Dimension>(it->min()) < lowest_low) {
	  lowest_low = geometry::get<Dimension>(it->min());
	}
      }


      // find the highest high
      it = boxes.begin();
      double highest_high = geometry::get<Dimension>(it->max());
      ++it;
      for(; it != boxes.end(); ++it) {
	if(geometry::get<Dimension>(it->max()) > highest_high) {
	  highest_high = geometry::get<Dimension>(it->max());
	}
      }

      // find the highest low      
      it = boxes.begin();
      double highest_low = geometry::get<Dimension>(it->min());
      unsigned int highest_low_index = 0;
      ++it;
      index = 1;
      for(; it != boxes.end(); ++it) {
	if(geometry::get<Dimension>(it->min()) > highest_low) {
	  highest_low = geometry::get<Dimension>(it->min());
	  highest_low_index = index;
	}
	index++;
      }
      std::cerr << "Highest Low: " << highest_low << " Index: " << highest_low_index << std::endl;


      double width = highest_high - lowest_low;

      std::cerr << "HH: " << highest_high << " LL: " << lowest_low << std::endl;

      std::cerr << "Width: " << width << std::endl;

      separation = (highest_low  - lowest_high) / width;
      first = highest_low_index;
      second = lowest_high_index;
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

