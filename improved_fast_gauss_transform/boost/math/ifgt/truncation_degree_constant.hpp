//////////////////////////////////////////////////////////////////////////////
// ifgt/truncation_degree_constant.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_ASSIGN_DEGREES_CONSTANT_HPP_ER_2009
#define BOOST_MATH_IFGT_ASSIGN_DEGREES_CONSTANT_HPP_ER_2009
#include <algorithm>
#include <boost/range.hpp>
#include <boost/parameter/parameters.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/math/ifgt/tag.hpp>
namespace boost{namespace math{namespace ifgt{

    /// Models TruncationDegreesPolicy
   template<typename RealType = double>
    class truncation_degree_constant{
     public:
        typedef RealType            value_type;

        truncation_degree_constant():p_(20){}
        explicit truncation_degree_constant(unsigned p):p_(p){}
        //default copy/assign should be fine

        template<typename ArgPack>
        truncation_degree_constant(const ArgPack& args)
        :p_(args[tag::degree|20]){}

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

}}}
#endif
