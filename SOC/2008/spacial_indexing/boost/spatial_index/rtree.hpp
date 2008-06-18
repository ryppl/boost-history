// Copyright 2008 Federico J. Fernandez.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_SPATIAL_INDEX_RTREE_HPP
#define BOOST_SPATIAL_INDEX_RTREE_HPP

#include <geometry/area.hpp>

#include "helpers.hpp"

#include "rtree_node.hpp"
#include "rtree_leaf.hpp"

namespace boost {
namespace spatial_index {

  template<typename Point, typename Value>
  class rtree : public spatial_index<Point, Value>
  {
  private:
    // numbers of elements in the tree
    unsigned int element_count;

    // minimum number of elements per node
    unsigned int m_;
    // maximum number of elements per node
    unsigned int M_;

    // tree root
    boost::shared_ptr< rtree_node<Point,Value> > root_;

  public:
    rtree(const geometry::box<Point> &initial_box, const unsigned int &M, const unsigned int &m)
      : element_count(0), m_(m), M_(M), root_(new rtree_node<Point, Value>(boost::shared_ptr< rtree_node<Point,Value> >(), 1))
    {
      root_->set_root();
      boost::shared_ptr< rtree_leaf<Point, Value> > new_leaf(new rtree_leaf<Point, Value>(root_));
      root_->add_leaf_node(initial_box, new_leaf);
    }

    /// remove the element with key 'k'
    virtual void remove(const geometry::box<Point> &k)
    {
      try {
	boost::shared_ptr<rtree_node<Point, Value> > l(choose_leaf(k));
	typename rtree_leaf<Point,Value>::leaves_map q_leaves;

	l->remove(k);

	if(l->elements() < m_) {
	  std::cerr << "Few elements in Node." << std::endl;

	  q_leaves = l->get_leaves();

	  // we remove the leaf_node in the parent node because now it's empty
	  l->get_parent()->remove(l->get_parent()->get_box(l));
	}

	condense_tree(l);

	// if the root has only one child, make it the root
	if(root_->elements() == 1) {
	  root_ = root_->first_element();
	  root_->set_root();
	}

	std::cerr << "Reinserting leaves " << q_leaves.size() << std::endl;

	for(typename rtree_leaf<Point,Value>::leaves_map::const_iterator it = q_leaves.begin(); it != q_leaves.end(); ++it) {
	  insert(it->first, it->second);
	}

	element_count--;
      } catch(std::logic_error &e) {
	// not found
	std::cerr << e.what() << std::endl;
	return;
      }
    }

    virtual void print(void) const
    {
      std::cerr << "===================================" << std::endl;
      std::cerr << " Min/Max: " << m_ << " / " << M_ << std::endl;
      root_->print();
      std::cerr << "===================================" << std::endl;
    }

    virtual void insert(const Point &k, const Value &v)
    {
      this->insert(geometry::box<Point>(k,k), v);
    }

    void insert(const geometry::box<Point> &e, const Value &v)
    {
      boost::shared_ptr<rtree_node<Point, Value> > l(choose_leaf(e));

      if(l->elements() >= M_) {
 	std::cerr << "Node full. Split." << std::endl;

	l->insert(e, v);
	
	// split!
	boost::shared_ptr< rtree_node<Point, Value> > n1(new rtree_leaf<Point,Value>(l->get_parent()));
	boost::shared_ptr< rtree_node<Point, Value> > n2(new rtree_leaf<Point,Value>(l->get_parent()));

	split_node(l, n1, n2);
//  	std::cerr << std::endl;
//  	std::cerr << std::endl;
//  	std::cerr << std::endl;
//   	std::cerr << "Node splited." << std::endl;

//   	std::cerr << "ORIG" << std::endl;
//   	l->print();
	
//   	std::cerr << "N1" << std::endl;
//    	n1->print();
//   	std::cerr << "N2" << std::endl;
//    	n2->print();

//  	std::cerr << "L parent." << std::endl;
//  	l->get_parent()->print();

	adjust_tree(l, n1, n2);
      } else {
// 	std::cerr << "Insert without split" << std::endl;
	l->insert(e, v);
	adjust_tree(l);
      }

      element_count++;
    }

