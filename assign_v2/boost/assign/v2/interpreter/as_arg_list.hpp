//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_INTERPRETER_AS_ARG_LIST_ER_2010_HPP
#define BOOST_ASSIGN_V2_INTERPRETER_AS_ARG_LIST_ER_2010_HPP
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/iterator_range.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_interpreter_as_arg_list
namespace interpreter_aux{    

    template<typename /*<<Range>>*/R>
    class as_arg_list_adapter/*<-*/
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

    }/*->*/;

namespace result_of{

    template<typename R>
    struct /*<<Metafunction>>*/as_arg_list/*<-*/
    {
        typedef as_arg_list_adapter<R> type;
    }/*->*/;

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

}// interpreter_aux
    using interpreter_aux::as_arg_list;
namespace result_of{

    template<typename R>
    struct as_arg_list/*<-*/
        : interpreter_aux::result_of::as_arg_list<R>
    {}/*->*/;

}// result_of
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_INTERPRETER_AS_ARG_LIST_ER_2010_HPP
