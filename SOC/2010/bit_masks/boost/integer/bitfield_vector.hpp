//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_VECTOR_HPP
#define BOOST_BITFIELD_VECTOR_HPP
#include <cstddef>
#include <memory>

namespace boost {

template <typename,typename>
class vector;

template <typename T,std::size_t>
struct bits;

/** Iterators. */
//@{
template<typename T, std::size_t Width>
struct bf_vector_iterator { };

template<typename T, std::size_t Width>
struct const_bf_vector_iterator { };


template<typename T, std::size_t Width>
struct bf_vector_reverse_iterator { };

template<typename T, std::size_t Width>
struct const_bf_vector_reverse_iterator { };
//@}


template <typename T, std::size_t Width, typename Allocator >
class vector< bits<T,Width>, Allocator > {
    typedef vector< bits<T,Width>, Allocator >  _self;
public:
    typedef bf_vector_iterator<T,Width> iterator;
    typedef const_bf_vector_iterator<T,Width> const_iterator;
    typedef bf_vector_reverse_iterator<T,Width> reverse_iterator;
    typedef const_bf_vector_reverse_iterator<T,Width> const_reverse_iterator;
    typedef T value_type;

    typedef Allocator allocator_type;
    typedef typename Allocator::size_type size_type;
    typedef typename Allocator::difference_type difference_type;
    typedef typename Allocator::pointer pointer;
    typedef typename Allocator::const_pointer const_pointer;
    
    class reference {
        friend class vector;
        reference();
    public:
        ~reference();
        operator value_type () const;
        reference& operator= (const value_type x);
        reference& operator= (const reference& x);
        void flip();
    };

    class const_reference {
        friend class vector;
        const_reference();
    public:
        ~const_reference();
        // operator value_type () const;
        const_reference& operator= (const value_type x);
        const_reference& operator= (const const_reference& x);
        void flip();
    };


    explicit vector(const Allocator& = Allocator() );
    explicit vector(size_type n, const T& value= T(),
        const Allocator& = Allocator() );
    template <class InputIterator>
    vector(InputIterator first, InputIterator last,
        const Allocator& = Allocator());
    vector( const vector<T,Allocator>& x );
    ~vector();

    _self& operator=(const _self& x);

    iterator    begin();
    iterator    end();
    iterator    rbegin();
    iterator    rend();
    reference   front();
    reference   back();

    const_iterator  begin()  const;
    const_iterator  end()    const;
    const_iterator  rbegin() const;
    const_iterator  rend()   const;
    const_reference front()  const;
    const_reference back()   const;

    size_type size() const;
    size_type max_size() const;


    void resize(size_type sz, value_type c = value_type() );    
    size_type capacity() const;
    bool empty() const;
    void reserve(size_type n);
    reference operator[](size_type n);
    const_reference operator[](size_type n) const;
    const_reference at(size_type n) const;
    reference at(size_type n);

    template <class InputIterator>
    void assign(InputIterator first, InputIterator last);

    void assign(size_type n, const value_type& u);
    void push_back(const value_type& x);
    void pop_back();
    iterator insert(iterator position, const value_type& x);
    void insert(iterator position, size_type n, const value_type& x);

    template <class InputIterator>
    void insert(iterator position, InputIterator first, InputIterator last);

    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);
    void swap(_self& vec);
    void clear();
    allocator_type get_allocator() const;
};

} // end boost

#endif
