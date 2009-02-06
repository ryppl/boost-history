//////////////////////////////////////////////////////////////////////////////
// find_nearest_neighbor.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ALGORITHM_FIND_NEAREST_NEIGHBOR_HPP_ER_2009
#define BOOST_ALGORITHM_FIND_NEAREST_NEIGHBOR_HPP_ER_2009
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <functional>
#include <iterator>
#include <boost/assert.hpp>
#include <boost/range.hpp>
#include <boost/bind.hpp>
//#include <boost/numeric/conversion/bounds.hpp>
//#include <boost/limits.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/algorithm/l2_distance_squared.hpp>
namespace boost{

    /// Distance::type f; f(x,y)
    template<typename R,typename Distance = l2_distance_squared>
    class find_nearest_neighbor{
        // TODO maybe replace const R by add_const<R>::type
        typedef typename range_iterator<const R>::type        iter_type;
        typedef typename Distance::type                     distance_type;
    public:
        typedef iter_type                                   result_type;
        find_nearest_neighbor(const R& points):points_(points){
            BOOST_ASSERT(size(points_)!=0);
        }
        find_nearest_neighbor(const find_nearest_neighbor& that)
        :points_(that.points_){}

        template<typename R1> result_type
        operator()(const R1& new_point){
            return std::min_element(
                make_transform_iterator(
                    begin(points_),
                    op<R1>(new_point)
                ),
                make_transform_iterator(
                    end(points_),
                    op<R1>(new_point)
                )
            ).base();
        }
        const R& points()const{ return points_; }
    private:
        find_nearest_neighbor& operator=(const find_nearest_neighbor& that);
        const R& points_;
        template<typename R1>
        class op{
            public:
            typedef typename range_value<R1>::type              result_type;
            op(const R1& new_point):new_point_(new_point){}
            op(const op& that):new_point_(that.new_point_){}
            op& operator=(const op& that){
                if(&that!=this){
                    if(new_point_!=that.new_point_){
                        throw std::runtime_error(
                            "find_nearest_cluster::op::operator=");
                    }
                    //throw std::runtime_error("op");
                }
                return *this;
            }
            template<typename R2>
            result_type operator()(const R2& point)const{
                //return squared_distance(point,new_point_);
                return distance(point,new_point_);
            }
            private:
            distance_type distance;
            op();
            const R1& new_point_;
        };
    };
    template<typename R>
    find_nearest_neighbor<R> make_find_nearest_neighbor(const R& r){
        return find_nearest_neighbor<R>(r);
    }
    struct find_nearest_neighbor_factory{
        template<typename R>
        static find_nearest_neighbor<R> make(const R& r){
            return make_find_nearest_neighbor(r);
        }
    };
}
#endif
