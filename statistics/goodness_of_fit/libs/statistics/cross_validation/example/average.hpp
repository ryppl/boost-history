//////////////////////////////////////////////////////////////////////////////
// statistics::goodness_of_fit::example::average.h                         //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef  LIBS_STATISTICS_GOODNESS_OF_FIT_EXAMPLE_AVERAGE_H_ER_2009
#define  LIBS_STATISTICS_GOODNESS_OF_FIT_EXAMPLE_AVERAGE_H_ER_2009
#include <vector>
#include <boost/tuple/tuple.hpp>
#include <boost/functional/mean_var_accumulator.hpp>
#include <boost/binary_op/meta/remove_ref_cv.hpp>

namespace boost{
namespace statistics{
namespace goodness_of_fit{
namespace detail{

    // For use by examples, not part of the library.
    template<typename T>
    struct average{
        typedef typename binary_op::tuple_remove_ref_cv<T>::type        tuple_;
        typedef typename tuples::element<1,tuple_>::type                y_;
        typedef typename 
            boost::functional::mean_var_accumulator<y_>::type mean_var_acc_;
        y_ y;
    };

}// detail
}// goodness_of_fit

    template<typename T,typename R>
    void train(
        goodness_of_fit::detail::average<T>& e,
        const R& data
    ){
        typedef goodness_of_fit::detail::average<T> average_;
        typedef typename average_::y_ y_;
        typedef std::vector<y_> vec_y_;
        vec_y_ vec_y;
        binary_op::tails(
            boost::begin(data),
            boost::end(data),
            std::back_inserter(vec_y)
        );
        
        typedef typename average_::mean_var_acc_ acc_;
        e.y = boost::accumulators::mean(
            std::for_each(
                boost::begin(vec_y),
                boost::end(vec_y),
                acc_()
            )
        );
    
    };

    template<typename T,typename X>
    typename goodness_of_fit::detail::average<T>::y_
    estimate(
        const goodness_of_fit::detail::average<T>& e,
        const X& x
    ){
        return e.y;
    }

}// statistics
}// boost

#endif