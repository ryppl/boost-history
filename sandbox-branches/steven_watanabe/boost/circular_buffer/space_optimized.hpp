// Implementation of the circular buffer adaptor.

// Copyright (c) 2003-2006 Jan Gaspar

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_CIRCULAR_BUFFER_SPACE_OPTIMIZED_HPP)
#define BOOST_CIRCULAR_BUFFER_SPACE_OPTIMIZED_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif

namespace boost {

/*!
    \class circular_buffer_space_optimized
    \brief Space optimized circular buffer container adaptor.
    \param T The type of the elements stored in the space optimized circular buffer.
    \par Type Requirements T
         The <code>T</code> has to be <a href="http://www.sgi.com/tech/stl/Assignable.html">
         SGIAssignable</a> (SGI STL defined combination of <a href="../../utility/Assignable.html">
         Assignable</a> and <a href="../../utility/CopyConstructible.html">CopyConstructible</a>).
         Moreover <code>T</code> has to be <a href="http://www.sgi.com/tech/stl/DefaultConstructible.html">
         DefaultConstructible</a> if supplied as a default parameter when invoking some of the methods.
    \param Alloc The allocator type used for all internal memory management.
    \par Type Requirements Alloc
         The <code>Alloc</code> has to meet the allocator requirements imposed by STL.
    \par Default Alloc
         std::allocator<T>
    \author Jan Gaspar
    \version 1.4
    \date 2006

    For detailed documentation of the space_optimized_circular_buffer visit:
    http://www.boost.org/libs/circular_buffer/doc/circular_buffer_adaptor.html
*/
template <class T, class Alloc>
class circular_buffer_space_optimized : private circular_buffer<T, Alloc> {
public:
// Typedefs

    typedef typename circular_buffer<T, Alloc>::value_type value_type;
    typedef typename circular_buffer<T, Alloc>::pointer pointer;
    typedef typename circular_buffer<T, Alloc>::const_pointer const_pointer;
    typedef typename circular_buffer<T, Alloc>::reference reference;
    typedef typename circular_buffer<T, Alloc>::const_reference const_reference;
    typedef typename circular_buffer<T, Alloc>::size_type size_type;
    typedef typename circular_buffer<T, Alloc>::difference_type difference_type;
    typedef typename circular_buffer<T, Alloc>::allocator_type allocator_type;
    typedef typename circular_buffer<T, Alloc>::const_iterator const_iterator;
    typedef typename circular_buffer<T, Alloc>::iterator iterator;
    typedef typename circular_buffer<T, Alloc>::const_reverse_iterator const_reverse_iterator;
    typedef typename circular_buffer<T, Alloc>::reverse_iterator reverse_iterator;
    typedef typename circular_buffer<T, Alloc>::array_range array_range;
    typedef typename circular_buffer<T, Alloc>::const_array_range const_array_range;
    typedef typename circular_buffer<T, Alloc>::param_value_type param_value_type;
    typedef typename circular_buffer<T, Alloc>::return_value_type return_value_type;

#if defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)
    //! Capacity controller of the space optimized circular buffer.
    /*!
        <p><pre>
    struct capacity_control {
       capacity_control(size_type capacity, size_type min_capacity = 0) m_capacity(capacity), m_min_capacity(min_capacity) {};
       size_type m_capacity;
       size_type m_min_capacity;
    };
        </pre></p>
        \pre <code>capacity >= min_capacity</code>
        <p>The <code>m_capacity</code> denotes the capacity of the
        <code>circular_buffer_space_optimized</code> and the <code>m_min_capacity</code>
        determines the minimal allocated size of its internal buffer.</p>
        <p>The converting constructor of the <code>capacity_control</code>
        allows implicit conversion from <code>size_type</code> like types
        which ensures compatibility of creating an instance of the
        <code>circular_buffer_space_optimized</code> with other STL containers.</p>
    */
    typedef cb_details::capacity_control<size_type, T, Alloc> capacity_type;
#else
    /*! \cond */
    typedef cb_details::capacity_control<size_type> capacity_type;
    /*! \endcond */
#endif

// Inherited

