//////////////////////////////////////////////////////////////////////////////
// serialization::detail::inherit::inherit.hpp         						//
//                                                                          //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SERIALIZATION_DETAIL_INHERIT_INHERIT_HPP_ER_2010
#define BOOST_SERIALIZATION_DETAIL_INHERIT_INHERIT_HPP_ER_2010
#include <boost/mpl/empty_base.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/detail/inherit/inherit2.hpp>

namespace boost{
namespace serialization{
namespace detail{
	                
    // Merges up to 5 serializable classes together into one that is also
    // serializable. 
    //
    // Requirements :
    // Each non-empty base is serializable and the default constructor or one
    // that takes an argument pack must be defined.
    //
    // Usage : let arg denote an argument pack :
    // inherit<T1,T2,T3> obj(args);
    // oa << obj;
    // ia >> obj;
    //
    // See Boost.Parameter regarding argument pack.
    // See sandbox/statistics/data for a small application
	//
    // Note that class inherit<> will not inherit from any empty_base passed
    // as a template argument. This selective inheritance principle was found 
    // in the class by the same name in Boost.MPL
    //
    // TODO : define a macro to extend this to arbitrary n.
    template<
    	typename T1 = boost::mpl::empty_base,
        typename T2 = boost::mpl::empty_base,
        typename T3 = boost::mpl::empty_base,
        typename T4 = boost::mpl::empty_base,
        typename T5 = boost::mpl::empty_base
    >
    struct inherit : 	
    	inherit2<
        	T1,
            inherit2<
            	T2,
                inherit2<
                	T3,
                    inherit2<T4,T5>
                >
            >
        >
    {
    	typedef inherit2<
        	T1,
            inherit2<
                T2,
                inherit2<
                    T3,
                    inherit2<T4,T5>
                >
            >
        > super_;
        
        typedef inherit<super_> type;
            
        inherit(){}
            
        template<typename Args>
        inherit(const Args& args):super_(args){}

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            // serialize base class information
            ar & boost::serialization::base_object<super_>(*this);
        }
    
    };

	// If a, b, c are instances of the subset of non-empty bases in {T1,...,T5},
    // writes "(a,b,c)" to os
    template<typename A,typename B,
    	typename T1,typename T2,typename T3,typename T4,typename T5>
	std::basic_ostream<A,B>&
    operator<<(std::basic_ostream<A,B>& os,const inherit<T1,T2,T3,T4,T5>& that){
		typedef typename inherit<T1,T2,T3,T4,T5>::super_ super_;
        return os << '(' << static_cast<const super_&>(that) << ')';
    }

}// detail
}// serialization
}// boost

#endif
