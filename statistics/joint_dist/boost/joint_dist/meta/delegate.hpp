///////////////////////////////////////////////////////////////////////////////
// joint_dist::meta::delegate.hpp                                            //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_JOINT_DIST_META_DELEGATE_ER_2009
#define BOOST_JOINT_DIST_META_DELEGATE_ER_2009
#include <boost/function.hpp>
#include <boost/joint_dist/meta/signature.hpp>
#include <boost/joint_dist/meta/pointer.hpp>

namespace boost{
namespace joint_dist{

    template<typename D,typename R>
    struct delegate{
        typedef signature<R> meta_sig;
        typedef typename meta_sig::type sig_;
        typedef boost::function<sig_> type;
    };

}// joint_dist
}// boost

#endif
