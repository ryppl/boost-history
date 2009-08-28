///////////////////////////////////////////////////////////////////////////////
// statistics::kernel::functional::benchmark_nw.hpp                          //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#ifndef BOOST_STATISTICS_KERNEL_FUNCTIONAL_BENCHMARK_NW_H_ER_2009
#define BOOST_STATISTICS_KERNEL_FUNCTIONAL_BENCHMARK_NW_H_ER_2009
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
#include <boost/binary_op/data/tuple_range.hpp>
#include <boost/statistics/kernel/functional/detail/mean_accumulator.hpp>
#include <boost/statistics/kernel/functional/nw_visitor_tuple.hpp>
#include <boost/statistics/kernel/functional/estimator.hpp>

namespace boost{
namespace statistics{
namespace kernel{
        
// Computes the error in estimating the conditional mean with a 
// particular kernel. see benchmark_rp.hpp
template<typename Sx,typename Sp,typename Sy,typename K, typename Ae  = 
        typename detail::mean_accumulator<typename K::result_type>::type
>
class benchmark_nw{
public:
    typedef typename K::result_type value_type;
    typedef typename statistics::kernel::detail::mean_accumulator<value_type>::type acc_;
    private:
    typedef is_reference<Sx> is_ref_sx_;
    typedef is_reference<Sp> is_ref_sp_;
    typedef is_reference<Sp> is_ref_sy_;
    typedef typename remove_reference<Sx>::type const_sx_;
    typedef typename remove_reference<Sp>::type const_sp_;
    typedef typename remove_reference<Sy>::type const_sy_;
    public:
    // Construct
    benchmark_nw();
    benchmark_nw(
        typename call_traits<Sx>::param_type,
        typename call_traits<Sp>::param_type,
        typename call_traits<Sy>::param_type,
        K k
    );
    benchmark_nw(
        typename call_traits<Sx>::param_type,
        typename call_traits<Sp>::param_type,
        typename call_traits<Sy>::param_type
    );
    benchmark_nw(const benchmark_nw&);
    benchmark_nw& operator=(const benchmark_nw&);
        
    void set_kernel(K k);
        
    // Update
    template<typename Sx1,typename Sy1> 
    void operator()(const Sx1& in_x,const Sy1& in_y);
        
    // Access
    const K& kernel()const;
    const Ae& error_accumulator_p()const;
    const Ae& error_accumulator_y()const;
    value_type error_p()const;
    value_type error_y()const;
    unsigned n()const; // in-sample size
    unsigned m()const; // out-sample size
    value_type time()const;
        
    static std::string header; 
        
