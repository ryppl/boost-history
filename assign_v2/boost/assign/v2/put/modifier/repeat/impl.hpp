//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_REPEAT_IMPL_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODIFIER_REPEAT_IMPL_ER_2010_HPP
#include <cstddef>
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_reference.hpp>
#endif
#include <boost/assign/v2/put/frame/modifier.hpp>
#include <boost/assign/v2/put/modifier/repeat/tag.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_aux{

    template<typename Arg>
    class modifier<modifier_tag::repeat<Arg> >
    {
        typedef modifier<Arg> inner_type;

        public:

        typedef std::size_t size_type;

        modifier() : n_( 0 ){}
        explicit modifier( inner_type inner, size_type n )
            : inner_( inner ), n_( n )
        {}

#if BOOST_ASSIGN_V2_ENABLE_CPP0X
		template<typename C, typename T>
			typename boost::disable_if<
			boost::is_reference<T>,
    		void
		>::type
        impl(C& cont, T&& t )const
        {
        	size_type m = this->size();\
        	while(m--) this->inner_.impl( cont, std::move( t ) /* TODO ? */ );
        }

#endif

		template<typename C, typename T>
		void impl(C& cont, T& t )const
        {
        	size_type m = this->size();
        	while(m--) this->inner_.impl( cont, t );
        }

        template<typename C, typename T>
        void impl(C& cont, T* t)const
        {
        	size_type m = this->size();
        	while(m--) this->inner_.impl( cont, t );
        }

        size_type const& size()const{ return this->n_; }

        protected:
        inner_type inner_;
        size_type n_;
    };

}// put_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_MODIFIER_REPEAT_IMPL_ER_2010_HPP
