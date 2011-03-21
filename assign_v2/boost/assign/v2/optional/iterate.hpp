//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_OPTIONAL_ITERATE_ER_2010_HPP
#define BOOST_ASSIGN_V2_OPTIONAL_ITERATE_ER_2010_HPP
#include <cstddef>
#include <boost/assign/v2/detail/keyword/ignore.hpp>
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#include <boost/assign/v2/detail/pp/ignore.hpp>
//#include <boost/assign/v2/interpreter/modifier.hpp>
#include <boost/assign/v2/interpreter/fwd.hpp>
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
namespace modifier_tag{ 

    struct iterate_arg
    {
        
        typedef std::size_t result_type;
        
        iterate_arg():i( 0 ){}
        iterate_arg( result_type i_ ):i( i_ ){}
        
        result_type operator()()const{ return this->i++; }

        typedef iterate_arg type;

        private:
        mutable result_type i;

    };

    template<typename Arg = iterate_arg::type > struct iterate{}; 

}// modifier_tag
namespace aux{
                
    template<typename Arg>
    class adapter_modifier< modifier_tag::iterate<Arg> >
    {

        typedef keyword_aux::ignore ignore_;
        typedef Arg arg_;
        // storing a copy of lambda::something has caused pbs, hence ptr
        typedef boost::shared_ptr<arg_> ptr_; 

        public:
                    
        adapter_modifier(): ptr( new arg_() ){}
        explicit adapter_modifier( 
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
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

#endif
                    
        template<typename C, typename T>
        void impl(C& cont, T& t )const
        {
            cont.at( (*this->ptr)() ) = t;
        }
        
        template<typename C, typename T>
        void impl(C& cont, T* t)const
        {
            cont.replace( (*this->ptr)(), t);
        }

        protected:
        ptr_ ptr;

    };

}// aux
}// v2
}// assign
}// boost

#include <boost/assign/v2/optional/modifier.hpp>
BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_KEYWORD(iterate)

#define BOOST_ASSIGN_V2_TAG modifier_tag::iterate<Arg>
BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_META_MODIFIER_TAG(iterate, BOOST_ASSIGN_V2_TAG)
#undef BOOST_ASSIGN_V2_TAG

#endif // BOOST_ASSIGN_V2_OPTIONAL_ITERATE_ER_2010_HPP
