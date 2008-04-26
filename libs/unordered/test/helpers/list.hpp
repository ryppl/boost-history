
// Copyright 2008 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Gratuitous single linked list.
//
// Sadly some STL implementations aren't up to scratch and I need a simple
// cross-platform container. So here it is.

#if !defined(UNORDERED_TEST_LIST_HEADER)
#define UNORDERED_TEST_LIST_HEADER

#include <boost/operators.hpp>
#include <boost/limits.hpp>
#include <functional>

namespace test
{
    namespace test_detail
    {
        template <typename T>
        class list_impl
        {
        public:
            class iterator;
            class const_iterator;

            typedef T value_type;
            typedef value_type& reference;
            typedef value_type const& const_reference;

            typedef unsigned int size_type;

            struct node {
                T value_;
                node* next_;
                        
                node(T const& v) : value_(v), next_(0) {}
                node(T const& v, node* n) : value_(v), next_(n) {}
            };

            class iterator
                : public boost::forward_iterator_helper<
                      iterator, value_type,
                      int, value_type*, value_type&>
            {
                friend class const_iterator;
                friend class list_impl;
                node* ptr_;
            public:
                iterator() : ptr_(0) {};
                explicit iterator(node* x) : ptr_(x) {}

                value_type& operator*() const { return ptr_->value_; }
                iterator& operator++() {
                    ptr_ = ptr_->next_; return *this; }
                friend bool operator==(iterator const& x,
                        iterator const& y) { return x.ptr_ == y.ptr_; }
            };

            class const_iterator
                : public boost::forward_iterator_helper<
                    const_iterator, value_type const,
                    int, value_type const*, value_type const&>
            {
                friend class list_impl;
                node* ptr_;
            public:
                const_iterator() : ptr_(0) {}
                const_iterator(iterator const& x) : ptr_(x.ptr_) {}

                value_type const& operator*() const { return ptr_->value_; }
                const_iterator& operator++() {
                    ptr_ = ptr_->next_; return *this; }
                friend bool operator==(const_iterator const& x,
                        const_iterator const& y) { return x.ptr_ == y.ptr_; }
            };

        protected:
            node* first_;
            node** last_ptr_;
            size_type size_;
            
        public:
            list_impl() : first_(0), last_ptr_(&first_), size_(0) {}

            ~list_impl() {
                while(first_) {
                    node* tmp = first_;
                    first_ = first_->next_;
                    delete tmp;
                }
            }

            iterator begin() { return iterator(first_); }
            iterator end() { return iterator(); }
            const_iterator begin() const { return iterator(first_); }
            const_iterator end() const { return iterator(); }
            const_iterator cbegin() const { return iterator(first_); }
            const_iterator cend() const { return iterator(); }

            template <class InputIterator>
            void insert(InputIterator i, InputIterator j) {
                for(; i != j; ++i)
                    push_back(*i);
            }

            void push_front(value_type const& v) {
                first_ = new node(v, first_);
                if(size_) last_ptr_ = &(*last_ptr_)->next_;
                ++size_;
            }
        
            void push_back(value_type const& v) {
                *last_ptr_ = new node(v);
                last_ptr_ = &(*last_ptr_)->next_;
                ++size_;
            }
            
            void clear() {
                while(first_) {
                    node* tmp = first_;
                    first_ = first_->next_;
                    --size_;
                    delete tmp;
                }
                last_ptr_ = &first_;
            }

            void erase(const_iterator start, const_iterator end) {
                node** ptr = &first_;

                while(*ptr != start.ptr_) {
                    ptr = &(*ptr)->next_;
                }

                while(*ptr != end.ptr_) {
                    node* to_delete = *ptr;
                    *ptr = (*ptr)->next_;
                    --size_;
                    delete to_delete;
                }

                if(!*ptr) last_ptr_ = ptr;
            }

            bool empty() const {
                return !size_;
            }

            size_type size() const {
                return size_;
            }

            void sort() {
                sort(std::less<T>());
            }

            template <typename Less>
            void sort(Less less = Less()) {
                if(!this->empty()) merge_sort(&this->first_,
                        (std::numeric_limits<size_type>::max)(), less);
            }

        private:

            template <typename Less>
            node** merge_sort(node** l, size_type recurse_limit, Less less)
            {
                node** ptr = &(*l)->next_;
                for(size_type count = 0; count < recurse_limit && *ptr; ++count)
                {
                    ptr = merge_adjacent_ranges(l, ptr,
                            merge_sort(ptr, count, less), less);
                }
                return ptr;
            }
            
            template <typename Less>
            node** merge_adjacent_ranges(node** first, node** second,
                    node** third, Less less)
            {
                while(first != second) {
                    if(less((*second)->value_, (*first)->value_)) {
                        swap_adjacent_ranges(first, second, third);
                        std::swap(second, third);
                    }
                    first = &(*first)->next_;
                }
                return third;
            }
            
            void swap_adjacent_ranges(node** first, node** second, node** third)
            {
                node* tmp = *first;
                *first = *second;
                *second = *third;
                *third = tmp;
                if(!*second) this->last_ptr_ = second;
            }

            list_impl(list_impl const&);
            list_impl& operator=(list_impl const&);
        };
    }

    template <typename T>
    class list
        : public test::test_detail::list_impl<T>,
            boost::equality_comparable1<list<T> >
            
    {
        typedef test::test_detail::list_impl<T> base;
    public:
        list() : base() {}

        list(list const& other) : base() {
            this->insert(other.begin(), other.end());
        }

        template <class InputIterator>
        list(InputIterator i, InputIterator j) : base() {
            this->insert(i, j);
        }

        list& operator=(list const& other) {
            this->clear();
            this->insert(other.begin(), other.end());
        }

        friend bool operator==(list const& x, list const& y) {
            return x.size() == y.size() &&
                std::equal(x.begin(), x.end(), y.begin());
        }
    };
}

#endif
