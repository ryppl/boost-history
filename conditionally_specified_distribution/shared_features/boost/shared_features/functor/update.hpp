////////////////////////////////////////////////////////////////////////////
// update.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SHARED_FEATURES_FUNCTOR_UPDATE_HPP_ER_2009
#define BOOST_SHARED_FEATURES_FUNCTOR_UPDATE_HPP_ER_2009

namespace boost{namespace shared_features{namespace functor{

class update{
    public:
    update(){};

    template<typename Wrapper,typename Args>
    void operator()(Wrapper& w,const Args& args)const{
        w.update(args);
    }
};

}
}
}

#endif // BOOST_SHARED_FEATURES_FUNCTORS_UPDATE_HPP_ER_2009
