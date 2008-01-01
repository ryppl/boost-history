
// Copyright (C) 2003-2004 Jeremy B. Maitin-Shepard.
// Copyright (C) 2005-2007 Daniel James
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNORDERED_DETAIL_HASH_TABLE_HPP_INCLUDED
#define BOOST_UNORDERED_DETAIL_HASH_TABLE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp>

#include <cstddef>
#include <cmath>
#include <algorithm>
#include <utility>
#include <stdexcept>

#include <boost/iterator.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/limits.hpp>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/unordered/detail/allocator.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/detail/workaround.hpp>

#include <boost/mpl/aux_/config/eti.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, <= 0x0551)
#define BOOST_UNORDERED_BORLAND_BOOL(x) (bool)(x)
#else
#define BOOST_UNORDERED_BORLAND_BOOL(x) x
#endif

#if BOOST_WORKAROUND(BOOST_MSVC, < 1300)
#define BOOST_UNORDERED_MSVC_RESET_PTR(x) unordered_detail::reset(x)
#else
#define BOOST_UNORDERED_MSVC_RESET_PTR(x)
#endif

namespace boost {
    namespace unordered_detail {
        template <class T> struct type_wrapper {};

        const static std::size_t default_initial_bucket_count = 50;
        const static float minimum_max_load_factor = 1e-3f;
        inline std::size_t next_prime(std::size_t n);

        template <class T>
        inline void hash_swap(T& x, T& y)
        {
#if defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
            std::swap(x,y);
#else
            using std::swap;
            swap(x, y);
#endif
        }

        inline std::size_t float_to_size_t(float f)
        {
            return f > static_cast<float>((std::numeric_limits<std::size_t>::max)()) ?
                (std::numeric_limits<std::size_t>::max)() :
                static_cast<std::size_t>(f);
        }

        // prime number list, accessor

        static const std::size_t prime_list[] = {
            53ul, 97ul, 193ul, 389ul, 769ul,
            1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
            49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
            1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
            50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
            1610612741ul, 3221225473ul, 4294967291ul };

        // no throw
        inline std::size_t next_prime(std::size_t n) {
            std::size_t const* const prime_list_end = prime_list +
                sizeof(prime_list) / sizeof(*prime_list);
            std::size_t const* bound =
                std::lower_bound(prime_list,prime_list_end, n);
            if(bound == prime_list_end)
                bound--;
            return *bound;
        }

        // no throw
        inline std::size_t prev_prime(std::size_t n) {
            std::size_t const* const prime_list_end = prime_list +
                sizeof(prime_list) / sizeof(*prime_list);
            std::size_t const* bound =
                std::upper_bound(prime_list,prime_list_end, n);
            if(bound != prime_list)
                bound--;
            return *bound;
        }

        // pair_cast - used to convert between pair types.

        template <class Dst1, class Dst2, class Src1, class Src2>
        inline std::pair<Dst1, Dst2> pair_cast(std::pair<Src1, Src2> const& x)
        {
            return std::pair<Dst1, Dst2>(Dst1(x.first), Dst2(x.second));
        }

        // Hash Bucket
        //
        // all no throw

        template <typename Alloc>
        struct bucket
        {
            typedef BOOST_DEDUCED_TYPENAME
                boost::unordered_detail::rebind_wrap<Alloc, bucket>::type
                bucket_allocator;

            typedef BOOST_DEDUCED_TYPENAME allocator_pointer<bucket_allocator>::type bucket_ptr;
            typedef BOOST_DEDUCED_TYPENAME allocator_reference<bucket_allocator>::type bucket_reference;

#if 1
            typedef bucket_ptr link_ptr;
#else
            // This alternative version of link_ptr is used to check that the
            // implementation is type safe wrt bucket_ptr and link_ptr.
            //
            // It's a sort of strict typedef.

            struct link_ptr {
                link_ptr() : ptr_() { BOOST_UNORDERED_MSVC_RESET_PTR(ptr_); }
                explicit link_ptr(bucket_ptr p) : ptr_(p) {}
                bucket_reference operator*() const { return *ptr_; }
                bucket* operator->() const { return &*ptr_; }
                operator bool() const { return ptr_; }
                bool operator==(link_ptr const& x) const { return ptr_ == x.ptr_; }
                bool operator!=(link_ptr const& x) const { return ptr_ != x.ptr_; }
            private:
                bucket_ptr ptr_;
            };
#endif

        private:
            bucket& operator=(bucket const&);
        public:
            link_ptr next_;

            bucket() : next_()
            {
                BOOST_UNORDERED_MSVC_RESET_PTR(next_);
            }

            bucket(bucket const& x) : next_(x.next_)
            {
                // Only copy construct when allocating.
                BOOST_ASSERT(!x.next_);
            }

            bool empty() const
            {
                return !this->next_;
            }
        };

        // Hash Node
        //
        // all no throw

        template <typename Alloc>
        struct node_base_equivalent_keys : bucket<Alloc>
        {
        public:
            typedef BOOST_DEDUCED_TYPENAME bucket<Alloc>::bucket_ptr bucket_ptr;
            typedef BOOST_DEDUCED_TYPENAME bucket<Alloc>::link_ptr link_ptr;
            typedef std::size_t size_type; 

            node_base_equivalent_keys() : group_prev_()
            {
                BOOST_UNORDERED_MSVC_RESET_PTR(group_prev_);
            }

            link_ptr group_prev_;

            static inline link_ptr& prev_in_group(link_ptr n) {
                return static_cast<node_base_equivalent_keys&>(*n).group_prev_;
            }

            // pre: Must be pointing to the first node in a group.
            static inline link_ptr last_in_group(link_ptr n) {
                BOOST_ASSERT(BOOST_UNORDERED_BORLAND_BOOL(n) && n != prev_in_group(n)->next_);
                return prev_in_group(n);
            }

            // pre: Must be pointing to the first node in a group.
            static inline link_ptr& next_group(link_ptr n) {
                BOOST_ASSERT(BOOST_UNORDERED_BORLAND_BOOL(n) && n != prev_in_group(n)->next_);
                return prev_in_group(n)->next_;
            }

            // pre: Must be pointing to a node
            static inline node_base_equivalent_keys& get_node(link_ptr p) {
                BOOST_ASSERT(p);
                return static_cast<node_base_equivalent_keys&>(*p);
            }

            static inline size_type group_count(link_ptr it)
            {
                size_type count = 0;
                link_ptr first = it;
                do {
                    ++count;
                    it = prev_in_group(it);
                } while (it != first); // throws, strong
                return count;
            }

            static inline link_ptr* get_for_erase(bucket_ptr bucket, link_ptr n)
            {
                // If the element isn't the first in its group, then
                // the link to it will be found in the previous element
                // in the group.
                link_ptr* it = &prev_in_group(n)->next_;
                if(*it == n) return it;

                // The element is the first in its group, so iterate
                // throught the groups, checking against the first element.
                it = &bucket->next_;
                while(*it != n) it = &next_group(*it);
                return it;
            }

            static inline void link_node(link_ptr n, link_ptr pos)
            {
                node_base_equivalent_keys& node_ref = get_node(n);
                node_base_equivalent_keys& pos_ref = get_node(pos);
                node_ref.next_ = pos_ref.group_prev_->next_;
                node_ref.group_prev_ = pos_ref.group_prev_;
                pos_ref.group_prev_->next_ = n;
                pos_ref.group_prev_ = n;
            }
            
            static inline void link_node_in_bucket(link_ptr n, bucket_ptr base)
            {
                node_base_equivalent_keys& node_ref = get_node(n);
                node_ref.next_ = base->next_;
                node_ref.group_prev_ = n;
                base->next_ = n;
            }

            static inline void link_group(link_ptr n, bucket_ptr base)
            {
                node_base_equivalent_keys& node_ref = get_node(n);
                node_base_equivalent_keys& last_ref = get_node(node_ref.group_prev_);
                last_ref.next_ = base->next_;
                base->next_ = n;
            }

            static inline void unlink_node(link_ptr* pos)
            {
                node_base_equivalent_keys* n = &get_node(*pos);
                link_ptr next = n->next_;

                if(n->group_prev_ == *pos) {
                    // The deleted node is the sole node in the group, so
                    // no need to unlink it from a group.
                }
                else if(BOOST_UNORDERED_BORLAND_BOOL(next) && prev_in_group(next) == *pos)
                {
                    // The deleted node is not at the end of the group, so
                    // change the link from the next node.
                    prev_in_group(next) = n->group_prev_;
                }
                else {
                    // The deleted node is at the end of the group, so the
                    // first node in the group is pointing to it.
                    // Find that to change its pointer.
                    link_ptr it = n->group_prev_;
                    while(prev_in_group(it) != *pos) {
                        it = prev_in_group(it);
                    }
                    prev_in_group(it) = n->group_prev_;
                }
                *pos = next;
            }