    using circular_buffer<T, Alloc>::get_allocator;
    using circular_buffer<T, Alloc>::begin;
    using circular_buffer<T, Alloc>::end;
    using circular_buffer<T, Alloc>::rbegin;
    using circular_buffer<T, Alloc>::rend;
    using circular_buffer<T, Alloc>::at;
    using circular_buffer<T, Alloc>::front;
    using circular_buffer<T, Alloc>::back;
    using circular_buffer<T, Alloc>::array_one;
    using circular_buffer<T, Alloc>::array_two;
    using circular_buffer<T, Alloc>::linearize;
    using circular_buffer<T, Alloc>::size;
    using circular_buffer<T, Alloc>::max_size;
    using circular_buffer<T, Alloc>::empty;

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    reference operator [] (size_type n) { return circular_buffer<T, Alloc>::operator[](n); }
    return_value_type operator [] (size_type n) const { return circular_buffer<T, Alloc>::operator[](n); }
#else
    using circular_buffer<T, Alloc>::operator[];
#endif

private:
// Member variables

    //! The capacity controller of the space optimized circular buffer.
    capacity_type m_capacity_ctrl;

public:
// Overridden

    //! See the circular_buffer source documentation.
    bool full() const { return capacity() == size(); }

    //! See the circular_buffer source documentation.
    size_type reserve() const { return capacity() - size(); }

    //! See the circular_buffer source documentation.
    const capacity_type& capacity() const { return m_capacity_ctrl; }

#if defined(BOOST_CB_TEST)

    // Return the current capacity of the adapted circular buffer.
    /*
       \note This method is not intended to be used directly by the user.
             It is defined only for testing purposes.
    */
    size_type internal_capacity() const { return circular_buffer<T, Alloc>::capacity(); }

#endif // #if defined(BOOST_CB_TEST)

    //! TODO Change the minimal guaranteed amount of allocated memory.
    /*!
        \pre <code>(*this).capacity() >= new_min_capacity</code>
        \post <code>(*this).min_capacity() == new_min_capacity</code>
              Allocates memory specified by the <code>new_min_capacity</code> parameter.
        \note It is considered as a bug if the precondition is not met (i.e. if
              <code>new_min_capacity > (*this).capacity()</code>) and an assertion
              will be invoked in the debug mode.

         \pre <code>min_capacity() <= new_capacity</code>
         \note It is considered as a bug if the precondition is not met (i.e. if
               <code>new_capacity > min_capacity()</code>) and an assertion
               will be invoked in the debug mode.
    */
    void set_capacity(const capacity_type& new_capacity) {
        m_capacity_ctrl = new_capacity;
        if (new_capacity.capacity() < circular_buffer<T, Alloc>::capacity())
            circular_buffer<T, Alloc>::set_capacity(new_capacity.capacity());
        set_min_capacity(new_capacity.min_capacity());
    }

    //! See the circular_buffer source documentation.
    void resize(size_type new_size, param_value_type item = value_type()) {
        if (new_size > size()) {
            if (new_size > capacity())
                m_capacity_ctrl.m_capacity = new_size;
            insert(end(), new_size - size(), item);
        } else {
            erase(end() - (size() - new_size), end());
        }
    }

    //! See the circular_buffer source documentation.
    /*!
         \pre <code>min_capacity() <= new_capacity</code>
         \note It is considered as a bug if the precondition is not met (i.e. if
               <code>new_capacity > min_capacity()</code>) and an assertion
               will be invoked in the debug mode.
    */
    void rset_capacity(const capacity_type& new_capacity) {
        m_capacity_ctrl = new_capacity;
        if (new_capacity.capacity() < circular_buffer<T, Alloc>::capacity())
            circular_buffer<T, Alloc>::rset_capacity(new_capacity.capacity());
        set_min_capacity(new_capacity.min_capacity());
    }

    //! See the circular_buffer source documentation.
    void rresize(size_type new_size, param_value_type item = value_type()) {
        if (new_size > size()) {
            if (new_size > capacity())
                m_capacity_ctrl.m_capacity = new_size;
            insert(begin(), new_size - size(), item);
        } else {
            erase(begin(), end() - new_size);
        }
    }

    //! Create an empty space optimized circular buffer with a maximum capacity.
    /*!
        TODO - doc
    */
    explicit circular_buffer_space_optimized(
        const allocator_type& alloc = allocator_type())
    : circular_buffer<T, Alloc>(0, alloc)
    , m_capacity_ctrl(max_size()) {}

