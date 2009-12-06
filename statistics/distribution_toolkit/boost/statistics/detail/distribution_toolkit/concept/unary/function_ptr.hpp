/*
//////////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::distributions::concept::unary::function_ptr.hpp        //
//                                                                              //
//  (C) Copyright 2009 Erwann Rogard                                            //
//  Use, modification and distribution are subject to the                       //
//  Boost Software License, Version 1.0. (See accompanying file                 //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)            //
//////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_DISTRIBUTION_TOOLKIT_CONCEPT_UNARY_FUNCTION_PTR_HPP_ER_2009
#define BOOST_DISTRIBUTION_TOOLKIT_CONCEPT_UNARY_FUNCTION_PTR_HPP_ER_2009
#include <boost/concept_check.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/pointer.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace toolkit{

namespace concept{

    template<typename D,template<typename> class F>
    class FunctionPtr{
        public:

        typedef meta::pointer<D> meta_;
        
        BOOST_CONCEPT_USAGE(FunctionPtr){
            ptr = meta_::template make<F>();
        }

        private:
        typename meta_::type ptr;
    };

}// concept
}// distribution
}// toolkit

}// statistics
}// detail
}// boost

#endif
*/