            // Break a ciruclar list into two, with split as the beginning
            // of the second group (if split is at the beginning then don't
            // split).
            static inline link_ptr split_group(link_ptr split) {
                // If split is at the beginning of the group then there's
                // nothing to split.
                if(prev_in_group(split)->next_ != split)
                    return unordered_detail::null_ptr<link_ptr>();

                // Find the start of the group.
                link_ptr start = split;
                do {
                    start = prev_in_group(start);
                } while(prev_in_group(start)->next_ == start);

                link_ptr last = prev_in_group(start);
                prev_in_group(start) = prev_in_group(split);
                prev_in_group(split) = last;

                return start;
            }

            static inline void split_group(link_ptr split1, link_ptr split2) {
                link_ptr begin1 = split_group(split1);
                link_ptr begin2 = split_group(split2);

                if(BOOST_UNORDERED_BORLAND_BOOL(begin1) && split1 == begin2) {
                    link_ptr end1 = prev_in_group(begin1);
                    prev_in_group(begin1) = prev_in_group(begin2);
                    prev_in_group(begin2) = end1;
                }
            }
        };

        template <class Alloc>
        struct node_base_unique_keys : bucket<Alloc>
        {
            typedef BOOST_DEDUCED_TYPENAME bucket<Alloc>::bucket_ptr bucket_ptr;
            typedef BOOST_DEDUCED_TYPENAME bucket<Alloc>::link_ptr link_ptr;
            typedef std::size_t size_type;

            static inline link_ptr& prev_in_group(link_ptr) {
                BOOST_ASSERT(false);
            }

            static inline link_ptr last_in_group(link_ptr n) {
                return n;
            }

            static inline link_ptr& next_group(link_ptr n) {
                BOOST_ASSERT(n);
                return n->next_;
            }

            // pre: Must be pointing to a node
            static inline node_base_unique_keys& get_node(link_ptr p) {
                BOOST_ASSERT(p);
                return static_cast<node_base_unique_keys&>(*p);
            }

            static inline size_type group_count(link_ptr){
                return 1;
            }

            static inline link_ptr* get_for_erase(bucket_ptr bucket, link_ptr n)
            {
                link_ptr* it = &bucket->next_;
                while(*it != n) it = &(*it)->next_;
                return it;
            }

            static inline void link_node(link_ptr n, link_ptr pos)
            {
                BOOST_ASSERT(false);
            }

            static inline void link_node_in_bucket(link_ptr n, bucket_ptr base)
            {
                n->next_ = base->next_;
                base->next_ = n;
            }

            static inline void link_group(link_ptr n, bucket_ptr base)
            {
                link_node_in_bucket(n, base);
            }

            static inline void unlink_node(link_ptr* pos)
            {
                *pos = (*pos)->next_;
            }

            static inline void split_group(link_ptr) {}
            static inline void split_group(link_ptr, link_ptr) {}
        };

        template <typename Alloc, typename EquivKeys>
        struct select_node_base
            : public boost::mpl::if_<
                    EquivKeys,
                    node_base_equivalent_keys<Alloc>,
                    node_base_unique_keys<Alloc> > {};

        //
        // Hash Table Data
        //
        // Responsible for managing the hash buckets.

        template <typename Alloc, typename EquivKeys>
        class hash_table_data
        {
        public:
            typedef bucket<Alloc> bucket;
            typedef BOOST_DEDUCED_TYPENAME bucket::bucket_allocator bucket_allocator;
            typedef BOOST_DEDUCED_TYPENAME bucket::bucket_ptr bucket_ptr;
            typedef BOOST_DEDUCED_TYPENAME bucket::link_ptr link_ptr;

            struct node;
            typedef std::size_t size_type;
            typedef std::ptrdiff_t difference_type;

            typedef Alloc value_allocator;

            typedef BOOST_DEDUCED_TYPENAME
                boost::unordered_detail::rebind_wrap<Alloc, node>::type
                node_allocator;

            typedef BOOST_DEDUCED_TYPENAME allocator_value_type<Alloc>::type value_type;
            typedef BOOST_DEDUCED_TYPENAME allocator_pointer<node_allocator>::type node_ptr;
            typedef BOOST_DEDUCED_TYPENAME allocator_reference<value_allocator>::type reference;

            typedef BOOST_DEDUCED_TYPENAME select_node_base<Alloc, EquivKeys>::type node_base;

            typedef BOOST_DEDUCED_TYPENAME
                boost::unordered_detail::rebind_wrap<Alloc, node_base>::type
                node_base_allocator;

            struct node : node_base
            {
            public:
                node(value_type const& v) : node_base(), value_(v) {}

                value_type value_;
            };

            // node_constructor
            //
            // Used to construct nodes in an exception safe manner.

            struct allocators
            {
                node_allocator node_alloc_;
                bucket_allocator bucket_alloc_;
                value_allocator value_alloc_;
                node_base_allocator node_base_alloc_;

                allocators(value_allocator const& a)
                    : node_alloc_(a), bucket_alloc_(a),
                    value_alloc_(a), node_base_alloc_(a)
                {}

                void destroy(link_ptr ptr)
                {
                    node_ptr n(node_alloc_.address(static_cast<node&>(*ptr)));
                    value_alloc_.destroy(value_alloc_.address(n->value_));
                    node_base_alloc_.destroy(node_base_alloc_.address(*n));
                    node_alloc_.deallocate(n, 1);
                }

                void swap(allocators& x)
                {
                    unordered_detail::hash_swap(node_alloc_, x.node_alloc_);
                    unordered_detail::hash_swap(bucket_alloc_, x.bucket_alloc_);
                    unordered_detail::hash_swap(value_alloc_, x.value_alloc_);
                    unordered_detail::hash_swap(node_base_alloc_, x.node_base_alloc_);
                }

                bool operator==(allocators const& x)
                {
                    return value_alloc_ == x.value_alloc_;
                }
            };

            class node_constructor
            {
                allocators& allocators_;

                node_ptr node_;
                bool value_constructed_;
                bool node_base_constructed_;

            public:

                node_constructor(allocators& a)
                    : allocators_(a),
                    node_(), value_constructed_(false), node_base_constructed_(false)
                {
                    BOOST_UNORDERED_MSVC_RESET_PTR(node_);
                }

                ~node_constructor()
                {
                    if (node_) {
                        if (value_constructed_)
                            allocators_.value_alloc_.destroy(
                                allocators_.value_alloc_.address(node_->value_));
                        if (node_base_constructed_)
                            allocators_.node_base_alloc_.destroy(
                                allocators_.node_base_alloc_.address(*node_));

                        allocators_.node_alloc_.deallocate(node_, 1);
                    }
                }

                template <typename V>
                void construct(V const& v)
                {
                    BOOST_ASSERT(!node_);
                    value_constructed_ = false;
                    node_base_constructed_ = false;

                    node_ = allocators_.node_alloc_.allocate(1);

                    allocators_.node_base_alloc_.construct(
                            allocators_.node_base_alloc_.address(*node_),
                            node_base());
                    node_base_constructed_ = true;

                    allocators_.value_alloc_.construct(
                            allocators_.value_alloc_.address(node_->value_), v);
                    value_constructed_ = true;
                }

                node_ptr get() const
                {
                    BOOST_ASSERT(node_);
                    return node_;
                }

                // no throw
                link_ptr release()
                {
                    node_ptr p = node_;
                    unordered_detail::reset(node_);
                    return link_ptr(allocators_.bucket_alloc_.address(*p));
                }

            private:
                node_constructor(node_constructor const&);
                node_constructor& operator=(node_constructor const&);
            };

            // Methods for navigating groups of elements with equal keys.

            static inline link_ptr& prev_in_group(link_ptr n) {
                return node_base::prev_in_group(n);
            }

            // pre: Must be pointing to the first node in a group.
            static inline link_ptr last_in_group(link_ptr n) {
                return node_base::last_in_group(n);
            }

            // pre: Must be pointing to the first node in a group.
            static inline link_ptr& next_group(link_ptr n) {
                return node_base::next_group(n);
            }

