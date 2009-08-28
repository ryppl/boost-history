///////////////////////////////////////////////////////////////////////////////
// statistics::kernel::functional::benchmark_rp.hpp                          //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#ifndef BOOST_STATISTICS_KERNEL_FUNCTIONAL_BENCHMARK_RP_H_ER_2009
#define BOOST_STATISTICS_KERNEL_FUNCTIONAL_BENCHMARK_RP_H_ER_2009
#include <cmath>
#include <ostream>
#include <string>
#include <boost/format.hpp>
#include <boost/timer.hpp>
#include <boost/foreach.hpp>
#include <boost/call_traits.hpp>
#include <boost/mpl/not.hpp>
#include <boost/range.hpp>
#include <boost/type_traits.hpp>
#include <boost/statistics/kernel/functional/detail/mean_accumulator.hpp>
#include <boost/statistics/kernel/functional/rp_visitor.hpp>
#include <boost/statistics/kernel/functional/estimator.hpp>

namespace boost{
namespace statistics{
namespace kernel{

// Computes the error in estimating the density with a particular kernel.
//
// Ae specifies the accumulator used to compute the error
//
// Let sx of type Sx denote a sequence of values and sp of type Sx
// their correponding density under a given probability distribution. Let
// sx1 of type Sx1 denote a (training) sample from the same distribution.
// 
// Usage:
// typedef benchmark_rp<Sx,Sp,K,Ae> bench_;
// bench_ bench(sx,sp);
// bench.set_kernel(bandwidth);
// bench(sx1);
// bench.error();
template<typename Sx,typename Sp,typename K, typename Ae  = 
    typename statistics::kernel::detail::mean_accumulator<typename K::result_type>::type
>
class benchmark_rp{
    public:
    typedef typename K::result_type value_type;
    typedef typename statistics::kernel::detail::mean_accumulator<value_type>::type acc_;
    private:
    typedef is_reference<Sx> is_ref_sx_;
    typedef is_reference<Sp> is_ref_sp_;
    typedef typename remove_reference<Sp>::type const_sp_;
    public:
    // Construct
    benchmark_rp();
    benchmark_rp(
        typename call_traits<Sx>::param_type,
        typename call_traits<Sp>::param_type,
        K k
    );
    benchmark_rp(
        typename call_traits<Sx>::param_type,
        typename call_traits<Sp>::param_type
    );
    benchmark_rp(const benchmark_rp&);
    benchmark_rp& operator=(const benchmark_rp&);

    void set_kernel(K k);

    // Update
    template<typename Sx1> void operator()(const Sx1& in_sample);
        
    // Access
    const K& kernel()const;
    const Ae& error_accumulator()const;
    value_type error()const;
    unsigned n()const; // in-sample size
    unsigned m()const; // out-sample size
    value_type time()const;

    static std::string header; 

