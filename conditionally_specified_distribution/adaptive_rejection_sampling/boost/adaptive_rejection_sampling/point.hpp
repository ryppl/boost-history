////////////////////////////////////////////////////////////////////////////
// point.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ADAPTIVE_REJECTION_SAMPLING_POINT_HPP_ER_2009
#define BOOST_ADAPTIVE_REJECTION_SAMPLING_POINT_HPP_ER_2009
#include <stdexcept>
#include <boost/format.hpp>
#include <boost/math_limit/miscellanea.hpp>
namespace boost{
namespace adaptive_rejection_sampling{

        template<typename RealType>
        class point{
            public:
            typedef RealType value_type;

            point(value_type x,value_type y,value_type dy)
            :x_(x),y_(y),dy_(dy){}
            value_type x()const{return x_;}
            value_type y()const{return y_;}
            value_type dy()const{return dy_;}

            private:
            value_type x_;
            value_type y_;
            value_type dy_;
        };

        template<typename RealType>
        RealType
        tangent_at_negative_infinity(const point<RealType>& p){
            typedef  math_limit::limits<RealType>  limits_t;

            RealType result = 0.0;
            if(p.dy()>0.0){
                result = limits_t::negative_infinity();
            }else{
                result = limits_t::infinity();
            }
            return result;
        }
        template<typename RealType>
        RealType
        tangent_at_infinity(const point<RealType>& p){
            typedef  math_limit::limits<RealType>  limits_t;
            RealType result = 0.0;
            if(p.dy()>0.0){
                result = limits_t::infinity();
            }else{
                result = limits_t::negative_infinity();
            }
            return result;
        }
        template<typename RealType>
        RealType tangent_at_finite(const point<RealType>& p,RealType x){
            return p.y()+(x-p.x()) * p.dy();
        }

        template<typename RealType>
        RealType tangent(const point<RealType>& p,RealType x){
            typedef  math_limit::limits<RealType>  limits_t;
            if(limits_t::is_infinity(x)){
                return tangent_at_infinity(p);
            }else{
                if(limits_t::is_negative_infinity(x)){
                    return tangent_at_negative_infinity(p);
                }else{
                    return tangent_at_finite(p,x);
                }
            }
        }


        template<typename RealType>
        RealType
        tangent_intersection(
            const point<RealType>& p0,
            const point<RealType>& p1
        ){
            typedef RealType value_t;
            value_t x0 = p0.x();
            value_t x1 = p1.x();
            value_t dy0 = p0.dy();
            value_t dy1 = p1.dy();
            std::string str = "adaptive_rejection_sampling::";
            str += "tangent_intersection";

            if(!(x0<x1)){
                str += "!(x0=%1%<x1=%2%)";
                format f(str);
                f%x0%x1;
                throw std::runtime_error(f.str());
            }
            if(!(dy0>dy1)){
                str += "!(dy0=%1%>dy1=%2%)";
                format f(str);
                f%x0%x1;
                throw std::runtime_error(f.str());
            }
            value_t zero = (value_t)(0);
            value_t t0 = tangent(p0,zero);
            value_t t1 = tangent(p1,zero);
            return -(t1-t0)/(dy1-dy0);
        }

        template<typename RealType>
        struct less{
            less(){};
            bool operator()(
                const point<RealType>& p1,
                const point<RealType>& p2
            )const{
                return (p1.x()<p2.x());
            }
        };

}
}

#endif // POINT_HPP_INCLUDED