            // pre: Must be pointing to a node
            static inline reference get_value(link_ptr p) {
                BOOST_ASSERT(p);
                return static_cast<node&>(*p).value_;
            }

            class iterator_base
            {
            public:
                bucket_ptr bucket_;
                link_ptr node_;

                iterator_base()
                    : bucket_(), node_()
                {
                    BOOST_UNORDERED_MSVC_RESET_PTR(bucket_);
                    BOOST_UNORDERED_MSVC_RESET_PTR(node_);
                }

                explicit iterator_base(bucket_ptr b)
                    : bucket_(b), node_(b->next_) {}

                iterator_base(bucket_ptr b, link_ptr n)
                    : bucket_(b), node_(n) {}

                bool operator==(iterator_base const& x) const
                {
                    return node_ == x.node_;
                }

                bool operator!=(iterator_base const& x) const
                {
                    return node_ != x.node_;
                }

                reference operator*() const
                {
                    return get_value(node_);
                }

                void increment()
                {
                    BOOST_ASSERT(bucket_);
                    node_ = node_->next_;

                    while (!node_) {
                        ++bucket_;
                        node_ = bucket_->next_;
                    }
                }

                void increment_group()
                {
                    node_ = hash_table_data::next_group(node_);

                    while (!node_) {
                        ++bucket_;
                        node_ = bucket_->next_;
                    }
                }
            };

            // Member Variables

            allocators allocators_;
            bucket_ptr buckets_;
            size_type bucket_count_;
            bucket_ptr cached_begin_bucket_;
            size_type size_;

            // Constructors/Deconstructor

            hash_table_data(size_type n, value_allocator const& a)
              : allocators_(a),
                buckets_(), bucket_count_(next_prime(n)),
                cached_begin_bucket_(), size_(0)
            {
                // The array constructor will clean up in the event of an
                // exception.
                allocator_array_constructor<bucket_allocator>
                    constructor(allocators_.bucket_alloc_);

                // Creates an extra bucket to act as a sentinel.
                constructor.construct(bucket(), bucket_count_ + 1);

                cached_begin_bucket_ = constructor.get() + static_cast<difference_type>(bucket_count_);

                // Set up the sentinel.
                cached_begin_bucket_->next_ = link_ptr(cached_begin_bucket_);

                // Only release the buckets once everything is successfully
                // done.
                buckets_ = constructor.release();
            }

            hash_table_data(hash_table_data const& x, size_type n)
              : allocators_(x.allocators_),
                buckets_(), bucket_count_(next_prime(n)),
                cached_begin_bucket_(), size_(0)
            {
                // The array constructor will clean up in the event of an
                // exception.
                allocator_array_constructor<bucket_allocator>
                    constructor(allocators_.bucket_alloc_);

                // Creates an extra bucket to act as a sentinel.
                constructor.construct(bucket(), bucket_count_ + 1);

                cached_begin_bucket_ = constructor.get() + static_cast<difference_type>(bucket_count_);

                // Set up the sentinel
                cached_begin_bucket_->next_ = link_ptr(cached_begin_bucket_);

                // Only release the buckets once everything is successfully
                // done.
                buckets_ = constructor.release();
            }

            // no throw
            ~hash_table_data()
            {
                if(buckets_) {
                    bucket_ptr begin = cached_begin_bucket_;
                    bucket_ptr end = buckets_end();
                    while(begin != end) {
                        clear_bucket(begin);
                        ++begin;
                    }

                    // Destroy an extra bucket for the sentinels.
                    ++end;
                    for(begin = buckets_; begin != end; ++begin)
                        allocators_.bucket_alloc_.destroy(begin);

                    allocators_.bucket_alloc_.deallocate(buckets_, bucket_count_ + 1);
                }
            }

        private:

            hash_table_data(hash_table_data const&);
            hash_table_data& operator=(hash_table_data const&);

        public:

            // no throw
            void swap(hash_table_data& other)
            {
                std::swap(buckets_, other.buckets_);
                std::swap(bucket_count_, other.bucket_count_);
                std::swap(cached_begin_bucket_, other.cached_begin_bucket_);
                std::swap(size_, other.size_);
            }

            // Return the bucket index for a hashed value.
            //
            // no throw
            bucket_ptr bucket_from_hash(size_type hashed) const
            {
                return buckets_ + static_cast<difference_type>(hashed % bucket_count_);
            }

            // Begin & End
            //
            // no throw

            bucket_ptr buckets_end() const
            {
                return buckets_ + static_cast<difference_type>(bucket_count_);
            }

            iterator_base begin() const
            {
                return size_
                    ? iterator_base(cached_begin_bucket_)
                    : end();
            }

            iterator_base end() const
            {
                return iterator_base(buckets_end());
            }

            link_ptr begin(size_type n) const
            {
                return (buckets_ + static_cast<difference_type>(n))->next_;
            }

            link_ptr end(size_type) const
            {
                return unordered_detail::null_ptr<link_ptr>();
            }

            link_ptr begin(bucket_ptr b) const
            {
                return b->next_;
            }

            // Bucket Size

            // no throw
            static inline size_type node_count(link_ptr it)
            {
                size_type count = 0;
                while(BOOST_UNORDERED_BORLAND_BOOL(it)) {
                    ++count;
                    it = it->next_;
                }
                return count;
            }

            static inline size_type node_count(link_ptr it1, link_ptr it2)
            {
                size_type count = 0;
                while(it1 != it2) {
                    ++count;
                    it1 = it1->next_;
                }
                return count;
            }

            size_type bucket_size(size_type n) const
            {
                return node_count(begin(n));
            }

            static inline size_type group_count(link_ptr first_node)
            {
                return node_base::group_count(first_node);
            }

            // get_for_erase
            //
            // Find the pointer to a node, for use when erasing.
            //
            // no throw

            static inline link_ptr* get_for_erase(iterator_base r)
            {
                return node_base::get_for_erase(r.bucket_, r.node_);
            }

            // Link/Unlink/Move Node
            //
            // For adding nodes to buckets, removing them and moving them to a
            // new bucket.
            //
            // no throw

            void link_node(link_ptr n, link_ptr pos)
            {
                node_base::link_node(n, pos);
                ++size_;
            }

            void link_node_in_bucket(link_ptr n, bucket_ptr base)
            {
                node_base::link_node_in_bucket(n, base);
                ++size_;
                if(base < cached_begin_bucket_) cached_begin_bucket_ = base;
            }

            void link_group(link_ptr n, bucket_ptr base, size_type count)
            {
                node_base::link_group(n, base);
                size_ += count;
                if(base < cached_begin_bucket_) cached_begin_bucket_ = base;
            }

            void unlink_node(iterator_base it)
            {
                node_base::unlink_node(get_for_erase(it));
                --size_;
            }

            size_type unlink_group(link_ptr* pos)
            {
                size_type count = group_count(*pos);
                size_ -= count;
                *pos = next_group(*pos);
                return count;
            }

            void unlink_nodes(iterator_base n)
            {
                link_ptr* it = get_for_erase(n);
                split_group(*it);
                unordered_detail::reset(*it);
                size_ -= node_count(n.node_);
            }

            void unlink_nodes(iterator_base begin, iterator_base end)
            {
                BOOST_ASSERT(begin.bucket_ == end.bucket_);
                size_ -= node_count(begin.node_, end.node_);
                link_ptr* it = get_for_erase(begin);
                split_group(*it, end.node_);
                *it = end.node_;
            }

            void unlink_nodes(bucket_ptr base, iterator_base end)
            {
                BOOST_ASSERT(base == end.bucket_);

                split_group(end.node_);
            
                link_ptr ptr(base->next_);
                base->next_ = end.node_;

                size_ -= node_count(ptr, end.node_);
            }

            // Break a ciruclar list into two, with split as the beginning
            // of the second group (if split is at the beginning then don't
            // split).
            static inline void split_group(link_ptr split)
            {
                node_base::split_group(split);
            }

            static inline void split_group(link_ptr split1, link_ptr split2)
            {
                node_base::split_group(split1, split2);
            }

            // throws, strong exception-safety:
            link_ptr construct_node(value_type const& v)
            {
                node_constructor a(allocators_);
                a.construct(v);
                return a.release();
            }

            // Create Node
            //
            // Create a node and add it to the buckets in the given position.
            //
            // strong exception safety.

            iterator_base create_node(value_type const& v, bucket_ptr base)
            {
                // throws, strong exception-safety:
                link_ptr n = construct_node(v);

                // Rest is no throw
                link_node_in_bucket(n, base);
                return iterator_base(base, n);
            }