    virtual void bulk_insert(std::vector<Value> &values,  std::vector<Point> &points)
    {
      typename std::vector<Point>::iterator it_point;
      typename std::vector<Value>::iterator it_value;
      it_point = points.begin();
      it_value = values.begin();
      while(it_value != values.end() && it_point != points.end()) {
	geometry::box<geometry::point_xy<double> > box(*it_point, *it_point);
	insert(box, *it_value);

	it_point++;
	it_value++;
      }
    }

    virtual Value find(const Point &k)
    {
      std::deque<Value> result;
      geometry::box<geometry::point_xy<double> > query_box(k, k);

      root_->find(query_box, result, true);
      if(result.size() >= 1) {
	return result[0];
      }
      return Value();
    }

    virtual std::deque<Value> find(const geometry::box<Point> &r)
    {
      std::deque<Value> result;
      root_->find(r, result, false);
      return result;
    }

    virtual unsigned int elements(void) { return element_count; }

    virtual ~rtree() {}

  private:

    void condense_tree(const boost::shared_ptr<rtree_node<Point,Value> > &l)
    {
      std::cerr << "Condensing tree." << std::endl;

      typename rtree_node<Point,Value>::node_map q_nodes;

      if(l->is_root()) {
	// if it's the root we are done
	return;
      }

      /// TODO: implement

      boost::shared_ptr<rtree_node<Point,Value> > parent = l->get_parent();
      parent->adjust_box(l);
      
      if(parent->elements() < m_) {
	std::cerr << "condense_node: underfull node" << std::endl;

	q_nodes = parent->get_nodes();

	if(parent->is_root()) {
	  std::cerr << "The underfull node is the root, returning." << std::endl;
	  return;
	} else {
	  // we remove the node in the parent node because now it should be re inserted
	  parent->get_parent()->remove(parent->get_parent()->get_box(parent));
	}
      }

      condense_tree(parent);
    }


    void adjust_tree(boost::shared_ptr<rtree_node<Point, Value> > &n)
    {
      if(n->is_root()) {
	// we finished the adjust
	std::cerr << "It's the root" << std::endl;
	return;
      }
      // as there are no splits just adjust the box of the parent and go on
      boost::shared_ptr<rtree_node<Point,Value> > parent = n->get_parent();
      parent->adjust_box(n);
//       parent->print();
      adjust_tree(parent);
    }

    void adjust_tree(boost::shared_ptr<rtree_node<Point, Value> > &l,
		     boost::shared_ptr<rtree_node<Point, Value> > &n1,
		     boost::shared_ptr<rtree_node<Point, Value> > &n2)
    {
      if(l->is_root()) {
 	std::cerr << "Root   ---------> split."<< std::endl;
	boost::shared_ptr< rtree_node<Point,Value> > new_root(new rtree_node<Point,Value>(boost::shared_ptr<rtree_node<Point,Value> >(), l->get_level()+1));
	new_root->set_root();
	new_root->add_node(n1->compute_box(), n1);
	new_root->add_node(n2->compute_box(), n2);

	n1->set_parent(new_root);
	n2->set_parent(new_root);

	n1->update_parent(n1);
	n2->update_parent(n2);

// 	std::cerr << "Adjust tree N1: " << std::endl;
// 	n1->get_parent()->print();
	root_ = new_root;
	return;
      }
      boost::shared_ptr<rtree_node<Point,Value> > parent = l->get_parent();
//       std::cerr << std::endl;
//       std::cerr << "1";
//       parent->print();
//       std::cerr << std::endl;
//       std::cerr << std::endl;

      parent->replace_node(l, n1);

//       std::cerr << "l parent" << l->get_parent().get() << std::endl;
//       std::cerr << "n1 parent" << n1->get_parent().get() << std::endl;
//       std::cerr << "l" << l.get() << std::endl;
//       std::cerr << "n1" << n1.get() << std::endl;
//       std::cerr << "n2" << n2.get() << std::endl;


//       std::cerr << std::endl;
//       std::cerr << "2";
//       parent->print();
//       std::cerr << std::endl;
//       std::cerr << std::endl;


      parent->add_node(n2->compute_box(), n2);

//       std::cerr << std::endl;
//       std::cerr << "3";
//       parent->print();
//       std::cerr << std::endl;
//       std::cerr << std::endl;


      if(parent->elements() > M_) {
//   	std::cerr << "parent is full" << std::endl;

	boost::shared_ptr< rtree_node<Point, Value> > p1(new rtree_node<Point,Value>(parent->get_parent(), parent->get_level()));
	boost::shared_ptr< rtree_node<Point, Value> > p2(new rtree_node<Point,Value>(parent->get_parent(), parent->get_level()));

	split_node(parent, p1, p2);

// 	std::cerr << "P1: " << std::endl;
// 	p1->print();
// 	std::cerr << "P2: " << std::endl;
// 	p2->print();

	adjust_tree(parent, p1, p2);
      } else {
// 	std::cerr << "parent is not full." << std::endl;
// 	parent->print();
	adjust_tree(parent);
      }
    }


