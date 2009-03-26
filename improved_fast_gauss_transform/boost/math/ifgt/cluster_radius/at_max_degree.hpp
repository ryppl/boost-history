//////////////////////////////////////////////////////////////////////////////
// at_max_degree.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_OPTIMAL_PARAMETER_GIVEN_MAX_DEGREE_HPP_ER_2009
#define BOOST_MATH_IFGT_OPTIMAL_PARAMETER_GIVEN_MAX_DEGREE_HPP_ER_2009
#include <stdexcept>
#include <boost/math/ifgt/detail/divider.hpp>
#include <boost/math/ifgt/cutoff_radius/rydg.hpp>
#include <boost/math/ifgt/cluster_radius/at_max_degree.hpp>
#include <boost/math/ifgt/truncation_degree/properties.hpp>
namespace boost{
namespace math{
namespace ifgt{


/// Finds optimal max cluster radius recursively,
/// given a cutoff radius policy and a maximum degree p.
// P0 : CutoffRadiusPolicy
// P1 : UpdatePolicy
template<
    typename RealType,
    template<typename> class P0 = cutoff_radius::rydg, //TODO lambda
    template<typename> class P1 = divider //TODO lambda
>
class cluster_radius_at_max_degree{
    typedef P0<RealType>                 p_0_t;
    typedef P1<RealType>                 p_1_t;
    public:
    typedef RealType                               value_type;
    typedef std::size_t                            size_type;
    template<typename ArgPack> cluster_radius_at_max_degree(
        const ArgPack& arg);
    cluster_radius_at_max_degree(
        const cluster_radius_at_max_degree& that);
    cluster_radius_at_max_degree&
    operator=(const cluster_radius_at_max_degree& that);

    value_type  error_tolerance()const{return error_tol_; };
    unsigned    max_degree()const{ return max_degree_; };
    value_type  bandwidth()const{return bandwidth_; };//models ClusterPolicy
    size_type   max_recursions()const{ return max_recursions_; }

    /// Set parameters
    void  operator()(
        unsigned max_degree,
        value_type bandwidth,
        value_type start_cluster_radius_over_bandwidth = 2.0,
        size_type max_recursions = 20);

    /// Output
    value_type  cutoff_radius()const{ return cutoff_radius_; }
    //models ClusterPolicy
    value_type  max_radius()const{ return max_cluster_radius_; }
    value_type  error()const { return error_; }
    size_type   recursions()const{ return recursions_; }

    private:
    void call_impl();
    value_type                      error_tol_;
    p_0_t                           p_0_;
    p_1_t                           p_1_;
    value_type                      bandwidth_;
    unsigned                        max_degree_;
    size_type                       max_recursions_;
    value_type                      cutoff_radius_;
    value_type                      max_cluster_radius_;
    value_type                      error_;
    size_type                       recursions_;
};

template<typename RealType,template<typename> class P0,
template<typename> class P1>
template<typename ArgPack>
cluster_radius_at_max_degree<RealType,P0,
P1>::cluster_radius_at_max_degree(
    const ArgPack& arg)
:error_tol_(arg[kwd<>::error_tolerance]),
p_0_(arg),
p_1_(arg),
bandwidth_(0.0),
max_degree_(0.0),
max_recursions_(0),
cutoff_radius_(0.0),
max_cluster_radius_(0.0),
error_(0.0),
recursions_(0){}

template<typename RealType,template<typename> class P0,
template<typename> class P1>
cluster_radius_at_max_degree<RealType,P0,
P1>::cluster_radius_at_max_degree(
const cluster_radius_at_max_degree& that)
:error_tol_(that.error_tol_),
p_0_(that.p_0_),
p_1_(that.p_1_),
bandwidth_(that.bandwidth_),
max_degree_(that.max_degree_),
max_recursions_(0),
cutoff_radius_(that.cutoff_radius_),
max_cluster_radius_(that.max_cluster_radius_),
error_(that.error_),
recursions_(0){}

template<typename RealType, template<typename> class P0,
template<typename> class P1>
typename
cluster_radius_at_max_degree<RealType,P0,
P1>::cluster_radius_at_max_degree&
cluster_radius_at_max_degree<RealType,P0,
P1>::operator=(
    const cluster_radius_at_max_degree& that){
        if(&that!=this){
            error_tol_              = that.error_tol_;
            p_0_   = that.p_0_;
            p_1_          = that.p_1_;
            bandwidth_              = that.bandwidth_;
            max_degree_             = that.max_degree_;
            max_recursions_         = that.max_recursions_;
            cutoff_radius_          = that.cutoff_radius_;
            max_cluster_radius_     = that.max_cluster_radius_;
            error_                  = that.error_;
            recursions_             = that.recursions_;
        }
        return *this;
};

template<typename RealType,template<typename> class P0,
template<typename> class P1>
void  cluster_radius_at_max_degree<RealType,P0,
P1>::operator()(
        unsigned max_degree,
        value_type bandwidth,
        value_type start_cluster_radius_over_bandwidth,
        size_type max_recursions ){
    bandwidth_          = bandwidth;
    max_degree_         = max_degree;
    max_recursions_     = max_recursions;
    recursions_         = 0;
    max_cluster_radius_ = start_cluster_radius_over_bandwidth
        /this->bandwidth();
    call_impl();
};

template<typename RealType,template<typename> class P0,
template<typename> class P1>
void cluster_radius_at_max_degree<RealType,P0,
P1>::call_impl(){
    ++recursions_;
    cutoff_radius_      = p_0_(*this);
    error_              = truncation_degree::properties::max_error_bound(
        max_degree(), bandwidth(),
        max_radius(), cutoff_radius()
    );

    if(recursions()<max_recursions()){
        if(error()>error_tolerance()){
            max_cluster_radius_ = p_1_(max_radius());
            ++recursions_;
            call_impl();
        }
    }else{
        std::string str = "cluster_radius_at_max_degree";
        str += "max number of recursions exceeded";
        throw std::runtime_error(str);
    }
}


}//ifgt
}//math
}//boost

#endif
