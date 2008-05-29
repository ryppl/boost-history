// Copyright 2008 Federico J. Fernandez.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_SPATIAL_INDEX_QUADTREE_NODE_HPP
#define BOOST_SPATIAL_INDEX_QUADTREE_NODE_HPP

#include <boost/shared_ptr.hpp>

#include <iostream>
#include <map>

namespace boost {
namespace spatial_index {

template<typename Key, typename Value>
class quadtree_node
{
private:
	boost::shared_ptr<quadtree_node> nw_;
	boost::shared_ptr<quadtree_node> ne_;
	boost::shared_ptr<quadtree_node> sw_;
	boost::shared_ptr<quadtree_node> se_;

	std::map<Key, Value> m_;
	
	double min_x_;
	double min_y_;

	double max_x_;
	double max_y_;
	
	// number of points in each node
	unsigned int node_size_;

public:
	quadtree_node(double min_x, double min_y, double max_x, double max_y, unsigned int node_size) 
		: min_x_(min_x), min_y_(min_y), max_x_(max_x), max_y_(max_y), node_size_(node_size)
	{
// 		std::cerr << "Creating quadtree_node with " << 
// 			"min_x: " << min_x << " min_y: " << min_y << " max_x: " << max_x << " max_y " << max_y << std::endl;
	}


	void insert(const Key &k, const Value &v)
	{
		if(m_.size() < node_size_) {
// 			std::cerr << "Empty quadtree_node --> inserting" << std::endl;
			m_[k] = v;
		} else {
//  			std::cerr << "Quadtree_Node division: ";
// 			// quadtree_node division
//  			std::cerr << " (" << k.first << ", " << k.second << ")" ;
//  			std::cerr << " in (" << min_x_ << ", " << min_y_ << ")"; 
//  			std::cerr << " x (" << max_x_ << ", " << max_y_ << ")" << std::endl; 
			
			if((k.first >= min_x_ && k.first < min_x_+(max_x_-min_x_)/2.0) && 
			   (k.second >= min_y_ && k.second < min_y_+(max_y_-min_y_)/2.0)) {
				if(this->ne_ == boost::shared_ptr<quadtree_node>()) {
					this->ne_ = boost::shared_ptr<quadtree_node>(new quadtree_node(min_x_, min_y_, min_x_+(max_x_-min_x_)/2.0, 
												       min_y_+(max_y_-min_y_)/2.0, node_size_));
				}
				ne_->insert(k, v);
			}
 			if((k.first >= min_x_ && k.first < min_x_+(max_x_-min_x_)/2.0) && 
 			   (k.second >= min_y_+(max_y_-min_y_)/2.0 && k.second < max_y_)) {
				if(this->se_ == boost::shared_ptr<quadtree_node>()) {
					this->se_ = boost::shared_ptr<quadtree_node>(new quadtree_node(min_x_, 
												       min_y_+(max_y_-min_y_)/2.0,
												       min_x_+(max_x_-min_x_)/2.0, 
												       max_y_, node_size_));
				}
				se_->insert(k, v);
			}
		      
 			if((k.first >= min_x_+(max_x_-min_x_)/2.0 && k.first < max_x_) &&
 			   (k.second >= min_y_ && k.second < min_y_+(max_y_-min_y_)/2.0)) {
			
				if(this->nw_ == boost::shared_ptr<quadtree_node>()) {
					this->nw_ = boost::shared_ptr<quadtree_node>(new quadtree_node(min_x_+(max_x_-min_x_)/2.0, min_y_, max_x_, 
												       min_y_+(max_y_-min_y_)/2.0, node_size_));
				}
				nw_->insert(k, v);
			}
		      

 			if((k.first >= min_x_+(max_x_-min_x_)/2.0 && k.first < max_x_) &&
 			   (k.second >= min_y_+(max_y_-min_y_)/2.0 && k.second < max_y_)) {
				
				if(this->sw_ == boost::shared_ptr<quadtree_node>()) {
					this->sw_ = boost::shared_ptr<quadtree_node>(new quadtree_node(min_x_+(max_x_-min_x_)/2.0, 
												     min_y_+(max_y_-min_y_)/2.0,
												       max_x_, max_y_, node_size_));
				}
				sw_->insert(k, v);
			}
			
		}
	}
      

