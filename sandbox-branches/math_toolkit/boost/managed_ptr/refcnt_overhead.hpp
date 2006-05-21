//overhead for reference counting
#ifndef BOOST_MANAGED_PTR_REFCNT_OVERHEAD_LJE20031109_HPP
#define BOOST_MANAGED_PTR_REFCNT_OVERHEAD_LJE20031109_HPP
namespace boost{namespace managed_ptr{

class refcnt_overhead
{
    typedef refcnt_overhead this_type;
 public:
    typedef unsigned count_type;
    
    ~refcnt_overhead()
    {
    }
    
    refcnt_overhead(count_type a_count=0)
    : my_count(a_count)
    {
    }
    
    count_type as_count()const
    {
        return my_count;
    }

    bool is_valid()const
    {
        return as_count() != 0;
    }
    
    this_type const& increment()
    {
        ++my_count;
        #ifdef BOOST_MANAGED_PTR_TRACE_MODE
        mout()<<"refcnt_overhead::increment:after="<<my_count<<"\n";
        #endif
        return *this;
    }
    
    this_type const& decrement()
    { 
        --my_count;
        #ifdef BOOST_MANAGED_PTR_TRACE_MODE
        mout()<<"refcnt_overhead::decrement:after="<<my_count<<"\n";
        #endif
        return *this;
    }
    
    this_type const& operator++()
    { 
        return increment();
    }
    
    this_type const& operator--()
    { 
        return decrement();
    }
    
 private:
    count_type my_count;
};//end refcnt_overhead class
    
}}//exit boost::managed_ptr namespace
#endif
