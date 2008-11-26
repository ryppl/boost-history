/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__GENERIC__PORT_BINARY_OPERATION_HPP
#define BOOST__DATAFLOW__GENERIC__PORT_BINARY_OPERATION_HPP


#include <boost/mpl/assert.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/dataflow/generic/framework_entity/default_framework_of.hpp>
#include <boost/dataflow/generic/framework_entity/traits_of.hpp>
#include <boost/dataflow/detail/not_specialized.hpp>


#include <boost/dataflow/generic/port/port.hpp>
namespace boost { namespace dataflow {

namespace extension
{
    template<typename Traits1,typename Traits2,typename Operation,typename Enable=void>
    struct port_binary_operation_impl
    {
        BOOST_MPL_ASSERT(( is_same<Enable, void> ));
        
        typedef detail::not_specialized result_type;

        template<typename T1,typename T2>
        result_type operator()(T1 &t1,T2 &t2,const Operation &operation)
        {
            // Error: port_binary_operation_impl has not been
            // specialized appropriately.
            BOOST_MPL_ASSERT(( mpl::bool_<sizeof(T1)==0> ));
            return result_type();
        }
    };
    
    template<typename Traits1,typename Traits2,typename Operation,typename Enable=void>
    struct port_binary_operation_will_succeed_impl
    {
        BOOST_MPL_ASSERT(( is_same<Enable, void> ));
        
        typedef bool result_type;

        template<typename T1,typename T2>
        result_type operator()(T1 &t1,T2 &t2,const Operation &operation)
        {
            return
                !is_same<
                        typename  extension::port_binary_operation_impl<
                            Traits1,Traits2,Operation
                            >::result_type,
                    detail::not_specialized
                >::value;
        }
    };
}

namespace detail
{

    template<typename T1,typename T2,typename Operation,typename Framework1=typename default_framework_of<T1>::type,typename Framework2=typename default_framework_of<T2>::type,typename Enable=void>
    struct are_port_binary_operable_specialized
        : public mpl::true_
    {};
    
    template<typename T1,typename T2,typename Operation,typename Framework1,typename Framework2>
    struct are_port_binary_operable_specialized<
        T1,T2,Operation,Framework1,Framework2
        ,
        typename enable_if<
            is_same<
               typename  extension::port_binary_operation_impl<
                typename traits_of<T1, Framework1>::type,typename traits_of<T2, Framework2>::type,Operation
                >::result_type,
                detail::not_specialized>
        >::type>
        : public mpl::false_
    {};

}

template<typename T1,typename T2,typename Operation,typename Framework1=typename default_framework_of<T1>::type,typename Framework2=typename default_framework_of<T2>::type,typename Enable=void>
struct are_port_binary_operable
    : public mpl::false_
{
    BOOST_MPL_ASSERT((mpl::and_<is_port<T1,Framework1>,is_port<T2,Framework2> >));
};

template<typename T1,typename T2,typename Operation,typename Framework1,typename Framework2>
struct are_port_binary_operable<
    T1,T2,Operation,Framework1,Framework2,
    typename enable_if<
        detail::are_port_binary_operable_specialized<
            T1,T2,Operation,Framework1,Framework2>
     >::type>
    : public mpl::true_
{
    BOOST_MPL_ASSERT((mpl::and_<is_port<T1,Framework1>,is_port<T2,Framework2> >));
};

namespace result_of {