            iterator_base create_node(value_type const& v, iterator_base position)
            {
                BOOST_ASSERT(EquivKeys::value);

                // throws, strong exception-safety:
                link_ptr n = construct_node(v);

                // Rest is no throw
                link_node(n, position.node_);
                return iterator_base(position.bucket_, n);
            }

            iterator_base create_node(value_type const& v,
                    bucket_ptr base, link_ptr position)
            {
                BOOST_ASSERT(EquivKeys::value);

                // throws, strong exception-safety:
                link_ptr n = construct_node(v);

                // Rest is no throw
                if(BOOST_UNORDERED_BORLAND_BOOL(position))
                    link_node(n, position);
                else
                    link_node_in_bucket(n, base);

                return iterator_base(base, n);
            }

            void copy_group(link_ptr it, bucket_ptr dst)
            {
                link_ptr end = next_group(it);
                iterator_base pos = create_node(get_value(it), dst);
                for(it = it->next_; it != end; it = it->next_)
                    create_node(get_value(it), pos);
            }

            // Delete Node
            //
            // Remove a node, or a range of nodes, from a bucket, and destroy
            // them.
            //
            // no throw

            void delete_to_bucket_end(link_ptr begin)
            {
                while(begin) {
                    link_ptr node = begin;
                    begin = begin->next_;
                    allocators_.destroy(node);
                }
            }

            void delete_nodes(link_ptr begin, link_ptr end)
            {
                while(begin != end) {
                    link_ptr node = begin;
                    begin = begin->next_;
                    allocators_.destroy(node);
                }
            }

            void delete_group(link_ptr first_node)
            {
                delete_nodes(first_node, next_group(first_node));
            }

            // Clear
            //
            // Remove all the nodes.
            //
            // no throw

            void clear_bucket(bucket_ptr b)
            {
                link_ptr first_node = b->next_;
                unordered_detail::reset(b->next_);
                delete_to_bucket_end(first_node);
            }

            void clear()
            {
                bucket_ptr begin = cached_begin_bucket_;
                bucket_ptr end = buckets_end();

                size_ = 0;
                cached_begin_bucket_ = end;

                while(begin != end) {
                    clear_bucket(begin);
                    ++begin;
                }
            }

            // Erase
            //
            // no throw

            iterator_base erase(iterator_base r)
            {
                BOOST_ASSERT(r != end());
                iterator_base next = r;
                next.increment();
                unlink_node(r);
                allocators_.destroy(r.node_);
                // r has been invalidated but its bucket is still valid
                recompute_begin_bucket(r.bucket_, next.bucket_);
                return next;
            }

            iterator_base erase(iterator_base r1, iterator_base r2)
            {
                if(r1 != r2)
                {
                    BOOST_ASSERT(r1 != end());

                    if (r1.bucket_ == r2.bucket_) {
                        unlink_nodes(r1, r2);
                        delete_nodes(r1.node_, r2.node_);

                        // No need to call recompute_begin_bucket because
                        // the nodes are only deleted from one bucket, which
                        // still contains r2 after the erase.
                        BOOST_ASSERT(!r1.bucket_->empty());
                    }
                    else {
                        BOOST_ASSERT(r1.bucket_ < r2.bucket_);

                        unlink_nodes(r1);
                        delete_to_bucket_end(r1.node_);

                        bucket_ptr i = r1.bucket_;
                        for(++i; i != r2.bucket_; ++i) {
                            size_ -= node_count(i->next_);
                            clear_bucket(i);
                        }

                        if(r2 != end()) {
                            link_ptr first = r2.bucket_->next_;
                            unlink_nodes(r2.bucket_, r2);
                            delete_nodes(first, r2.node_);
                        }

                        // r1 has been invalidated but its bucket is still
                        // valid.
                        recompute_begin_bucket(r1.bucket_, r2.bucket_);
                    }
                }

                return r2;
            }

            // recompute_begin_bucket
            //
            // After an erase cached_begin_bucket_ might be left pointing to
            // an empty bucket, so this is called to update it
            //
            // no throw

            void recompute_begin_bucket(bucket_ptr b)
            {
                BOOST_ASSERT(!(b < cached_begin_bucket_));

                if(b == cached_begin_bucket_)
                {
                    if (size_ != 0) {
                        while (cached_begin_bucket_->empty())
                            ++cached_begin_bucket_;
                    } else {
                        cached_begin_bucket_ = buckets_end();
                    }
                }
            }

            // This is called when a range has been erased
            //
            // no throw

            void recompute_begin_bucket(bucket_ptr b1, bucket_ptr b2)
            {
                BOOST_ASSERT(!(b1 < cached_begin_bucket_) && !(b2 < b1));
                BOOST_ASSERT(b2 == buckets_end() || !b2->empty());

                if(b1 == cached_begin_bucket_ && b1->empty())
                    cached_begin_bucket_ = b2;
            }

            size_type erase_group(link_ptr* it, bucket_ptr bucket)
            {
                link_ptr pos = *it;
                size_type count = unlink_group(it);
                delete_group(pos);

                this->recompute_begin_bucket(bucket);

                return count;
            }
        };

#if defined(BOOST_MPL_CFG_MSVC_ETI_BUG)
        template <>
        class hash_table_data<int, int>
        {
        public:
            typedef int size_type;
            typedef int iterator_base;
        };
#endif

        //
        // Hash Table
        //

