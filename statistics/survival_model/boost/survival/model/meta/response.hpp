///////////////////////////////////////////////////////////////////////////////
// survival::model::meta::response.hpp                                       //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SURVIVAL_MODEL_META_RESPONSE_HPP_ER_2009
#define BOOST_SURVIVAL_MODEL_META_RESPONSE_HPP_ER_2009
#include <boost/model/wrap/unary/response.hpp>
#include <boost/survival/data/data/event.hpp>

namespace boost{
namespace survival{
namespace model{
namespace meta{        

    // See statistics/model/libs/doc/readme
    template<typename T>
    struct response{
        typedef survival::data::event<T> y_;
        typedef boost::model::response_<y_> type;
    };
    
}// meta
}// model    
}// survival
}// boost

#endif