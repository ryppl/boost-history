///////////////////////////////////////////////////////////////////////////////
// survival::model::meta::model_data.hpp                                     //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SURVIVAL_MODEL_META_MODEL_DATA_HPP_ER_2009
#define BOOST_SURVIVAL_MODEL_META_MODEL_DATA_HPP_ER_2009
#include <boost/model/wrap/aggregate/model_data.hpp>
#include <boost/survival/data/data/event.hpp>

namespace boost{
namespace survival{
namespace model{
namespace meta{        

    // See statistics/model/libs/doc/readme
    template<typename T,typename M,typename X>
    struct model_data{
        typedef boost::survival::data::event<T> y_;
        typedef boost::model::model_data_< M, X, y_ > type;
    };
    
}// meta
}// model    
}// survival
}// boost

#endif