    //! Create an empty space optimized circular buffer with the specified capacity.
    /*!
        \param capacity_ctrl The capacity of the buffer.
        (param min_capacity The minimal guaranteed amount of allocated memory.)
               (The metrics of the min_capacity is number of items.)
        \param alloc The allocator.
        \pre <code>capacity >= min_capacity</code>
        \post <code>(*this).capacity() == capacity \&\& (*this).size == 0</code><br>
              Allocates memory specified by the <code>min_capacity</code> parameter.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is used).
        \note It is considered as a bug if the precondition is not met (i.e. if
              <code>capacity < min_capacity</code>) and an assertion will be invoked
              in the debug mode.
    */
    explicit circular_buffer_space_optimized(
        capacity_type capacity_ctrl,
        const allocator_type& alloc = allocator_type())
    : circular_buffer<T, Alloc>(capacity_ctrl.m_min_capacity, alloc)
    , m_capacity_ctrl(capacity_ctrl) {}

    //! Create a full space optimized circular buffer filled with copies of <code>item</code>.
    /*!
        \param capacity_ctrl The capacity of the buffer.
        (param min_capacity The minimal guaranteed amount of allocated memory.)
               (The metrics of the min_capacity is number of items.)
        \param item The item to be filled with.
        \param alloc The allocator.
        \pre <code>capacity >= min_capacity</code>
        \post <code>(*this).size() == capacity \&\& (*this)[0] == (*this)[1] == ... == (*this).back() == item</code>
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is used).
        \throws Whatever T::T(const T&) throws.
        \note It is considered as a bug if the precondition is not met (i.e. if
              <code>capacity < min_capacity</code>) and an assertion will be invoked
              in the debug mode.
    */
    circular_buffer_space_optimized(
        capacity_type capacity_ctrl,
        param_value_type item,
        const allocator_type& alloc = allocator_type())
    : circular_buffer<T, Alloc>(capacity_ctrl.m_capacity, item, alloc)
    , m_capacity_ctrl(capacity_ctrl) {}

    //! TODO doc
    circular_buffer_space_optimized(
        capacity_type capacity_ctrl,
        size_type n,
        param_value_type item,
        const allocator_type& alloc = allocator_type())
    : circular_buffer<T, Alloc>(init_capacity(capacity_ctrl, n), n, item, alloc)
    , m_capacity_ctrl(capacity_ctrl) {}

#if defined(BOOST_CB_NEVER_DEFINED)

    // Default copy constructor - declared only for documentation purpose.
    /*
       \note This section will never be compiled. The default copy constructor
             will be generated instead.
    */
    //! TODO doc
    circular_buffer_space_optimized(const circular_buffer_space_optimized<T, Alloc>& cb);

#endif // #if defined(BOOST_CB_NEVER_DEFINED)

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1200)

    // TODO describe workaround
    template <class InputIterator>
    circular_buffer_space_optimized(
        InputIterator first,
        InputIterator last)
    : circular_buffer<T, Alloc>(first, last)
    , m_capacity_ctrl(circular_buffer<T, Alloc>::capacity()) {}

  // TODO describe workaround
    template <class InputIterator>
    circular_buffer_space_optimized(
        capacity_type capacity_ctrl,
        InputIterator first,
        InputIterator last)
    : circular_buffer<T, Alloc>(
        init_capacity(capacity_ctrl, first, last, is_integral<InputIterator>()),
        first, last)
    , m_capacity_ctrl(capacity_ctrl) {
        check_high_capacity(is_integral<InputIterator>());
    }

#else

    //! TODO doc
    template <class InputIterator>
    circular_buffer_space_optimized(
        InputIterator first,
        InputIterator last,
        const allocator_type& alloc = allocator_type())
    : circular_buffer<T, Alloc>(first, last, alloc)
    , m_capacity_ctrl(circular_buffer<T, Alloc>::capacity()) {}


