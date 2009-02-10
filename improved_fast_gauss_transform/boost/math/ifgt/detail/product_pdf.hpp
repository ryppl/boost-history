//////////////////////////////////////////////////////////////////////////////
// product_pdf.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_PRODUCT_PDF_HPP_ER_2009
#define BOOST_MATH_IFGT_PRODUCT_PDF_HPP_ER_2009
#include <cmath>
#include <numeric>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/math/distributions/chi_squared.hpp>
namespace boost{namespace math{namespace ifgt{
    //TODO not a part of ifgt per se, but used in some of the
    // example/*.cpp files
    template<typename Dist>
    class product_pdf{
        public:
        typedef typename Dist::value_type                   result_type;
        product_pdf(const Dist& dist):call_log_pdf_(dist){}
        product_pdf(const product_pdf& that)
         : call_log_pdf_(that.call_log_pdf_){}
        product_pdf& operator=(const product_pdf& that){
            if(&that!=this){
                call_log_pdf_ = that.call_log_pdf_;
            }
            return *this;
        }

        template<typename R>
        result_type operator()(const R& x)const{
                result_type log_pdf = std::accumulate(
                    begin(x),
                    end(x),
                    (result_type)(0),
                    bind(
                        cref(call_log_pdf_),
                        _1,
                        _2
                    )
                );
                return exp(log_pdf);
        };
        private:
        product_pdf();
        class call_log_pdf{
            public:
            typedef typename product_pdf::result_type result_type;
            typedef result_type first_argument_type;
            typedef result_type second_argument_type;
            call_log_pdf(const Dist& dist):dist_(dist){}
            call_log_pdf(const call_log_pdf& that):dist_(that.dist_){}
            call_log_pdf& operator()(const call_log_pdf& that){
                if(&that!=this){
                    dist_ = that.dist_;
                }
                return dist_;
            }

            result_type operator()(result_type x,result_type y)const{
                return x+log(pdf(dist_,y));
            }
            private:
            call_log_pdf();
            Dist dist_;
        };
        call_log_pdf call_log_pdf_;
    };

    template<typename Dist>
    product_pdf<Dist> make_product_pdf(const Dist& dist){
        return product_pdf<Dist>(dist);
    };


}}}
#endif
