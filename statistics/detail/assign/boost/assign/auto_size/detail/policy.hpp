//////////////////////////////////////////////////////////////////////////////
// assign::detail::auto_size::policy.hpp                                    //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_POLICY_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_POLICY_ER_2010_HPP
#include <boost/mpl/empty_base.hpp>

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{

    namespace tag{
        struct no_policy{};
    }
	
    template<typename Tag>
	struct policy{
    	typedef Tag tag;
    };


    template<>
	struct policy<tag::no_policy>{
        template<typename E,template<typename> class  Ref>
        struct apply{
            typedef boost::mpl::empty_base type;
    	};
    };

    
}// auto_size
}// detail
}// assign
}// boost

#endif
