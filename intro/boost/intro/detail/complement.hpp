//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_DETAIL_COMPLEMENT_HPP
#define BOOST_INTRO_DETAIL_COMPLEMENT_HPP

namespace boost{
namespace intro{
namespace detail{

template<class F>
struct complement{
    typedef void result_type;
    //typedef typename F::result_type result_type;
    explicit complement(F const &f) : f(f){}
    template<class T0>
    result_type operator()(T0 &t0) const{
        return (~f)(t0);
    }
    template<class T0>
    result_type operator()(T0 const &t0) const{
        return (~f)(t0);
    }
    template<class T0,class T1>
    result_type operator()(T0 const &t0,T1 const &t1) const{
        return (~f)(t0,t1);
    }
    template<class T0,class T1>
    result_type operator()(T0 &t0,T1 const &t1) const{
        return (~f)(t0,t1);
    }
    template<class T0,class T1>
    result_type operator()(T0 const &t0,T1 &t1) const{
        return (~f)(t0,t1);
    }
    template<class T0,class T1>
    result_type operator()(T0 &t0,T1 &t1) const{
        return (~f)(t0,t1);
    }
    template<class T0,class T1,class T2>
    result_type operator()(T0 const &t0,T1 const &t1,T2 const &t2) const{
        return (~f)(t0,t1,t2);
    }
    template<class T0,class T1,class T2>
    result_type operator()(T0 &t0,T1 const &t1,T2 const &t2) const{
        return (~f)(t0,t1,t2);
    }
    template<class T0,class T1,class T2>
    result_type operator()(T0 const &t0,T1 &t1,T2 const &t2) const{
        return (~f)(t0,t1,t2);
    }
    template<class T0,class T1,class T2>
    result_type operator()(T0 const &t0,T1 const &t1,T2 &t2) const{
        return (~f)(t0,t1,t2);
    }
    template<class T0,class T1,class T2>
    result_type operator()(T0 &t0,T1 &t1,T2 const &t2) const{
        return (~f)(t0,t1,t2);
    }
    template<class T0,class T1,class T2>
    result_type operator()(T0 &t0,T1 const &t1,T2 &t2) const{
        return (~f)(t0,t1,t2);
    }
    template<class T0,class T1,class T2>
    result_type operator()(T0 const &t0,T1 &t1,T2 &t2) const{
        return (~f)(t0,t1,t2);
    }
    template<class T0,class T1,class T2>
    result_type operator()(T0 &t0,T1 &t1,T2 &t2) const{
        return (~f)(t0,t1,t2);
    }
private:
    F const &f;
};


}
}
}



#endif
