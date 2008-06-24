// Copyright 2008 Federico J. Fernandez.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_SPATIAL_INDEX_QUADTREE_NODE_HPP
#define BOOST_SPATIAL_INDEX_QUADTREE_NODE_HPP

#include <boost/shared_ptr.hpp>

#include <geometry/within.hpp>

#include <iostream>
#include <map>

namespace boost {
namespace spatial_index {

template<typename Point, typename Value>
class quadtree_node
{
private:
  /// pointers to child nodes
  boost::shared_ptr<quadtree_node> nw_;
  boost::shared_ptr<quadtree_node> ne_;
  boost::shared_ptr<quadtree_node> sw_;
  boost::shared_ptr<quadtree_node> se_;

  /// this node's points
  std::map<Point, Value> m_;

  /// the bounding rectangle for the node
  geometry::box<Point> bounding_rectangle_;
	
  /// number of points in each node
  unsigned int node_size_;

  /// divide the node in four quadrants
  void divide_quadrants(geometry::box<Point> &ne_box, geometry::box<Point> &sw_box, geometry::box<Point> &se_box, geometry::box<Point> &nw_box) const
  {

    geometry::get<0>(ne_box.min()) = (geometry::get<0>(bounding_rectangle_.max()) - geometry::get<0>(bounding_rectangle_.min())) / 2.0 
      + geometry::get<0>(bounding_rectangle_.min());
    geometry::get<1>(ne_box.min()) = (geometry::get<1>(bounding_rectangle_.max()) - geometry::get<1>(bounding_rectangle_.min())) / 2.0 
      + geometry::get<1>(bounding_rectangle_.min());
      geometry::get<0>(ne_box.max()) = geometry::get<0>(bounding_rectangle_.max());
      geometry::get<1>(ne_box.max()) = geometry::get<1>(bounding_rectangle_.max());

      geometry::get<0>(sw_box.min()) = geometry::get<0>(bounding_rectangle_.min());
      geometry::get<1>(sw_box.min()) = geometry::get<1>(bounding_rectangle_.min());
      geometry::get<0>(sw_box.max()) = (geometry::get<0>(bounding_rectangle_.max()) - geometry::get<0>(bounding_rectangle_.min())) / 2.0 
	+ geometry::get<0>(bounding_rectangle_.min());
      geometry::get<1>(sw_box.max()) = (geometry::get<1>(bounding_rectangle_.max()) - geometry::get<1>(bounding_rectangle_.min())) / 2.0 
	+ geometry::get<1>(bounding_rectangle_.min());

      geometry::get<0>(se_box.min()) = (geometry::get<0>(bounding_rectangle_.max()) - geometry::get<0>(bounding_rectangle_.min())) / 2.0 
	+ geometry::get<0>(bounding_rectangle_.min());
      geometry::get<1>(se_box.min()) = geometry::get<1>(bounding_rectangle_.min());
      geometry::get<0>(se_box.max()) = geometry::get<0>(bounding_rectangle_.max());
      geometry::get<1>(se_box.max()) = (geometry::get<1>(bounding_rectangle_.max()) - geometry::get<1>(bounding_rectangle_.min())) / 2.0 
	+ geometry::get<1>(bounding_rectangle_.min());

      geometry::get<0>(nw_box.min()) = geometry::get<0>(bounding_rectangle_.min());
      geometry::get<1>(nw_box.min()) = (geometry::get<1>(bounding_rectangle_.max()) - geometry::get<1>(bounding_rectangle_.min())) / 2.0 
	+ geometry::get<1>(bounding_rectangle_.min());
      geometry::get<0>(nw_box.max()) = (geometry::get<0>(bounding_rectangle_.max()) - geometry::get<0>(bounding_rectangle_.min())) / 2.0 
	+ geometry::get<0>(bounding_rectangle_.min());
      geometry::get<1>(nw_box.max()) = geometry::get<1>(bounding_rectangle_.max());
  }


public:
  quadtree_node(const geometry::box<Point> &r, const unsigned int node_size) 
    : bounding_rectangle_(r), node_size_(node_size)
  {
    // std::cerr << "Creating quadtree_node with " << 
    // "min_x: " << min_x << " min_y: " << min_y << " max_x: " << max_x << " max_y " << max_y << std::endl;
  }


  void clean(void)
  {
    if(nw_ != boost::shared_ptr<quadtree_node>()) {
      if(nw_->empty_leaf()) {
	nw_ = boost::shared_ptr<quadtree_node>();
      } else {
	nw_->clean();
      }
    }

    if(sw_ != boost::shared_ptr<quadtree_node>()) {
      if(sw_->empty_leaf()) {
	sw_ = boost::shared_ptr<quadtree_node>();
      } else {
	sw_->clean();
      }
    }

    if(se_ != boost::shared_ptr<quadtree_node>()) {
      if(se_->empty_leaf()) {
	se_ = boost::shared_ptr<quadtree_node>();
      } else {
	se_->clean();
      }
    }

    if(ne_ != boost::shared_ptr<quadtree_node>()) {
      if(ne_->empty_leaf()) {
	ne_ = boost::shared_ptr<quadtree_node>();
      } else {
	ne_->clean();
      }
    }

  }

