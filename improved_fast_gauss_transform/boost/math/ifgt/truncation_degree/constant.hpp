//////////////////////////////////////////////////////////////////////////////
// constant.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_TRUNCATION_DEGREE_CONSTANT_HPP_ER_2009
#define BOOST_MATH_IFGT_TRUNCATION_DEGREE_CONSTANT_HPP_ER_2009
#include <algorithm>
#include <boost/range.hpp>
#include <boost/parameter/parameters.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/math/ifgt/keyword.hpp>
namespace boost{
namespace math{
namespace ifgt{
namespace truncation_degree{

    /// Models TruncationDegreesPolicy
   template<typename RealType = double>
    class constant{
     public:
        typedef RealType            value_type;

        constant():p_(20){}
        explicit constant(unsigned p):p_(p){}
        //default copy/assign should be fine

        template<typename ArgPack>
        constant(const ArgPack& args)
        :p_(args[kwd<>::degree|20]){}

        unsigned degree()const{return p_;}

        template<typename Cluster,typename R0,typename R1>
        void operator()(
            const Cluster& cluster,
            value_type radius_source_to_center,
            const R0& weight,
            R1& degrees
        ){
            std::fill(
                begin(degrees),
                end(degrees),
                degree()
            );
        }
        private:
        unsigned p_;
    };
}
}
}
}
#endif
