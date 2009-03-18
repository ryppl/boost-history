/////////////////////////////////////////////////////////////////////////////
// visitor.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
// Disclaimer :
//
// This code is a minor modification to Boost.Accumulator and serves a
// particular purpose not readily provided by the original library.
// Contact the Boost library for copyright implications.
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SHARED_FEATURES_VISITOR_HPP_ER_2009
#define BOOST_SHARED_FEATURES_VISITOR_HPP_ER_2009

namespace boost{namespace shared_features{

namespace detail{

template<typename Fun,typename Args>
class visitor{
    public:
    explicit visitor(Fun const & f,const Args& args)
    : f_(f),args_(args){}

    visitor(visitor const& that)
    : f_(that.f_),args_(that.args_){}

    template<typename Wrapper>
    void operator()(Wrapper& w)const{
        f_(w,args_);
    }

    private:
    visitor& operator=(visitor const &);
    Fun     f_; //warning copy
    Args    const & args_;
};

template<typename Fun,typename Args>
visitor<Fun,Args> const make_visitor(Fun const& f,Args const & args){
    return visitor<Fun,Args>(f,args);
}

}
}
}

#endif // BOOST_SHARED_FEATURES_VISITOR_HPP_ER_2009