  bool empty_leaf(void) const
  {
    return (m_.size() == 0) && 
      (ne_ == boost::shared_ptr<quadtree_node>()) && 
      (se_ == boost::shared_ptr<quadtree_node>()) && 
      (nw_ == boost::shared_ptr<quadtree_node>()) && 
      (sw_ == boost::shared_ptr<quadtree_node>())
      ;
  }

  void insert(const Point &k, const Value &v)
  {
    if(m_.size() < node_size_) {
//       std::cerr << "Empty quadtree_node --> inserting (" << v << ")" << std::endl;
      m_[k] = v;
    } else {
      // std::cerr << "Quadtree_Node division: ";
      // // quadtree_node division
      // std::cerr << " (" << k.first << ", " << k.second << ")" ;
      // std::cerr << " in (" << min_x_ << ", " << min_y_ << ")"; 
      // std::cerr << " x (" << max_x_ << ", " << max_y_ << ")" << std::endl; 

      // IMP: maybe this could be done only one time at node creation
      geometry::box<Point> ne_box, sw_box, se_box, nw_box;
      divide_quadrants(ne_box, sw_box, se_box, nw_box);

      if(geometry::within(k, ne_box)) {
	if(this->ne_ == boost::shared_ptr<quadtree_node>()) {
	  this->ne_ = boost::shared_ptr<quadtree_node>(new quadtree_node(ne_box, node_size_));
	}
	ne_->insert(k, v);
	return;
      }
      if(geometry::within(k, se_box)) {
	if(this->se_ == boost::shared_ptr<quadtree_node>()) {
	  this->se_ = boost::shared_ptr<quadtree_node>(new quadtree_node(se_box, node_size_));
	}
	se_->insert(k, v);
	return;
      }
      if(geometry::within(k, nw_box)) {
	if(this->nw_ == boost::shared_ptr<quadtree_node>()) {
	  this->nw_ = boost::shared_ptr<quadtree_node>(new quadtree_node(nw_box, node_size_));
	}
	nw_->insert(k, v);
	return;
      }
      if(geometry::within(k, sw_box)) {	
	if(this->sw_ == boost::shared_ptr<quadtree_node>()) {
	  this->sw_ = boost::shared_ptr<quadtree_node>(new quadtree_node(sw_box, node_size_));
	}
	sw_->insert(k, v);
	return;
      }
    }
  }
      

  void find(std::deque<Value> &result, const geometry::box<Point> &r)
  {
    if(m_.size() != 0) {
      // std::cerr << "Node: X1:" << min_x_ << " X2:" << max_x_ << " Y1:" << min_y_ << " Y2: " << max_y_ << std::endl;
      // std::cerr << "Query: X1:" << x1 << " X2:" << x2 << " Y1:" << y1 << " Y2: " << y2 << std::endl;

      if(geometry::get<0>(r.min()) > geometry::get<0>(bounding_rectangle_.max())
	 || geometry::get<0>(r.max()) < geometry::get<0>(bounding_rectangle_.min())
	 || geometry::get<1>(r.min()) > geometry::get<1>(bounding_rectangle_.max())
	 || geometry::get<1>(r.max()) < geometry::get<1>(bounding_rectangle_.min())) {
	// std::cerr << "Not Inside" << std::endl;
	return;
      }

      // std::cerr << "Inside" << std::endl;

      for(typename std::map<Point,Value>::const_iterator it = m_.begin(); it != m_.end(); ++it) {
// 	std::cerr << "Checking: (" << geometry::get<0>(it->first) << "," << geometry::get<1>(it->first) << ")" << std::endl;
	if(geometry::get<0>(it->first) >= geometry::get<0>(r.min()) && geometry::get<0>(it->first) <= geometry::get<0>(r.max()) && 
	   geometry::get<1>(it->first) >= geometry::get<1>(r.min()) && geometry::get<1>(it->first) <= geometry::get<1>(r.max())) {
	  result.push_back(it->second);
	}
      }

      if(ne_ != boost::shared_ptr<quadtree_node>()) {
	ne_->find(result, r);
      }
      if(se_ != boost::shared_ptr<quadtree_node>()) {
	se_->find(result, r);
      }
      if(nw_ != boost::shared_ptr<quadtree_node>()) {
	nw_->find(result, r);
      }
      if(sw_ != boost::shared_ptr<quadtree_node>()) {
	sw_->find(result, r);
      }

      // std::cerr << std::endl;
    }
  }


