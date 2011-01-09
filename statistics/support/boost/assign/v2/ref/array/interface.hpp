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
#include <algorithm>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/call_traits.hpp>

#include <boost/range.hpp>
#include <boost/call_traits.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/iterator_range.hpp> //iterator_range_detail::

#include <boost/assign/v2/ref/wrapper/framework.hpp>
#include <boost/assign/v2/ref/wrapper/crtp.hpp>
#include <boost/assign/v2/ref/detail/unwrap/range.hpp>
/*  #include <boost/assign/v2/detail/relational_op.hpp> */

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace array_aux{

    template<typename Impl>
    struct interface_traits{

        typedef Impl impl_;
        typedef impl_ const cimpl_;

		typedef typename Impl::value_type wrapper_;

		typedef typename ref::result_of::unwrap_range<impl_>::type range_;
		typedef typename ref::result_of::unwrap_range<cimpl_>::type crange_;
        typedef typename boost::unwrap_reference<wrapper_>::type value_type;
        typedef typename boost::add_reference<value_type>::type reference;
        typedef typename boost::call_traits<
            reference>::const_reference const_reference;
        typedef typename boost::range_iterator<range_>::type iterator;
        typedef typename boost::range_iterator<crange_>::type const_iterator;

        // wrappers
        #ifndef BOOST_MSVC
		    typedef typename boost::range_size<impl_>::type size_type;
        #endif
        #ifdef BOOST_MSVC
            typedef std::size_t size_type;
        #endif
        typedef typename boost::range_difference<range_>::type difference_type;

    };

    template<typename Impl, typename D>
    class interface
    /*:
       public relational_op_aux::crtp<
      		interface<Impl, D>
        >
	*/
    {

        typedef interface_traits<Impl> traits_;
        typedef typename traits_::wrapper_ wrapper_;
        typedef typename traits_::impl_ impl_;
        typedef typename traits_::cimpl_ cimpl_;
        typedef interface<Impl, D> this_;

        public:
        typedef wrapper_ wrapper_type;
        typedef typename traits_::value_type value_type;
        typedef typename traits_::iterator iterator;
        typedef typename traits_::const_iterator const_iterator;
        typedef typename traits_::size_type size_type;
        typedef typename traits_::difference_type difference_type;
        typedef typename traits_::reference reference;
        typedef typename traits_::const_reference const_reference;
		typedef typename boost::call_traits<value_type>::param_type param_type;

        BOOST_STATIC_CONSTANT(size_type, static_size = Impl::static_size);

        iterator begin()
        {
        	return boost::begin( this->wrappers() );
        }
        iterator end()
        {
        	return boost::end( this->wrappers() );
        }
        const_iterator begin()const
        {
        	return boost::begin( this->wrappers() );
        }
        const_iterator end()const
        {
        	return boost::end( this->wrappers() );
        }

        size_type size() const
        {
        	return this->wrappers().size();
        }
        bool empty() const
        {
        	return this->wrappers().empty();
        }

		void rebind(size_type i, reference t)
        {
        	return (this->wrappers())[i].rebind( t );
        }

        reference operator[](size_type i)
        {
        	return (this->wrappers())[i].unwrap();
        }
        const_reference operator[](size_type i)const
        {
             return (this->wrappers())[i].unwrap();
        }

        reference front()
        {
        	return (this->wrappers()).front().unwrap();
        }
        const_reference front() const
        {
        	return (this->wrappers()).front().unwrap();
        }
        reference back()
        {
        	return (this->wrappers()).back().unwrap();
        }
        const_reference back() const
        {
        	return (this->wrappers()).back().unwrap();
        }

        void assign(param_type val)
        {
        	typedef boost::mpl::int_<0> int_;
        	this->assign_impl( val, int_() );
        }

/*
        // Relational op
		template<typename R>
        bool equal_to(const R& r)const{
         	return ::boost::iterator_range_detail::equal(
            	(*this), r );
		}

		template<typename R>
        bool less_than(const R& r)const{
         	return ::boost::iterator_range_detail::less_than(
            	(*this), r );
		}
*/
		protected:

        void assign_impl(param_type val, boost::mpl::int_<static_size>)
        {
        	/* exit */
        }

		template<int i>
        void assign_impl(param_type val, boost::mpl::int_<i>)
        {
            this->wrappers()[i] = val;
            typedef boost::mpl::int_<i+1> next_;
            this->assign_impl( val, next_() );
        }

        public:

        typename traits_::range_ unwrap()
        {
        	return ref::unwrap_range( this->wrappers() );
        }
        typename traits_::crange_ unwrap()const{
        	return ref::unwrap_range( this->wrappers() );
        }

        impl_& wrappers(){
            return static_cast<D&>(*this).impl();
        }

        cimpl_& wrappers()const{
            return static_cast<const D&>(*this).impl();
        }

    };

}// array_aux
}// ref
}// v2
}// assign
}// boost

#endif
