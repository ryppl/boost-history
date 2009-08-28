///////////////////////////////////////////////////////////////////////////////
// statistics::kernel::functional::rp_visitor.hpp                            //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#ifndef BOOST_STATISTICS_KERNEL_FUNCTIONAL_DENSITY_VISITOR_HPP_ER_2009
#define BOOST_STATISTICS_KERNEL_FUNCTIONAL_DENSITY_VISITOR_HPP_ER_2009
#include <algorithm>
#include <boost/call_traits.hpp>
#include <boost/call_traits.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/mpl/nested_type.hpp>
#include <boost/statistics/estimator_concept/meta/result_of_estimate.hpp>
#include <boost/statistics/kernel/functional/detail/mean_accumulator.hpp>

namespace boost{
namespace statistics{
namespace kernel{

    // R:   range of training_data 
    // V:   visitor
    // K:   kernel
    // A:   accumulator
    //
    // V                        R
    // rp_visitor               {x[i]:i=1,...,n}
    // nw_visitor_tuple         {(x,y)[i]:i=1,...,n}
    //
    // See statistics/estimator_concept
    // Models TrainableEstimator 
    // If V == nw_visitor_tuple, models TrainableRegressionEstimator 
    template<
        typename R, 
        template<typename,typename,typename> class V,
        typename K,
        typename A = typename 
            detail::mean_accumulator<typename K::result_type>::type 
    >
    class estimator{
    
        public:
        typedef R training_data_type;
        typedef K kernel_type;
        typedef A accumulator_type;

        template<typename X>
        struct result{
            typedef V<K,const X&,A> arg_;
            typedef typename mpl::nested_type<arg_>::type type;
        };
    
        // Constructor
        estimator();
        estimator(K k);
        estimator(K k,const A&);
        estimator(const estimator&);
        estimator& operator=(const estimator&);
    
        // Evaluate
        template<typename X>
        typename result<X>::type  operator()(const X& x)const;    

        // Access
        const R& train_data()const;
        const K& kernel()const;
        const A& accumulator()const;

        void set_train_data(const R& train_data);

    private:
        R train_data_;
        K k_;
        A a_; // Initialized accumulator
    };

}//kernel

    template<
        typename R, 
        template<typename,typename,typename> class V,
        typename K,
        typename A
    >
    void train(kernel::estimator<R,V,K,A>& e, const R& train_data){
        e.set_train_data(train_data);
    }

    template<
        typename R, 
        template<typename,typename,typename> class V,
        typename K, typename A, typename X
    >
    struct result_of_estimate< kernel::estimator<R,V,K,A>,X > {
        typedef kernel::estimator<R,V,K,A>  est_;
        typedef typename est_::template result<X>    meta_vis_;
        typedef typename meta_vis_::type    vis_;
        typedef typename vis_::result_type  type;
    };
    
    template<
        typename R, 
        template<typename,typename,typename> class V,
        typename K, typename A, typename X
    >
    typename result_of_estimate< kernel::estimator<R,V,K,A>, X >::type
    estimate(const kernel::estimator<R,V,K,A>& e, const X& x){
        return e(x).estimate();
    }

// Implementation //
namespace kernel{
    
// Constructor

template<
    typename R, 
    template<typename,typename,typename> class V,
    typename K,
    typename A
>
estimator<R,V,K,A>::estimator():train_data_(),k_(),a_(){}

template<
    typename R, 
    template<typename,typename,typename> class V,
    typename K,
    typename A
>
estimator<R,V,K,A>::estimator(K k):k_(k),a_(){}

template<
    typename R, 
    template<typename,typename,typename> class V,
    typename K,
    typename A
>
estimator<R,V,K,A>::estimator(K k,const A& a)
:train_data_(),k_(k),a_(a){}
    
template<
    typename R, 
    template<typename,typename,typename> class V,
    typename K,
    typename A
>
estimator<R,V,K,A>::estimator(const estimator& that)
:train_data_(that.train_data_),k_(that.k_),a_(that.a_){}
    
template<
    typename R, 
    template<typename,typename,typename> class V,
    typename K,
    typename A
>
estimator<R,V,K,A>& 
estimator<R,V,K,A>::operator=(const estimator& that){
    if(&that = this){
        this->train_data_ = that.train_data_;
        this->k_ = that.k_;
        this->a_ = that.a_;
    }
    return *this;
}

template<
    typename R, 
    template<typename,typename,typename> class V,
    typename K,
    typename A
>
void estimator<R,V,K,A>::set_train_data(const R& train_data){
    this->train_data_ = train_data;
}

// Evaluate
template<
    typename R, 
    template<typename,typename,typename> class V,
    typename K,
    typename A
>
    template<typename X>
typename estimator<R,V,K,A>::template result<X>::type 
estimator<R,V,K,A>::operator()(const X& x)const{
    typedef typename estimator<R,V,K,A>::template result<X>::type result_;
    return std::for_each(
        boost::begin( this->train_data() ),
        boost::end( this->train_data() ),
        result_( this->kernel(), x, this->a_)
    );
}

// Access
template<
    typename R, 
    template<typename,typename,typename> class V,
    typename K,
    typename A
>
const R&
estimator<R,V,K,A>::train_data()const{ return this->train_data_; }

template<
    typename R, 
    template<typename,typename,typename> class V,
    typename K,
    typename A
>
const K& 
estimator<R,V,K,A>::kernel()const{ return this->k_; }
    
template<
    typename R, 
    template<typename,typename,typename> class V,
    typename K,
    typename A
>
const A&
estimator<R,V,K,A>::accumulator()const{
    return this->a_;
}
    
}// kernel
}// statistics
}// boost

#endif