    void split_node(const boost::shared_ptr<rtree_node<Point, Value> > &n, boost::shared_ptr<rtree_node<Point, Value> > &n1
		    , boost::shared_ptr<rtree_node<Point, Value> > &n2) const
    {
      // TODO: unify
//       std::cerr << "Split Node." << std::endl;

      unsigned int seed1, seed2;
      std::vector< geometry::box<Point> > boxes = n->get_boxes();

      n1->set_parent(n->get_parent());
      n2->set_parent(n->get_parent());

      linear_pick_seeds(n, seed1, seed2);

      if(n->is_leaf()) {
 	n1->add_value(boxes[seed1], n->get_value(seed1));
	n2->add_value(boxes[seed2], n->get_value(seed2));

	unsigned int index = 0;
	typename rtree_leaf<Point,Value>::leaves_map nodes = n->get_leaves();
	unsigned int remaining = nodes.size()-2;
	for(typename rtree_leaf<Point,Value>::leaves_map::const_iterator it = nodes.begin(); it != nodes.end(); ++it, index++) {
	  if(index != seed1 && index != seed2) {

// 	    std::cerr << "n1.elements: " << n1->elements() << std::endl;
// 	    std::cerr << "n2.elements: " << n2->elements() << std::endl;
// 	    std::cerr << "size: " << nodes.size() << std::endl;
// 	    std::cerr << "index: " << index << std::endl;
// 	    std::cerr << "remaining: " << remaining << std::endl;

	    if(n1->elements() + remaining == m_) {
// 	      std::cerr << "Adding to n1 because few elements" << std::endl;
	      n1->add_value(it->first, it->second);
	      continue;
	    }
	    if(n2->elements() + remaining == m_) {
// 	      std::cerr << "Adding to n2 because few elements" << std::endl;
	      n2->add_value(it->first, it->second);
	      continue;
	    }

	    remaining--;

	    /// current boxes of each group
	    geometry::box<Point> b1, b2;

	    /// enlarged boxes of each group
	    geometry::box<Point> eb1, eb2;
	    b1 = n1->compute_box();
	    b2 = n2->compute_box();

	    /// areas
	    double b1_area, b2_area;
	    double eb1_area, eb2_area;
	    b1_area = geometry::area(b1);
	    b2_area = geometry::area(b2);

	    eb1_area = compute_union_area(b1, it->first);
	    eb2_area = compute_union_area(b2, it->first);

	    if(eb1_area - b1_area > eb2_area - b2_area) {
	      n2->add_value(it->first, it->second);
	    }
	    if(eb1_area - b1_area < eb2_area - b2_area) {
	      n1->add_value(it->first, it->second);
	    }
	    if(eb1_area - b1_area == eb2_area - b2_area) {
	      if(b1_area < b2_area) {
		n1->add_value(it->first, it->second);
	      }
	      if(b1_area > b2_area) {
		n2->add_value(it->first, it->second);
	      }
	      if(b1_area == b2_area) {
		if(n1->elements() > n2->elements()) {
		  n2->add_value(it->first, it->second);
		} else {
		  n1->add_value(it->first, it->second);
		}
	      }
	    }
	  }
	}
      } else {
 	n1->add_node(boxes[seed1], n->get_node(seed1));
	n2->add_node(boxes[seed2], n->get_node(seed2));

	unsigned int index = 0;
	typename rtree_node<Point,Value>::node_map nodes = n->get_nodes();
	unsigned int remaining = nodes.size()-2;
	for(typename rtree_node<Point,Value>::node_map::const_iterator it = nodes.begin(); it != nodes.end(); ++it, index++) {
	  if(index != seed1 && index != seed2) {

	    if(n1->elements() + remaining == m_) {
	      n1->add_node(it->first, it->second);
	      continue;
	    }
	    if(n2->elements() + remaining == m_) {
	      n2->add_node(it->first, it->second);
	      continue;
	    }

	    remaining--;

	    /// current boxes of each group
	    geometry::box<Point> b1, b2;

	    /// enlarged boxes of each group
	    geometry::box<Point> eb1, eb2;
	    b1 = n1->compute_box();
	    b2 = n2->compute_box();

	    /// areas
	    double b1_area, b2_area;
	    double eb1_area, eb2_area;
	    b1_area = geometry::area(b1);
	    b2_area = geometry::area(b2);

	    eb1_area = compute_union_area(b1, it->first);
	    eb2_area = compute_union_area(b2, it->first);

	    if(eb1_area - b1_area > eb2_area - b2_area) {
	      n2->add_node(it->first, it->second);
	    }
	    if(eb1_area - b1_area < eb2_area - b2_area) {
	      n1->add_node(it->first, it->second);
	    }
	    if(eb1_area - b1_area == eb2_area - b2_area) {
	      if(b1_area < b2_area) {
		n1->add_node(it->first, it->second);
	      }
	      if(b1_area > b2_area) {
		n2->add_node(it->first, it->second);
	      }
	      if(b1_area == b2_area) {
		if(n1->elements() > n2->elements()) {
		  n2->add_node(it->first, it->second);
		} else {
		  n1->add_node(it->first, it->second);
		}
	      }
	    }

	  }
	}
      }
    }


