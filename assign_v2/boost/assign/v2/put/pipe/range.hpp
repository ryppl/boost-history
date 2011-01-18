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
#include <boost/range/algorithm/for_each.hpp>
#include <boost/assign/v2/put/sub/functor.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_range_aux
{
    template<typename From>
    class helper1 : ref::wrapper<ref::assign_tag::copy, From const>
    {
        typedef ref::wrapper<ref::assign_tag::copy, From const> super_t;

        public:

    	helper1(From const& from) : super_t(from){}

        From const& from()const{ return this->get(); }

        private:
        helper1();
    };

    struct helper2
    {
    	helper2(){}
        template<typename From>
        helper1<From> operator()(const From& from)const
        {
        	typedef helper1<From> result_;
        	return result_( from );
        }
    };

    // This decl must come first, it seems.
    template<typename To, typename From>
    To operator|(::boost::type<To>, put_range_aux::helper1<From> const& h);

    template<typename To,typename From>
    To& operator|(To& to, put_range_aux::helper1<From> const& h)
    {
        ::boost::for_each( h.from(), put( to ) );
        return to;
    }

    template<typename To, typename From>
    To operator|(::boost::type<To>, put_range_aux::helper1<From> const& h)
    {
		To to;
        to | h;
        return to;
    }

}// put_range_aux

    put_range_aux::helper2 const _put_range = put_range_aux::helper2();

}// v2
}// assign
}// boost

#include <boost/assign/v2/detail/checking/fwd.hpp>

// Must be preceded by
// #include <boost/assign/v2/detail/checking/container.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace checking{
namespace put_range{

    template<typename To, typename From>
    void do_check(From const& from)
    {
    	To to;
        namespace ns = checking::container;
        ns::do_check( to | _put_range( from ) );
    }

}// put_range
}// checking
}// v2
}// assign
}// boost

#endif
