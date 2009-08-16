//////////////////////////////////////////////////////////////////////////////
// scalar_dist::fun_wrap::detail::log_unnormalized_pdf.hpp                 //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SCALAR_DIST_FUN_WRAP_UNNORMALIZED_PDF_HPP_ER_2009
#define BOOST_SCALAR_DIST_FUN_WRAP_UNNORMALIZED_PDF_HPP_ER_2009
#include <boost/standard_distribution/unnormalized_pdf/unnormalized_pdf.hpp>
#include <boost/scalar_dist/fun_wrap/detail/fun_wrap.hpp>

namespace boost{
namespace math{
                
    namespace fun_wrap{    
        template<typename D>
        struct unnormalized_pdf_ 
            : detail::fun_wrap<D,math::unnormalized_pdf>{};
    }
}
}

#endif
