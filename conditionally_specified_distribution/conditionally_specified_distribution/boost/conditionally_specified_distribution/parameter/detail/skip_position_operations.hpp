/////////////////////////////////////////////////////////////////////////////
// skip_position_operations.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_PARAMETER_DETAIL_SKIP_POSITION_OPERATIONS_HPP_ER2009
#define BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_PARAMETER_DETAIL_SKIP_POSITION_OPERATIONS_HPP_ER2009
#include <vector>
#include <boost/range.hpp>
#include <boost/algorithm/inner_product_skip_one_position.hpp>

namespace boost{
namespace conditionally_specified_distribution{
namespace parameter{
namespace detail{

/// $\eta_{i,-j} = x_{i,0} \beta_0 + ... + x_{i,-j}\beta_{-j}$
/// $x_{i,j+1} \beta_{j+1}+...+x_{i,J-1} \beta_{J}$


class skip_position_operations{
    public:
    typedef std::size_t                         size_type;
    skip_position_operations(size_type pos)
    :position_(pos){}

    skip_position_operations(const skip_position_operations& that)
    :position_(that.position_){}

    skip_position_operations&
    operator=(const skip_position_operations& that)
    {
        if(&that!=this){
            position_   = that.position_;
        }
        return *this;
    }
    size_type position()const{ return position_; }

    template<typename R>
    typename range_value<R>::type
    matching_element(const R& range)const{
        typedef typename range_iterator<const R>::type iter_type;
        iter_type iter = begin(range);
        std::advance(iter,position());
        return (*iter);
    }
    template<typename R0,typename R1>
    typename range_value<R0>::type
    inner_product_skip_position(const R0& r0,const R1& r1)const{
        typedef typename range_value<R0>::type value_type;
        return inner_product_skip_one_position(
            r0,
            r1,
            this->position(),
            (value_type)(0)
        );
    }

    private:
        skip_position_operations();
        size_type   position_;
};

}//detail
}//parameter
}//distribution
}//boost


#endif // REGRESSION_COEFFICIENT_HPP_INCLUDED
