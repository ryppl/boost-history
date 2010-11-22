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
#ifndef BOOST_ASSIGN_V2_RANGE_CHAIN_AUX_BITWISE_OR_ER_2010_HPP
#define BOOST_ASSIGN_V2_RANGE_CHAIN_AUX_BITWISE_OR_ER_2010_HPP
#include <boost/mpl/void.hpp>
#include <boost/assign/v2/chain/traits/result.hpp>
#include <boost/assign/v2/chain/aux_/adaptor.hpp>
#include <boost/assign/v2/chain/aux_/sub.hpp>

// Design:
// - The original design is boost::chain in RangeEx
// - ER modified as follows:
// 	- sets the underlying iterator's Reference to one which both inputs are
// 	convertible to.
// 	- Constness is determined by that of the elements in each range, not that
//  of the range.
// - Finally, MPG proposed a way to compose chains http://gist.github.com/287791
// and a variant of it is used below.

namespace boost{
namespace assign{
namespace v2{
namespace adaptor{
namespace{
	const chain_aux::adaptor2<> _chain = chain_aux::adaptor2<>();
}
}// adaptor
namespace result_of{

    template<typename R1, typename R2, typename Tag = use_default>
    struct chain : chain_aux::result<R1, R2, Tag>{};

}// result_of
namespace chain_aux{

#define BOOST_ASSIGN_V2_FRAMEWORK_CHAIN(U1) \
    template<typename R1,typename U2,typename Tag> \
    typename chain_aux::result<U1, U2, Tag>::type \
    operator|(U1 & r1, chain_aux::adaptor1<U2, Tag> const & h) \
    { \
        typedef chain_aux::result<U1, U2, Tag> caller_; \
        return chain_aux::make_sub( caller_::call( r1, h.value ) ); \
    } \
\
/**/

BOOST_ASSIGN_V2_FRAMEWORK_CHAIN(R1      )
BOOST_ASSIGN_V2_FRAMEWORK_CHAIN(R1 const)
#undef BOOST_ASSIGN_V2_FRAMEWORK_CHAIN

}// chain_aux

/*#define MACRO(U1, U2) \
    template<typename Tag,typename R1,typename R2> \
    typename chain_aux::result<U1, U2, Tag>::type \
    chain(U1 & r1, U2 & r2) \
    { \
        return r1 | adaptor::_chain( r2 ); \
    } \
\
MACRO(R1      , R2        )
MACRO(R1      , R2 const  )
MACRO(R1 const, R2        )
MACRO(R1 const, R2 const  )
#undef MACRO
*/


}// v2
}// assign
}// boost

#endif
