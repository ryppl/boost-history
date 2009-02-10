//////////////////////////////////////////////////////////////////////////////
// ifgt/rest_weights_wrapper.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_REST_WEIGHTS_WRAPPER_HPP_ER_2009
#define BOOST_MATH_IFGT_REST_WEIGHTS_WRAPPER_HPP_ER_2009
namespace boost{namespace math{namespace ifgt{
        /// This wrapper indicates that each weight in the sequence
    /// has yet to be prepend by one in order to be NW compliant.
    template<typename R1>
    class rest_weights_wrapper{
        public:
        rest_weights_wrapper(const R1& r):r_(r){}
        rest_weights_wrapper(const rest_weights_wrapper& that):r_(that.r_){}
        const R1& operator()()const{ return r_; }
        private:
        rest_weights_wrapper();
        const R1& r_;
    };
    template<typename R1>
    rest_weights_wrapper<R1> make_rest_weights_wrapper(const R1& r){
        typedef rest_weights_wrapper<R1> result_type;
        return result_type(r);
    }
}}}
#endif
