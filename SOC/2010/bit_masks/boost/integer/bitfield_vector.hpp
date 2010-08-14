//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_VECTOR_HPP
#define BOOST_BITFIELD_VECTOR_HPP
#include <memory>
#include <boost/integer/detail/bitfield_vector/bitfield_vector_base.hpp>
#include <boost/integer/detail/bitfield_vector/bitfield_vector_member_impl.hpp>
#include <boost/integer/detail/bitfield_vector/iterator_base.hpp>

namespace boost {

// template <typename,typename>
// class vector;

template <typename T,std::size_t>
struct bits;

/** Iterators. */
//@{
template<typename T, std::size_t Width>
class bf_vector_iterator
    :protected detail::bitfield_vector_iterator_base<T,Width>,
    public detail::safe_bool_impl::safe_bool<
        detail::bitfield_vector_iterator_base<T,Width>
    >
{
    typedef detail::bitfield_vector_iterator_base<T,Width>  _base;
    typedef detail::safe_bool_impl::safe_bool< _base >      _safe_bool;
    typedef bf_vector_iterator<T,Width>                     _self;
public:

    typedef typename _base::proxy_ref_type          proxy_ref_type;
    typedef typename _base::const_proxy_ref_type    const_proxy_ref_type;
    typedef typename _base::iterator_category       iterator_category;
    typedef typename _base::value_type              value_type;
    typedef typename _base::pointer                 pointer;
    typedef typename _base::proxy_ref_type          reference;
    typedef typename _base::difference_type         difference_type;


    bf_vector_iterator()
        :_base(),
        _safe_bool()
    { }

    bf_vector_iterator(_self const& rhs)
        :_base( static_cast<_base>(rhs) ),
        _safe_bool()
    { }

    explicit bf_vector_iterator(reference const& x)
        :_base( static_cast<_base>(x) ),
        _safe_bool()
    { }

    reference operator*() const {
        return this->deref();
    }

    _self& operator++() {
        this->next();
        return *this;
    }

    _self operator++(int) {
        _self ret(*this);
        this->next();
        return ret;
    }

    _self& operator--() {
        this->previous();
        return *this;
    }

    _self operator--(int) {
        _self ret(*this);
        this->previous();
        return ret;
    }
    
    bool operator==(_self const& rhs) const {
        return this->is_equal(rhs);
    }

    bool operator!=(_self const& rhs) const {
        return !this->is_equal(rhs);
    }

};

template<typename T, std::size_t Width>
class const_bf_vector_iterator
    :protected detail::bitfield_vector_iterator_base<T,Width>,
    public detail::safe_bool_impl::safe_bool<
        detail::bitfield_vector_iterator_base<T,Width>
    >
{ };

template<typename T, std::size_t Width>
class bf_vector_reverse_iterator
    :protected detail::bitfield_vector_iterator_base<T,Width>,
    public detail::safe_bool_impl::safe_bool<
        detail::bitfield_vector_iterator_base<T,Width>
    >
{ };

template<typename T, std::size_t Width>
class const_bf_vector_reverse_iterator
    :protected detail::bitfield_vector_iterator_base<T,Width>,
    public detail::safe_bool_impl::safe_bool<
        detail::bitfield_vector_iterator_base<T,Width>
    >
{ };
//@}


template <  typename T,
            std::size_t Width,
            typename Allocator = std::allocator<unsigned char>
>
class bitfield_vector
    :protected detail::bitfield_vector_base<T,Allocator>
{
    typedef bitfield_vector< T,Width, Allocator >  _self;
    typedef detail::bitfield_vector_base<T,Allocator> _base;

public:
    typedef bf_vector_iterator<T,Width>                 iterator;
    typedef const_bf_vector_iterator<T,Width>           const_iterator;
    typedef bf_vector_reverse_iterator<T,Width>         reverse_iterator;
    typedef const_bf_vector_reverse_iterator<T,Width>   const_reverse_iterator;
    typedef T                                           value_type;
    typedef Allocator                                   allocator_type;
    typedef std::size_t                                 size_type;
    typedef std::ptrdiff_t                              difference_type;
    typedef typename _base::storage_type*               pointer;
    typedef typename _base::storage_type const*         const_pointer;
    
    class reference {
        friend class bitfield_vector;
        reference();
    public:
        ~reference();
        operator value_type() const;
        reference& operator= (value_type const&  x);
        reference& operator= (reference const & x);
        void flip();
    };

    class const_reference {
        friend class bitfield_vector;
        const_reference();
    public:
        ~const_reference();
        operator value_type () const;
        // const_reference& operator=(const value_type x);
        const_reference& operator=(const_reference const& x);
        void flip();
    };


    explicit bitfield_vector(Allocator const& alloc = Allocator() )
        :_base(alloc)
    { }

    explicit bitfield_vector(size_type n, T const& value= T(),
        Allocator const& = Allocator() );
    template <class InputIterator>
    bitfield_vector(InputIterator first, InputIterator last,
        Allocator const& = Allocator());
    bitfield_vector(_self const& x );
    ~bitfield_vector();

    _self& operator=(_self const& x);

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

    void assign(size_type n, value_type const& u);
    void push_back(value_type const& x);
    void pop_back();
    iterator insert(iterator position, value_type const& x);
    void insert(iterator position, size_type n, value_type const& x);

    template <class InputIterator>
    void insert(iterator position, InputIterator first, InputIterator last);

    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);
    void swap(_self& vec);
    void clear();
    allocator_type get_allocator() const;

protected:
    void check_for_resizing() {
        size_type size_of_alloc = (this->_impl._end - this->_impl._start);
        difference_type remaing_bits = ((size_of_alloc*8) - this->_impl._bits_in_use);
        if(remaing_bits < Width) {
            std::size_t next_allocation_size =
                detail::next_allocation_size<Width>()(
                    size_of_alloc,
                    this->_impl._bits_in_use
                );
            pointer ptr = this->allocate_impl( next_allocation_size );

            std::memcpy(static_cast<void*>(ptr),
                        static_cast<void*>(this->_impl._start),
                        size_of_alloc);
            this->deallocate_impl(this->_impl._start, size_of_alloc);
            this->_impl._start = ptr;
            this->_impl._end = ptr + next_allocation_size;
        }
    }
    
};

} // end boost

#endif
