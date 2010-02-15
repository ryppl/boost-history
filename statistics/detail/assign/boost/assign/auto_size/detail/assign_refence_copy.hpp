//////////////////////////////////////////////////////////////////////////////
// assign::detail::assign_reference_copy.hpp                               	//
//                                                                          //
//  (C) Copyright 2010 M.P.G                                        		//
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_ASSIGN_REFERENCE_COPY_MPG_2010_HPP
#define BOOST_ASSIGN_DETAIL_ASSIGN_REFERENCE_COPY_MPG_2010_HPP

namespace boost{
namespace assign{
namespace detail{

	// This is a reference wrapper whose assignment operator copies the value of
    // the rhs to the object pointed to.
    //
    // This is in contrast to assign_reference whose operator= rebinds the 
    // address of the internal pointer. To that effect, here, call rebind() 
	template< class T >
    struct assign_reference_copy
    {
        assign_reference_copy()
        { /* intentionally empty */ }

        assign_reference_copy( T& r ) : ref_(&r)
        { }

        void operator=( const T& r )
        {
			*ref_ = r;
        }

        operator T&() const
        {
            return *ref_;
        }

        void swap( assign_reference_copy& r )
        {
            std::swap( *ref_, *r.ref_ );
        }

		// destructor?

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
    
}// detail
}// assign
}// boost

#endif

