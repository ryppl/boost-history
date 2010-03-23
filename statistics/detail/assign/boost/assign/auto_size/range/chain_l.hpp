//////////////////////////////////////////////////////////////////////////////
// assign::detail::range::chain_l.hpp                                       //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_RANGE_CHAIN_L_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_RANGE_CHAIN_L_ER_2010_HPP
#include <boost/mpl/int.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/void.hpp>
#include <boost/range.hpp>
#include <boost/range/chain.hpp>
#include <boost/assign/auto_size/range/result_of_chain.hpp>

// This is based on
// http://gist.github.com/287791
// developed by MPG, but allows lvalues.

// Usage : Let r1, r2 and r3 denote ranges,
//     boost::copy( from, boost::begin( chain_l(r1)(r2)(r3) ) );
// Note: if either of r, r2 or r3 is a range of reference wrappers, then all 
// three also have to. 

namespace boost{
namespace assign{
namespace detail{
namespace chain_l_impl{

    template<typename E,typename R1,int N>
	class expr;

    template<typename E,typename R2>
    struct next_expr{
        typedef expr<E,R2,E::static_size+1> type;
    };
    
    template<typename E,typename R1,int N>
    struct super_of_expr : detail::result_of::chain<typename E::super_,R1>{};

    template<typename E,typename R1>
    struct super_of_expr<E,R1,1>{
        typedef boost::sub_range<R1> type;
    };
    
    template<typename E,typename R1,int N>
	class expr : public super_of_expr<E,R1,N>::type{
        typedef boost::mpl::int_<1> int_1_;
        typedef boost::mpl::int_<N> int_n_;
        typedef typename boost::mpl::equal_to<int_1_,int_n_>::type is_first_;
        typedef typename boost::mpl::if_<is_first_,E,E&>::type previous_; 

        typedef expr<E,R1,N> this_;

        public:
        
        template<typename R2>
        struct result_impl : next_expr<this_,R2>{};
        
        BOOST_STATIC_CONSTANT(int,static_size = N);
        
        typedef typename super_of_expr<E,R1,N>::type super_;
    
        super_& super(){ return (*this); }
        const super_& super()const{ return (*this); }
    
        explicit expr(super_ s):super_(s){}
        explicit expr(E& p,super_ s):previous(p),super_(s){}

        template<typename R2>
        typename result_impl<R2>::type 
        operator()(R2& r2){
            typedef typename result_impl<R2>::type res_;
            typedef typename res_::super_ super_;
            return res_(
                *this,
                super_(
                    boost::chain(this->super(),r2)
                )
            );
        }

        mutable previous_ previous;
    };

    typedef boost::mpl::void_ top_;
    
    template<typename R1>
    struct first_expr{ 
        typedef expr<top_,R1,1> type; 
        typedef typename type::super_ super_;
        static type call(R1& r1){
            return type(super_(r1));
        }
    };
    
}// chain_l_impl
}// detail

    template<typename R1>
    typename detail::chain_l_impl::first_expr<R1>::type    
    chain_l(R1& r1){
        typedef detail::chain_l_impl::first_expr<R1> caller_;
        return caller_::call(r1);
    }

}// assign
}// boost

#endif