    //! Create a space optimized circular buffer with a copy of a range.
    /*!
        \param capacity_ctrl The capacity of the buffer.
        (param min_capacity The minimal guaranteed amount of allocated memory.)
              (The metrics of the min_capacity is number of items.)
        \param first The beginning of the range.
        \param last The end of the range.
        \param alloc The allocator.
        \pre <code>capacity >= min_capacity</code> and valid range <code>[first, last)</code>.
        \post <code>(*this).capacity() == capacity</code><br>
              Allocates at least as much memory as specified by the - TODO change
              <code>min_capacity</code> parameter.<br>
              If the number of items to copy from the range
              <code>[first, last)</code> is greater than the specified
              <code>capacity</code> then only elements from the range
              <code>[last - capacity, last)</code> will be copied.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is used).
        \throws Whatever T::T(const T&) throws.
        \note It is considered as a bug if the precondition is not met (i.e. if
              <code>capacity < min_capacity</code>) and an assertion will be invoked
              in the debug mode.
    */
    template <class InputIterator>
    circular_buffer_space_optimized(
        capacity_type capacity_ctrl,
        InputIterator first,
        InputIterator last,
        const allocator_type& alloc = allocator_type())
    : circular_buffer<T, Alloc>(
        init_capacity(capacity_ctrl, first, last, is_integral<InputIterator>()),
        first, last, alloc)
    , m_capacity_ctrl(capacity_ctrl) {
        check_high_capacity(is_integral<InputIterator>());
    }

#endif // #if BOOST_WORKAROUND(BOOST_MSVC, <= 1200)

#if defined(BOOST_CB_NEVER_DEFINED)

    // Default destructor
    //! TODO doc
    ~circular_buffer_space_optimized();


    // Assignment operator - declared only for documentation purpose.
    /*
       \note This section will never be compiled. The default assignment
             operator will be generated instead.
    */
    //! TODO doc
    circular_buffer_space_optimized<T, Alloc>& operator = (const circular_buffer_space_optimized<T, Alloc>& cb);

#endif // #if defined(BOOST_CB_NEVER_DEFINED)

    //! See the circular_buffer source documentation.
    void assign(size_type n, param_value_type item) {
        circular_buffer<T, Alloc>::assign(n, item);
        m_capacity_ctrl.m_capacity = n;
        m_capacity_ctrl.m_min_capacity = 0;
    }

    //! See the circular_buffer source documentation.
    void assign(capacity_type capacity_ctrl, size_type n, param_value_type item) {
       BOOST_CB_ASSERT(capacity_ctrl.m_capacity >= n); // check for new capacity lower than n
       circular_buffer<T, Alloc>::assign(std::max(capacity_ctrl.m_min_capacity, n), n, item);
       m_capacity_ctrl = capacity_ctrl;
    }

    //! See the circular_buffer source documentation.
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last) {
        circular_buffer<T, Alloc>::assign(first, last);
        m_capacity_ctrl.m_capacity = circular_buffer<T, Alloc>::capacity();
        m_capacity_ctrl.m_min_capacity = 0;
    }

    //! See the circular_buffer source documentation.
    template <class InputIterator>
    void assign(capacity_type capacity_ctrl, InputIterator first, InputIterator last) {
       m_capacity_ctrl = capacity_ctrl;
       circular_buffer<T, Alloc>::assign(capacity(), first, last);
       check_high_capacity();
    }

    //! See the circular_buffer source documentation.
    void swap(circular_buffer_space_optimized<T, Alloc>& cb) {
        std::swap(m_capacity_ctrl, cb.m_capacity_ctrl);
        circular_buffer<T, Alloc>::swap(cb);
    }

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    void push_back(param_value_type item = value_type()) {
        check_low_capacity();
        circular_buffer<T, Alloc>::push_back(item);
    }

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    void push_front(param_value_type item = value_type()) {
        check_low_capacity();
        circular_buffer<T, Alloc>::push_front(item);
    }

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    void pop_back() {
        circular_buffer<T, Alloc>::pop_back();
        check_high_capacity();
    }

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    void pop_front() {
        circular_buffer<T, Alloc>::pop_front();
        check_high_capacity();
    }

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    iterator insert(iterator pos, param_value_type item = value_type()) {
        size_type index = pos - begin();
        check_low_capacity();
        return circular_buffer<T, Alloc>::insert(begin() + index, item);
    }

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    void insert(iterator pos, size_type n, param_value_type item) {
        size_type index = pos - begin();
        check_low_capacity(n);
        circular_buffer<T, Alloc>::insert(begin() + index, n, item);
    }

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    template <class InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last) {
        insert(pos, first, last, is_integral<InputIterator>());
    }

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    iterator rinsert(iterator pos, param_value_type item = value_type()) {
        size_type index = pos - begin();
        check_low_capacity();
        return circular_buffer<T, Alloc>::rinsert(begin() + index, item);
    }

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    void rinsert(iterator pos, size_type n, param_value_type item) {
        size_type index = pos - begin();
        check_low_capacity(n);
        circular_buffer<T, Alloc>::rinsert(begin() + index, n, item);
    }

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    template <class InputIterator>
    void rinsert(iterator pos, InputIterator first, InputIterator last) {
        rinsert(pos, first, last, is_integral<InputIterator>());
    }

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    iterator erase(iterator pos) {
        iterator it = circular_buffer<T, Alloc>::erase(pos);
        size_type index = it - begin();
        check_high_capacity();
        return begin() + index;
    }

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    iterator erase(iterator first, iterator last) {
        iterator it = circular_buffer<T, Alloc>::erase(first, last);
        size_type index = it - begin();
        check_high_capacity();
        return begin() + index;
    }

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    iterator rerase(iterator pos) {
        iterator it = circular_buffer<T, Alloc>::rerase(pos);
        size_type index = it - begin();
        check_high_capacity();
        return begin() + index;
    }

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    iterator rerase(iterator first, iterator last) {
        iterator it = circular_buffer<T, Alloc>::rerase(first, last);
        size_type index = it - begin();
        check_high_capacity();
        return begin() + index;
    }

    //! See the circular_buffer source documentation.
    void clear() { erase(begin(), end()); }

