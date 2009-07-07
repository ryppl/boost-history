// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_SET_HPP
#define BOOST_CLONEABLE_SET_HPP

#include <boost/ptr_container/ptr_set.hpp>
#include <boost/foreach.hpp>

#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/detail/container_base.hpp>
#include <boost/cloneable/adaptor.hpp>

namespace boost 
{
    namespace cloneable
    {
        /// a vector of heterogenous objects
        // TODO: move to boost/heterogenous/set.hpp, or boost/cloneable/containers/set.hpp
        template <class Base, class Pred, class Alloc>
        struct set
            : detail::associative_container_base<
                ptr_set<
                    abstract_base<Base>
                    , Pred
                    , allocator
                    , typename detail::make_clone_allocator<Alloc>::type>
                , Pred
                , Base
                , Alloc>
        {
            typedef detail::associative_container_base<
                    ptr_set<
                        abstract_base<Base>
                        , Pred
                        , allocator
                        , typename detail::make_clone_allocator<Alloc>::type>
                    , Pred
                    , Base
                    , Alloc>
                parent_type;

            typedef typename parent_type::container_type container_type;
            typedef typename parent_type::base_type base_type;
            typedef typename parent_type::abstract_base_type abstract_base_type;
            typedef typename parent_type::allocator_type allocator_type;
            typedef typename parent_type::value_type value_type;
            typedef typename parent_type::reference reference;
            typedef typename parent_type::const_reference const_reference;
            typedef typename parent_type::iterator iterator;
            typedef typename parent_type::const_iterator const_iterator;

            using parent_type::new_instance;
            using parent_type::validate;

            typedef set<Base, Pred, Alloc> this_type;

        public:
            set() 
            {
            }
            set(allocator_type &a) 
                : parent_type(a)
            {
            }

            set(predicate_type pred)
                : parnet_type(pred)
            {
            }
            set(predicate_type pred, allocator_type &a) 
                : parent_type(pred, a)
            {
            }

            template <class II>
            set(II F, II L, predicate_type pred = predicate_type())
                : parent_type(F, L, pred)
            {
            }

            template <class II>
            set(II F, II L, allocator_type &a)
                : parent_type(F, L, a)
            {
            }
            template <class II>
            set(II F, II L, predicate_type pred, allocator_type &a)
                : parent_type(F, L, pred, a)
            {
            }

        public:
            typedef std::pair<iterator, bool> insert_result;

            template <class U>
            struct emplace_result
            {
                typedef instance<U> instance_type;
                instance_type value;
                bool inserted;
                iterator where;
                emplace_result() : inserted(false) { }
                emplace_result(instance_type ins, const insert_result &result)
                    : value(ins), inserted(result.second), where(result.first) { }
            };

            template <class U>
            emplace_result<U> emplace(instance<U> value)
            {
                insert_result result = impl().insert(value.to_abstract());
                if (!result.second)
                    value.release();
                return emplace_result<U>(value, result);
            }

            template <class U>
            emplace_result<U> emplace()
            {
                return emplace(new_instance<U>());
            }

            template <class U, class A0>
            emplace_result<U> emplace(A0 a0)
            {
                return emplace(new_instance<U>(a0));
            }
            template <class U, class A0, class A1>
            emplace_result<U> emplace(A0 a0, A1 a1)
            {
                return emplace(new_instance<U>(a0, a1));
            }
            template <class U, class A0, class A1, class A2>
            emplace_result<U> emplace(A0 a0, A1 a1, A2 a2)
            {
                return emplace(new_instance<U>(a0, a1, a2));
            }

            struct detail
            {
                template <class U>
                struct find
                {
                    template <class Instance>
                    static iterator search(this_type *cont, Instance &value)
                    {
                        BOOST_SCOPE_EXIT((&value))
                        {
                            value.release();
                        }
                        BOOST_SCOPE_EXIT_END
                        return cont->find_instance<U>(value);
                    }

                    static iterator given(this_type *cont)
                    {
                        return search(cont, cont->new_instance<U>());
                    }

                    template <class A0>
                    static iterator given(this_type *cont, A0 a0)
                    {
                        return search(cont, cont->new_instance<U>(a0));
                    }
                    template <class A0, class A1>
                    static iterator given(this_type *cont, A0 a0, A1 a1)
                    {
                        return search(cont, cont->new_instance<U>(a0, a1));
                    }
                };
                struct default_key : base<default_key, base_type>
                {
                    default_key() { }
                    default_key(base_type const &X) : base<default_key, base_type>::base_type(X) { }
                };

                template <>
                struct find<base_type>
                {
                    template <class A0>
                    static iterator given(this_type *cont, A0 a0)
                    {
                        return cont->impl().find(default_key(base_type(a0)));
                    }
                    template <class A0, class A1>
                    static iterator given(this_type *cont, A0 a0, A1 a1)
                    {
                        return cont->impl().find(default_key(base_type(a0, a1)));
                    }
                };
            };
            template <class U>
            iterator find_instance(instance<U> value)
            {
                if (!value.exists())
                    return end();
                iterator found = impl().find(*value.to_abstract());
                if (found == impl().end())
                    return found;
                std::type_info const &found_type = found->get_type();
                if (found_type == typeid(U))
                    return found;
                return impl().end();
            }

            template <class U, class A0>
            iterator find(A0 a0)
            {
                return detail::find<U>::given(this, a0);
            }

        };
    
    } // namespace heterogenous

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_SET_HPP

//EOF
