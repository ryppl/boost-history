///////////////////////////////////////////////////////////////////////////////
// prior::wrap::unary::prior.hpp                                             //
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
    class prior_ : detail::base_<D>{
        typedef detail::base_<D> base_;
    public:
        typedef D                 prior_type;
        typedef prior_<D>         prior_w_;

            
        // Construction
        prior_();
        prior_(const D& d);
        prior_(const prior_&);
            
        // Access
        const D& prior()const;
    };

    // Implementation //
            
    template<typename D>
    prior_<D>::prior_() : base_(){}
        
    template<typename D>
    prior_<D>::prior_( const D& d ) : base_(d){}
        
    template<typename D>
    prior_<D>::prior_(const prior_& that)
    :base_(that){}

    template<typename D>
    const D& prior_<D>::prior()const{ return (this->value); }
                
    // Free functions
        
    template<typename D>
    prior_<D> 
    make_prior(const D& d){
        typedef prior_<D> result_type;
        return result_type(d);
    }

}// prior    
}// statistics
}// boost

#endif