  Value find(const Point &k)
  {
      typename std::map<Point, Value>::const_iterator it = m_.find(k);
      if(it != m_.end()) {
	return it->second;
      }
      return recursive_search(k);
  }


  Value recursive_search(const Point &k)
  {
    // IMP: maybe this could be done only one time at node creation
    // but it will consume more memory...

    geometry::box<Point> ne_box, sw_box, se_box, nw_box;
    divide_quadrants(ne_box, sw_box, se_box, nw_box);

    if(geometry::within(k, ne_box)) {
      if(ne_ != boost::shared_ptr<quadtree_node>()) {
	return ne_->find(k);
      } else {
	return Value();
      }
    }
    if(geometry::within(k, se_box)) {
      if(se_ != boost::shared_ptr<quadtree_node>()) {
	return se_->find(k);
      } else {
	return Value();
      }
    }
    if(geometry::within(k, nw_box)) {
      if(nw_ != boost::shared_ptr<quadtree_node>()) {
	return nw_->find(k);
      } else {
	return Value();
      }
    }
    if(geometry::within(k, sw_box)) {
      if(sw_ != boost::shared_ptr<quadtree_node>()) {
	return sw_->find(k);
      } else {
	return Value();
      }
    }
    // never reached
    return Value();
  }

  void remove(const Point &k)
  {
    typename std::map<Point, Value>::iterator it = m_.find(k);
    if(it != m_.end()) {
      m_.erase(it);
      return;
    }
    recursive_remove(k);
  }

  void recursive_remove(const Point &k)
  {
//     std::cerr << "Recursive_remove" << std::endl;

//     std::cerr << "Checking: (" << geometry::get<0>(k) << "," << geometry::get<1>(k) << ")" << std::endl;
//     std::cerr << "in : (" << geometry::get<0>(bounding_rectangle_.min()) << "," << geometry::get<1>(bounding_rectangle_.min()) << ") x ";
//     std::cerr << "(" << geometry::get<0>(bounding_rectangle_.max()) << "," << geometry::get<1>(bounding_rectangle_.max()) << ")" << std::endl;

    // IMP: maybe this could be done only one time at node creation
    // but it will consume more memory...
    geometry::box<Point> ne_box, sw_box, se_box, nw_box;
    divide_quadrants(ne_box, sw_box, se_box, nw_box);


    if(geometry::within(k, ne_box)) {
      if(ne_ != boost::shared_ptr<quadtree_node>()) {
	ne_->remove(k);
	if(ne_->empty_leaf()) {
	  ne_ = boost::shared_ptr<quadtree_node>();
	}
	return;
      } else {
	throw std::logic_error("Not found");
      }
    }
    if(geometry::within(k, se_box)) {
      if(se_ != boost::shared_ptr<quadtree_node>()) {
	se_->remove(k);
	if(se_->empty_leaf()) {
	  se_ = boost::shared_ptr<quadtree_node>();
	}
	return;
      } else {
	throw std::logic_error("Not found");
      }
    }
    if(geometry::within(k, nw_box)) {
      if(nw_ != boost::shared_ptr<quadtree_node>()) {
	nw_->remove(k);
	if(nw_->empty_leaf()) {
	  nw_ = boost::shared_ptr<quadtree_node>();
	}
	return;
      } else {
	throw std::logic_error("Not found");
      }
    }
    if(geometry::within(k, sw_box)) {
      if(sw_ != boost::shared_ptr<quadtree_node>()) {
	sw_->remove(k);
	if(sw_->empty_leaf()) {
	  sw_ = boost::shared_ptr<quadtree_node>();
	}
	return;
      } else {
	throw std::logic_error("Not found");
      }
    }
  }


  void print(void) const
  {
    std::cerr << "--------------------------------------" << std::endl;
    std::cerr << " [ ";
    for(typename std::map<Point,Value>::const_iterator it = m_.begin(); it != m_.end(); ++it) {
      std::cerr << it->second << " ";
    }
    std::cerr << " ] " << std::endl;;

    if(sw_.get() != 0) {
      sw_->print();
    } else {
      std::cerr << "SW not defined" << std::endl;
    }

    if(nw_.get() != 0) {
      nw_->print();
    } else {
      std::cerr << "NW not defined" << std::endl;
    }

    if(se_.get() != 0) {
      se_->print();
    } else {
      std::cerr << "SE not defined" << std::endl;
    }

    if(ne_.get() != 0) {
      ne_->print();
    } else {
      std::cerr << "NE not defined" << std::endl;
    }
    std::cerr << "--------------------------------------" << std::endl;
  }

};


} // namespace spatial_index
} // namespace boost

#endif // BOOST_SPATIAL_INDEX_QUADTREE_NODE_HPP
