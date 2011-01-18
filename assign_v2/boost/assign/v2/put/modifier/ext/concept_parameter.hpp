//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_EXT_CONCEPT_PARAMETER_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODIFIER_EXT_CONCEPT_PARAMETER_ER_2010_HPP
#include <boost/concept_check.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_concept{

	// P models Param with respect to modifier M
    template<typename M, typename P>
	struct Parameter
    {

		BOOST_CONCEPT_USAGE(Parameter)
        {
        	M m( p.get() );
        }

        private:
		static P & p;
    };

}// put_concept
}// v2
}// assign
}// boost

#endif
