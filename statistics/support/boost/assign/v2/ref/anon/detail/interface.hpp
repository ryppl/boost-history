//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
/*
#ifndef BOOST_ASSIGN_V2_REF_CONTAINER_STATIC_ARRAY_DETAIL_INTERFACE_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONTAINER_STATIC_ARRAY_DETAIL_INTERFACE_ER_2010_HPP
#include <algorithm>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/array.hpp>
#include <boost/range.hpp>
#include <boost/call_traits.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/iterator_range.hpp> //iterator_range_detail::

#include <boost/assign/v2/ref/wrapper/framework.hpp>
#include <boost/assign/v2/ref/wrapper/crtp.hpp>
#include <boost/assign/v2/ref/detail/unwrap/range.hpp>
#include <boost/assign/v2/detail/relational_op/relational_op.hpp>
#include <boost/assign/v2/put/range/convert.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace anon_aux{

    template<std::size_t N,typename Tag,typename T>
    struct interface_traits{
    
        typedef Tag assign_tag_;
        typedef typename ref::wrapper<assign_tag_,T> wrapper_;
        typedef boost::array<wrapper_,N> impl_;
		typedef typename ref::result_of::unwrap_range<impl_>::type range_;
		typedef typename ref::result_of::unwrap_range<
        	impl_ const>::type range_c_;
        
        typedef typename boost::unwrap_reference<T>::type value_type;
		typedef typename boost::add_reference<value_type>::type reference;
        typedef typename boost::call_traits<reference>::const_reference 
        	const_reference;
        typedef typename boost::range_iterator<range_>::type iterator;
        typedef typename boost::range_iterator<range_c_>::type const_iterator;
        // wrappers
        #ifndef BOOST_MSVC
		    typedef typename boost::range_size<impl_>::type size_type;
        #endif
        #ifdef BOOST_MSVC 
            typedef std::size_t size_type; 
        #endif
        typedef typename boost::range_difference<range_>::type difference_type;

    };
                   
    // Requirements: 
    // D.impl()				Returns an array of ref-wrappers
    // D.impl()const		Returns a const array of ref-wrappers
    template<std::size_t N,typename Tag,typename T,typename D>
    class interface 
      :  public relational_op_aux::crtp<
      		interface<N,Tag,T,D>
        >
    {
    
        typedef interface_traits<N,Tag,T> traits_;
        typedef typename traits_::wrapper_ wrapper_;
        typedef typename traits_::impl_ impl_;
        typedef interface<N,T,Tag,D> this_;

        public:
        typedef typename traits_::value_type value_type;
        typedef typename traits_::iterator iterator;
        typedef typename traits_::const_iterator const_iterator;
        typedef typename traits_::size_type size_type;
        typedef typename traits_::difference_type difference_type;
        typedef typename traits_::reference reference;
        typedef typename traits_::const_reference const_reference;
                
        BOOST_STATIC_CONSTANT(size_type, static_size = N);
                                            
        iterator begin() 
        { 
        	return boost::begin( this->unwrap() ); 
        }
        iterator end() 
        { 
        	return boost::end( this->unwrap() ); 
        }
        const_iterator begin()const 
        { 
        	return boost::begin( this->unwrap() ); 
        }
        const_iterator end()const 
        { 
        	return boost::end( this->unwrap() ); 
        }

        size_type size() const
        { 
        	return this->impl().size(); 
        }
        bool empty() const
        { 
        	return this->impl().empty(); 
        }
                
        reference operator[](size_type i)
        { 
        	return (this->impl())[i].unwrap(); 
        }
        const_reference operator[](size_type i)const
        { 
             return (this->impl())[i].unwrap(); 
        }
			
        reference front()
        { 
        	return (this->impl()).front().unwrap(); 
        }
        const_reference front() const
        { 
        	return (this->impl()).front().unwrap(); 
        }
        reference back()
        { 
        	return (this->impl()).back().unwrap(); 
        }
        const_reference back() const
        { 
        	return (this->impl()).back().unwrap(); 
        }
        
        void swap(interface& other)
        { 
            typedef boost::mpl::int_<N> int_n_;
            this->swap_impl(other,int_n_());
        }

        void assign(T& val)
        { 
            impl_& ra = this->impl();
            boost::for_each(ra, functional::assigner<T>( val ));
        }

		BOOST_ASSIGN_V2_CONVERT_CONVERT_MF
		BOOST_ASSIGN_V2_CONVERT_OPERATOR_MF

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

        protected:
        typedef boost::mpl::false_ false_;
        typedef boost::mpl::true_ true_;

        void swap_impl(interface& other, boost::mpl::int_<0>){}

        template<int N1>
        void swap_impl(interface& other, boost::mpl::int_<N1>)
        {
            reference lhs = (*this)[N1-1];
            reference rhs = (other)[N1-1];
            lhs.swap(rhs); 
            typedef boost::mpl::int_<N1-1> next_int_;
            this->swap_impl( other, next_int_() );
        }
        
        typename traits_::range_ unwrap()
        { 
        	return ref::unwrap_range( this->impl() ); 
        }
        typename traits_::range_c_ unwrap()const{ 	
        	return ref::unwrap_range( this->impl() ); 
        }
        
        impl_& impl(){ 
            return static_cast<D&>(*this).impl();
        }

        impl_ const& impl()const{ 
            return static_cast<const D&>(*this).impl();
        }

    };

}// anon_aux

namespace container_type_traits{
	// TODO

//    template<typename T,std::size_t sz>
//    struct is_static_array<boost::array<T, sz> > : boost::mpl::true_{};

}// container_type_traits
}// ref
}// v2
}// assign
}// boost

#endif
*/