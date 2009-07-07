// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_VECTOR_HPP
#define BOOST_CLONEABLE_VECTOR_HPP

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/foreach.hpp>

#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/detail/sequence_container_base.hpp>

namespace boost 
{
    namespace cloneable
    {
        template <class Base, class Alloc>
        struct vector
            : detail::sequence_container_base<
                ptr_vector<
                    abstract_base<Base>
                    , allocator
                    , typename detail::make_clone_allocator<Alloc>::type >
                , Base
                , Alloc>
        {
            typedef detail::sequence_container_base<
                    ptr_vector<abstract_base<Base>
                        , allocator
                        , typename detail::make_clone_allocator<Alloc>::type >
                    , Base
                    , Alloc> 
                parent_type;
            using parent_type::base_type;
            using parent_type::abstract_base_type;
            using parent_type::allocator_type;
            using parent_type::validate;
            using parent_type::new_instance;
            using parent_type::value_type;
            using parent_type::reference;
            using parent_type::const_reference;
            using parent_type::iterator;
            using parent_type::const_iterator;

        public:

            vector()
            {
            }

            vector(allocator_type &a) 
                : parent_type(a)
            {
            }

            reference at(size_t n)
            {
                return impl().at(n);
            }
            const_reference at(size_t n) const
            {
                return impl().at(n);
            }
            reference operator[](size_t n)
            {
                return impl()[n];
            }
            const_reference operator[](size_t n) const
            {
                return impl()[n];
            }

            template <class Other>
            bool is_type_at(size_t n) const
            {
                return ptr_at<Other>(n) != 0;
            }
            
            template <class Other>
            Other &as(size_t n)
            {
                Other *ptr = pointer<Other>(n);
                if (ptr == 0)
                    throw std::bad_cast();
                return *ptr;
            }
            template <class Other>
            const Other &as(size_t n) const
            {
                const Other *ptr = pointer<const Other>(n);
                if (ptr == 0)
                    throw std::bad_cast();
                return *ptr;
            }

            template <class Other>
            Other *pointer(size_t n)
            {
                return dynamic_cast<Other *>(&at(n));
            }
            template <class Other>
            const Other *pointer(size_t n) const
            {
                return dynamic_cast<const Other *>(&at(n));
            }

        };
    
    } // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_VECTOR_HPP

//EOF
