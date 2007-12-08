
// Copyright (C) 2003-2004 Jeremy B. Maitin-Shepard.
// Copyright (C) 2005-2007 Daniel James
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if BOOST_UNORDERED_HASH_EQUIVALENT
#define HASH_TABLE_DATA hash_table_data_equivalent_keys
#else
#define HASH_TABLE_DATA hash_table_data_unique_keys
#endif

namespace boost {
    namespace unordered_detail {

        //
        // Hash Table Data
        //
        // Responsible for managing the hash buckets.

        template <typename Alloc>
        class HASH_TABLE_DATA
        {
        public:
            typedef bucket<Alloc> bucket;
            typedef typename bucket::bucket_allocator bucket_allocator;
            typedef typename bucket::bucket_ptr bucket_ptr;
            typedef typename bucket::link_ptr link_ptr;

            struct node;
            typedef std::size_t size_type;

            typedef Alloc value_allocator;

            typedef BOOST_DEDUCED_TYPENAME
                boost::unordered_detail::rebind_wrap<Alloc, node>::type
                node_allocator;

            typedef BOOST_DEDUCED_TYPENAME allocator_value_type<Alloc>::type value_type;
            typedef BOOST_DEDUCED_TYPENAME allocator_pointer<node_allocator>::type node_ptr;
            typedef BOOST_DEDUCED_TYPENAME allocator_reference<value_allocator>::type reference;

            // Hash Node
            //
            // all no throw

            struct node_base_equivalent_keys : bucket
            {
            public:
                node_base_equivalent_keys() : group_prev_()
                {
                    BOOST_HASH_MSVC_RESET_PTR(group_prev_);
                }

                link_ptr group_prev_;

                static link_ptr& prev_in_group(link_ptr n) {
                    return static_cast<node_base_equivalent_keys&>(*n).group_prev_;
                }

                // pre: Must be pointing to the first node in a group.
                static link_ptr last_in_group(link_ptr n) {
                    BOOST_ASSERT(BOOST_HASH_BORLAND_BOOL(n) && n != prev_in_group(n)->next_);
                    return prev_in_group(n);
                }

                // pre: Must be pointing to the first node in a group.
                static link_ptr& next_group(link_ptr n) {
                    BOOST_ASSERT(BOOST_HASH_BORLAND_BOOL(n) && n != prev_in_group(n)->next_);
                    return prev_in_group(n)->next_;
                }

                // pre: Must be pointing to a node
                static node_base_equivalent_keys& get_node(link_ptr p) {
                    BOOST_ASSERT(p);
                    return static_cast<node_base_equivalent_keys&>(*p);
                }

                static size_type group_count(link_ptr it)
                {
                    size_type count = 0;
                    link_ptr first = it;
                    do {
                        ++count;
                        it = prev_in_group(it);
                    } while (it != first); // throws, strong
                    return count;
                }

                static link_ptr* get_for_erase(bucket_ptr bucket, link_ptr n)
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

                static void link_node(link_ptr n, link_ptr pos)
                {
                    node_base_equivalent_keys& node_ref = get_node(n);
                    node_base_equivalent_keys& pos_ref = get_node(pos);
                    node_ref.next_ = pos_ref.group_prev_->next_;
                    node_ref.group_prev_ = pos_ref.group_prev_;
                    pos_ref.group_prev_->next_ = n;
                    pos_ref.group_prev_ = n;
                }
                
                static void link_node_in_bucket(link_ptr n, bucket_ptr base)
                {
                    node_base_equivalent_keys& node_ref = get_node(n);
                    node_ref.next_ = base->next_;
                    node_ref.group_prev_ = n;
                    base->next_ = n;
                }

                static void link_group(link_ptr n, bucket_ptr base)
                {
                    node_base_equivalent_keys& node_ref = get_node(n);
                    node_base_equivalent_keys& last_ref = get_node(node_ref.group_prev_);
                    last_ref.next_ = base->next_;
                    base->next_ = n;
                }

