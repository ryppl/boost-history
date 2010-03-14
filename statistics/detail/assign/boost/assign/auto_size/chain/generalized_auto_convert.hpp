//////////////////////////////////////////////////////////////////////////////
// assign::detail::generalized_auto_convert.hpp                             //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_GENERALIZED_AUTO_CONVERT_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_GENERALIZED_AUTO_CONVERT_ER_2010_HPP
#include <boost/mpl/vector.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/assign/auto_size/chain/auto_convert.hpp>
#include <boost/assign/auto_size/chain/generalized_chain.hpp>

namespace boost{
namespace assign{

namespace result_of{
namespace chain_auto_convert{
        
    namespace impl{
        template<typename Conv>
        struct binary{
            template<typename R1,typename R2> 
            struct apply
                : result_of::chain_auto_convert::generic<Conv,R1,R2>{};
        };
    }
    template<typename Conv,typename Seq>
    struct generalized_generic 
        : result_of::generalized_chain::generic<
            result_of::chain_auto_convert::impl::binary<Conv>,
            Seq
        >{};

    template<typename Seq>
    struct generalized_apply_conversion  
        : result_of::chain_auto_convert::generalized_generic<
            detail::pair_traits::meta::apply_conversion,
            Seq
        >
        {};

}// chain_auto_convert
}// result_of


    template<typename Conv,typename R1,typename R2,typename R3>
    typename result_of::chain_auto_convert::generalized_generic<
        Conv,
        boost::mpl::vector3<R1,R2,R3>
    >::type
    chain_auto_convert(R1& r1, R2& r2, R3& r3){
        typedef boost::mpl::vector3<R1,R2,R3> seq_;
        typedef result_of::chain_auto_convert::generalized_generic<
            Conv,
            seq_
        > caller_;
        return caller_::call(
           boost::fusion::make_vector(r1,r2,r3)
        );
    }    

    template<typename Conv,typename R1,typename R2,typename R3>
    typename result_of::chain_auto_convert::generalized_generic<
        Conv,
        boost::mpl::vector3<const R1,const R2,const R3>
    >::type
    chain_auto_convert(const R1& r1, const R2& r2, const R3& r3){
        typedef boost::mpl::vector3<const R1,const R2,const R3> seq_;
        typedef result_of::chain_auto_convert::generalized_generic<
            Conv,
            seq_
        > caller_;
        return caller_::call(
           boost::fusion::make_vector(r1,r2,r3)
        );
    }    

    // default :

    template<typename R1,typename R2,typename R3>
    typename result_of::chain_auto_convert::generalized_apply_conversion<
        boost::mpl::vector3<R1,R2,R3>
    >::type
    chain_auto_convert(R1& r1, R2& r2, R3& r3){
        typedef detail::pair_traits::meta::apply_conversion conv_;
        return chain_auto_convert<conv_,R1,R2,R3>(r1,r2,r3);
    }    

    template<typename R1,typename R2,typename R3>
    typename result_of::chain_auto_convert::generalized_apply_conversion<
        boost::mpl::vector3<const R1,const R2,const R3>
    >::type
    chain_auto_convert(const R1& r1, const R2& r2, const R3& r3){
        typedef detail::pair_traits::meta::apply_conversion conv_;
        return chain_auto_convert<conv_,R1,R2,R3>(r1,r2,r3);
    }    


    // TODO BOOST_PP for 5, 6 etc.

}// assign
}// boost

#endif