private:
// Helper methods

    //! Change the minimal guaranteed amount of allocated memory.
    void set_min_capacity(size_type new_min_capacity) {
        if (new_min_capacity > circular_buffer<T, Alloc>::capacity())
            circular_buffer<T, Alloc>::set_capacity(new_min_capacity);
        else
            check_high_capacity();
    }

    //! Ensure the reserve for possible growth up.
    size_type ensure_reserve(size_type new_capacity, size_type size) const {
        if (size + new_capacity / 5 >= new_capacity)
            new_capacity *= 2; // ensure at least 20% reserve
        if (new_capacity > capacity())
            return capacity();
        return new_capacity;
    }

    //! Check for low capacity.
    /*
        \post If the capacity is low it will be increased.
    */
    void check_low_capacity(size_type n = 1) {
        size_type new_size = size() + n;
        size_type new_capacity = circular_buffer<T, Alloc>::capacity();
        if (new_size > new_capacity) {
            if (new_capacity == 0)
                new_capacity = 1;
            for (; new_size > new_capacity; new_capacity *= 2);
            circular_buffer<T, Alloc>::set_capacity(
                ensure_reserve(new_capacity, new_size));
        }
    }

    //! Check for high capacity.
    /*
        \post If the capacity is high it will be decreased.
    */
    void check_high_capacity() {
        size_type new_capacity = circular_buffer<T, Alloc>::capacity();
        while (new_capacity / 3 >= size()) { // (new_capacity / 3) -> avoid oscillations
            new_capacity /= 2;
            if (new_capacity <= m_capacity_ctrl.m_min_capacity) {
                new_capacity = m_capacity_ctrl.m_min_capacity;
                break;
            }
        }
        circular_buffer<T, Alloc>::set_capacity(
            ensure_reserve(new_capacity, size()));
    }

    //! TODO doc
    void check_high_capacity(const true_type&) {}

    //! TODO doc
    void check_high_capacity(const false_type&) {
        check_high_capacity();
    }

    //! TODO doc
    static size_type init_capacity(const capacity_type& capacity_ctrl, size_type n) {
        BOOST_CB_ASSERT(capacity_ctrl.m_capacity >= n); // check for capacity lower than n
        return std::max(capacity_ctrl.m_min_capacity, n);
    }

    //! Specialized method for determining the initial capacity.
    template <class IntegralType>
    static size_type init_capacity(const capacity_type& capacity_ctrl, IntegralType n, IntegralType item, const true_type&) {
        return init_capacity(capacity_ctrl, static_cast<size_type>(n));
    }

    //! Specialized method for determining the initial capacity.
    template <class Iterator>
    static size_type init_capacity(const capacity_type& capacity_ctrl, Iterator first, Iterator last, const false_type&) {
        BOOST_CB_IS_CONVERTIBLE(Iterator, value_type); // check for invalid iterator type
        return init_capacity(capacity_ctrl, first, last, BOOST_DEDUCED_TYPENAME BOOST_ITERATOR_CATEGORY<Iterator>::type());
    }

    //! Specialized method for determining the initial capacity.
    template <class InputIterator>
    static size_type init_capacity(const capacity_type& capacity_ctrl, InputIterator first, InputIterator last, const std::input_iterator_tag&) {
        return capacity_ctrl.m_capacity;
    }

    //! Specialized method for determining the initial capacity.
    template <class ForwardIterator>
    static size_type init_capacity(const capacity_type& capacity_ctrl, ForwardIterator first, ForwardIterator last, const std::forward_iterator_tag&) {
        BOOST_CB_ASSERT(std::distance(first, last) >= 0); // check for wrong range
        return std::min(capacity_ctrl.m_capacity, std::max(capacity_ctrl.m_min_capacity, static_cast<size_type>(std::distance(first, last))));
    }

    //! Specialized insert method.
    template <class IntegralType>
    void insert(const iterator& pos, IntegralType n, IntegralType item, const true_type&) {
        insert(pos, static_cast<size_type>(n), static_cast<value_type>(item));
    }

    //! Specialized insert method.
    template <class Iterator>
    void insert(const iterator& pos, Iterator first, Iterator last, const false_type&) {
        size_type index = pos - begin();
        check_low_capacity(std::distance(first, last));
        circular_buffer<T, Alloc>::insert(begin() + index, first, last);
    }

    //! Specialized rinsert method.
    template <class IntegralType>
    void rinsert(const iterator& pos, IntegralType n, IntegralType item, const true_type&) {
        rinsert(pos, static_cast<size_type>(n), static_cast<value_type>(item));
    }

    //! Specialized rinsert method.
    template <class Iterator>
    void rinsert(const iterator& pos, Iterator first, Iterator last, const false_type&) {
        size_type index = pos - begin();
        check_low_capacity(std::distance(first, last));
        circular_buffer<T, Alloc>::rinsert(begin() + index, first, last);
    }
};

