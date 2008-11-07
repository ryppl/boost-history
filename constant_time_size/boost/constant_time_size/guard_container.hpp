#ifndef BOOST_CONSTANT_TIME_SIZE_GUARD_CONTAINER_H_
#define BOOST_CONSTANT_TIME_SIZE_GUARD_CONTAINER_H_

namespace boost { 
namespace constant_time_size {


template < 
    class Wrapper
> struct non_const {
    typedef typename Wrapper::container            container;

    non_const(Wrapper& w) : w_(w) {}
    ~non_const() {
        w_.reinit_size(w_.cont_);
    }
 
    operator container&()
    {
        return w_.cont_;
    }
    operator container*()
    {
        return &w_.cont_;
    }
    container& as_container()
    {
        return w_.cont_;
    }

    Wrapper& w_;
};

template < 
    class Container
> struct non_const_cont {
    typedef Container            container;

    non_const_cont(container& cont) : cont_(cont) {}
    ~non_const_cont() {
    }
 
    operator container&()
    {
        return cont_;
    }
    operator container*()
    {
        return &cont_;
    }
    container& as_container()
    {
        return cont_;
    }

    container& cont_;
};


}
}
  
#endif // BOOST_CONSTANT_TIME_SIZE_GUARD_CONTAINER_H_