    private:
    K k_;
    typename call_traits<Sx>::value_type sx_; 
    typename call_traits<Sp>::value_type sp_; 
    typename call_traits<Sy>::value_type sy_;
    Ae error_acc_p_;
    Ae error_acc_y_;
    value_type time_;
    unsigned n_;
};
    
template<typename Sx,typename Sp, typename Sy,typename K,typename Ae>
std::string benchmark_nw<Sx,Sp,Sy,K,Ae>::header = "(n, avg_t,e_p,e_y)";
    
template<typename Sx,typename Sp, typename Sy,typename K,typename Ae>
std::ostream& 
operator<<(std::ostream& out,const benchmark_nw<Sx,Sp,Sy,K,Ae>& b){
    static const char* str = "(%1%, %2%, %3%, %4%)";
    typedef benchmark_nw<Sx,Sp,Sy,K,Ae> bench_;
    typedef typename bench_::value_type value_type;
    value_type m_ = static_cast<value_type>(b.m());
    value_type n_ = static_cast<value_type>(b.n());
    value_type avg_t = (b.time()) / (m_); //Should increase linearly with n
    format f(str); 
    f%(b.n())%(avg_t)%(b.error_p())%(b.error_y());
    out << f.str();
    return out;
}

// Construction
template<typename Sx,typename Sp, typename Sy,typename K,typename Ae>
benchmark_nw<Sx,Sp,Sy,K,Ae>::benchmark_nw(){
    BOOST_MPL_ASSERT((mpl::not_<is_ref_sx_>));
    BOOST_MPL_ASSERT((mpl::not_<is_ref_sp_>));
    BOOST_MPL_ASSERT((mpl::not_<is_ref_sy_>));
}
    
template<typename Sx,typename Sp, typename Sy,typename K,typename Ae>
benchmark_nw<Sx,Sp,Sy,K,Ae>::benchmark_nw(
    typename call_traits<Sx>::param_type sx,
    typename call_traits<Sp>::param_type sp,
    typename call_traits<Sy>::param_type sy,
    K k
):k_(k),sx_(sx),sp_(sp),sy_(sy),
error_acc_p_(),error_acc_y_(),time_(static_cast<value_type>(0)){}
    
template<typename Sx,typename Sp, typename Sy,typename K,typename Ae>
benchmark_nw<Sx,Sp,Sy,K,Ae>::benchmark_nw(
    typename call_traits<Sx>::param_type sx,
    typename call_traits<Sp>::param_type sp,
    typename call_traits<Sy>::param_type sy
):k_(K()),sx_(sx),sp_(sp),sy_(sy),
error_acc_p_(),error_acc_y_(),time_(static_cast<value_type>(0)){}
    
template<typename Sx,typename Sp, typename Sy,typename K,typename Ae>
benchmark_nw<Sx,Sp,Sy,K,Ae>::benchmark_nw(const benchmark_nw& that)
:k_(that.k_),sx_(that.sx_),sp_(that.sp_),sy_(that.sy_),
error_acc_p_(that.error_acc_p_),error_acc_y_(that.error_acc_y_),
time_(that.time_){}
    
template<typename Sx,typename Sp, typename Sy,typename K,typename Ae>
benchmark_nw<Sx,Sp,Sy,K,Ae>& 
benchmark_nw<Sx,Sp,Sy,K,Ae>::operator=(const benchmark_nw& that){
    if(&that!=this){
        BOOST_MPL_ASSERT((mpl::not_<is_ref_sx_>));
        BOOST_MPL_ASSERT((mpl::not_<is_ref_sp_>));
        BOOST_MPL_ASSERT((mpl::not_<is_ref_sy_>));
        k_ = (that.k_);
        sx_ = (that.sx_);
        sp_ = (that.sp_);
        sy_ = (that.sy_);
        error_acc_p_ = (that.error_acc_p_);
        error_acc_p_ = (that.error_acc_y_);
        time_ = (that.time_);    
    }
    return (*this);
}
    
template<typename Sx,typename Sp, typename Sy,typename K,typename Ae>
void benchmark_nw<Sx,Sp,Sy,K,Ae>::set_kernel(K k){
    k_ = k;
}
    
// Update
template<typename Sx,typename Sp, typename Sy,typename K,typename Ae>
template<typename Sx1,typename Sy1>
void benchmark_nw<Sx,Sp,Sy,K,Ae>::operator()(const Sx1& sx1,const Sy1& sy1){
    typedef const Sx1& call_x1_;
    typedef const Sy1& call_y1_;
    typedef binary_op::tuple_range<call_x1_,call_y1_> factory_;
    typedef typename factory_::type range_tuple_;
    typedef estimator<range_tuple_,nw_visitor_tuple,K,acc_> estimator_type;

    timer t;
    range_tuple_ range_tuple = factory_::make(sx1,sy1);
    estimator_type estimator(this->kernel());
    train(estimator,range_tuple);
        
    typedef typename range_iterator<const_sx_>::type iter_sx_;
    typedef typename range_iterator<const_sp_>::type iter_sp_;
    typedef typename range_iterator<const_sy_>::type iter_sy_;
    typedef typename range_iterator<range_tuple_>::type iter_data_;

    iter_sx_ i_x = boost::begin(sx_);
    iter_sx_ e_x = boost::end(sx_);
    iter_sp_ i_p = boost::begin(sp_);
    iter_sy_ i_y = boost::begin(sy_);
    value_type a, b;
    while(i_x != e_x){
        BOOST_AUTO(obj,estimator(*i_x));
        a =  obj.rp_estimate();
        b = (*i_p);
        error_acc_p_(fabs(a-b));
        a =  obj.nw_estimate();
        b = (*i_y);
        error_acc_y_(fabs(a-b));
        ++i_x;
        ++i_p;
        ++i_y;
    }
    n_ = boost::size(range_tuple);
    time_ = t.elapsed();
}
    
// Access
template<typename Sx,typename Sp, typename Sy,typename K,typename Ae>
const K& benchmark_nw<Sx,Sp,Sy,K,Ae>::kernel()const{
    return (this->k_);
}
    
template<typename Sx,typename Sp, typename Sy,typename K,typename Ae>
const Ae& benchmark_nw<Sx,Sp,Sy,K,Ae>::error_accumulator_p()const{
    return (this->error_acc_p_);
}

template<typename Sx,typename Sp, typename Sy,typename K,typename Ae>
const Ae& benchmark_nw<Sx,Sp,Sy,K,Ae>::error_accumulator_y()const{
    return (this->error_acc_y_);
}
    
template<typename Sx,typename Sp, typename Sy,typename K,typename Ae>
typename benchmark_nw<Sx,Sp,Sy,K,Ae>::value_type 
benchmark_nw<Sx,Sp,Sy,K,Ae>::error_p()const{
    return accumulators::mean(this->error_acc_p_);
}

template<typename Sx,typename Sp, typename Sy,typename K,typename Ae>
typename benchmark_nw<Sx,Sp,Sy,K,Ae>::value_type 
benchmark_nw<Sx,Sp,Sy,K,Ae>::error_y()const{
    return accumulators::mean(this->error_acc_y_);
}
    
template<typename Sx,typename Sp, typename Sy,typename K,typename Ae>
unsigned benchmark_nw<Sx,Sp,Sy,K,Ae>::n()const{
    return (this->n_);
}
    
template<typename Sx,typename Sp, typename Sy,typename K,typename Ae>
unsigned benchmark_nw<Sx,Sp,Sy,K,Ae>::m()const{
    return accumulators::count(this->error_acc_p_);
}
    
template<typename Sx,typename Sp, typename Sy,typename K,typename Ae>
typename benchmark_nw<Sx,Sp,Sy,K,Ae>::value_type 
benchmark_nw<Sx,Sp,Sy,K,Ae>::time()const{
    return (this->time_);
}
    
}// kernel
}// statistics
}// boost

#endif
