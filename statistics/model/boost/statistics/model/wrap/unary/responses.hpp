///////////////////////////////////////////////////////////////////////////////
// statistics::model::wrap::unary::responses.hpp                             //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_WRAP_UNARY_RESPONSES_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_WRAP_UNARY_RESPONSES_HPP_ER_2009
#include <boost/statistics/model/wrap/unary/detail/base.hpp>

namespace boost{ 
namespace statistics{
namespace model{
        
    template<typename Ry>
    class responses_wrapper : detail::base_<Ry>{
        typedef detail::base_<Ry> base_;
    public:
        typedef Ry                      responses_type;
        typedef responses_wrapper<Ry>   responses_wrapper_type;
            
        // Construction
        responses_wrapper();
        responses_wrapper(const Ry& x);
        responses_wrapper(const responses_wrapper&);
            
        // Access
        const Ry& responses()const;
    };

    // Implementation //
            
    template<typename Ry>
    responses_wrapper<Ry>::responses_wrapper() : base_(){}
        
    template<typename Ry>
    responses_wrapper<Ry>::responses_wrapper( const Ry& ry ) : base_(ry){}
        
    template<typename Ry>
    responses_wrapper<Ry>::responses_wrapper(const responses_wrapper& that)
    :base_(that){}

    template<typename Ry>
    const Ry& responses_wrapper<Ry>::responses()const{ 
        return (this->value); 
    }
                
    // Free functions
        
    template<typename Ry>
    responses_wrapper<Ry> 
    make_responses_wrapper(const Ry& ry){
        typedef responses_wrapper<Ry> result_type;
        return result_type(ry);
    }

}// model    
}// statistics
}// boost

#endif
