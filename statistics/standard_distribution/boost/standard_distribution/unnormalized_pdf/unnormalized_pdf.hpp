//////////////////////////////////////////////////////////////////////////////
// distributions::unnormalized_pdf::unnormalized_pdf.hpp                    //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STANDARD_DISTRIBUTION_UNNORMALIZED_PDF_UNNORMALIZED_HPP_ER_2009
#define BOOST_STANDARD_DISTRIBUTION_UNNORMALIZED_PDF_UNNORMALIZED_HPP_ER_2009
#include <cmath>

namespace boost{
namespace math{

template<typename D>
typename D::value_type  
log_unnormalized_pdf( //Fwd Declare
    const D& d,
    const typename D::value_type& x
);
        
template<typename D>
typename D::value_type  
unnormalized_pdf(
    const D& d,
    const typename D::value_type& x
){
    typedef typename D::value_type value_type;
    value_type l = log_unnormalized_pdf(d,x);
    return exp(l);
}

}
}
#endif

