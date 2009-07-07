// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_DETAIL_ASSOCIATIVE_CONTAINER_BASE_HPP
#define BOOST_CLONEABLE_DETAIL_ASSOCIATIVE_CONTAINER_BASE_HPP

#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/detail/container_base.hpp>
#include <boost/cloneable/base.hpp>

namespace boost
{
    namespace cloneable
    {
        namespace detail
        {
            /// common base for associative containers
            template <class Cont, class Pred, class Base, class Alloc>
            struct associative_container_base 
                : container_base<Cont,Base,Alloc>
            {
                typedef container_base<Cont,Base,Alloc> parent_type;
                typedef Pred predicate_type;

                using parent_type::container_type;
                using parent_type::base_type;
                using parent_type::abstract_base_type;
                using parent_type::allocator_type;
                using parent_type::validate;
                using parent_type::new_instance;
                using parent_type::instance;

                using parent_type::iterator;
                using parent_type::const_iterator;
                using parent_type::reference;
                using parent_type::const_reference;
                using parent_type::value_type;

            private:
                predicate_type predicate;
                container_type container;

            protected:
                const container_type &impl() const
                {
                    return container;
                }
                container_type &impl()
                {
                    return container;
                }

            public:
                associative_container_base() 
                { 
                }
                associative_container_base(predicate_type p)
                    : predicate(p), container(p)
                {
                }
                associative_container_base(allocator_type &a)
                    : parent_type(a), container(a)
                {
                }
                associative_container_base(predicate_type p, allocator_type &a)
                    : parent_type(a), predicate(p), container(p,a)
                {
                }
                template <class II>
                associative_container_base(II F, II L, predicate_type pred = predicate_type())
                    : predicate(pred), container(F,L)
                {
                }

                template <class II>
                associative_container_base(II F, II L, allocator_type &a)
                    : parent_type(a), container(F,L)
                {
                }
                template <class II>
                associative_container_base(II F, II L, predicate_type pred, allocator_type &a)
                    : parent_type(a), predicate(pred), container(F,L)
                {
                }

                size_t size() const
                {
                    return impl().size();
                }
                bool empty() const
                {
                    return impl().empty();
                }

                iterator begin()
                {
                    return impl().begin();
                }
                iterator end()
                {
                    return impl().end();
                }
                const_iterator begin() const
                {
                    return impl().begin();
                }
                const_iterator end() const
                {
                    return impl().end();
                }

                template <class Fun>
                Fun for_each(Fun fun)
                {
                    BOOST_FOREACH(value_type &value, *this)
                    {
                        fun(value);
                    }
                }

            };

            template <class Cont, class Pred, class Base, class Alloc>
            bool operator==(const associative_container_base<Cont,Pred,Base,Alloc> &left, const associative_container_base<Cont,Pred,Base,Alloc> &right)
            {
                return left.size() == right.size() && std::equal(left.begin(), left.end(), right.begin());
            }

            template <class Cont, class Pred, class Base, class Alloc>
            bool operator<(const associative_container_base<Cont,Pred,Base,Alloc> &left, const associative_container_base<Cont,Pred,Base,Alloc> &right)
            {
                return std::lexicographical_compare(left.begin(), left.end(), right.begin(), right.end());
            }

        } // namespace detail

    } // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif //  BOOST_CLONEABLE_DETAIL_ASSOCIATIVE_CONTAINER_BASE_HPP

//EOF
