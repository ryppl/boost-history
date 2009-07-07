// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_MAP_HPP
#define BOOST_CLONEABLE_MAP_HPP

#include <boost/ptr_container/ptr_map.hpp>
#include <boost/foreach.hpp>

#include <boost/cloneable/detail/make_clone_allocator.hpp>
#include <boost/cloneable/detail/associative_container_base.hpp>

namespace boost 
{
    namespace cloneable
    {
        //namespace heterogenous
        //{
            /// a vector of heterogenous objects
            // TODO: move to boost/heterogenous/map
            template <class Key, class Base, class Pred, class Alloc>
            struct map
                : detail::associative_container_base<
                    ptr_map<
                        Key
                        , abstract_base<Base>
                        , Pred
                        , allocator
                        , typename detail::make_clone_allocator<Alloc>::type>
                    , Pred
                    , Base
                    , Alloc>
            {
                typedef detail::associative_container_base<
                    ptr_map<
                            Key
                            , abstract_base<Base>
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

                typedef typename container_type::key_type key_type;
                typedef typename container_type::mapped_type mapped_type;

                typedef map<Key, Base, Pred, Alloc> this_type;

            public:
                map() 
                {
                }
                map(allocator_type &a) 
                    : parent_type(a)
                {
                }

                //template <class II>
                //map(II F, II L)
                //    : impl(F,L, get_allocator())
                //{
                //}
                //template <class II>
                //map(II F, II L, allocator_type &a)
                //    : parent_type(a), impl(F,L, get_allocator())
                //{
                //}


            public:
                typedef std::pair<iterator, bool> map_insert_result;

                template <class V>
                map_insert_result insert(key_type key)
                {
                    return impl().insert(key, new_instance<V>().to_abstract());
                }
                template <class V, class A0>
                map_insert_result insert(key_type key, A0 a0)
                {
                    return impl().insert(key, new_instance<V>(a0).to_abstract());
                }
                template <class K, class A0, class A1>
                map_insert_result insert(key_type key, A0 a0, A1 a1)
                {
                    return impl().insert(key, new_instance<V>(a0,a1).to_abstract());
                }
                template <class K, class A0, class A1, class A2>
                map_insert_result insert(key_type key, A0 a0, A1 a1, A2 a2)
                {
                    return impl().insert(key, new_instance<V>(a0,a1,a2).to_abstract());
                }

                template <class Fun>
                Fun for_each_key(Fun fun)
                {
                    BOOST_FOREACH(value_type &val, *this)
                    {
                        fun(val.first);
                    }
                    return fun;
                }

                template <class Ty, class Fun>
                Fun for_each_mapped(Fun fun) const
                {
                    BOOST_FOREACH(value_type const &value, *this)
                    {
                        if (Ty *ptr = dynamic_cast<Ty *>(&val.second))
                        {
                            fun(*ptr);
                        }
                    }
                    return fun;
                }

                iterator find(key_type const &key)
                {
                    return impl().find(key);
                }

                const_iterator find(key_type const &key) const
                {
                    return impl().find(key);
                }
            };
        
        //} // namespace heterogenous

    } // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_MAP_HPP

//EOF