    private:
    K k_;
    typename call_traits<Sx>::value_type sx_; 
    typename call_traits<Sp>::value_type sp_; 
    Ae error_acc_;
    value_type time_;
    unsigned n_;
};

template<typename Sx,typename Sp, typename K,typename Ae>
std::string benchmark_rp<Sx,Sp,K,Ae>::header = "(n, avg_t,e)";

template<typename Sx,typename Sp, typename K,typename Ae>
std::ostream& operator<<(std::ostream& out,const benchmark_rp<Sx,Sp,K,Ae>& b){
    static const char* str = "(%1%, %2%, %3%)";
    typedef benchmark_rp<Sx,Sp,K,Ae> bench_;
    typedef typename bench_::value_type value_type;
    value_type m_ = static_cast<value_type>(b.m());
    value_type n_ = static_cast<value_type>(b.n());
    value_type avg_t = (b.time()) / (m_); //Should increase linearly with n
    format f(str); 
    f%(b.n())%(avg_t)%(b.error());
    out << f.str();
    return out;
}


// Construction
template<typename Sx,typename Sp, typename K,typename Ae>
benchmark_rp<Sx,Sp,K,Ae>::benchmark_rp(){
    BOOST_MPL_ASSERT((mpl::not_<is_ref_sx_>));
    BOOST_MPL_ASSERT((mpl::not_<is_ref_sp_>));
}

template<typename Sx,typename Sp, typename K,typename Ae>
benchmark_rp<Sx,Sp,K,Ae>::benchmark_rp(
    typename call_traits<Sx>::param_type sx,
    typename call_traits<Sp>::param_type sp,
    K k
):k_(k),sx_(sx),sp_(sp),
error_acc_(),time_(static_cast<value_type>(0)){
    BOOST_ASSERT(size(sx) == size(sp));
}

template<typename Sx,typename Sp, typename K,typename Ae>
benchmark_rp<Sx,Sp,K,Ae>::benchmark_rp(
    typename call_traits<Sx>::param_type sx,
    typename call_traits<Sp>::param_type sp
):k_(K()),sx_(sx),sp_(sp),
error_acc_(),time_(static_cast<value_type>(0)){
    BOOST_ASSERT(size(sx) == size(sp));
}

template<typename Sx,typename Sp, typename K,typename Ae>
benchmark_rp<Sx,Sp,K,Ae>::benchmark_rp(const benchmark_rp& that)
:k_(that.k_),sx_(that.sx_),sp_(that.sp_),
error_acc_(that.error_acc_),time_(that.time_){}

template<typename Sx,typename Sp, typename K,typename Ae>
benchmark_rp<Sx,Sp,K,Ae>& 
benchmark_rp<Sx,Sp,K,Ae>::operator=(const benchmark_rp& that){
    if(&that!=this){
        BOOST_MPL_ASSERT((mpl::not_<is_ref_sx_>));
        BOOST_MPL_ASSERT((mpl::not_<is_ref_sp_>));
        k_ = (that.k_);
        sx_ = (that.sx_);
        sp_ = (that.sp_);
        error_acc_ = (that.error_acc_);
        time_ = (that.time_);    
    }
    return (*this);
}
    
template<typename Sx,typename Sp, typename K,typename Ae>
void benchmark_rp<Sx,Sp,K,Ae>::set_kernel(K k){
    k_ = k;
}

// Update
template<typename Sx,typename Sp, typename K,typename Ae>
template<typename Sx1>
void benchmark_rp<Sx,Sp,K,Ae>::operator()(const Sx1& data){
    typedef const Sx1& call_;
    typedef estimator<call_,rp_visitor,K,acc_> estimator_type;
    timer t;
    estimator_type estimator(data,this->kernel());
    typedef typename range_value<Sx1>::type unit_;
    typedef typename range_iterator<const_sp_>::type iter_y_;
    
    iter_y_ i = boost::begin(sp_);
    BOOST_FOREACH(const unit_& u, sx_){
        value_type a =  estimator(u).estimate();
        value_type b = (*i);
        error_acc_(fabs(a-b));
        ++i;
    }
    n_ = boost::size(data);
    time_ = t.elapsed();
}

// Access
template<typename Sx,typename Sp, typename K,typename Ae>
const K& benchmark_rp<Sx,Sp,K,Ae>::kernel()const{
    return (this->k_);
}

template<typename Sx,typename Sp, typename K,typename Ae>
const Ae& benchmark_rp<Sx,Sp,K,Ae>::error_accumulator()const{
    return (this->error_acc_);
}

template<typename Sx,typename Sp, typename K,typename Ae>
typename benchmark_rp<Sx,Sp,K,Ae>::value_type 
benchmark_rp<Sx,Sp,K,Ae>::error()const{
    return accumulators::mean(this->error_acc_);
}

template<typename Sx,typename Sp, typename K,typename Ae>
unsigned benchmark_rp<Sx,Sp,K,Ae>::n()const{
    return (this->n_);
}

template<typename Sx,typename Sp, typename K,typename Ae>
unsigned benchmark_rp<Sx,Sp,K,Ae>::m()const{
    return accumulators::count(this->error_acc_);
}

template<typename Sx,typename Sp, typename K,typename Ae>
typename benchmark_rp<Sx,Sp,K,Ae>::value_type 
benchmark_rp<Sx,Sp,K,Ae>::time()const{
    return (this->time_);
}
    
}// kernel
}// statistics
}// boost

#endif
