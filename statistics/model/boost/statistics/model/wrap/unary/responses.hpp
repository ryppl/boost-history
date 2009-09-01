///////////////////////////////////////////////////////////////////////////////
// model::wrap::unary::responses.hpp                                          //
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
    class responses_ : detail::base_<Ry>{
        typedef detail::base_<Ry> base_;
    public:
        typedef Ry                 responses_type;
        typedef responses_<Ry>     responses_w_;
            
        // Construction
        responses_();
        responses_(const Ry& x);
        responses_(const responses_&);
            
        // Access
        const Ry& responses()const;
    };

    // Implementation //
            
    template<typename Ry>
    responses_<Ry>::responses_() : base_(){}
        
    template<typename Ry>
    responses_<Ry>::responses_( const Ry& ry ) : base_(ry){}
        
    template<typename Ry>
    responses_<Ry>::responses_(const responses_& that)
    :base_(that){}

    template<typename Ry>
    const Ry& responses_<Ry>::responses()const{ return (this->value); }
                
    // Free functions
        
    template<typename Ry>
    responses_<Ry> 
    make_responses(const Ry& ry){
        typedef responses_<Ry> result_type;
        return result_type(ry);
    }

}// model    
}// statistics
}// boost

#endif
