///////////////////////////////////////////////////////////////////////////////
// statistics::model::wrap::unary::model.hpp                                 //
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
    class model_wrapper : detail::base_<M>{
        typedef detail::base_<M> base_;
    public:
        typedef M                            model_type;
        typedef model_wrapper<M>             model_wrapper_type;
            
        // Construction
        model_wrapper();
        model_wrapper(const M& m);
        model_wrapper(const model_wrapper&);
            
        // Access
        const M& model()const;
    };

    // Implementation //
            
    template<typename M>
    model_wrapper<M>::model_wrapper() : base_(){}
        
    template<typename M>
    model_wrapper<M>::model_wrapper( const M& m ) : base_(m){}
        
    template<typename M>
    model_wrapper<M>::model_wrapper(const model_wrapper& that)
    :base_(that){}

    template<typename M>
    const M& model_wrapper<M>::model()const{ return (this->value); }
                
    // Free functions
        
    template<typename M>
    model_wrapper<M> 
    make_model_wrapper(const M& m){
        typedef model_wrapper<M> result_type;
        return result_type(m);
    }

}// model    
}// statistics
}// boost

#endif
