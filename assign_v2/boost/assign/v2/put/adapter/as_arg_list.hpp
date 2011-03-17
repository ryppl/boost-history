//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_ADAPTER_AS_ARG_LIST_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_ADAPTER_AS_ARG_LIST_ER_2010_HPP
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/iterator_range.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_aux{

    template<typename R>
    class as_arg_list_adapter
    {

        typedef typename boost::range_iterator<R>::type ri_;
        typedef boost::iterator_range<ri_> ir_;
        public:

        as_arg_list_adapter(R& range)
            :ir(
                boost::make_iterator_range( range )
            )
        {}

        typedef ir_ result_type;

        result_type operator()()const{ return this->ir; }

        private:
        as_arg_list_adapter();
        ir_ ir;

    };

//[syntax_put_adapter_as_arg_list
namespace result_of{

    template<typename R>
    struct as_arg_list
    {
        typedef/*<-*/ BOOST_ASSIGN_V2_IGNORE(/*->*/ unspecified /*<-*/) as_arg_list_adapter<R> /*->*/type;
    };

}// result_of

    template<typename R>
    typename result_of::as_arg_list<R>::type
    as_arg_list(R& range)/*<-*/
    {

        return typename result_of::as_arg_list<R>::type( range );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<typename R>
    typename result_of::as_arg_list<R const>::type
    as_arg_list(R const& range)/*<-*/
    {
        return typename result_of::as_arg_list<R const>::type( range );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//]
}// put_aux
    using put_aux::as_arg_list;
namespace result_of{

    template<typename R>
    struct as_arg_list
        : put_aux::result_of::as_arg_list<R>
    {};

}// result_of
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_ADAPTER_AS_ARG_LIST_ER_2010_HPP