    void linear_pick_seeds(const boost::shared_ptr< rtree_node<Point,Value> > &n, 
			   unsigned int &seed1,
			   unsigned int &seed2) const
    {
//       std::cerr << "Linear Pick Seeds." << std::endl;

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

//       std::cerr << "Separation X: " << separation_x << " between " << first_x << " and " << second_x << std::endl;
//       std::cerr << "Separation Y: " << separation_y << " between " << first_y << " and " << second_y << std::endl;

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

//       std::cerr << "Boxes: " << boxes.size() << std::endl;

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
//       std::cerr << "Lowest High: " << lowest_high << " Index: " << lowest_high_index << std::endl;

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
//       std::cerr << "Highest Low: " << highest_low << " Index: " << highest_low_index << std::endl;


      double width = highest_high - lowest_low;

//       std::cerr << "HH: " << highest_high << " LL: " << lowest_low << std::endl;

//       std::cerr << "Width: " << width << std::endl;

      separation = (highest_low  - lowest_high) / width;
      first = highest_low_index;
      second = lowest_high_index;
    }


    boost::shared_ptr<rtree_node<Point, Value> > choose_leaf(const geometry::box<Point> e)
    {
      boost::shared_ptr< rtree_node<Point, Value> > N = root_;

//       std::cerr << "Choosing." << std::endl;

      while(!N->is_leaf()) {
	/// traverse N's map to see which node we should select
// 	std::cerr << "Not a leaf." << std::endl;
	N = N->choose_node(e);
      }
      return N;
    }


  };


} // namespace spatial_index
} // namespace boost

#endif // BOOST_SPATIAL_INDEX_RTREE_HPP