                static void unlink_node(link_ptr* pos)
                {
                    node_base_equivalent_keys* n = &get_node(*pos);
                    link_ptr next = n->next_;

                    if(n->group_prev_ == *pos) {
                        // The deleted node is the sole node in the group, so
                        // no need to unlink it from a group.
                    }
                    else if(BOOST_HASH_BORLAND_BOOL(next) && prev_in_group(next) == *pos)
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
                static link_ptr split_group(link_ptr split) {
                    // If split is at the beginning of the group then there's
                    // nothing to split.
                    if(prev_in_group(split)->next_ != split)
                        return link_ptr();

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

                static void split_group(link_ptr split1, link_ptr split2) {
                    link_ptr begin1 = split_group(split1);
                    link_ptr begin2 = split_group(split2);

                    if(BOOST_HASH_BORLAND_BOOL(begin1) && split1 == begin2) {
                        link_ptr end1 = prev_in_group(begin1);
                        prev_in_group(begin1) = prev_in_group(begin2);
                        prev_in_group(begin2) = end1;
                    }
                }
            };

            struct node_base_unique_keys : bucket
            {
                link_ptr& prev_in_group(link_ptr) const {
                    BOOST_ASSERT(false);
                }

                static link_ptr last_in_group(link_ptr n) {
                    return n;
                }

                static link_ptr& next_group(link_ptr n) {
                    BOOST_ASSERT(n);
                    return n->next_;
                }

                // pre: Must be pointing to a node
                static node_base_unique_keys& get_node(link_ptr p) {
                    BOOST_ASSERT(p);
                    return static_cast<node_base_unique_keys&>(*p);
                }

                static size_type group_count(link_ptr){
                    return 1;
                }

                static link_ptr* get_for_erase(bucket_ptr bucket, link_ptr n)
                {
                    link_ptr* it = &bucket->next_;
                    while(*it != n) it = &(*it)->next_;
                    return it;
                }

                static void link_node(link_ptr n, link_ptr pos)
                {
                    BOOST_ASSERT(false);
                }

                static void link_node_in_bucket(link_ptr n, bucket_ptr base)
                {
                    n->next_ = base->next_;
                    base->next_ = n;
                }

                static void link_group(link_ptr n, bucket_ptr base)
                {
                    link_node_in_bucket(n, base);
                }

                static void unlink_node(link_ptr* pos)
                {
                    *pos = (*pos)->next_;
                }

                static void split_group(link_ptr) {}
                static void split_group(link_ptr, link_ptr) {}
            };

#if BOOST_UNORDERED_HASH_EQUIVALENT
            typedef node_base_equivalent_keys node_base;
#else
            typedef node_base_unique_keys node_base;
#endif

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
                    hash_swap(node_alloc_, x.node_alloc_);
                    hash_swap(bucket_alloc_, x.bucket_alloc_);
                    hash_swap(value_alloc_, x.value_alloc_);
                    hash_swap(node_base_alloc_, x.node_base_alloc_);
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
                    BOOST_HASH_MSVC_RESET_PTR(node_);
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

            static link_ptr& prev_in_group(link_ptr n) {
                return node_base::prev_in_group(n);
            }

            // pre: Must be pointing to the first node in a group.
            static link_ptr last_in_group(link_ptr n) {
                return node_base::last_in_group(n);
            }

            // pre: Must be pointing to the first node in a group.
            static link_ptr& next_group(link_ptr n) {
                return node_base::next_group(n);
            }

            // pre: Must be pointing to a node
            static reference get_value(link_ptr p) {
                BOOST_ASSERT(p);
                return static_cast<node&>(*p).value_;
            }

            class local_iterator_base
            {
            public:
                link_ptr node_;

                local_iterator_base()
                    : node_()
                {
                    BOOST_HASH_MSVC_RESET_PTR(node_);
                }

                explicit local_iterator_base(link_ptr n)
                    : node_(n) {}

                bool not_finished() const
                {
                    return node_ ? true : false;
                }

                bool operator==(local_iterator_base const& x) const
                {
                    return node_ == x.node_;
                }

                bool operator!=(local_iterator_base const& x) const
                {
                    return node_ != x.node_;
                }

                reference operator*() const
                {
                    return get_value(node_);
                }

                void increment()
                {
                    BOOST_ASSERT(node_);
                    node_ = node_->next_;
                }

                void next_group()
                {
                    node_ = HASH_TABLE_DATA::next_group(node_);
                }
            };

            class iterator_base
            {
            public:
                bucket_ptr bucket_;
                local_iterator_base local_;

                iterator_base()
                    : bucket_(), local_() {}

                explicit iterator_base(bucket_ptr b)
                    : bucket_(b), local_(b->next_) {}

                iterator_base(bucket_ptr b, link_ptr n)
                    : bucket_(b), local_(n) {}

                iterator_base(bucket_ptr b, local_iterator_base const& it)
                    : bucket_(b), local_(it) {}

                bool operator==(iterator_base const& x) const
                {
                    return local_ == x.local_;
                }

                bool operator!=(iterator_base const& x) const
                {
                    return local_ != x.local_;
                }

                reference operator*() const
                {
                    return *local_;
                }

                void increment()
                {
                    BOOST_ASSERT(bucket_);
                    local_.increment();

                    while (!local_.node_) {
                        ++bucket_;
                        local_ = local_iterator_base(bucket_->next_);
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

            HASH_TABLE_DATA(size_type n, value_allocator const& a)
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

                cached_begin_bucket_ = constructor.get() + bucket_count_;

                // Set up the sentinel.
                cached_begin_bucket_->next_ = link_ptr(cached_begin_bucket_);

                // Only release the buckets once everything is successfully
                // done.
                buckets_ = constructor.release();
            }

            HASH_TABLE_DATA(HASH_TABLE_DATA const& x, size_type n)
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

                cached_begin_bucket_ = constructor.get() + bucket_count_;

                // Set up the sentinel
                cached_begin_bucket_->next_ = link_ptr(cached_begin_bucket_);

                // Only release the buckets once everything is successfully
                // done.
                buckets_ = constructor.release();
            }

            // no throw
            ~HASH_TABLE_DATA()
            {
                if(buckets_) {
                    bucket_ptr begin = cached_begin_bucket_;
                    bucket_ptr end = buckets_ + bucket_count_;
                    while(begin != end) {
                        clear_bucket(begin);
                        ++begin;
                    }

                    // Destroy an extra bucket for the sentinels.
                    for(size_type i2 = 0; i2 < bucket_count_ + 1; ++i2)
                        allocators_.bucket_alloc_.destroy(buckets_ + i2);

                    allocators_.bucket_alloc_.deallocate(buckets_, bucket_count_ + 1);
                }
            }

        private:

            HASH_TABLE_DATA(HASH_TABLE_DATA const&);
            HASH_TABLE_DATA& operator=(HASH_TABLE_DATA const&);

        public:

            // no throw
            void swap(HASH_TABLE_DATA& other)
            {
                std::swap(buckets_, other.buckets_);
                std::swap(bucket_count_, other.bucket_count_);
                std::swap(cached_begin_bucket_, other.cached_begin_bucket_);
                std::swap(size_, other.size_);
            }

            // Return the bucket index for a hashed value.
            //
            // no throw
            size_type index_from_hash(size_type hashed) const
            {
                return hashed % bucket_count_;
            }

            // Begin & End
            //
            // no throw

            iterator_base begin() const
            {
                return size_
                    ? iterator_base(cached_begin_bucket_)
                    : end();
            }

            iterator_base end() const
            {
                return iterator_base(buckets_ + bucket_count_);
            }

            local_iterator_base begin(size_type n) const
            {
                return local_iterator_base(buckets_[n].next_);
            }

            local_iterator_base end(size_type) const
            {
                return local_iterator_base();
            }

            local_iterator_base begin(bucket_ptr b) const
            {
                return local_iterator_base(b->next_);
            }

            // Bucket Size

            // no throw
            size_type node_count(local_iterator_base it) const
            {
                size_type count = 0;
                while(it.not_finished()) {
                    ++count;
                    it.increment();
                }
                return count;
            }

            size_type node_count(local_iterator_base it1,
                    local_iterator_base it2) const
            {
                size_type count = 0;
                while(it1 != it2) {
                    ++count;
                    it1.increment();
                }
                return count;
            }

            size_type bucket_size(size_type n) const
            {
                return node_count(begin(n));
            }

            size_type group_count(local_iterator_base first_node) const
            {
                return node_base::group_count(first_node.node_);
            }

            // get_for_erase
            //
            // Find the pointer to a node, for use when erasing.
            //
            // no throw

            link_ptr* get_for_erase(iterator_base r) const
            {
                return node_base::get_for_erase(r.bucket_, r.local_.node_);
            }

            // Link/Unlink/Move Node
            //
            // For adding nodes to buckets, removing them and moving them to a
            // new bucket.
            //
            // no throw

            void link_node(link_ptr n, local_iterator_base pos)
            {
                node_base::link_node(n, pos.node_);
                ++size_;
            }

            void link_node(link_ptr n, bucket_ptr base)
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
                size_type count = group_count(local_iterator_base(*pos));
                size_ -= count;
                *pos = next_group(*pos);
                return count;
            }

            void unlink_nodes(iterator_base n)
            {
                link_ptr* it = get_for_erase(n);
                split_group(*it);
                unordered_detail::reset(*it);
                size_ -= node_count(n.local_);
            }

            void unlink_nodes(iterator_base begin, iterator_base end)
            {
                BOOST_ASSERT(begin.bucket_ == end.bucket_);
                local_iterator_base local_end = end.local_;

                size_ -= node_count(begin.local_, local_end);
                link_ptr* it = get_for_erase(begin);
                split_group(*it, local_end.node_);
                *it = local_end.node_;
            }

            void unlink_nodes(bucket_ptr base, iterator_base end)
            {
                BOOST_ASSERT(base == end.bucket_);

                local_iterator_base local_end = end.local_;
                split_group(local_end.node_);
            
                link_ptr ptr(base->next_);
                base->next_ = local_end.node_;

                size_ -= node_count(local_iterator_base(ptr), local_end);
            }

            // Break a ciruclar list into two, with split as the beginning
            // of the second group (if split is at the beginning then don't
            // split).
            void split_group(link_ptr split)
            {
                node_base::split_group(split);
            }

            void split_group(link_ptr split1, link_ptr split2)
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
                link_node(n, base);
                return iterator_base(base, n);
            }