    template<typename T1,typename T2,typename Operation,typename Framework1=typename default_framework_of<T1>::type,typename Framework2=typename default_framework_of<T2>::type>
    struct port_binary_operation
    {
        typedef typename boost::result_of<
            extension::port_binary_operation_impl<
                typename traits_of<T1, Framework1>::type,typename traits_of<T2, Framework2>::type,Operation
            >
            (T1,T2,Operation)
        >::type type;
    };
    
}


template<typename Operation,typename Framework1,typename Framework2,typename T1,typename T2>
inline typename result_of::port_binary_operation<
    T1,T2,Operation,Framework1,Framework2
    >::type 
port_binary_operation_framework(T1 &t1,T2 &t2,const Operation &operation=Operation())
{
    return extension::port_binary_operation_impl<
        typename traits_of<T1, Framework1>::type,typename traits_of<T2, Framework2>::type,Operation
        >()(t1,t2,operation);
}

template<typename Operation,typename T1,typename T2>
inline typename result_of::port_binary_operation<
    T1,T2,Operation,typename default_framework_of<T1>::type,typename default_framework_of<T2>::type
    >::type 
port_binary_operation(T1 &t1,T2 &t2,const Operation &operation=Operation())
{
    typedef typename default_framework_of<T1>::type Framework1;
typedef typename default_framework_of<T2>::type Framework2;
    return extension::port_binary_operation_impl<
        typename traits_of<T1, Framework1>::type,typename traits_of<T2, Framework2>::type,Operation
        >()(t1,t2,operation);
}

template<typename Operation,typename Framework1,typename Framework2,typename T1,typename T2>
inline bool
port_binary_operation_will_succeed_framework(T1 &t1,T2 &t2,const Operation &operation=Operation())
{
    return extension::port_binary_operation_will_succeed_impl<
        typename traits_of<T1, Framework1>::type,typename traits_of<T2, Framework2>::type,Operation
        >()(t1,t2,operation);
}

template<typename Operation,typename T1,typename T2>
inline bool 
port_binary_operation_will_succeed(T1 &t1,T2 &t2,const Operation &operation=Operation())
{
    typedef typename default_framework_of<T1>::type Framework1;
typedef typename default_framework_of<T2>::type Framework2;
    return extension::port_binary_operation_will_succeed_impl<
        typename traits_of<T1, Framework1>::type,typename traits_of<T2, Framework2>::type,Operation
        >()(t1,t2,operation);
}

template<typename Operation,typename Framework1,typename Framework2,typename T1,typename T2>
inline typename result_of::port_binary_operation<
    T1,T2,Operation,Framework1,Framework2
    >::type 
port_binary_operation_framework(const T1 &t1,T2 &t2,const Operation &operation=Operation())
{
    return extension::port_binary_operation_impl<
        typename traits_of<T1, Framework1>::type,typename traits_of<T2, Framework2>::type,Operation
        >()(t1,t2,operation);
}

template<typename Operation,typename T1,typename T2>
inline typename result_of::port_binary_operation<
    T1,T2,Operation,typename default_framework_of<T1>::type,typename default_framework_of<T2>::type
    >::type 
port_binary_operation(const T1 &t1,T2 &t2,const Operation &operation=Operation())
{
    typedef typename default_framework_of<T1>::type Framework1;
typedef typename default_framework_of<T2>::type Framework2;
    return extension::port_binary_operation_impl<
        typename traits_of<T1, Framework1>::type,typename traits_of<T2, Framework2>::type,Operation
        >()(t1,t2,operation);
}

template<typename Operation,typename Framework1,typename Framework2,typename T1,typename T2>
inline bool
port_binary_operation_will_succeed_framework(const T1 &t1,T2 &t2,const Operation &operation=Operation())
{
    return extension::port_binary_operation_will_succeed_impl<
        typename traits_of<T1, Framework1>::type,typename traits_of<T2, Framework2>::type,Operation
        >()(t1,t2,operation);
}

template<typename Operation,typename T1,typename T2>
inline bool 
port_binary_operation_will_succeed(const T1 &t1,T2 &t2,const Operation &operation=Operation())
{
    typedef typename default_framework_of<T1>::type Framework1;
typedef typename default_framework_of<T2>::type Framework2;
    return extension::port_binary_operation_will_succeed_impl<
        typename traits_of<T1, Framework1>::type,typename traits_of<T2, Framework2>::type,Operation
        >()(t1,t2,operation);
}

template<typename Operation,typename Framework1,typename Framework2,typename T1,typename T2>
inline typename result_of::port_binary_operation<
    T1,T2,Operation,Framework1,Framework2
    >::type 
port_binary_operation_framework(T1 &t1,const T2 &t2,const Operation &operation=Operation())
{
    return extension::port_binary_operation_impl<
        typename traits_of<T1, Framework1>::type,typename traits_of<T2, Framework2>::type,Operation
        >()(t1,t2,operation);
}

template<typename Operation,typename T1,typename T2>
inline typename result_of::port_binary_operation<
    T1,T2,Operation,typename default_framework_of<T1>::type,typename default_framework_of<T2>::type
    >::type 
port_binary_operation(T1 &t1,const T2 &t2,const Operation &operation=Operation())
{
    typedef typename default_framework_of<T1>::type Framework1;
typedef typename default_framework_of<T2>::type Framework2;
    return extension::port_binary_operation_impl<
        typename traits_of<T1, Framework1>::type,typename traits_of<T2, Framework2>::type,Operation
        >()(t1,t2,operation);
}

template<typename Operation,typename Framework1,typename Framework2,typename T1,typename T2>
inline bool
port_binary_operation_will_succeed_framework(T1 &t1,const T2 &t2,const Operation &operation=Operation())
{
    return extension::port_binary_operation_will_succeed_impl<
        typename traits_of<T1, Framework1>::type,typename traits_of<T2, Framework2>::type,Operation
        >()(t1,t2,operation);
}

template<typename Operation,typename T1,typename T2>
inline bool 
port_binary_operation_will_succeed(T1 &t1,const T2 &t2,const Operation &operation=Operation())
{
    typedef typename default_framework_of<T1>::type Framework1;
typedef typename default_framework_of<T2>::type Framework2;
    return extension::port_binary_operation_will_succeed_impl<
        typename traits_of<T1, Framework1>::type,typename traits_of<T2, Framework2>::type,Operation
        >()(t1,t2,operation);
}

template<typename Operation,typename Framework1,typename Framework2,typename T1,typename T2>
inline typename result_of::port_binary_operation<
    T1,T2,Operation,Framework1,Framework2
    >::type 
port_binary_operation_framework(const T1 &t1,const T2 &t2,const Operation &operation=Operation())
{
    return extension::port_binary_operation_impl<
        typename traits_of<T1, Framework1>::type,typename traits_of<T2, Framework2>::type,Operation
        >()(t1,t2,operation);
}

template<typename Operation,typename T1,typename T2>
inline typename result_of::port_binary_operation<
    T1,T2,Operation,typename default_framework_of<T1>::type,typename default_framework_of<T2>::type
    >::type 
port_binary_operation(const T1 &t1,const T2 &t2,const Operation &operation=Operation())
{
    typedef typename default_framework_of<T1>::type Framework1;
typedef typename default_framework_of<T2>::type Framework2;
    return extension::port_binary_operation_impl<
        typename traits_of<T1, Framework1>::type,typename traits_of<T2, Framework2>::type,Operation
        >()(t1,t2,operation);
}

template<typename Operation,typename Framework1,typename Framework2,typename T1,typename T2>
inline bool
port_binary_operation_will_succeed_framework(const T1 &t1,const T2 &t2,const Operation &operation=Operation())
{
    return extension::port_binary_operation_will_succeed_impl<
        typename traits_of<T1, Framework1>::type,typename traits_of<T2, Framework2>::type,Operation
        >()(t1,t2,operation);
}

template<typename Operation,typename T1,typename T2>
inline bool 
port_binary_operation_will_succeed(const T1 &t1,const T2 &t2,const Operation &operation=Operation())
{
    typedef typename default_framework_of<T1>::type Framework1;
typedef typename default_framework_of<T2>::type Framework2;
    return extension::port_binary_operation_will_succeed_impl<
        typename traits_of<T1, Framework1>::type,typename traits_of<T2, Framework2>::type,Operation
        >()(t1,t2,operation);
}

}}


#endif // BOOST__DATAFLOW__GENERIC__PORT_BINARY_OPERATION_HPP
