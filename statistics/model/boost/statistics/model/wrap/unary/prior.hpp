///////////////////////////////////////////////////////////////////////////////
// statistics::model::prior::wrap::unary::prior.hpp                          //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_WRAP_UNARY_PRIOR_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_WRAP_UNARY_PRIOR_HPP_ER_2009
#include <boost/statistics/model/wrap/unary/detail/base.hpp>

namespace boost{ 
namespace statistics{
namespace model{
        
    template<typename D>
    class prior_wrapper : detail::base_<D>{
        typedef detail::base_<D> base_;
    public:
        typedef D                 prior_type;
        typedef prior_wrapper<D>  prior_wrapper_type;

            
        // Construction
        prior_wrapper();
        prior_wrapper(const D& d);
        prior_wrapper(const prior_wrapper&);
            
        // Access
        const D& prior()const;
    };

    // Implementation //
            
    template<typename D>
    prior_wrapper<D>::prior_wrapper() : base_(){}
        
    template<typename D>
    prior_wrapper<D>::prior_wrapper( const D& d ) : base_(d){}
        
    template<typename D>
    prior_wrapper<D>::prior_wrapper(const prior_wrapper& that)
    :base_(that){}

    template<typename D>
    const D& prior_wrapper<D>::prior()const{ return (this->value); }
                
    // Free functions
        
    template<typename D>
    prior_wrapper<D> 
    make_prior_wrapper(const D& d){
        typedef prior_wrapper<D> result_type;
        return result_type(d);
    }

}// prior    
}// statistics
}// boost

#endif
