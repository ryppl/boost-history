//////////////////////////////////////////////////////////////////////////////
// serialization::detail::inherit::inherit_linearly.hpp                     //
//                                                                          //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SERIALIZATION_DETAIL_INHERIT_INHERIT_LINEARLY_HPP_ER_2010
#define BOOST_SERIALIZATION_DETAIL_INHERIT_INHERIT_LINEARLY_HPP_ER_2010
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/reverse_fold.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/serialization/base_object.hpp>

#include <boost/concept/assert.hpp>
#include <boost/serialization/detail/inherit/inherit2.hpp>

namespace boost{
namespace serialization{
namespace detail{

    // Merges serializable classes together into one that is also serializable. 
    // In addition to a default constructor, one that takes an argument pack
    // forwards the latter to each of its components.
    //
    // Requirements :
    // S is a sequence of non-empy-base serializable class types, optionally 
    // with an argument pack constructor, and operator<< as needed.
    //
    // Usage : let arg denote an argument pack and T one element of S
    // inherit_linearly<S> obj(args);
    // static_cast<const S&>(obj);
    // oa << obj;
    // ia >> obj;
    //
    // See Boost.Parameter regarding argument pack.
    // See sandbox/statistics/data for a small application
	//
	// Background : See class by the same name in the MPL framework.

	template<typename S,typename R = boost::mpl::empty_base>
    struct inherit_linearly : boost::mpl::reverse_fold<
    	S,R,inherit2<boost::mpl::_2,boost::mpl::_1>
    >::type{
		typedef typename boost::mpl::reverse_fold<
        	S,R,inherit2<boost::mpl::_2,boost::mpl::_1>
    	>::type super_;
        
        inherit_linearly(){}
            
        template<typename Args>
        inherit_linearly(const Args& args):super_(args){}

		private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            // serialize base class information
            ar & boost::serialization::base_object<super_>(*this);
        }                  

    };

	// If a, b, c are instances of the subset of non-empty bases in S,
    // writes "(a,b,c)" to os
    template<typename A,typename B,typename S,typename R>
	std::basic_ostream<A,B>&
    operator<<(std::basic_ostream<A,B>& os,const inherit_linearly<S,R>& that){
		typedef typename inherit_linearly<S,R>::super_ super_;
        return os << '(' << static_cast<const super_&>(that) << ')';
    }

}// detail
}// serialization
}// boost

#endif