        template <typename ValueType, typename KeyType,
            typename Hash, typename Pred,
            typename Alloc, typename EquivKeys>
        class hash_table
            : public hash_table_data<Alloc, EquivKeys>
        {
            typedef hash_table_data<Alloc, EquivKeys> data;
            typedef BOOST_DEDUCED_TYPENAME data::node_constructor node_constructor;
            typedef BOOST_DEDUCED_TYPENAME data::bucket_ptr bucket_ptr;
            typedef BOOST_DEDUCED_TYPENAME data::link_ptr link_ptr;

        public:

            typedef BOOST_DEDUCED_TYPENAME data::value_allocator value_allocator;
            typedef BOOST_DEDUCED_TYPENAME data::node_allocator node_allocator;

            // Type definitions

            typedef KeyType key_type;
            typedef Hash hasher;
            typedef Pred key_equal;
            typedef ValueType value_type;
            typedef std::size_t size_type;
            typedef std::ptrdiff_t difference_type;

            // iterators

            typedef BOOST_DEDUCED_TYPENAME data::iterator_base iterator_base;

        private:

            class functions
            {
                std::pair<hasher, key_equal> functions_;

            public:

                functions(hasher const& h, key_equal const& k)
                    : functions_(h, k) {}

                hasher const& hash_function() const
                {
                    return functions_.first;
                }

                key_equal const& key_eq() const
                {
                    return functions_.second;
                }
            };

            // Both hasher and key_equal's copy/assign can throw so double
            // buffering is used to copy them. func_ points to the currently
            // active function objects.

            typedef functions hash_table::*functions_ptr;

            functions func1_;
            functions func2_;
            functions_ptr func_;

            float mlf_;
            size_type max_load_;

        public:

            // Constructors
            //
            // In the constructors, if anything throws an exception,
            // data's destructor will clean up.

            hash_table(size_type n,
                    hasher const& hf, key_equal const& eq,
                    value_allocator const& a)
                : data(n, a),         // throws, cleans itself up
                func1_(hf, eq),       // throws      "     "
                func2_(hf, eq),       // throws      "     "
                func_(&hash_table::func1_), // no throw
                mlf_(1.0f)            // no throw
            {
                calculate_max_load(); // no throw
            }

            // Construct from iterators

            // initial_size
            //
            // A helper function for the copy constructor to calculate how many
            // nodes will be created if the iterator's support it. Might get it
            // totally wrong for containers with unique keys.
            //
            // no throw

            template <typename I>
            size_type initial_size(I i, I j, size_type n,
                    boost::forward_traversal_tag)
            {
                // max load factor isn't set yet, but when it is, it'll be 1.0.
                return (std::max)(static_cast<size_type>(std::distance(i, j)) + 1, n);
            }

            template <typename I>
            size_type initial_size(I, I, size_type n,
                    boost::incrementable_traversal_tag)
            {
                return n;
            }

            template <typename I>
            size_type initial_size(I i, I j, size_type n)
            {
                BOOST_DEDUCED_TYPENAME boost::iterator_traversal<I>::type
                    iterator_traversal_tag;
                return initial_size(i, j, n, iterator_traversal_tag);
            }

            template <typename I>
            hash_table(I i, I j, size_type n,
                    hasher const& hf, key_equal const& eq,
                    value_allocator const& a)
                : data(initial_size(i, j, n), a),  // throws, cleans itself up
                    func1_(hf, eq),                // throws    "      "
                    func2_(hf, eq),                // throws    "      "
                    func_(&hash_table::func1_),    // no throw
                    mlf_(1.0f)                     // no throw
            {
                calculate_max_load(); // no throw
                // Inserting elements is left to the containing class.
                // This is a little odd, but hopefully will be cleaned up with
                // future development.
            }
            // Copy Construct

            hash_table(hash_table const& x)
                : data(x, x.min_buckets_for_size(x.size())), // throws
                func1_(x.current_functions()), // throws
                func2_(x.current_functions()), // throws
                func_(&hash_table::func1_), // no throw
                mlf_(x.mlf_) // no throw
            {
                calculate_max_load(); // no throw

                // This can throw, but data's destructor will clean
                // up.
                copy_buckets(x, *this, current_functions());
            }

            // Assign
            //
            // basic exception safety, if copy_functions of reserver throws
            // the container is left in a sane, empty state. If copy_buckets
            // throws the container is left with whatever was successfully
            // copied.

            hash_table& operator=(hash_table const& x)
            {
                if(this != &x)
                {
                    this->clear();                        // no throw
                    func_ = copy_functions(x);            // throws, strong
                    mlf_ = x.mlf_;                        // no throw
                    calculate_max_load();                 // no throw
                    reserve(x.size());                    // throws
                    copy_buckets(x, *this, current_functions()); // throws
                }

                return *this;
            }

            // Swap
            //
            // Swap's behaviour when allocators aren't equal is in dispute, for
            // details see:
            // 
            // http://unordered.nfshost.com/doc/html/unordered/rationale.html#swapping_containers_with_unequal_allocators
            //
            // ----------------------------------------------------------------
            //
            // Strong exception safety (might change unused function objects)
            //
            // Can throw if hash or predicate object's copy constructor throws
            // or if allocators are unequal

            void swap(hash_table& x)
            {
                // This only effects the function objects that aren't in use
                // so it is strongly exception safe, via. double buffering.
                functions_ptr new_func_this = copy_functions(x);       // throws
                functions_ptr new_func_that = x.copy_functions(*this); // throws

                if(this->allocators_ == x.allocators_) {
                    this->data::swap(x); // no throw
                }
                else {
                    // Create new buckets in separate HASH_TABLE_DATA objects
                    // which will clean up if anything throws an exception.
                    // (all can throw, but with no effect as these are new objects).
                    data new_this(*this, x.min_buckets_for_size(x.size_));
                    copy_buckets(x, new_this, this->*new_func_this);

                    data new_that(x, min_buckets_for_size(this->size_));
                    x.copy_buckets(*this, new_that, x.*new_func_that);

                    // Start updating the data here, no throw from now on.
                    this->data::swap(new_this);
                    x.data::swap(new_that);
                }

                // We've made it, the rest is no throw.
                std::swap(mlf_, x.mlf_);

                func_ = new_func_this;
                x.func_ = new_func_that;

                calculate_max_load();
                x.calculate_max_load();
            }

        private:

            functions const& current_functions() const
            {
                return this->*func_;
            }

            // This copies the given function objects into the currently unused
            // function objects and returns a pointer, that func_ can later be
            // set to, to commit the change.
            //
            // Strong exception safety (since only usued function objects are
            // changed).
            functions_ptr copy_functions(hash_table const& x)
            {
                // no throw:
                functions_ptr ptr = func_ == &hash_table::func1_
                    ? &hash_table::func2_ : &hash_table::func1_;
                // throws, functions not in use, so strong
                this->*ptr = x.current_functions();
                return ptr;
            }

        public:

            // accessors

            // no throw
            value_allocator get_allocator() const
            {
                return this->allocators_.value_alloc_;
            }

            // no throw
            hasher const& hash_function() const
            {
                return current_functions().hash_function();
            }

            // no throw
            key_equal const& key_eq() const
            {
                return current_functions().key_eq();
            }

            // no throw
            size_type size() const
            {
                return this->size_;
            }

            // no throw
            bool empty() const
            {
                return this->size_ == 0;
            }

            // no throw
            size_type max_size() const
            {
                // size < mlf_ * count
                return float_to_size_t(ceil(
                        max_bucket_count() * mlf_)) - 1;
            }

            // strong safety
            size_type bucket(key_type const& k) const
            {
                // hash_function can throw:
                return hash_function()(k) % this->bucket_count_;
            }

            // strong safety
            bucket_ptr get_bucket(key_type const& k) const
            {
                return this->buckets_ + static_cast<difference_type>(bucket(k));
            }

            // no throw
            size_type bucket_count() const
            {
                return this->bucket_count_;
            }

            // no throw
            size_type max_bucket_count() const
            {
                // -1 to account for the end marker.
                return prev_prime(this->allocators_.bucket_alloc_.max_size() - 1);
            }

        private:

            // no throw
            size_type min_buckets_for_size(size_type n) const
            {
                BOOST_ASSERT(mlf_ != 0);

                using namespace std;

                // From 6.3.1/13:
                // size < mlf_ * count
                // => count > size / mlf_
                //
                // Or from rehash post-condition:
                // count > size / mlf_
                return static_cast<size_type>(floor(n / mlf_)) + 1;
            }

            // no throw
            void calculate_max_load()
            {
                using namespace std;

                // From 6.3.1/13:
                // Only resize when size >= mlf_ * count
                max_load_ = float_to_size_t(ceil(mlf_ * this->bucket_count_));
            }

            // basic exception safety
            bool reserve(size_type n)
            {
                bool need_to_reserve = n >= max_load_;
                // throws - basic:
                if (need_to_reserve) rehash_impl(min_buckets_for_size(n));
                BOOST_ASSERT(n < max_load_ || n > max_size());
                return need_to_reserve;
            }

            // basic exception safety
            //
            // This version of reserve is called when inserting a range
            // into a container with equivalent keys, it creates more buckets
            // if the resulting load factor would be over 80% of the load
            // factor. This is to try to avoid excessive rehashes.
            bool reserve_extra(size_type n)
            {
                bool need_to_reserve = n >= max_load_;
                // throws - basic:
                if (need_to_reserve) {
                    rehash_impl(static_cast<size_type>(floor(n / mlf_ * 1.25)) + 1);
                }
                BOOST_ASSERT(n < max_load_ || n > max_size());
                return need_to_reserve;
            }

        public:

            // no throw
            float max_load_factor() const
            {
                return mlf_;
            }

            // no throw
            void max_load_factor(float z)
            {
                BOOST_ASSERT(z > 0);
                mlf_ = (std::max)(z, minimum_max_load_factor);
                calculate_max_load();
            }

            // no throw
            float load_factor() const
            {
                BOOST_ASSERT(this->bucket_count_ != 0);
                return static_cast<float>(this->size_)
                    / static_cast<float>(this->bucket_count_);
            }

        private:

            // key extractors

            // no throw
            static key_type const& extract_key(value_type const& v)
            {
                return extract(v, (type_wrapper<value_type>*)0);
            }

            static key_type const& extract(value_type const& v,
                    type_wrapper<key_type>*)
            {
                return v;
            }

            static key_type const& extract(value_type const& v,
                    void*)
            {
                return v.first;
            }

        public:

            // if hash function throws, basic exception safety
            // strong otherwise.
            void rehash(size_type n)
            {
                using namespace std;

                // no throw:
                size_type min_size = min_buckets_for_size(size());
                // basic/strong:
                rehash_impl(min_size > n ? min_size : n);

                BOOST_ASSERT((float) bucket_count() > (float) size() / max_load_factor()
                        && bucket_count() >= n);
            }

        private:

            // if hash function throws, basic exception safety
            // strong otherwise
            void rehash_impl(size_type n)
            {
                n = next_prime(n); // no throw

                if (n == bucket_count())  // no throw
                    return;

                data new_buckets(*this, n); // throws, seperate
                move_buckets(*this, new_buckets, hash_function());
                                                        // basic/no throw
                new_buckets.swap(*this);                // no throw
                calculate_max_load();                   // no throw
            }

            // move_buckets & copy_buckets
            //
            // if the hash function throws, basic excpetion safety
            // no throw otherwise

            static void move_buckets(data& src, data& dst, hasher const& hf)
            {
                BOOST_ASSERT(dst.size_ == 0);
                //BOOST_ASSERT(src.allocators_.node_alloc_ == dst.allocators_.node_alloc_);

                bucket_ptr end = src.buckets_end();

                for(; src.cached_begin_bucket_ != end;
                        ++src.cached_begin_bucket_) {
                    bucket_ptr src_bucket = src.cached_begin_bucket_;
                    while(src_bucket->next_) {
                        // Move the first group of equivalent nodes in
                        // src_bucket to dst.

                        // This next line throws iff the hash function throws.
                        bucket_ptr dst_bucket = dst.bucket_from_hash(
                                hf(extract_key(data::get_value(src_bucket->next_))));

                        link_ptr n = src_bucket->next_;
                        size_type count = src.unlink_group(&src_bucket->next_);
                        dst.link_group(n, dst_bucket, count);
                    }
                }
            }

            // basic excpetion safety. If an exception is thrown this will
            // leave dst partially filled.

            static void copy_buckets(data const& src, data& dst, functions const& f)
            {
                BOOST_ASSERT(dst.size_ == 0);
                // no throw:
                bucket_ptr end = src.buckets_end();
                hasher const& hf = f.hash_function();

                // no throw:
                for(bucket_ptr i = src.cached_begin_bucket_; i != end; ++i) {
                    // no throw:
                    for(link_ptr it = src.begin(i);
                            BOOST_UNORDERED_BORLAND_BOOL(it); it = data::next_group(it)) {
                        // hash function can throw.
                        bucket_ptr dst_bucket = dst.bucket_from_hash(
                               hf(extract_key(data::get_value(it))));
                        // throws, strong
                        dst.copy_group(it, dst_bucket);
                    }
                }
            }

        public:

            // Insert functions
            //
            // basic exception safety, if hash function throws
            // strong otherwise.

            // Insert (equivalent key containers)

            // if hash function throws, basic exception safety
            // strong otherwise
            iterator_base insert_equiv(value_type const& v)
            {
                key_type const& k = extract_key(v);
                size_type hash_value = hash_function()(k);
                bucket_ptr bucket = this->bucket_from_hash(hash_value);
                link_ptr position = find_iterator(bucket, k);

                // Create the node before rehashing in case it throws an
                // exception (need strong safety in such a case).
                node_constructor a(this->allocators_);
                a.construct(v);

                // reserve has basic exception safety if the hash function
                // throws, strong otherwise.
                if(reserve(size() + 1))
                    bucket = this->bucket_from_hash(hash_value);

                // Nothing after the point can throw.

                link_ptr n = a.release();

                // I'm relying on link_ptr not being invalidated by
                // the rehash here.
                if(BOOST_UNORDERED_BORLAND_BOOL(position))
                    this->link_node(n, position);
                else
                    this->link_node_in_bucket(n, bucket);

                return iterator_base(bucket, n);
            }

            // Insert (equivalent key containers)

            // if hash function throws, basic exception safety
            // strong otherwise
            iterator_base insert_equiv(iterator_base const& it, value_type const& v)
            {
                // equal can throw, but with no effects
                if (it == this->end() || !equal(extract_key(v), *it)) {
                    // Use the standard insert if the iterator doesn't point
                    // to a matching key.
                    return insert_equiv(v);
                }
                else {
                    // Find the first node in the group - so that the node
                    // will be inserted at the end of the group.

                    link_ptr start(it.node_);
                    while(this->prev_in_group(start)->next_ == start)
                        start = this->prev_in_group(start);

                    // Create the node before rehashing in case it throws an
                    // exception (need strong safety in such a case).
                    node_constructor a(this->allocators_);
                    a.construct(v);

                    // reserve has basic exception safety if the hash function
                    // throws, strong otherwise.
                    bucket_ptr base = reserve(size() + 1) ?
                        get_bucket(extract_key(v)) : it.bucket_;

                    // Nothing after this point can throw

                    link_ptr n = a.release();
                    this->link_node(n, start);

                    return iterator_base(base, n);
                }
            }

            // Insert from iterator range (equivalent key containers)

        private:

            // if hash function throws, or inserting > 1 element, basic exception safety
            // strong otherwise
            template <typename I>
            void insert_for_range_equiv(I i, I j, forward_traversal_tag)
            {
                size_type distance = std::distance(i, j);
                if(distance == 1) {
                    insert_equiv(*i);
                }
                else {
                    // Only require basic exception safety here
                    reserve_extra(size() + distance);
                    node_constructor a(this->allocators_);

                    for (; i != j; ++i) {
                        a.construct(*i);

                        key_type const& k = extract_key(a.get()->value_);
                        bucket_ptr bucket = get_bucket(k);
                        link_ptr position = find_iterator(bucket, k);

                        if(BOOST_UNORDERED_BORLAND_BOOL(position))
                            this->link_node(a.release(), position);
                        else
                            this->link_node_in_bucket(a.release(), bucket);
                    }
                }
            }

            // if hash function throws, or inserting > 1 element, basic exception safety
            // strong otherwise
            template <typename I>
            void insert_for_range_equiv(I i, I j,
                    boost::incrementable_traversal_tag)
            {
                // If only inserting 1 element, get the required
                // safety since insert is only called once.
                for (; i != j; ++i) insert_equiv(*i);
            }

        public:

            // if hash function throws, or inserting > 1 element, basic exception safety
            // strong otherwise
            template <typename I>
            void insert_equiv(I i, I j)
            {
                BOOST_DEDUCED_TYPENAME boost::iterator_traversal<I>::type
                    iterator_traversal_tag;
                insert_for_range_equiv(i, j, iterator_traversal_tag);
            }

            // if hash function throws, basic exception safety
            // strong otherwise
            value_type& operator[](key_type const& k)
            {
                BOOST_STATIC_ASSERT((
                            !boost::is_same<value_type, key_type>::value));
                typedef BOOST_DEDUCED_TYPENAME value_type::second_type mapped_type;

                size_type hash_value = hash_function()(k);
                bucket_ptr bucket = this->bucket_from_hash(hash_value);
                link_ptr pos = find_iterator(bucket, k);

                if (BOOST_UNORDERED_BORLAND_BOOL(pos))
                    return data::get_value(pos);
                else
                {
                    // Side effects only in this block.

                    // Create the node before rehashing in case it throws an
                    // exception (need strong safety in such a case).
                    node_constructor a(this->allocators_);
                    a.construct(value_type(k, mapped_type()));

                    // reserve has basic exception safety if the hash function
                    // throws, strong otherwise.
                    if(reserve(size() + 1))
                        bucket = this->bucket_from_hash(hash_value);

                    // Nothing after this point can throw.

                    link_ptr n = a.release();
                    this->link_node_in_bucket(n, bucket);

                    return data::get_value(n);
                }
            }

            // Insert (unique keys)

            // if hash function throws, basic exception safety
            // strong otherwise
            std::pair<iterator_base, bool> insert_unique(value_type const& v)
            {
                // No side effects in this initial code
                key_type const& k = extract_key(v);
                size_type hash_value = hash_function()(k);
                bucket_ptr bucket = this->bucket_from_hash(hash_value);
                link_ptr pos = find_iterator(bucket, k);
                
                if (BOOST_UNORDERED_BORLAND_BOOL(pos)) {
                    // Found an existing key, return it (no throw).
                    return std::pair<iterator_base, bool>(
                        iterator_base(bucket, pos), false);

                } else {
                    // Doesn't already exist, add to bucket.
                    // Side effects only in this block.

                    // Create the node before rehashing in case it throws an
                    // exception (need strong safety in such a case).
                    node_constructor a(this->allocators_);
                    a.construct(v);

                    // reserve has basic exception safety if the hash function
                    // throws, strong otherwise.
                    if(reserve(size() + 1))
                        bucket = this->bucket_from_hash(hash_value);

                    // Nothing after this point can throw.

                    link_ptr n = a.release();
                    this->link_node_in_bucket(n, bucket);

                    return std::pair<iterator_base, bool>(
                        iterator_base(bucket, n), true);
                }
            }

            // Insert (unique keys)

            // if hash function throws, basic exception safety
            // strong otherwise
            iterator_base insert_unique(iterator_base const& it, value_type const& v)
            {
                if(it != this->end() && equal(extract_key(v), *it))
                    return it;
                else
                    return insert_unique(v).first;
            }

            // Insert from iterators (unique keys)

            template <typename I>
            size_type insert_size_unique(I i, I j, boost::forward_traversal_tag)
            {
                return std::distance(i, j);
            }

            template <typename I>
            size_type insert_size_unique(I, I, boost::incrementable_traversal_tag)
            {
                return 1;
            }

            template <typename I>
            size_type insert_size_unique(I i, I j)
            {
                BOOST_DEDUCED_TYPENAME boost::iterator_traversal<I>::type
                    iterator_traversal_tag;
                return insert_size_unique(i, j, iterator_traversal_tag);
            }

            // if hash function throws, or inserting > 1 element, basic exception safety
            // strong otherwise
            template <typename InputIterator>
            void insert_unique(InputIterator i, InputIterator j)
            {
                node_constructor a(this->allocators_);

                for (; i != j; ++i) {
                    // No side effects in this initial code
                    size_type hash_value = hash_function()(extract_key(*i));
                    bucket_ptr bucket = this->bucket_from_hash(hash_value);
                    link_ptr pos = find_iterator(bucket, extract_key(*i));
                    
                    if (!BOOST_UNORDERED_BORLAND_BOOL(pos)) {
                        // Doesn't already exist, add to bucket.
                        // Side effects only in this block.

                        // Create the node before rehashing in case it throws an
                        // exception (need strong safety in such a case).
                        value_type const& v = *i;
                        a.construct(v);

                        // reserve has basic exception safety if the hash function
                        // throws, strong otherwise.
                        if(size() + 1 >= max_load_) {
                            reserve(size() + insert_size_unique(i, j));
                            bucket = this->bucket_from_hash(hash_value);
                        }

                        // Nothing after this point can throw.
                        this->link_node_in_bucket(a.release(), bucket);
                    }
                }
            }

        public:

            // erase

            // no throw
            iterator_base erase(iterator_base const& r)
            {
                return this->data::erase(r);
            }

            // strong exception safety
            size_type erase(key_type const& k)
            {
                // No side effects in initial section
                bucket_ptr bucket = get_bucket(k);
                link_ptr* it = find_for_erase(bucket, k);

                // No throw.
                return *it ? this->erase_group(it, bucket) : 0;
            }

            // no throw
            iterator_base erase(iterator_base const& r1, iterator_base const& r2)
            {
                return this->data::erase(r1, r2);
            }

            // count
            //
            // strong exception safety, no side effects
            size_type count(key_type const& k) const
            {
                link_ptr it = find_iterator(k); // throws, strong
                return BOOST_UNORDERED_BORLAND_BOOL(it) ? data::group_count(it) : 0;
            }

            // find
            //
            // strong exception safety, no side effects
            iterator_base find(key_type const& k) const
            {
                bucket_ptr bucket = get_bucket(k);
                link_ptr it = find_iterator(bucket, k);

                if (BOOST_UNORDERED_BORLAND_BOOL(it))
                    return iterator_base(bucket, it);
                else
                    return this->end();
            }

            value_type& at(key_type const& k) const
            {
                bucket_ptr bucket = get_bucket(k);
                link_ptr it = find_iterator(bucket, k);

                if (BOOST_UNORDERED_BORLAND_BOOL(it))
                    return data::get_value(it);
                else
                    throw std::out_of_range("Unable to find key in unordered_map.");
            }

            // equal_range
            //
            // strong exception safety, no side effects
            std::pair<iterator_base, iterator_base> equal_range(key_type const& k) const
            {
                bucket_ptr bucket = get_bucket(k);
                link_ptr it = find_iterator(bucket, k);
                if (BOOST_UNORDERED_BORLAND_BOOL(it)) {
                    iterator_base first(iterator_base(bucket, it));
                    iterator_base second(first);
                    second.increment_group();
                    return std::pair<iterator_base, iterator_base>(first, second);
                }
                else {
                    return std::pair<iterator_base, iterator_base>(
                            this->end(), this->end());
                }
            }

            //
            // equals
            //

private:
            static inline bool group_equals(link_ptr it1, link_ptr it2,
                    type_wrapper<key_type>*)
            {
                return data::group_count(it1) == data::group_count(it2);
            }

            static inline bool group_equals(link_ptr it1, link_ptr it2, void*)
            {
                if(!BOOST_UNORDERED_BORLAND_BOOL(it2)) return false;
                link_ptr end1 = data::next_group(it1);
                link_ptr end2 = data::next_group(it2);
                do {
                    if(data::get_value(it1).second != data::get_value(it2).second) return false;
                    it1 = it1->next_;
                    it2 = it2->next_;
                } while(it1 != end1 && it2 != end2);
                return it1 == end1 && it2 == end2;
            }
public:
            bool equals(hash_table const& other) const
            {
                if(this->size() != other.size()) return false;

                for(bucket_ptr i = this->cached_begin_bucket_,
                        j = this->buckets_ + this->bucket_count_; i != j; ++i)
                {
                    for(link_ptr it(i->next_); BOOST_UNORDERED_BORLAND_BOOL(it); it = data::next_group(it))
                    {
                        link_ptr other_pos = other.find_iterator(other.extract_key(data::get_value(it)));
                        if(!BOOST_UNORDERED_BORLAND_BOOL(other_pos) ||
                            !group_equals(it, other_pos, (type_wrapper<value_type>*)0))
                            return false;
                    }
                }

                return true;
            }

            inline bool group_hash(link_ptr it, type_wrapper<key_type>*) const
            {
                std::size_t seed = data::group_count(it);
                boost::hash_combine(seed, hash_function()(data::get_value(it)));
                return seed;
            }

            inline bool group_hash(link_ptr it, void*) const
            {
                std::size_t seed = hash_function()(data::get_value(it).first);

                link_ptr end = data::next_group(it);

                do {
                    boost::hash_combine(seed, data::get_value(it).second);
                    it = it->next_;
                } while(it != end);

                return seed;
            }

            std::size_t hash_value() const
            {
                std::size_t seed = 0;

                for(bucket_ptr i = this->cached_begin_bucket_,
                        j = this->buckets_ + this->bucket_count_; i != j; ++i)
                {
                    for(link_ptr it(i->next_); BOOST_UNORDERED_BORLAND_BOOL(it); it = data::next_group(it))
                        seed ^= group_hash(it, (type_wrapper<value_type>*)0);
                }

                return seed;
            }

        private:

            // strong exception safety, no side effects
            bool equal(key_type const& k, value_type const& v) const
            {
                return key_eq()(k, extract_key(v));
            }

            // strong exception safety, no side effects
            link_ptr find_iterator(key_type const& k) const
            {
                return find_iterator(get_bucket(k), k);
            }

            // strong exception safety, no side effects
            link_ptr find_iterator(bucket_ptr bucket,
                    key_type const& k) const
            {
                link_ptr it = this->begin(bucket);
                while (BOOST_UNORDERED_BORLAND_BOOL(it) && !equal(k, data::get_value(it)))
                    it = data::next_group(it);

                return it;
            }

            // strong exception safety, no side effects
            link_ptr* find_for_erase(bucket_ptr bucket, key_type const& k) const
            {
                link_ptr* it = &bucket->next_;
                while(BOOST_UNORDERED_BORLAND_BOOL(*it) && !equal(k, data::get_value(*it)))
                    it = &data::next_group(*it);

                return it;
            }
        };

