/////////////////////////////////////////////////////////////////////////////
// pair_independent.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_SUPPORT_PAIR_INDEPENDENT_HPP_ER_2009
#define BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_SUPPORT_PAIR_INDEPENDENT_HPP_ER_2009
#include <boost/conditionally_specified_distribution/result_of/function_support.hpp>
#include <boost/conditionally_specified_distribution/crtp/pair_independent.hpp>
namespace boost{
namespace conditionally_specified_distribution{
namespace function{

template <typename D,typename Args>
inline
typename function_support<D>::type
support(const crtp::pair_independent<D>& d,const Args& args)
{
    const D& d_ = static_cast<const D&>(d);

    // TODO :
    // pdf(x) = pdf_1(x)pdf_2(x) = 0 if either pdf_1(x) = 0 or pdf_2(y) =0
    // so support (x,y) is intersection(suppor_1,support_2)

    // For now, assume same support:
    return support(d_.first(args));
}

}
}
}
#endif // BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_SUPPORT_PAIR_INDEPENDENT_HPP_ER_2009
