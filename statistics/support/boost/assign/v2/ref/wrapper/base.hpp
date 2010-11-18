//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_ASSIGN_V2_REF_WRAPPER_BASE
#include <boost/config.hpp>
#include <boost/type_traits/is_const.hpp>
    template<typename D, typename T>
    class wrapper_crtp;
#define BOOST_ASSIGN_V2_REF_WRAPPER_BASE(X) \
        typedef T type; \
        BOOST_STATIC_CONSTANT( \
            bool, \
            is_const = boost::is_const<T>::value \
        ); \
        \
        X(){} \
        \
        explicit X( T& r ) : ref_(&r) \
        { } \
        \
        void rebind(T& r ) \
    	{ \
            this->ref_ = &r; \
    	} \
        \
        typedef T& unwrap_type;\
        T& unwrap() const \
        { \
            return *this->ref_; \
        } \
        \
        T* get_pointer() const { return this->ref_; } \
        \
        using wrapper_crtp<wrapper,T>::operator=; \
        \
        protected: \
		friend class wrapper_crtp<wrapper,T>; \
        \
        T* ref_; \
        public: \
/**/

#endif
