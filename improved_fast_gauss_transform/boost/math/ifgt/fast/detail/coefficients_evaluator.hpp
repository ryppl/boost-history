//////////////////////////////////////////////////////////////////////////////
// coefficients_evaluator.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_COLLECTED_SOURCES_EVALUATOR_HPP_ER_2009
#define BOOST_MATH_IFGT_COLLECTED_SOURCES_EVALUATOR_HPP_ER_2009
#include <vector>
#include <functional>
#include <iterator>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <iostream> //TODO remove
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/bind.hpp>
#include <boost/range.hpp>
#include <boost/math/ifgt/detail/zscore.hpp>
#include <boost/math/monomials_properties.hpp>
namespace boost{namespace math{namespace ifgt{

    // coefficients_evaluator keeps a ref to a target
    // each time a (coefficients,center,bandwidth) is passed to it,
    // it returns the gauss transform, if the target
    // is within a cutoff radius, 0 otherwise
    //
    // This class is primarly intended for use within clusters_evaluator.hpp

    /// R0 ForwardRange (that of the target)
    template<typename R0>
    class coefficients_evaluator{

        typedef typename range_value<R0>::type              value_type;
        typedef std::vector<value_type>                     vec_type;
        typedef typename range_size<R0>::type               target_size_type;
     public:
        typedef value_type                                  result_type;
        coefficients_evaluator(const R0& target):target_(target){}

        coefficients_evaluator(const coefficients_evaluator& that)
            :target_(that.target_){}

        const R0& target()const{return target_; }
        target_size_type target_size()const{return size(target()); }

        template<typename R1,typename Coeffs>
        result_type operator()(
            result_type cutoff_radius,
            const R1& center,
            result_type bandwidth,
            result_type inv_nc,
            const Coeffs& coeffs
        ){
            return (*this)(
                cutoff_radius,
                center,
                bandwidth,
                inv_nc,
                coeffs(),
                coeffs.max_degree()
            );
        }

        template<typename R1,typename R2>
        result_type operator()(
            //ignore target if dist to center less than:
            result_type cutoff_radius,
            const R1& center,
            result_type bandwidth,
            result_type inv_nc,
            const R2& coefficients,
            unsigned degree
        )const{
            typedef typename range_value<R0>::type value0_type;
            typedef typename range_value<R1>::type value1_type;
            typedef typename range_value<R2>::type value2_type;
            //TODO check that value0_type == ValueType, likewise 1,2
            BOOST_MPL_ASSERT((is_same<value_type,value1_type>));
            BOOST_MPL_ASSERT((is_same<value_type,value2_type>));
            BOOST_ASSERT((target_size_type)(size(center))==target_size());
            {
                unsigned n = monomials_properties<>::number_degree_less_than(
                    degree,(unsigned)(target_size())
                );
                BOOST_ASSERT((size(coefficients)-n>=0)||(degree == 0));
            }

            result_type res = (result_type)(0);
            if(degree>0){

                static vec_type                     zscore_val(target_size());
                static monomials<vec_type>          monoms;
                typedef zscore<vec_type>            zscore_type;
                {
                    zscore_type zscore(center);
                    zscore(target(),bandwidth,zscore_val);
                }

                result_type zscore_sqnorm = std::inner_product(
                    begin(zscore_val),
                    end(zscore_val),
                    begin(zscore_val),
                    (result_type)(0)
                );

                //TODO rename target_radius
                result_type sq_d = zscore_sqnorm * (bandwidth * bandwidth);

                // Previously, which is wrong
                // if( (sq_d<cutoff_radius)
                //    || (sq_d<cutoff_radius * cutoff_radius) ){
                // Takes of the case cutoff_radius = highest()

                result_type squared_cutoff = cutoff_radius * cutoff_radius;
                if( sq_d< squared_cutoff ){
                    result_type gauss_kernel
                        = inv_nc * exp(-zscore_sqnorm);
                    monoms(zscore_val,degree);
                    res = std::inner_product(
                        begin(coefficients),
                        end(coefficients),
                        begin(monoms()),
                        (result_type)(0),
                        std::plus<result_type>(),
                        bind(
                            std::multiplies<result_type>(),
                            gauss_kernel,
                            bind(
                                std::multiplies<result_type>(),
                                _1,
                                _2
                            )
                        )
                    );
                }
            }
            return res;
        }
      private:
        coefficients_evaluator& operator=(
            const coefficients_evaluator& that);
        const R0& target_;
    };

    template<typename R0>
    coefficients_evaluator<R0>
    make_coefficients_evaluator(const R0& target){
        return coefficients_evaluator<R0>(target);
    }

}}}

#endif
