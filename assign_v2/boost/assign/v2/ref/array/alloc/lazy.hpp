//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ARRAY_ALLOC_LAZY_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ARRAY_ALLOC_LAZY_ER_2010_HPP
#include <boost/assign/v2/ref/array/alloc/impl_traits.hpp>
#include <boost/assign/v2/ref/array/alloc/lazy_fwd.hpp>
#include <boost/assign/v2/ref/array/interface.hpp>
#include <boost/assign/v2/ref/wrapper.hpp>
#include <boost/shared_ptr.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace array_aux{

    template<size_type N, typename T, typename D>
    class lazy_alloc : public array_aux::interface<
        typename array_aux::impl_traits<N, T>::array_type,
        lazy_alloc<N, T, D>
    >
    {
        typedef typename array_aux::impl_traits<N, T>::array_type impl_;

        typedef ::boost::mpl::int_<N> size_;

        void alloc_if()const{
            if(!this->ptr){
               return this->alloc();
            }
        }

        void alloc()const{
            this->ptr = smart_ptr_(new impl_);
            assign_array(
                *this->ptr,
                static_cast<const D&>(*this)
            );
        }

        typedef array_aux::interface<impl_, lazy_alloc> super_t;

        public:

        impl_& impl(){
            this->alloc_if();
            return (*this->ptr);
        }

        impl_ const & impl()const{
            this->alloc_if();
            return (*this->ptr);
        }

        using super_t::operator[];

        private:

        typedef boost::shared_ptr<impl_> smart_ptr_;
        mutable smart_ptr_ ptr;

    };

}// array_aux
}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_ARRAY_ALLOC_LAZY_ER_2010_HPP
