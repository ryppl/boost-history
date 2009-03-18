////////////////////////////////////////////////////////////////////////////
// sample.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SHARED_FEATURES_FUNCTOR_SAMPLE_HPP_ER_2009
#define BOOST_SHARED_FEATURES_FUNCTOR_SAMPLE_HPP_ER_2009

namespace boost{namespace shared_features{namespace functor{

class sample{
    public:
    sample(){};

    template<typename Wrapper,typename Args>
    void operator()(Wrapper& w,const Args& args)const{
        w.sample(args);
    }
};

}
}
}

#endif // BOOST_SHARED_FEATURES_FUNCTOR_SAMPLE_HPP_ER_2009
