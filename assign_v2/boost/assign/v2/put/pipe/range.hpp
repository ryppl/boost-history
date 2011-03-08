//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PIPE_RANGE_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_PIPE_RANGE_ER_2010_HPP
#include <boost/type.hpp>
#include <boost/assign/v2/put/container/range.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_range_aux
{
    template<typename R>
    class helper1 : ref::copy_wrapper<R const>::type
    {
        typedef typename ref::copy_wrapper<R const>::type super_t;

        public:

        helper1(R const& range) : super_t( range ){}

        typename super_t::result_of_get_type 
        range()const{ return this->get(); }

        private:
        helper1();
    };

    struct helper2
    {
        helper2(){}
        template<typename R>
        helper1<R> operator()(const R& range)const
        {
            typedef helper1<R> result_;
            return result_( range );
        }
    };

    template<typename C,typename R>
    C& operator|(C& cont, put_range_aux::helper1<R> const& h)
    {
        return put_range( cont, h.range() );
    }

    template<typename C, typename R>
    C operator|( ::boost::type<C>, put_range_aux::helper1<R> const& h)
    {
        return put_range<C>( h.range() );
    }

}// put_range_aux
namespace{
    put_range_aux::helper2 const _put_range = put_range_aux::helper2();
}
}// v2
}// assign
}// boost

#endif
