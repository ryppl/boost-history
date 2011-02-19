//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2009 Neil Groves                                          //
//  Copyright (C) 2010 Manuel Peinado Gallego                               //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHAIN_PIPE_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHAIN_PIPE_ER_2010_HPP
#include <boost/mpl/void.hpp>
#include <boost/assign/v2/utility/chain/result.hpp>
#include <boost/assign/v2/utility/chain/adaptor.hpp>

// Design:
// - Original design was boost::chain in RangeEx
// - ER modified as follows:
//     - sets the underlying iterator's Reference to one which both inputs are
//     convertible to, and is lvalue-preserving
// - Finally, MPG proposed a way to compose chains http://gist.github.com/287791

namespace boost{
namespace assign{
namespace v2{
namespace result_of{

    template<typename R1, typename R2, typename Tag = use_default>
    struct chain : chain_aux::result<R1, R2, Tag>{};

}// result_of
namespace chain_aux{

#define BOOST_ASSIGN_V2_FRAMEWORK_CHAIN(U1) \
    template<typename R1, typename U2, typename Tag> \
    typename chain_aux::result<U1, U2, Tag>::type \
    operator|(U1 & r1, chain_aux::adaptor1<U2, Tag> const & h) \
    { \
        typedef chain_aux::result<U1, U2, Tag> caller_; \
        return caller_::call( r1, h.value ); \
    } \
\
/**/

BOOST_ASSIGN_V2_FRAMEWORK_CHAIN(R1      )
BOOST_ASSIGN_V2_FRAMEWORK_CHAIN(R1 const)
#undef BOOST_ASSIGN_V2_FRAMEWORK_CHAIN

}// chain_aux

}// v2
}// assign
}// boost

#endif