        // Iterators
        
        template <typename Alloc, typename EquivKeys> class hash_iterator;
        template <typename Alloc, typename EquivKeys> class hash_const_iterator;
        template <typename Alloc, typename EquivKeys> class hash_local_iterator;
        template <typename Alloc, typename EquivKeys> class hash_const_local_iterator;
        class iterator_access;

        // Local Iterators
        //
        // all no throw

        template <typename Alloc, typename EquivKeys>
        class hash_local_iterator
            : public boost::iterator <
                std::forward_iterator_tag,
                BOOST_DEDUCED_TYPENAME allocator_value_type<Alloc>::type,
                std::ptrdiff_t,
                BOOST_DEDUCED_TYPENAME allocator_pointer<Alloc>::type,
                BOOST_DEDUCED_TYPENAME allocator_reference<Alloc>::type >
        {
        public:
            typedef BOOST_DEDUCED_TYPENAME allocator_value_type<Alloc>::type value_type;

        private:
            typedef hash_table_data<Alloc, EquivKeys> data;
            typedef BOOST_DEDUCED_TYPENAME data::link_ptr ptr;
            typedef hash_const_local_iterator<Alloc, EquivKeys> const_local_iterator;

            friend class hash_const_local_iterator<Alloc, EquivKeys>;
            ptr ptr_;

        public:
            hash_local_iterator() : ptr_() {
                BOOST_UNORDERED_MSVC_RESET_PTR(ptr_);
            }
            explicit hash_local_iterator(ptr x) : ptr_(x) {}
            BOOST_DEDUCED_TYPENAME allocator_reference<Alloc>::type operator*() const
                { return data::get_value(ptr_); }
            value_type* operator->() const { return &data::get_value(ptr_); }
            hash_local_iterator& operator++() { ptr_ = ptr_->next_; return *this; }
            hash_local_iterator operator++(int) { hash_local_iterator tmp(ptr_); ptr_ = ptr_->next_; return tmp; }
            bool operator==(hash_local_iterator x) const { return ptr_ == x.ptr_; }
            bool operator==(const_local_iterator x) const { return ptr_ == x.ptr_; }
            bool operator!=(hash_local_iterator x) const { return ptr_ != x.ptr_; }
            bool operator!=(const_local_iterator x) const { return ptr_ != x.ptr_; }
        };

