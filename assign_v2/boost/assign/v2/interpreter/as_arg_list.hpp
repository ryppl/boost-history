//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_INTERPRETER_AS_ARG_LIST_ER_2011_HPP
#define BOOST_ASSIGN_V2_INTERPRETER_AS_ARG_LIST_ER_2011_HPP
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/iterator_range.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_interpreter_as_arg_list
namespace interpreter_aux{    

    template<
        typename R    // Range
    >
    struct as_arg_list_adapter 
        : boost::iterator_range<
            typename boost::range_iterator<R>::type
        >/*<-*/
    {
        
        typedef boost::iterator_range<
            typename boost::range_iterator<R>::type
        > super_t;
        
        public:

        as_arg_list_adapter(R& range)
            :super_t(
                boost::make_iterator_range( range )
            )
        {}

        private:
        as_arg_list_adapter();
    }/*->*/;

namespace result_of{

    template<typename R>
    struct as_arg_list/*<-*/
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
    using interpreter_aux::result_of::as_arg_list;
}// result_of

//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_INTERPRETER_AS_ARG_LIST_ER_2011_HPP
