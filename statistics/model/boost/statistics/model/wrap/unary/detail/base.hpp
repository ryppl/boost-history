///////////////////////////////////////////////////////////////////////////////
// statistics::model::wrap::unary::detail::base.hpp                          //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_WRAP_UNARY_DETAIL_BASE_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_WRAP_UNARY_DETAIL_BASE_HPP_ER_2009
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_reference.hpp>

namespace boost{ 
namespace statistics{
namespace model{
namespace detail{

    // D represents the prior distribution
    template<typename T>
    class base_{
        typedef is_reference<T> is_ref_;
        BOOST_MPL_ASSERT((
            mpl::not_<is_ref_>
        ));
    public:
        // Construction
        base_();
        base_(const T&);
        base_(const base_&);
        const T& value;
    private:
        base_& operator=(const base_&);
        static const T& make();
    };

    // Implementation //

    template<typename T>
    base_<T>::base_() : value(make()){}
                    
    template<typename T>
    base_<T>::base_(
        const T& t
    ) : value(t){}
        
    template<typename T>
    base_<T>::base_(const base_& that)
    :value( that.value ){}

    template<typename T>
    const T& base_<T>::make(){
        static T t = T();
        return t;
    }
      
}// detail
}// model        
}// statistics
}// boost

#endif
