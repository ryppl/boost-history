///////////////////////////////////////////////////////////////////////////////
// model::wrap::unary::model.hpp                                             //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_WRAP_UNARY_MODEL_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_WRAP_UNARY_MODEL_HPP_ER_2009
#include <boost/statistics/model/wrap/unary/detail/base.hpp>

namespace boost{ 
namespace statistics{
namespace model{

    template<typename M>
    class model_ : detail::base_<M>{
        typedef detail::base_<M> base_;
    public:
        typedef M                                   model_type;
        typedef model_<M>                    model_w_;
            
        // Construction
        model_();
        model_(const M& m);
        model_(const model_&);
            
        // Access
        const M& model()const;
    };

    // Implementation //
            
    template<typename M>
    model_<M>::model_() : base_(){}
        
    template<typename M>
    model_<M>::model_( const M& m ) : base_(m){}
        
    template<typename M>
    model_<M>::model_(const model_& that)
    :base_(that){}

    template<typename M>
    const M& model_<M>::model()const{ return (this->value); }
                
    // Free functions
        
    template<typename M>
    model_<M> 
    make_model(const M& m){
        typedef model_<M> result_type;
        return result_type(m);
    }

}// model    
}// statistics
}// boost

#endif
