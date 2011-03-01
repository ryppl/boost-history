//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_FRAME_MODIFIER_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_FRAME_MODIFIER_ER_2010_HPP
#include <boost/concept_check.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace modifier_tag{
    // add as necessary
}
namespace put_aux{

    template<typename Tag>
    struct modifier{
     /* Specialize on Tag:
        template<typename V, typename T>
        void impl(V& v, T& t)const; */
    };

}// put_aux
namespace put_concept{

    //[concept_modifier
    template<typename Tag, typename C, typename T>
    struct Modifier
    {

        BOOST_CONCEPT_USAGE(Modifier)
        {
            m.impl( cont, t ); 
        }

        private:
        static put_aux::modifier<Tag>& m;
        static C& cont;
        static T t;
    };
	//]

}// put_concept
}// v2
}// assign
}// boost

#endif