        template <typename Alloc, typename EquivKeys>
        class hash_const_local_iterator
            : public boost::iterator <
                std::forward_iterator_tag,
                BOOST_DEDUCED_TYPENAME allocator_value_type<Alloc>::type,
                std::ptrdiff_t,
                BOOST_DEDUCED_TYPENAME allocator_const_pointer<Alloc>::type,
                BOOST_DEDUCED_TYPENAME allocator_const_reference<Alloc>::type >
        {
        public:
            typedef BOOST_DEDUCED_TYPENAME allocator_value_type<Alloc>::type value_type;

        private:
            typedef hash_table_data<Alloc, EquivKeys> data;
            typedef BOOST_DEDUCED_TYPENAME data::link_ptr ptr;
            typedef hash_local_iterator<Alloc, EquivKeys> local_iterator;
            friend class hash_local_iterator<Alloc, EquivKeys>;
            ptr ptr_;

        public:
            hash_const_local_iterator() : ptr_() {
                BOOST_UNORDERED_MSVC_RESET_PTR(ptr_);
            }
            explicit hash_const_local_iterator(ptr x) : ptr_(x) {}
            hash_const_local_iterator(local_iterator x) : ptr_(x.ptr_) {}
            BOOST_DEDUCED_TYPENAME allocator_const_reference<Alloc>::type
                operator*() const { return data::get_value(ptr_); }
            value_type const* operator->() const { return &data::get_value(ptr_); }
            hash_const_local_iterator& operator++() { ptr_ = ptr_->next_; return *this; }
            hash_const_local_iterator operator++(int) { hash_const_local_iterator tmp(ptr_); ptr_ = ptr_->next_; return tmp; }
            bool operator==(local_iterator x) const { return ptr_ == x.ptr_; }
            bool operator==(hash_const_local_iterator x) const { return ptr_ == x.ptr_; }
            bool operator!=(local_iterator x) const { return ptr_ != x.ptr_; }
            bool operator!=(hash_const_local_iterator x) const { return ptr_ != x.ptr_; }
        };

