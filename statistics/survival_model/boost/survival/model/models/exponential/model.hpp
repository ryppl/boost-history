///////////////////////////////////////////////////////////////////////////////
// survival::model::exponential::model.hpp                                   //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_SURVIVAL_MODEL_MODELS_EXPONENTIAL_MODEL_HPP_ER_2009
#define BOOST_STATISTICS_SURVIVAL_MODEL_MODELS_EXPONENTIAL_MODEL_HPP_ER_2009
#include <numeric>
#include <boost/mpl/assert.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/type_traits/is_scalar.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/range.hpp>

namespace boost{
namespace statistics{

namespace survival{
namespace model{
namespace exponential{

    template<typename T>
    class model{
        public:
        typedef T value_type;
    
        model();
                
        template<typename X,typename B>
        static typename boost::enable_if<boost::is_scalar<X>,T>::type
        log_rate(const X& x,const B& b);

        template<typename X,typename B>
        static typename boost::disable_if<boost::is_scalar<X>,T>::type
        log_rate(const X& x,const B& b);

        protected:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version){
            // no member variables
        }

    };

    // Implementation //
    
    template<typename T>
    model<T>::model(){}

    template<typename T>
    template<typename X,typename B>
    typename boost::enable_if<boost::is_scalar<X>,T>::type
    model<T>::log_rate(const X& x,const B& b){
        BOOST_MPL_ASSERT((
            is_scalar<B>
        ));
        return ( static_cast<T>( x ) * static_cast<T>( b ) );
    }

    template<typename T>
    template<typename X,typename B>
    typename boost::disable_if<is_scalar<X>,T>::type
    model<T>::log_rate(const X& x,const B& b){
        BOOST_ASSERT(size(x) == size(b));
        return std::inner_product( 
            boost::begin(x), 
            boost::end(x),
            boost::begin(b),
            static_cast<T>(0)
        );
    }

}// exponential
}// model    
}// survival
}// statistics
}// boost

#endif 
