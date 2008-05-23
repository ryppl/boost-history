// Copyright 2008 Federico J. Fernandez.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_SPATIAL_INDEX_QUADTREE_NODE_HPP
#define BOOST_SPATIAL_INDEX_QUADTREE_NODE_HPP

#include <boost/shared_ptr.hpp>

#include <iostream>

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

	Key k_;
	Value v_;
	
	double min_x_;
	double min_y_;

	double max_x_;
	double max_y_;
	
public:
	quadtree_node(double min_x, double min_y, double max_x, double max_y) 
		: min_x_(min_x), min_y_(min_y), max_x_(max_x), max_y_(max_y) 
	{
// 		std::cerr << "Creating quadtree_node with " << 
// 			"min_x: " << min_x << " min_y: " << min_y << " max_x: " << max_x << " max_y " << max_y << std::endl;
	}


	void insert(const Key &k, const Value &v)
	{
		if(v_ == Value()) {
// 			std::cerr << "Empty quadtree_node --> inserting" << std::endl;
			
			v_ = v;
			k_ = k;
			
		} else {
// 			std::cerr << "Quadtree_Node division: ";
			// quadtree_node division
// 			std::cerr << " (" << k.first << ", " << k.second << ")" ;
// 			std::cerr << " in (" << min_x_ << ", " << min_y_ << ")"; 
// 			std::cerr << " x (" << max_x_ << ", " << max_y_ << ")" << std::endl; 
			
			if((k.first >= min_x_ && k.first < min_x_+(max_x_-min_x_)/2.0) && 
			   (k.second >= min_y_ && k.second < min_y_+(max_y_-min_y_)/2.0)) {
				if(this->ne_ == boost::shared_ptr<quadtree_node>()) {
					this->ne_ = boost::shared_ptr<quadtree_node>(new quadtree_node(min_x_, min_y_, min_x_+(max_x_-min_x_)/2.0, 
												       min_y_+(max_y_-min_y_)/2.0));
				}
				ne_->insert(k, v);
			}
 			if((k.first >= min_x_ && k.first < min_x_+(max_x_-min_x_)/2.0) && 
 			   (k.second >= min_y_+(max_y_-min_y_)/2.0 && k.second < max_y_)) {
				if(this->se_ == boost::shared_ptr<quadtree_node>()) {
					this->se_ = boost::shared_ptr<quadtree_node>(new quadtree_node(min_x_, 
												       min_y_+(max_y_-min_y_)/2.0,
												       min_x_+(max_x_-min_x_)/2.0, 
												       max_y_));
				}
				se_->insert(k, v);
			}
		      
 			if((k.first >= min_x_+(max_x_-min_x_)/2.0 && k.first < max_x_) &&
 			   (k.second >= min_y_ && k.second < min_y_+(max_y_-min_y_)/2.0)) {
			
				if(this->nw_ == boost::shared_ptr<quadtree_node>()) {
					this->nw_ = boost::shared_ptr<quadtree_node>(new quadtree_node(min_x_+(max_x_-min_x_)/2.0, min_y_, max_x_, 
												       min_y_+(max_y_-min_y_)/2.0));
				}
				nw_->insert(k, v);
			}
		      

 			if((k.first >= min_x_+(max_x_-min_x_)/2.0 && k.first < max_x_) &&
 			   (k.second >= min_y_+(max_y_-min_y_)/2.0 && k.second < max_y_)) {
				
				if(this->sw_ == boost::shared_ptr<quadtree_node>()) {
					this->sw_ = boost::shared_ptr<quadtree_node>(new quadtree_node(min_x_+(max_x_-min_x_)/2.0, 
												     min_y_+(max_y_-min_y_)/2.0,
												       max_x_, max_y_));
				}
				sw_->insert(k, v);
			}
			
		}
	}
      

	void find(std::deque<Value> &r, const double x1, const double x2, const double y1, const double y2)
	{
		if(v_ != Value()) {

			if(k_.first >= x1 && k_.first <= x2 && k_.second >= y1 && k_.second <= y2) {
				r.push_back(v_);
			}

			if(
			   (x1 >= min_x_ && x1 < min_x_+(max_x_-min_x_)/2.0) && 
			   (y1 >= min_y_ && y1 < min_y_+(max_y_-min_y_)/2.0) ||

			   (x1 >= min_x_ && x1 < min_x_+(max_x_-min_x_)/2.0) && 
			   (y2 >= min_y_ && y2 < min_y_+(max_y_-min_y_)/2.0) ||

			   (x2 >= min_x_ && x2 < min_x_+(max_x_-min_x_)/2.0) && 
			   (y1 >= min_y_ && y1 < min_y_+(max_y_-min_y_)/2.0) ||

			   (x2 >= min_x_ && x2 < min_x_+(max_x_-min_x_)/2.0) && 
			   (y2 >= min_y_ && y2 < min_y_+(max_y_-min_y_)/2.0)

			   ) {
				if(ne_ != boost::shared_ptr<quadtree_node>()) {
					ne_->find(r, x1, x2, y1, y2);
				}
			}
 			if(
			   (x1 >= min_x_ && x1 < min_x_+(max_x_-min_x_)/2.0) && 
 			   (y1 >= min_y_+(max_y_-min_y_)/2.0 && y1 < max_y_) ||

			   (x1 >= min_x_ && x1 < min_x_+(max_x_-min_x_)/2.0) && 
 			   (y2 >= min_y_+(max_y_-min_y_)/2.0 && y2 < max_y_) ||

			   (x2 >= min_x_ && x2 < min_x_+(max_x_-min_x_)/2.0) && 
 			   (y1 >= min_y_+(max_y_-min_y_)/2.0 && y1 < max_y_) ||

			   (x2 >= min_x_ && x2 < min_x_+(max_x_-min_x_)/2.0) && 
 			   (y2 >= min_y_+(max_y_-min_y_)/2.0 && y2 < max_y_)

			   ) {
				if(se_ != boost::shared_ptr<quadtree_node>()) {
					se_->find(r, x1, x2, y1, y2);
				}
			      
			}

 			if(
			   (x1 >= min_x_+(max_x_-min_x_)/2.0 && x1 < max_x_) &&
 			   (y1 >= min_y_ && y1 < min_y_+(max_y_-min_y_)/2.0) ||

			   (x1 >= min_x_+(max_x_-min_x_)/2.0 && x1 < max_x_) &&
 			   (y2 >= min_y_ && y2 < min_y_+(max_y_-min_y_)/2.0) ||


			   (x2 >= min_x_+(max_x_-min_x_)/2.0 && x2 < max_x_) &&
 			   (y1 >= min_y_ && y1 < min_y_+(max_y_-min_y_)/2.0) ||

			   (x2 >= min_x_+(max_x_-min_x_)/2.0 && x2 < max_x_) &&
 			   (y2 >= min_y_ && y2 < min_y_+(max_y_-min_y_)/2.0)

			   ) {
				if(nw_ != boost::shared_ptr<quadtree_node>()) {
					nw_->find(r, x1, x2, y1, y2);
				}
			      
			}
 			if(
			   (x1 >= min_x_+(max_x_-min_x_)/2.0 && x1 < max_x_) &&
 			   (y1 >= min_y_+(max_y_-min_y_)/2.0 && y1 < max_y_) ||

			   (x1 >= min_x_+(max_x_-min_x_)/2.0 && x1 < max_x_) &&
 			   (y2 >= min_y_+(max_y_-min_y_)/2.0 && y2 < max_y_) ||

			   (x2 >= min_x_+(max_x_-min_x_)/2.0 && x2 < max_x_) &&
 			   (y1 >= min_y_+(max_y_-min_y_)/2.0 && y1 < max_y_) ||

			   (x2 >= min_x_+(max_x_-min_x_)/2.0 && x2 < max_x_) &&
 			   (y2 >= min_y_+(max_y_-min_y_)/2.0 && y2 < max_y_)

			   ) {
				if(sw_ != boost::shared_ptr<quadtree_node>()) {
					sw_->find(r, x1, x2, y1, y2);
				}
			      
			}
		}
	}

	Value find(const Key &k)
	{
		if(v_ == Value()) {
			return Value();
		} else {
// 			std::cerr << "compare: " << k_.first << " with " << k.first;
// 			std::cerr << " " << k_.second << " with " << k.second << std::endl;
			if(k_ == k) {
// 				std::cerr << "ok" << std::endl;
				return v_;
			}

			if((k.first >= min_x_ && k.first < min_x_+(max_x_-min_x_)/2.0) && 
			   (k.second >= min_y_ && k.second < min_y_+(max_y_-min_y_)/2.0)) {
				if(ne_ != boost::shared_ptr<quadtree_node>()) {
					return ne_->find(k);
				} else {
					return v_;
				}
			}
 			if((k.first >= min_x_ && k.first < min_x_+(max_x_-min_x_)/2.0) && 
 			   (k.second >= min_y_+(max_y_-min_y_)/2.0 && k.second < max_y_)) {
				if(se_ != boost::shared_ptr<quadtree_node>()) {
					return se_->find(k);
				} else {
					return v_;
				}
			      
			}

 			if((k.first >= min_x_+(max_x_-min_x_)/2.0 && k.first < max_x_) &&
 			   (k.second >= min_y_ && k.second < min_y_+(max_y_-min_y_)/2.0)) {
				if(nw_ != boost::shared_ptr<quadtree_node>()) {
					return nw_->find(k);
				} else {
					return v_;
				}
			      
			}
 			if((k.first >= min_x_+(max_x_-min_x_)/2.0 && k.first < max_x_) &&
 			   (k.second >= min_y_+(max_y_-min_y_)/2.0 && k.second < max_y_)) {
				if(sw_ != boost::shared_ptr<quadtree_node>()) {
					return sw_->find(k);
				} else {
					return v_;
				}
			      
			}
		}
		return Value();
	}
	      
};


} // namespace spatial_index
} // namespace boost

#endif // BOOST_SPATIAL_INDEX_QUADTREE_NODE_HPP
