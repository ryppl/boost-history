// Copyright 2008 Federico J. Fernandez.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_SPATIAL_INDEX_RTREE_HPP
#define BOOST_SPATIAL_INDEX_RTREE_HPP

#include <geometry/area.hpp>

#include "rtree_node.hpp"
#include "rtree_leaf.hpp"

namespace boost {
namespace spatial_index {

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

      unsigned int seed1, seed2;
      std::vector< geometry::box<Point> > boxes = n->get_boxes();

      linear_pick_seeds(n, seed1, seed2);

      if(n->is_leaf()) {
	n->get_value(seed1);
// 	n1->add_value(boxes[seed1], n->get_value(seed1));
	// n2->add_node(boxes[seed2], n->get_node(seed2));
      }
    }


    void linear_pick_seeds(const boost::shared_ptr< rtree_node<Point,Value> > &n, 
			   unsigned int &seed1,
			   unsigned int &seed2) const
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

      std::cout << "Separation X: " << separation_x << " between " << first_x << " and " << second_x << std::endl;
      std::cout << "Separation Y: " << separation_y << " between " << first_y << " and " << second_y << std::endl;

      if(separation_x > separation_y) {
	seed1 = first_x;
	seed2 = second_x;
      } else {
	seed1 = first_y;
	seed2 = second_y;
      }
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
