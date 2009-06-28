//
// Boost.Pointer Container
//
//  Copyright Thorsten Ottosen 2003-2005. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/ptr_container/
//

#ifndef BOOST_PTR_CONTAINER_PTR_VECTOR_HPP
#define BOOST_PTR_CONTAINER_PTR_VECTOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <vector>
#include <boost/ptr_container/ptr_sequence_adapter.hpp>

namespace boost
{


	//template
	//< 
	//	class T, 
	//	class Allocator      = std::allocator<void*>,
	//	class CloneAllocator = cloneable::make_cloneable_allocator<Al>
	//>

    template
    < 
        class T, 
        class CloneAllocator = heap_clone_allocator,
        class Allocator      = std::allocator<void*>
    >
    class ptr_vector : public 
        ptr_sequence_adapter< T, 
                              std::vector<void*,Allocator>, 
                              CloneAllocator >
    {  
        typedef ptr_sequence_adapter< T, 
                                      std::vector<void*,Allocator>, 
                                      CloneAllocator > 
            base_class;

        typedef ptr_vector<T,CloneAllocator,Allocator> this_type;
        
    public:

        BOOST_PTR_CONTAINER_DEFINE_SEQEUENCE_MEMBERS( ptr_vector, 
                                                      base_class,
                                                      this_type )
        
        explicit ptr_vector( size_type n,
                             const allocator_type& alloc = allocator_type() )
          : base_class(alloc)
        {
            this->base().reserve( n );
        }        

		template <class U>
		U *allocate_type()
		{
			typename Allocator::template rebind<U>::other alloc(get_allocator());
			return alloc.allocate(1);
		}
		template <class U>
		U *construct_type()
		{
			typename Allocator::template rebind<U>::other alloc(get_allocator());
			U *ptr = alloc.allocate(1);
			alloc.construct(ptr);
			return ptr;
		}
		template <class U>
		void push_back()
		{
			U *ptr = construct_type<U>();
			base().push_back(ptr);
		}
		template <class U, class A0>
		void push_back(A0 a0)
		{
			U *ptr = allocate_type<U>();
			new (ptr) U(a0);
			base().push_back(ptr);
		}
		template <class U, class A0, class A1>
		void push_back(A0 a0, A1 a1)
		{
			U *ptr = allocate_type<U>();
			new (ptr) U(a0, a1);
			base().push_back(ptr);
		}
		template <class U, class A0, class A1, class A2>
		void push_back(A0 a0, A1 a1, A2 a2)
		{
			U *ptr = allocate_type<U>();
			new (ptr) U(a0, a1, a2);
			base().push_back(ptr);
		}
	};

    //////////////////////////////////////////////////////////////////////////////
    // clonability

    template< typename T, typename CA, typename A >
    inline ptr_vector<T,CA,A>* new_clone( const ptr_vector<T,CA,A>& r )
    {
        return r.clone().release();
    }

    /////////////////////////////////////////////////////////////////////////
    // swap

    template< typename T, typename CA, typename A >
    inline void swap( ptr_vector<T,CA,A>& l, ptr_vector<T,CA,A>& r )
    {
        l.swap(r);
    }
    
}

#endif
