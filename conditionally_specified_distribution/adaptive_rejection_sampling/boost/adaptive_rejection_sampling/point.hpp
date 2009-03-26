////////////////////////////////////////////////////////////////////////////
// point.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ADAPTIVE_REJECTION_SAMPLING_POINT_HPP_ER_2009
#define BOOST_ADAPTIVE_REJECTION_SAMPLING_POINT_HPP_ER_2009
#include <string>
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

            std::string as_string()const{
                std::string str ="x = %1%, y = %2%, dy = %3%";
                format f(str);
                f%x()%y()%dy();
                return f.str();
            }
            private:
            value_type x_;
            value_type y_;
            value_type dy_;
        };

        template<typename RealType>
        RealType
        linear_approximation(
            const point<RealType>& p0,
            const point<RealType>& p1,
            RealType x
        )
        {   try{
                if(x<=p1.x()){
                }else{
                    std::string str = "x<=p1.x()";
                    throw std::runtime_error(str);
                }
                if(p0.x()<=x){}else{
                    std::string str = "p0.x()<=x";
                    throw std::runtime_error(str);
                }
                if(p0.x()<p1.x()){}else{
                    std::string str = "p0.x()<p1.x()";
                    throw std::runtime_error(str);
                }
            }catch(std::exception& e){
                std::string str = "linear_approximation(p0,p1,x) : ";
                str+= e.what();
                str+= " p0 : "; str+= p0.as_string();
                str+= " p1 : "; str+= p1.as_string();
                str+= " x = %1%.";
                format f(str); f%x;
                throw std::runtime_error(f.str());
            }
            RealType result =  (x-p0.x()) * (p1.y()-p0.y());
            result /= (p1.x()-p0.x());
            result += p0.y();
            return result;
        }


        template<typename RealType>
        RealType tangent(const point<RealType>& p,RealType x){
            return p.y()+(x-p.x()) * p.dy();
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