        // iterators
        //
        // all no throw

        template <typename Alloc, typename EquivKeys>
        class hash_iterator
            : public boost::iterator <
                std::forward_iterator_tag,
                BOOST_DEDUCED_TYPENAME allocator_value_type<Alloc>::type,
                std::ptrdiff_t,
                BOOST_DEDUCED_TYPENAME allocator_pointer<Alloc>::type,
                BOOST_DEDUCED_TYPENAME allocator_reference<Alloc>::type >
        {
        public:
            typedef BOOST_DEDUCED_TYPENAME allocator_value_type<Alloc>::type value_type;

        private:
            typedef hash_table_data<Alloc, EquivKeys> data;
            typedef BOOST_DEDUCED_TYPENAME data::iterator_base base;
            typedef hash_const_iterator<Alloc, EquivKeys> const_iterator;
            friend class hash_const_iterator<Alloc, EquivKeys>;
            base base_;

        public:

            hash_iterator() : base_() {}
            explicit hash_iterator(base const& x) : base_(x) {}
            BOOST_DEDUCED_TYPENAME allocator_reference<Alloc>::type
                operator*() const { return *base_; }
            value_type* operator->() const { return &*base_; }
            hash_iterator& operator++() { base_.increment(); return *this; }
            hash_iterator operator++(int) { hash_iterator tmp(base_); base_.increment(); return tmp; }
            bool operator==(hash_iterator const& x) const { return base_ == x.base_; }
            bool operator==(const_iterator const& x) const { return base_ == x.base_; }
            bool operator!=(hash_iterator const& x) const { return base_ != x.base_; }
            bool operator!=(const_iterator const& x) const { return base_ != x.base_; }
        };

        template <typename Alloc, typename EquivKeys>
        class hash_const_iterator
            : public boost::iterator <
                std::forward_iterator_tag,
                BOOST_DEDUCED_TYPENAME allocator_value_type<Alloc>::type,
                std::ptrdiff_t,
                BOOST_DEDUCED_TYPENAME allocator_const_pointer<Alloc>::type,
                BOOST_DEDUCED_TYPENAME allocator_const_reference<Alloc>::type >
        {
        public:
            typedef BOOST_DEDUCED_TYPENAME allocator_value_type<Alloc>::type value_type;

        private:
            typedef hash_table_data<Alloc, EquivKeys> data;
            typedef BOOST_DEDUCED_TYPENAME data::iterator_base base;
            typedef hash_iterator<Alloc, EquivKeys> iterator;
            friend class hash_iterator<Alloc, EquivKeys>;
            friend class iterator_access;
            base base_;

        public:

            hash_const_iterator() : base_() {}
            explicit hash_const_iterator(base const& x) : base_(x) {}
            hash_const_iterator(iterator const& x) : base_(x.base_) {}
            BOOST_DEDUCED_TYPENAME allocator_const_reference<Alloc>::type
                operator*() const { return *base_; }
            value_type const* operator->() const { return &*base_; }
            hash_const_iterator& operator++() { base_.increment(); return *this; }
            hash_const_iterator operator++(int) { hash_const_iterator tmp(base_); base_.increment(); return tmp; }
            bool operator==(iterator const& x) const { return base_ == x.base_; }
            bool operator==(hash_const_iterator const& x) const { return base_ == x.base_; }
            bool operator!=(iterator const& x) const { return base_ != x.base_; }
            bool operator!=(hash_const_iterator const& x) const { return base_ != x.base_; }
        };

        class iterator_access
        {
        public:
            template <class Iterator>
            static BOOST_DEDUCED_TYPENAME Iterator::base const& get(Iterator const& it) {
                return it.base_;
            }
        };

        template <class ValueType, class KeyType,
            class Hash, class Pred, class Alloc, class EquivKeys>
        class hash_types
        {
        public:
            typedef BOOST_DEDUCED_TYPENAME
                boost::unordered_detail::rebind_wrap<Alloc, ValueType>::type
                value_allocator;

            typedef hash_table<ValueType, KeyType, Hash, Pred,
                    value_allocator, EquivKeys> hash_table;
            typedef BOOST_DEDUCED_TYPENAME hash_table::iterator_base iterator_base;

            typedef hash_const_local_iterator<value_allocator, EquivKeys> const_local_iterator;
            typedef hash_local_iterator<value_allocator, EquivKeys> local_iterator;
            typedef hash_const_iterator<value_allocator, EquivKeys> const_iterator;
            typedef hash_iterator<value_allocator, EquivKeys> iterator;

            typedef BOOST_DEDUCED_TYPENAME hash_table::size_type size_type;
            typedef std::ptrdiff_t difference_type;
        };

        template <class ValueType, class KeyType, class Hash, class Pred, class Alloc>
        class hash_types_unique_keys
            : public hash_types<ValueType, KeyType, Hash, Pred, Alloc, boost::mpl::false_> {};

        template <class ValueType, class KeyType, class Hash, class Pred, class Alloc>
        class hash_types_equivalent_keys
            : public hash_types<ValueType, KeyType, Hash, Pred, Alloc, boost::mpl::true_> {};

    } // namespace boost::unordered_detail
} // namespace boost

#undef BOOST_UNORDERED_BORLAND_BOOL
#undef BOOST_UNORDERED_MSVC_RESET_PTR

#endif // BOOST_UNORDERED_DETAIL_HASH_TABLE_HPP_INCLUDED
