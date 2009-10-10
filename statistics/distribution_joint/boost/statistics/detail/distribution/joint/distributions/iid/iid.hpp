//////////////////////////////////////////////////////////////////////////////
// distribution::joint::distributions::iid::iid.hpp                         //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_DISTRIBUTIONS_IID_IID_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_DISTRIBUTIONS_IID_IID_HPP_ER_2009
#include <string>
#include <sstream>
#include <boost/format.hpp>
#include <boost/range.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/statistics/detail/distribution_common/meta/value.hpp>
#include <boost/statistics/detail/distribution_common/meta/inherit_policy.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/is_pseudo_scalar_distribution.hpp>
#include <boost/statistics/detail/distribution_toolkit/unscope/os.hpp>

#include <boost/statistics/detail/distribution/joint/distributions/detail/dim.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace joint{

    // An iid distribution
    template<typename D>
    class iid : 
        D, 
        public detail::dim,
        public common::meta::inherit_policy<D>
    {
        typedef detail::dim dim_;
        typedef std::string str_;
        public:
        
            //BOOST_MPL_ASSERT((
            //    distribution::toolkit::is_pseudo_scalar_distribution<D>
            //));
        
        typedef typename distribution::common::meta::value<
            D
        >::type value_type;
        typedef D  element_type;
    
        iid(): D(), dim_(0){}
        iid(const D& d,size_type dim):D(d),dim_(dim){}
        iid(const iid& that):D(that),dim_(that){}

        iid& operator=(const iid& that)
        {
            if(&that!=this){
                static_cast<D&>(*this) = that;
                static_cast<dim_&>(*this) = that;
            }
            return *this;
        }

        const element_type& element()const{ 
            return static_cast<const D&>(*this); 
        }

        str_ description()const
        {
            static const char* msg = "iid{%1%:i=0,...,%2%}";
            std::stringstream sstr; 
            
            sstr << this->element();

            return (
                boost::format(msg) 
                    % sstr.str() 
                        % this->dimension() 
            ).str();
        }
    };

}// joint
}// distribution
}// detail
}// statistics
}// boost

#endif