// Non-member functions

//! Test two space optimized circular buffers for equality.
template <class T, class Alloc>
inline bool operator == (const circular_buffer_space_optimized<T, Alloc>& lhs,
                         const circular_buffer_space_optimized<T, Alloc>& rhs) {
    return lhs.size() == rhs.size() &&
        std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

//! Lexicographical comparison.
template <class T, class Alloc>
inline bool operator < (const circular_buffer_space_optimized<T, Alloc>& lhs,
                        const circular_buffer_space_optimized<T, Alloc>& rhs) {
    return std::lexicographical_compare(
        lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

#if !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING) || BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1310))

//! Test two space optimized circular buffers for non-equality.
template <class T, class Alloc>
inline bool operator != (const circular_buffer_space_optimized<T, Alloc>& lhs,
                         const circular_buffer_space_optimized<T, Alloc>& rhs) {
    return !(lhs == rhs);
}

//! Lexicographical comparison.
template <class T, class Alloc>
inline bool operator > (const circular_buffer_space_optimized<T, Alloc>& lhs,
                        const circular_buffer_space_optimized<T, Alloc>& rhs) {
    return rhs < lhs;
}

//! Lexicographical comparison.
template <class T, class Alloc>
inline bool operator <= (const circular_buffer_space_optimized<T, Alloc>& lhs,
                         const circular_buffer_space_optimized<T, Alloc>& rhs) {
    return !(rhs < lhs);
}

//! Lexicographical comparison.
template <class T, class Alloc>
inline bool operator >= (const circular_buffer_space_optimized<T, Alloc>& lhs,
                         const circular_buffer_space_optimized<T, Alloc>& rhs) {
    return !(lhs < rhs);
}

//! Swap the contents of two space optimized circular buffers.
template <class T, class Alloc>
inline void swap(circular_buffer_space_optimized<T, Alloc>& lhs,
                 circular_buffer_space_optimized<T, Alloc>& rhs) {
    lhs.swap(rhs);
}

#endif // #if !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING) || BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1310))

} // namespace boost

#endif // #if !defined(BOOST_CIRCULAR_BUFFER_SPACE_OPTIMIZED_HPP)
