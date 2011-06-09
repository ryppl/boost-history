//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_DO_PUT_ER_2011_HPP
#define BOOST_ASSIGN_V2_PUT_DO_PUT_ER_2011_HPP
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/assign/v2/put/put.hpp>
#include <boost/assign/v2/support/pp/forward.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#endif

namespace boost{
namespace assign{
namespace v2{
//[syntax_do_put
namespace interpreter_aux{

//<-
    template<typename R>
    struct do_put_super
    {
        typedef boost::iterator_range<
            typename boost::range_iterator<
                typename type_traits::param<R>::type
            >::type
        > type;
    };
//->

    template<
        typename R, typename O = empty_list_option_/*<-*/, int I = 0/*->*/
    >
    struct put_for_each_adapter/*<-*/
        : do_put_super<R>::type/*->*/
    {
//<-
        typedef typename do_put_super<R>::type super_t;

        public:

        put_for_each_adapter( BOOST_ASSIGN_V2_FORWARD_PARAM(R, range) )
            :super_t( BOOST_ASSIGN_V2_FORWARD_ARG(R, range) )
        {}

        put_for_each_adapter(
            O const& o, BOOST_ASSIGN_V2_FORWARD_PARAM(R, range)
        )
            :super_t( BOOST_ASSIGN_V2_FORWARD_ARG(R, range) ),
            options_( o )
        {}

        put_for_each_adapter(super_t const& super, O const& o)
            :super_t( super ), options_( o )
        {}

        O const& options()const{ return this->options_; }

        private:
        put_for_each_adapter();
        O options_;
//->
    };

    template<typename O = empty_list_option_>
    struct put_adapter
    {
//<-
        put_adapter(){}
        put_adapter(O const& o)
            :options_( o )
        {}
//->

        template<typename R>
        put_for_each_adapter<R, O>
        for_each( /*<-*/BOOST_ASSIGN_V2_IGNORE(/*->*/R&& range/*<-*/)
            BOOST_ASSIGN_V2_FORWARD_PARAM( R, range )/*->*/ )const/*<-*/
        {
            typedef put_for_each_adapter<R, O> result_;
            return result_( this->options_, range );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

        template<int I, typename R>
        put_for_each_adapter<R, O, I>
        for_each( /*<-*/BOOST_ASSIGN_V2_IGNORE(/*->*/R&& range/*<-*/)
            BOOST_ASSIGN_V2_FORWARD_PARAM( R, range )/*->*/ )const/*<-*/
        {
            typedef put_for_each_adapter<R, O, I> result_;
            return result_( this->options_, range );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

        template<typename Options>
        put_adapter<Options>
        operator%(Options const& options) const/*<-*/
        {
            return put_adapter<Options>( options );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
//<-
#if! BOOST_ASSIGN_V2_ENABLE_CPP0X

        // R const&

        template<typename R>
        put_for_each_adapter<R const, O>
        for_each( R const& range )const
        {
            typedef put_for_each_adapter<R const, O> result_;
            return result_( this->options_, range );
        }

        template<int I, typename R>
        put_for_each_adapter<R const, O, I>
        for_each( R const& range )const
        {
            typedef put_for_each_adapter<R const, O, I> result_;
            return result_( this->options_, range );
        }
#endif
        private:
        O options_;
//->
    };

    template<typename C, typename R, typename O>
    C& operator|(C& cont, put_for_each_adapter<R, O> const& a)/*<-*/
    {
        return (
            put( cont ) % a.options()
        ).for_each( a ).container();
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<typename C, typename R, typename O, int I>
    C& operator|(C& cont, put_for_each_adapter<R, O, I> const& a)/*<-*/
    {
        return (
            put( cont ) % a.options()
        ).template for_each<I>( a ).container();
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

}// interpreter_aux

//<-
namespace{
//->
    const interpreter_aux::put_adapter<> _do_put/*<-*/
        = interpreter_aux::put_adapter<>()/*->*/;
//<-
}
//->
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_DO_PUT_ER_2011_HPP
