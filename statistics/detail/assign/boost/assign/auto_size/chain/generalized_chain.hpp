//////////////////////////////////////////////////////////////////////////////
// assign::detail::generalized_chain.hpp                                    //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_GENERALIZED_CHAIN_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_GENERALIZED_CHAIN_ER_2010_HPP
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/fusion/include/iterator.hpp>
#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/end.hpp>

namespace boost{
namespace assign{
namespace result_of{
namespace generalized_chain{

    namespace impl{
    
    
        // There shouldn't be a need to add const, it's taken care of by
        // Binary?
        template<typename Binary,typename R1,typename R2>
        struct caller : Binary::template apply<R1,R2>{};
    
        template<typename F,typename L>
        struct exit_cond;

        template<typename Binary,typename F,typename L, 
            bool exit = exit_cond<F,L>::value>
        struct generic{
                          
            typedef typename boost::mpl::deref<F>::type arg1_;
            typedef typename boost::mpl::next<F>::type next_it_;
            typedef typename impl::generic<Binary,next_it_,L> next_;
            typedef typename next_::type arg2_;
            typedef generalized_chain::impl::caller<
                Binary,
                typename boost::add_const<arg1_>::type,
                typename boost::add_const<arg2_>::type
            > caller_;
            typedef typename caller_::type type;

            BOOST_STATIC_CONSTANT(int,iteration =  next_::iteration + 1);
            
            template<typename F1,typename L1>
            static type call(const F1& f,const L1& l){
                 return caller_::call(
                     boost::fusion::deref(f), 
                     next_::call(boost::fusion::next(f),l)
                );
            }

            // Usage: call(boost::fusion::make_vector(r1,...,ri));
            template<typename Seq1>
            static type call(const Seq1& seq){
                return call(boost::fusion::begin(seq),boost::fusion::end(seq));
            }
             
        };
        
        template<typename F,typename L>
        struct exit_cond 
          : boost::mpl::equal_to<
              typename boost::mpl::distance<F,L>::type, 
              boost::mpl::int_<2> 
        >{};

        template<typename Binary,typename F,typename L>
        struct generic<Binary,F,L,true>{
            BOOST_STATIC_CONSTANT( int, iteration = 2 );

            typedef typename boost::mpl::deref<F>::type arg1_;
            typedef typename boost::mpl::next<F>::type next_it_;
            typedef typename boost::mpl::deref<next_it_>::type arg2_;
                        
            typedef generalized_chain::impl::caller<Binary,arg1_,arg2_> caller_;
            typedef typename caller_::type type;

            template<typename F1,typename L1>
            static type call(const F1& f, const L1& l){ 
                return  caller_::call(
                     boost::fusion::deref(f),
                     boost::fusion::deref(boost::fusion::next(f))
                ); 
            }
        };
    }        

    template<typename Binary,typename Seq>
    struct generic : result_of::generalized_chain::impl::generic<
        Binary,
        typename boost::mpl::begin<Seq>::type,
        typename boost::mpl::end<Seq>::type
    >{};
    
}// generalized_chain
}// result_of
}// assign
}// boost

#endif
