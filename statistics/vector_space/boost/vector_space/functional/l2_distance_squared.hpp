///////////////////////////////////////////////////////////////////////////////
// vector_space::functional::l2_distance_squared.hpp                         //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_VECTOR_SPACE_FUNCTIONAL_l2_DISTANCE_SQUARED_HPP_ER_2009
#define BOOST_VECTOR_SPACE_FUNCTIONAL_l2_DISTANCE_SQUARED_HPP_ER_2009
#include <cmath>
#include <numeric>
#include <functional>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/function.hpp>
#include <boost/call_traits.hpp>
#include <boost/range.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <boost/math/tools/precision.hpp>

namespace boost{
namespace vector_space{

    // Stores a range r, and when passed another range r1, computes the squared
    // Euclidian distance of r-r1.
    //
    // Examples: 
    // R == const std::vector<double>& 
    // R == iterator_range<std::vector<double> >
    template<typename R>
    class l2_distance_squared{
        typedef typename is_reference<R>::type is_ref_;
    public:
        typedef typename remove_reference<R>::type const_range_type;
        typedef typename remove_const<const_range_type>::type range_type;
        typedef typename range_value<range_type>::type result_type;
                    
        // Constructor
        l2_distance_squared(typename call_traits<R>::param_type x);
        l2_distance_squared(const l2_distance_squared& that);
        l2_distance_squared& operator=(const l2_distance_squared& that);                    

        // Call
        template<typename R1> 
        result_type operator()(const R1& y)const;
                        
        private:
        l2_distance_squared();
        typename call_traits<R>::value_type x_;
    };
    
    template<typename R> 
    l2_distance_squared<R> 
    make_l2_distance_squared(const R& x){
        return l2_distance_squared<R>(x);
    }

    // Definitions
    template<typename R>
    l2_distance_squared<R>::l2_distance_squared(
        typename call_traits<R>::param_type x
    ):x_(x){
        BOOST_MPL_ASSERT((mpl::not<is_ref_>));
    }

    template<typename R>
    l2_distance_squared<R>::l2_distance_squared(
        const l2_distance_squared& that
    ):x_(that.x_){}

    template<typename R>
    l2_distance_squared<R>& 
    l2_distance_squared<R>::operator=(const l2_distance_squared& that){
        if(&that != this){
            BOOST_MPL_ASSERT((mpl::not<is_ref_>));
            x_ = that.x_;
        }
        return *this;
    }
    
    template<typename R>
    template<typename R1> 
    typename l2_distance_squared<R>::result_type 
    l2_distance_squared<R>::operator()(const R1& y)const{
        BOOST_ASSERT( 
            size(x_) == size(y) 
        );
        return std::inner_product(
            begin(y),
            end(y),
            begin(x_),
            static_cast<result_type>(0),
            std::plus<result_type>(),
            (lambda::_1 * lambda::_1)(lambda::_1 - lambda::_2)
        );
    };

}// range
}// boost

#endif