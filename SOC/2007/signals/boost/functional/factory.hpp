/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger
  
    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#ifndef BOOST_FUNCTIONAL_FACTORY_HPP_INCLUDED
#   ifndef BOOST_PP_IS_ITERATING

#     include <boost/preprocessor/iteration/iterate.hpp>
#     include <boost/preprocessor/repetition/enum_params.hpp>
#     include <boost/preprocessor/repetition/enum_binary_params.hpp>

#     include <new>
#     include <boost/pointee.hpp>
#     include <boost/get_pointer.hpp>

#     ifndef BOOST_FUNCTIONAL_FACTORY_MAX_ARITY
#       define BOOST_FUNCTIONAL_FACTORY_MAX_ARITY 10
#     elif BOOST_FUNCTIONAL_FACTORY_MAX_ARITY < 3
#       undef  BOOST_FUNCTIONAL_FACTORY_MAX_ARITY
#       define BOOST_FUNCTIONAL_FACTORY_MAX_ARITY 3
#     endif

namespace boost
{
    template< typename Pointer, class Allocator = std::allocator<void> > 
    struct factory
    {
        typedef typename boost::remove_cv<Pointer>::type result_type;
        typedef typename boost::pointee<result_type>::type value_type;
        typedef typename Allocator::template rebind<value_type>::other
            allocator_type;
      private:
        mutable allocator_type obj_allocator;

        class memory;
      public:

        explicit factory(allocator_type const & a = allocator_type())
          : obj_allocator(a)
        { }

        inline result_type operator()() const
        {
            memory m(this->obj_allocator);
//            result_type result( new(m.get()) value_type() );
            obj_allocator.construct(m.get(), value_type());
            result_type result(m.get());
            m.release();
            return result;
        }

#     define BOOST_PP_FILENAME_1 <boost/functional/factory.hpp>
#     define BOOST_PP_ITERATION_LIMITS (1,BOOST_FUNCTIONAL_FACTORY_MAX_ARITY)
#     include BOOST_PP_ITERATE()

    };

    template< typename Pointer, class Allocator > 
    struct factory<Pointer&, Allocator>;
    // forbidden, would create a dangling reference

    template< typename Pointer, class Allocator > 
    class factory<Pointer,Allocator>::memory
    // implements scope-guarded memory allocation
    {
        allocator_type &  ref_allocator;
        value_type *      ptr_memory;
      public:

        explicit memory(allocator_type & a)
          : ref_allocator(a)
          , ptr_memory(0l)
        {
            this->ptr_memory = a.allocate(1);
        }

        value_type * get() const
        {
            return this->ptr_memory;
        }

        void release()
        {
            this->ptr_memory = 0l;
        }

        ~memory()
        {
            if (!! this->ptr_memory)
                this->ref_allocator.deallocate(this->ptr_memory,1);
        }
    };
}

#     define BOOST_FUNCTIONAL_FACTORY_HPP_INCLUDED
#   else // defined(BOOST_PP_IS_ITERATING)
#     define N BOOST_PP_ITERATION()

        template< BOOST_PP_ENUM_PARAMS(N, typename T) >
        inline result_type operator()(BOOST_PP_ENUM_BINARY_PARAMS(N,T,& a)) const
        {
            memory m(this->obj_allocator);
            result_type result( new(m.get()) value_type(
                BOOST_PP_ENUM_PARAMS(N, a)) );
            m.release();
            return result;
        }

#     undef N
#   endif // defined(BOOST_PP_IS_ITERATING)

#endif // include guard

