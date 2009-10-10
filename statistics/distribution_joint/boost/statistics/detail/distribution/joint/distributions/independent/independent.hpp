//////////////////////////////////////////////////////////////////////////////
// distribution::joint::distributions::independent::independent.hpp         //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_DISTRIBUTION_INDEPENDENT_INDEPENDENT_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_DISTRIBUTION_INDEPENDENT_INDEPENDENT_HPP_ER_2009
#include <string>
#include <sstream>
#include <boost/format.hpp>
#include <boost/range.hpp>
#include <boost/foreach.hpp>
#include <boost/type_traits.hpp>
#include <boost/statistics/detail/distribution_common/meta/value.hpp>
#include <boost/statistics/detail/distribution_common/meta/inherit_policy.hpp>

#include <boost/statistics/detail/distribution/joint/distributions/detail/dim.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace joint{

    // Joint distribution of a collection of indepdendent variables, each from 
    // the same distribution, but with different parameters
    template<typename R>
    class independent
        : public detail::dim,
        public common::meta::inherit_policy<
            typename remove_cv<
                typename remove_reference<
                    typename range_value<R>::type
                >::type
            >::type
        >
    {
        typedef detail::dim dim_;
        typedef std::string str_;

        public:

        typedef 
            typename remove_cv<
                typename remove_reference<
                    typename range_value<R>::type
                >::type
            >::type element_type;

        typedef typename distribution::common::meta::value<
            element_type
        >::type value_type;
    
        independent(){}
        template<typename It>
        independent(typename call_traits<R>::param_type elems )
            :dim_(boost::size(elems)),elements_(elements){}
        independent(const independent& that)
            :dim_(that),elements_(that.elements_){}

        independent& operator=(const independent& that)
        {
            if(&that!=this){
                static_cast<dim_&>(*this) = that;
                this->elements_ = that.elements_;
            }
        }

        typename call_traits<R>::const_reference elements()const{
            return this->elements_;
        }

        str_ decription()const{
            std::stringstream sstr;
            sstr << "independent(";
            BOOST_FOREACH(const element_type& e,this->elements()){
                sstr <<  e;
                sstr << ' ';
            }
            sstr << ')';
            return sstr.str();
        }
    
        private:
        typename call_traits<R>::value_type elements_;
    };        

}// joint
}// distribution
}// detail
}// statistics
}// boost

#endif
