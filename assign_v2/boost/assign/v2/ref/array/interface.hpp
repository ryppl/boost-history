//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ARRAY_INTERFACE_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ARRAY_INTERFACE_ER_2010_HPP
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/ref/wrapper.hpp>
#include <boost/call_traits.hpp>
#include <boost/config.hpp>
#include <boost/mpl/int.hpp>
#include <boost/range.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_ref_array_interface
namespace ref{
//<-
namespace array_aux{

    template<typename Impl>
    struct interface_traits{

        typedef Impl impl_;
        typedef impl_ const cimpl_;

        typedef typename Impl::value_type wrapper_;
        typedef typename boost::unwrap_reference<wrapper_>::type inner_type;
        typedef impl_ range_;
        typedef cimpl_ crange_;
        typedef typename boost::range_iterator<range_>::type iterator;
        typedef typename boost::range_iterator<crange_>::type const_iterator;

        typedef typename boost::range_value<range_>::type value_type;
        typedef typename boost::range_reference<range_>::type reference;
        typedef typename boost::range_reference<crange_>::type const_reference;

        typedef typename ref::result_of::get<wrapper_>::type result_of_get_;
        typedef typename boost::call_traits<inner_type>::param_type param_type;

        #ifndef BOOST_MSVC
            typedef typename boost::range_size<impl_>::type size_type;
        #else
            typedef std::size_t size_type;
        #endif
        typedef typename boost::range_difference<range_>::type difference_type;

    };
//->

    template<typename Impl, typename D>
    class interface
    {
//<-
        typedef interface_traits<Impl> traits_;
        typedef typename traits_::wrapper_ wrapper_;
        typedef typename traits_::impl_ impl_;
        typedef typename traits_::cimpl_ cimpl_;
        typedef interface<Impl, D> this_;
        typedef typename traits_::result_of_get_ result_of_get_;
//->
        typedef /*<-*/typename traits_::param_type BOOST_ASSIGN_V2_IGNORE(/*->*/unspecified/*<-*/)/*->*/param_type;

        public:

        typedef /*<-*/wrapper_ BOOST_ASSIGN_V2_IGNORE(/*->*/unspecified/*<-*/)/*->*/ wrapper_type;
        typedef /*<-*/typename traits_::inner_type BOOST_ASSIGN_V2_IGNORE(/*->*/unspecified/*<-*/)/*->*/ inner_type;
        typedef /*<-*/typename traits_::value_type BOOST_ASSIGN_V2_IGNORE(/*->*/unspecified/*<-*/)/*->*/ value_type;
        typedef /*<-*/typename traits_::iterator BOOST_ASSIGN_V2_IGNORE(/*->*/unspecified/*<-*/)/*->*/ iterator;
        typedef /*<-*/typename traits_::const_iterator BOOST_ASSIGN_V2_IGNORE(/*->*/unspecified/*<-*/)/*->*/ const_iterator;
        typedef /*<-*/typename traits_::size_type BOOST_ASSIGN_V2_IGNORE(/*->*/unspecified/*<-*/)/*->*/ size_type;
        typedef /*<-*/typename traits_::difference_type BOOST_ASSIGN_V2_IGNORE(/*->*/unspecified/*<-*/)/*->*/ difference_type;
        typedef /*<-*/typename traits_::reference BOOST_ASSIGN_V2_IGNORE(/*->*/unspecified/*<-*/)/*->*/ reference;
        typedef /*<-*/typename traits_::const_reference BOOST_ASSIGN_V2_IGNORE(/*->*/unspecified/*<-*/)/*->*/ const_reference;

//<-
        BOOST_STATIC_CONSTANT(size_type, static_size/*<-*/ = Impl::static_size/*->*/);
//->
        /*<-*/BOOST_ASSIGN_V2_IGNORE(/*->*/size_type static_size/*<-*/)/*->*/;

        iterator begin()/*<-*/
        {
            return boost::begin( this->wrappers() );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        iterator end()/*<-*/
        {
            return boost::end( this->wrappers() );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        const_iterator begin()const/*<-*/
        {
            return boost::begin( this->wrappers() );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        const_iterator end()const/*<-*/
        {
            return boost::end( this->wrappers() );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        size_type size() const/*<-*/
        {
            return this->wrappers().size();
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        bool empty() const/*<-*/
        {
            return this->wrappers().empty();
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        void rebind(size_type i, result_of_get_ t)/*<-*/
        {
            return (this->wrappers())[i].rebind( t );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        reference operator[](size_type i)/*<-*/
        {
            return this->elem_impl( this->wrappers()[i] );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        const_reference operator[](size_type i)const/*<-*/
        {
            return this->elem_impl( this->wrappers()[i] );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        reference front()/*<-*/
        {
            return this->elem_impl( this->wrappers().front() );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        const_reference front() const/*<-*/
        {
            return this->elem_impl( this->wrappers().front() );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        reference back()/*<-*/
        {
            return this->elem_impl( this->wrappers().back() );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        const_reference back() const/*<-*/
        {
            return this->elem_impl( this->wrappers().back() );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        void assign(param_type val)/*<-*/
        {
            typedef ::boost::mpl::int_<0> int_;
            this->assign_impl( val, int_() );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
//<-
        protected:

        void assign_impl(param_type val, ::boost::mpl::int_<static_size>)
        {
            /* exit */
        }

        template<int i>
        void assign_impl(param_type val, ::boost::mpl::int_<i>)
        {
            this->wrappers()[i] = val;
            typedef ::boost::mpl::int_<i+1> next_;
            this->assign_impl( val, next_() );
        }

        template<typename W>
        reference elem_impl(W& w)
        {
            return w; 
        }
        template<typename W>
        const_reference elem_impl(W & w)const
        {
            return w;
        }

        public:

        impl_& wrappers()/*<-*/
        {
            return static_cast<D&>(*this).impl();
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        cimpl_& wrappers()const/*<-*/
        {
            return static_cast<const D&>(*this).impl();
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//->
    };

//<-
}// array_aux
//->
}// ref
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_ARRAY_INTERFACE_ER_2010_HPP
