///////////////////////////////////////////////////////////////////////////////
// joint_dist::meta::pointer.hpp                                             //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_JOINT_DIST_META_POINTER_ER_2009
#define BOOST_JOINT_DIST_META_POINTER_ER_2009
#include <boost/joint_dist/meta/signature.hpp>

namespace boost{
namespace joint_dist{

    template<typename D,typename R>
    struct pointer{
        typedef signature<D,R> meta_sig_;
        typedef typename meta_sig_::type sig_;
        typedef sig_* type;
    };

}// joint_dist
}// boost

#endif