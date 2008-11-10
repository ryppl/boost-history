/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__GENERIC__GET_PORT_HPP
#define BOOST__DATAFLOW__GENERIC__GET_PORT_HPP


#include <boost/mpl/assert.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/dataflow/generic/framework_entity/default_framework_of.hpp>
#include <boost/dataflow/generic/framework_entity/traits_of.hpp>
#include <boost/dataflow/generic/framework_entity.hpp>
namespace boost { namespace dataflow {

namespace detail {

    struct not_specialized
    {};

}

namespace extension
{
    template<typename Traits1,typename Enable=void>
    struct get_port_impl
    {
        BOOST_MPL_ASSERT(( is_same<Enable, void> ));
        
        typedef detail::not_specialized result_type;

        template<typename T1,typename Index>
        result_type operator()(T1 &t1,const Index &index)
        {
            // Error: get_port_impl has not been
            // specialized appropriately.
            BOOST_MPL_ASSERT(( mpl::bool_<sizeof(T1)==0> ));
            return result_type();
        }
    };
    
    template<typename Traits1,typename Enable=void>
    struct get_port_will_succeed_impl
    {
        BOOST_MPL_ASSERT(( is_same<Enable, void> ));
        
        typedef bool result_type;

        template<typename T1,typename Index>
        result_type operator()(T1 &t1,const Index &index)
        {
            return
                !is_same<
                        typename  extension::get_port_impl<
                            Traits1
                            >::result_type,
                    detail::not_specialized
                >::value;
        }
    };
}

namespace detail
{

    template<typename T1,typename Index,typename Framework1=typename default_framework_of<T1>::type,typename Enable=void>
    struct has_ports_specialized
        : public mpl::true_
    {};
    
    template<typename T1,typename Index,typename Framework1>
    struct has_ports_specialized<
        T1,Index,Framework1
        ,
        typename enable_if<
            is_same<
               typename  extension::get_port_impl<
                typename traits_of<T1, Framework1>::type
                >::result_type,
                detail::not_specialized>
        >::type>
        : public mpl::false_
    {};

}

template<typename T1,typename Framework1=typename default_framework_of<T1>::type,typename Enable=void>
struct has_ports
    : public mpl::false_
{
    BOOST_MPL_ASSERT((mpl::and_<is_framework_entity<T1,Framework1> >));
};

template<typename T1,typename Framework1>
struct has_ports<
    T1,Framework1,
    typename enable_if<
        detail::has_ports_specialized<
            T1,Framework1>
     >::type>
    : public mpl::true_
{
    BOOST_MPL_ASSERT((mpl::and_<is_framework_entity<T1,Framework1> >));
};

namespace result_of {

    template<typename T1,typename Index,typename Framework1=typename default_framework_of<T1>::type>
    struct get_port
    {
        typedef typename boost::result_of<
            extension::get_port_impl<
                typename traits_of<T1, Framework1>::type
            >
            (T1,Index)
        >::type type;
    };
    
}


template<typename Index,typename Framework1,typename T1>
inline typename result_of::get_port<
    T1,Index,Framework1
    >::type 
get_port_framework(T1 &t1,const Index &index=Index())
{
    return extension::get_port_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1,index);
}

template<typename Index,typename T1>
inline typename result_of::get_port<
    T1,Index,typename default_framework_of<T1>::type
    >::type 
get_port(T1 &t1,const Index &index=Index())
{
    typedef typename default_framework_of<T1>::type Framework1;
    return extension::get_port_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1,index);
}

template<typename Index,typename Framework1,typename T1>
inline bool
get_port_will_succeed_framework(T1 &t1,const Index &index=Index())
{
    return extension::get_port_will_succeed_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1,index);
}

template<typename Index,typename T1>
inline bool 
get_port_will_succeed(T1 &t1,const Index &index=Index())
{
    typedef typename default_framework_of<T1>::type Framework1;
    return extension::get_port_will_succeed_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1,index);
}

template<typename Index,typename Framework1,typename T1>
inline typename result_of::get_port<
    T1,Index,Framework1
    >::type 
get_port_framework(const T1 &t1,const Index &index=Index())
{
    return extension::get_port_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1,index);
}

template<typename Index,typename T1>
inline typename result_of::get_port<
    T1,Index,typename default_framework_of<T1>::type
    >::type 
get_port(const T1 &t1,const Index &index=Index())
{
    typedef typename default_framework_of<T1>::type Framework1;
    return extension::get_port_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1,index);
}

template<typename Index,typename Framework1,typename T1>
inline bool
get_port_will_succeed_framework(const T1 &t1,const Index &index=Index())
{
    return extension::get_port_will_succeed_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1,index);
}

template<typename Index,typename T1>
inline bool 
get_port_will_succeed(const T1 &t1,const Index &index=Index())
{
    typedef typename default_framework_of<T1>::type Framework1;
    return extension::get_port_will_succeed_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1,index);
}

}}


#endif // BOOST__DATAFLOW__GENERIC__GET_PORT_HPP
