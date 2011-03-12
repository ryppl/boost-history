//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_ITERATE_IMPL_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODIFIER_ITERATE_IMPL_ER_2010_HPP
#include <boost/assign/v2/detail/keyword/ignore.hpp>
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#include <boost/assign/v2/put/frame/modifier.hpp>
#include <boost/assign/v2/put/modifier/iterate/tag.hpp>
#include <boost/call_traits.hpp>
#include <boost/shared_ptr.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#include <boost/type_traits/is_reference.hpp>
#include <boost/utility/enable_if.hpp>
#endif

namespace boost{
namespace assign{
namespace v2{
namespace put_aux{
                
    template<typename Arg>
    class modifier< modifier_tag::iterate<Arg> >
    {

        typedef keyword_aux::ignore ignore_;
        typedef Arg arg_;
        // storing a copy of lambda::something has caused pbs, hence ptr
        typedef boost::shared_ptr<arg_> ptr_; 
                    
        public:
                    
        modifier(): ptr( new arg_() ){}
        explicit modifier( 
            ignore_,  
            typename boost::call_traits<arg_>::param_type arg 
        ) : ptr( new arg_( arg ) )
        {}

#if BOOST_ASSIGN_V2_ENABLE_CPP0X
        template<typename C, typename T>
            typename boost::disable_if<
            boost::is_reference<T>,
            void
        >::type
        impl(C& cont, T&& t )const
        {
            cont.at( (*this->ptr)() ) = std::move( t ); 
        }

#endif
                    
        template<typename C, typename T>
        void impl(C& cont, T& t )const
        {
            cont.at( (*this->ptr)() ) = t;
        }
        
        // TODO verify
        template<typename C, typename T>
        void impl(C& cont, T* t)const
        {
            cont.replace( this->arg_() , t);
        }
                    
        protected:
        ptr_ ptr;

    };

}// put_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_MODIFIER_ITERATE_IMPL_ER_2010_HPP
