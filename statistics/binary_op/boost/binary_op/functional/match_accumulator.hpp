//////////////////////////////////////////////////////////////////////////////
// binary_op::match_accumulator.hpp                                         //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_BINARY_OP_FUNCTIONAL_MATCH_ACCUMULATOR_HPP_ER_2009
#define BOOST_BINARY_OP_FUNCTIONAL_MATCH_ACCUMULATOR_HPP_ER_2009
#include <boost/type_traits.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/concept_check.hpp>
#include <boost/concept/assert.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/function.hpp>
#include <boost/range.hpp>
#include <boost/call_traits.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/mpl/nested_type.hpp>

namespace boost{
namespace binary_op{
// Notation
// S is the type of a sequence of pairs, each of type P = tuple<H,A>. Let
// P1 = tuple<H,T> another pair type;
// Let ItH, and ItA denote iterators to the heads and tails, respectively.
// F_H = F<ItH> is the type of functor. Let s,p=(h,a),p=(h,t), f, and m denote
// instances of S, P, F_H and match_accumulator, respectively. Let [b_h,e_h) and
//  [b_a,e_a) denote the heads and tails in s. 
//
// Requirements:
// i=f(b_h,e_h,h) is an iterator in in [b_h,e_h]
//
// Operations:
// m(p1) calls i=f(b_h,e_h,h1), and
// if i!= e_h, calls a(t1) else makes a new p=(h1,a=A()); calls a(t1);
// and pushes p at the back of s.
//
// NB: H is typically a "feature" or covariate, used to partition a sample.
// A is typically an accumulator, and T the quantity to be accumulated.
    
template<typename S,typename F>
class match_accumulator {
    typedef typename remove_reference<S>::type tuples_type;
    typedef is_const<tuples_type> tuples_is_const_;
    typedef is_reference<tuples_type> tuples_is_ref_;
    typedef is_reference<F> f_is_ref_;
    BOOST_MPL_ASSERT((
        mpl::not_<tuples_is_const_>
    ));
    typedef typename range_reference<tuples_type>::type reference_type;
    typedef typename range_iterator<tuples_type>::type iterator_type;
    typedef typename range_value<tuples_type>::type value_type;
    
    template<unsigned i>
    struct element{
        typedef typename 
            boost::tuples::element<i,value_type>::type value_; 
        typedef typename add_reference<value_>::type reference_;
        typedef reference_ (sig) (reference_type);
        struct get{
            typedef reference_ result_type;
            get(){}
            result_type operator()(reference_type t)const{ 
                return tuples::get<i>(t); 
            }
        };
    
    };

    typedef element<0> head_;
    typedef element<1> tail_;

    public:

    // Constructor    
    match_accumulator();
    match_accumulator(
        typename call_traits<S>::param_type tuples
    );
    match_accumulator(
        typename call_traits<S>::param_type tuples,
        typename call_traits<F>::param_type f
    );
    match_accumulator(const match_accumulator& that);
    match_accumulator& operator=(const match_accumulator& that);

    // TODO see below for loosening the requirement
    BOOST_CONCEPT_ASSERT((Sequence<tuples_type>));

    // Update
    typedef bool result_type;
    template<typename T>
    result_type match_accumulator::operator()(const T& t);

    //Access
    typename call_traits<S>::const_reference tuples()const;

    private:
    template<typename T> iterator_type find(const T& t)const;
    template<typename T> void process(reference_type p, const T& t);
    template<typename T> void new_(const T& t);
    typename call_traits<S>::value_type tuples_;
    typename call_traits<F>::value_type f_;
};
        
template<typename S,typename F>
match_accumulator<S,F>::match_accumulator():tuples_(),f_(){
    BOOST_MPL_ASSERT((mpl::not_<tuples_is_ref_>));
    BOOST_MPL_ASSERT((mpl::not_<f_is_ref_>));
}

template<typename S,typename F>
match_accumulator<S,F>::match_accumulator(
    typename call_traits<S>::param_type tuples
):tuples_(tuples),f_(){
    BOOST_MPL_ASSERT((mpl::not_<f_is_ref_>));
}

template<typename S,typename F>
match_accumulator<S,F>::match_accumulator(
    typename call_traits<S>::param_type tuples,
    typename call_traits<F>::param_type f
):tuples_(tuples),f_(f){}

template<typename S,typename F>
match_accumulator<S,F>::match_accumulator(const match_accumulator& that)
:tuples_(that.tuples_),f_(that.f_){}

template<typename S,typename F>
match_accumulator<S,F>& match_accumulator<S,F>::operator=(
    const match_accumulator& that){
    if(&that!=this){
        BOOST_MPL_ASSERT((mpl::not_<tuples_is_ref_>));
        BOOST_MPL_ASSERT((mpl::not_<f_is_ref_>));
        tuples_ = that.tuples_;
        f_ = that.f_;
    }
    return *this;
}

//Query
template<typename S,typename F>
typename call_traits<S>::const_reference 
match_accumulator<S,F>::tuples()const{ return this->tuples_; }

//Update        
template<typename S,typename F>
template<typename T>
typename match_accumulator<S,F>::result_type 
match_accumulator<S,F>::operator()(const T& t){
    iterator_type it = this->find(t); 
    if(it!=boost::end(tuples_)){
        this->process(*it,t);
        return false;
    }else{
        this->new_(t); 
        return true;
    }
}

//Private

template<typename S,typename F>
template<typename T>
void match_accumulator<S,F>::process(reference_type p, const T& t){
    //typename tail_::reference_ tail = get<1>(p);
    (get<1>(p))(get<1>(t));
}    

template<typename S,typename F>
template<typename T>
void match_accumulator<S,F>::new_(const T& t){
    typename tail_::value_ tp;
    value_type p(get<0>(t),tp);
    // iterator_type b = begin(tuples);
    iterator_type back = (this->tuples_).insert(
        boost::end(this->tuples_), 
        p
    );
    this->process(*back,t);
    { 
        // TODO, conditional on mpl::not_<is_container> i.e. range
        // tuples_ tuples_type(b,next(back));
    }
}    

template<typename S,typename F>
template<typename T>
typename match_accumulator<S,F>::iterator_type
match_accumulator<S,F>::find(const T& t)const{
//    function<typename head_::sig> get_hp = get<0>; //fails

    typedef typename head_::get get_;
    return this->f_(
        make_transform_iterator(
            boost::begin(tuples_),
            get_()
        ),
        make_transform_iterator(
            boost::end(tuples_),
            get_()
        ),
        get<0>(t)
    ).base();
}
    
}// binary_op
}// boost


#endif
