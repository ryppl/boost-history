///////////////////////////////////////////////////////////////////////////////
// binary_op::meta::detail::tuple_has_no.hpp                                 //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_BINARY_OP_META_TUPLE_HAS_NO_HPP_ER_2009
#define BOOST_BINARY_OP_META_TUPLE_HAS_NO_HPP_ER_2009
#include <boost/mpl/not.hpp>
#include <boost/mpl/or.hpp>

namespace boost{
namespace binary_op{
    
    namespace detail{
        template<typename T,template<unsigned,typename> class F>
        struct tuple_has_no
            : mpl::not_<
                mpl::or_<
                    F<0,T> ,
                    F<1,T>
                >
            >{};
    }
    
}// binary_op
}// boost


#endif