	void find(std::deque<Value> &r, const double x1, const double x2, const double y1, const double y2)
	{
		if(m_.size() != 0) {
// 			std::cerr << "Node: X1:" << min_x_ << " X2:" << max_x_ << " Y1:" << min_y_ << " Y2: " << max_y_ << std::endl;
// 			std::cerr << "Query: X1:" << x1 << " X2:" << x2 << " Y1:" << y1 << " Y2: " << y2 << std::endl;

			if(x1 > max_x_ || x2 < min_x_ || y1 > max_y_ || y2 < min_y_) {
// 				std::cerr << "Not Inside" << std::endl;
				return;
			}

// 			std::cerr << "Inside" << std::endl;

			for(typename std::map<Key,Value>::const_iterator it = m_.begin(); it != m_.end(); ++it) {
// 				std::cerr << "Checking: (" << it->first.first << "," << it->first.second << ")" << std::endl;
				if(it->first.first >= x1 && it->first.first <= x2 && it->first.second >= y1 && it->first.second <= y2) {
					r.push_back(it->second);
				}
			}

			if(ne_ != boost::shared_ptr<quadtree_node>()) {
				ne_->find(r, x1, x2, y1, y2);
			}
			if(se_ != boost::shared_ptr<quadtree_node>()) {
				se_->find(r, x1, x2, y1, y2);
			}
			if(nw_ != boost::shared_ptr<quadtree_node>()) {
				nw_->find(r, x1, x2, y1, y2);
			}
			if(sw_ != boost::shared_ptr<quadtree_node>()) {
				sw_->find(r, x1, x2, y1, y2);
			}
			
// 			std::cerr << std::endl;
		}
	}


	Value find(const Key &k)
	{
		if(m_.size() == 0) {
			return Value();
		} else {
// 			std::cerr << "compare: " << k_.first << " with " << k.first;
// 			std::cerr << " " << k_.second << " with " << k.second << std::endl;
			typename std::map<Key, Value>::const_iterator it = m_.find(k);
			if(it != m_.end()) {
// 				std::cerr << "ok" << std::endl;
				return it->second;
			}

			if((k.first >= min_x_ && k.first < min_x_+(max_x_-min_x_)/2.0) && 
			   (k.second >= min_y_ && k.second < min_y_+(max_y_-min_y_)/2.0)) {
				if(ne_ != boost::shared_ptr<quadtree_node>()) {
					return ne_->find(k);
				} else {
					return Value();
// 					return v_;
				}
			}
 			if((k.first >= min_x_ && k.first < min_x_+(max_x_-min_x_)/2.0) && 
 			   (k.second >= min_y_+(max_y_-min_y_)/2.0 && k.second < max_y_)) {
				if(se_ != boost::shared_ptr<quadtree_node>()) {
					return se_->find(k);
				} else {
					return Value();
// 					return v_;
				}
			      
			}

 			if((k.first >= min_x_+(max_x_-min_x_)/2.0 && k.first < max_x_) &&
 			   (k.second >= min_y_ && k.second < min_y_+(max_y_-min_y_)/2.0)) {
				if(nw_ != boost::shared_ptr<quadtree_node>()) {
					return nw_->find(k);
				} else {
					return Value();
// 					return v_;
				}
			      
			}
 			if((k.first >= min_x_+(max_x_-min_x_)/2.0 && k.first < max_x_) &&
 			   (k.second >= min_y_+(max_y_-min_y_)/2.0 && k.second < max_y_)) {
				if(sw_ != boost::shared_ptr<quadtree_node>()) {
					return sw_->find(k);
				} else {
					return Value();
// 					return v_;
				}
			      
			}
		}
		// never reached
		return Value();
	}
	      
};


} // namespace spatial_index
} // namespace boost

#endif // BOOST_SPATIAL_INDEX_QUADTREE_NODE_HPP