            iterator_base create_node(value_type const& v, iterator_base position)
            {
                BOOST_ASSERT(BOOST_UNORDERED_HASH_EQUIVALENT);

                // throws, strong exception-safety:
                link_ptr n = construct_node(v);

                // Rest is no throw
                link_node(n, position.local_);
                return iterator_base(position.bucket_, n);
            }

            iterator_base create_node(value_type const& v,
                    bucket_ptr base, local_iterator_base position)
            {
                BOOST_ASSERT(BOOST_UNORDERED_HASH_EQUIVALENT);

                // throws, strong exception-safety:
                link_ptr n = construct_node(v);

                // Rest is no throw
                if(position.not_finished())
                    link_node(n, position);
                else
                    link_node(n, base);

                return iterator_base(base, n);
            }

            void copy_group(local_iterator_base it, bucket_ptr dst)
            {
                local_iterator_base end = it;
                end.next_group();
                iterator_base pos = create_node(*it, dst);
                for(it.increment(); it != end; it.increment())
                    create_node(*it, pos);
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
                bucket_ptr begin = buckets_;
                bucket_ptr end = buckets_ + bucket_count_;

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
                allocators_.destroy(r.local_.node_);
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
                        delete_nodes(r1.local_.node_, r2.local_.node_);

                        // No need to call recompute_begin_bucket because
                        // the nodes are only deleted from one bucket, which
                        // still contains r2 after the erase.
                        BOOST_ASSERT(!r1.bucket_->empty());
                    }
                    else {
                        BOOST_ASSERT(r1.bucket_ < r2.bucket_);

                        unlink_nodes(r1);
                        delete_to_bucket_end(r1.local_.node_);

                        for(bucket_ptr i = r1.bucket_ + 1; i != r2.bucket_; ++i) {
                            size_ -= node_count(local_iterator_base(i->next_));
                            clear_bucket(i);
                        }

                        if(r2 != end()) {
                            link_ptr first = r2.bucket_->next_;
                            unlink_nodes(r2.bucket_, r2);
                            delete_nodes(first, r2.local_.node_);
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
                        cached_begin_bucket_ = buckets_ + bucket_count_;
                    }
                }
            }

            // This is called when a range has been erased
            //
            // no throw

            void recompute_begin_bucket(bucket_ptr b1, bucket_ptr b2)
            {
                BOOST_ASSERT(!(b1 < cached_begin_bucket_) && !(b2 < b1));
                BOOST_ASSERT(b2 == buckets_ + bucket_count_ || !b2->empty());

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
        class HASH_TABLE_DATA<int>
        {
        public:
            typedef int size_type;
            typedef int iterator_base;
        };
#endif

    }
}

#undef HASH_TABLE_DATA
