//////////////////////////////////////////////////////////////////////////////
// contribution.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_EXACT_CONTRIBUTION_HPP_ER_2009
#define BOOST_MATH_IFGT_EXACT_CONTRIBUTION_HPP_ER_2009
#include <algorithm>
#include <functional>
#include <numeric>
#include <cmath>
#include <stdexcept>
#include <boost/range.hpp>
#include <boost/assert.hpp>
#include <boost/utility.hpp>
#include <boost/algorithm/l2_distance_squared.hpp>
#include <boost/math/ifgt//detail/normal_kernel_properties.hpp>
#include <boost/iterator/range_same.hpp>

namespace boost{
namespace math{
namespace ifgt{
namespace exact{

    // A source, weight, and bandwidth
    // and associated operations.
    template<
        unsigned SourceSz,
        unsigned WeightSz,
        typename Cont = std::vector<double> > //TODO add cutoff radius policy
    class contribution{
     public:
        typedef typename Cont::value_type       value_type;
        typedef Cont                            source_type;
        typedef Cont                            weight_type;
        static const std::size_t source_size = SourceSz;
        static const std::size_t weight_size = WeightSz;
        template<typename R0,typename R1>
        contribution(
            const R0& source,
            const R1& weight,
            value_type bandwidth
        ):
            source_(source_size),
            weight_(weight_size),
            bandwidth_(bandwidth)
        {
            BOOST_ASSERT(size(source_)-source_size==0);
            BOOST_ASSERT(size(weight_)-weight_size==0);

            copy(
                begin(source),
                end(source),
                begin(source_)
            );

            copy(
                begin(weight),
                end(weight),
                begin(weight_)
            );
        }

        contribution(const contribution& that)
        :   source_(that.source_),
            weight_(that.weight_),
            bandwidth_(that.bandwidth_){}

        contribution& operator=(
            const contribution& that){
            if(&that!=this){
                source_     = that.source_;
                weight_     = that.weight_;
                bandwidth_  = that.bandwidth_;
            }
            return *this;
        }

        const source_type& source()const{ return source_; }
        const source_type& weight()const{ return weight_; }
        value_type bandwidth()const{ return bandwidth_; }
        value_type normalizing_constant()const{
            static value_type sqrt2 = sqrt((value_type)(2));
            typedef normal_kernel_properties<
                source_size,value_type>                             nkp_type;
            static value_type result
                = nkp_type::normalizing_constant(bandwidth()/sqrt2);
            return result;
        }
        template<typename R>
        value_type unnormalized_gauss_kernel(const R& target)const{
            BOOST_ASSERT(size(target)-source_size==0);
            typedef l2_distance_squared::type dist_type;
            static dist_type dist;
            static value_type squared_bandwidth
                = (bandwidth() * bandwidth());
            value_type zscore_sqnorm
                = ( dist(source_,target) / squared_bandwidth );
            return exp(-zscore_sqnorm);
        };

        template<typename R>
        value_type gauss_kernel(const R& target)const{
            return (unnormalized_gauss_kernel(target)/normalizing_constant());
        };

        template<typename R0,typename R1>
        void evaluate(const R0& target,R1& range_out)const{
            static range_same policy;
            return evaluate(target,range_out,policy);
        }
        /// \warning *adds* to range_out
        template<typename R0,typename R1,typename SubsetRangePolicy>
        void evaluate(const R0& target,R1& range_out,
            const SubsetRangePolicy& policy)const{


            value_type gk = gauss_kernel(target);

            typedef const weight_type&                      arg_type;
            typedef typename result_of<SubsetRangePolicy(arg_type)>::type
                                                            subset_type;
            subset_type subset = policy(weight_);

            BOOST_ASSERT(
                size(subset)==size(range_out)
            );

            transform(
                begin(subset),
                end(subset),
                begin(range_out),
                begin(range_out),
                bind(
                    std::plus<value_type>(),
                    bind(
                        std::multiplies<value_type>(),
                        _1,
                        gk
                    ),
                    _2
                )
            );
        }

        template<typename R0>
        value_type
        nadaraya_watson_normalizing_constant(const R0& target)const{
            return gauss_kernel(target);
        }

    private:
        contribution();//dont implement
        source_type     source_;
        weight_type     weight_;
        value_type      bandwidth_;
   };

}
}
}
}
#endif
