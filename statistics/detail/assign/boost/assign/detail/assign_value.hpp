//////////////////////////////////////////////////////////////////////////////
// assign::detail::assign_value.hpp                               			//
//                                                                          //
//  (C) Copyright 2010 M.P.G                                        		//
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_ASSIGN_VALUE_MPG_2010_HPP
#define BOOST_ASSIGN_DETAIL_ASSIGN_VALUE_MPG_2010_HPP

namespace boost{
namespace assign_detail{

	// This is a reference wrapper whose assignment operator copies the value of
    // the rhs to the object pointed to.
    //
    // This is in contrast to assign_reference whose operator= rebinds the 
    // address of the internal pointer. To that effect, call instead rebind() 
	template< class T >
    struct assign_value
    {
        assign_value()
        { /* intentionally empty */ }

        assign_value( T& r ) : ref_(&r)
        { }

        void operator=( const T& r )
        {
			*ref_ = r;
        }

        operator T&() const
        {
            return *ref_;
        }

        void swap( assign_value& r )
        {
            std::swap( *ref_, *r.ref_ );
        }

        T& get_ref() const
        {
            return *ref_;
        }

		void rebind( T & r )
    	{
       		ref_ = &r;
    	}
        
    private:
        T* ref_;

    };
    
}// assign_detail
}// boost

#endif

