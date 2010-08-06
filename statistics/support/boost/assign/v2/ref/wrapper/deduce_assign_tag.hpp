//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_WRAPPER_DEDUCE_ASSIGN_TAG_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_WRAPPER_DEDUCE_ASSIGN_TAG_ER_2010_HPP
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost{
	struct use_default;
namespace assign{ 
namespace v2{
namespace ref{
namespace assign_tag{
	struct copy;
}// assign_tag
namespace wrapper_aux{

	template<typename Tag>
	struct deduce_assign_tag : boost::mpl::eval_if<
        boost::is_same<Tag, boost::use_default>,
        boost::mpl::identity<ref::assign_tag::copy>,
        boost::mpl::identity<Tag>
    >
    {};

}// wrapper_aux
}// ref
}// v2
}// assign
}// boost

#endif
