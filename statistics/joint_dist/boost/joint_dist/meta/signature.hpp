///////////////////////////////////////////////////////////////////////////////
// joint_dist::meta::signature.hpp                                           //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_JOINT_DIST_META_SIGNATURE_ER_2009
#define BOOST_JOINT_DIST_META_SIGNATURE_ER_2009

namespace boost{
namespace joint_dist{

    template<typename D,typename R>
    struct signature{
        typedef typename D::value_type value_type;
        typedef value_type type(
            const D&,
            const R&
        );
    };
    
}// joint_dist
}// boost

#endif