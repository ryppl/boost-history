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


namespace detail {
/** Iterators. */
//@{
template<typename T, std::size_t Width>
class bf_vector_iterator
    :protected detail::bitfield_vector_iterator_base<T,Width>
{
    typedef detail::bitfield_vector_iterator_base<T,Width>  _base;
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
        :_base()
    { }

    bf_vector_iterator(_self const& rhs)
        :_base( static_cast<_base>(rhs) )
    { }

    explicit bf_vector_iterator(reference const& x)
        :_base( static_cast<proxy_ref_type>(x) )
    { }

    bf_vector_iterator(storage_ptr_t ptr, std::size_t offset)
        :_base(ptr, offset)
    { }

    using _base::operator typename _base::bool_type;

    _self operator=(_self const& rhs) {
        this->assign(static_cast<_base>(rhs));
    }

    reference operator*() const {
        return this->deref();
    }

    reference operator[](intmax_t index) const {
        _self ret(*this);
        ret.advance(index);
        return ret.deref();
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

    _self& operator+=(int rhs) {
        this->advance(rhs);
        return *this;
    }

    _self& operator-=(int rhs) {
        this->advance(rhs * -1);
        return *this;
    }

    _self operator+(int rhs) const {
        _self ret(*this);
        ret.advance(rhs);
        return ret;
    }

    _self operator-(int rhs) const {
        _self ret(*this);
        ret.advance(rhs * -1);
        return ret;
    }

    difference_type operator-(_self const& rhs) const {
        return this->distance( static_cast<_base>(rhs) );
    }
        
    bool operator==(_self const& rhs) const {
        return this->is_equal(rhs);
    }

    bool operator!=(_self const& rhs) const {
        return !this->is_equal(rhs);
    }

    bool operator<(_self const& rhs) const {
        return this->is_less(static_cast<_base>(rhs));
    }
};

template<typename T, std::size_t Width>
class const_bf_vector_iterator
    :protected detail::bitfield_vector_iterator_base<T,Width>
{
    typedef detail::bitfield_vector_iterator_base<T,Width>  _base;
    typedef const_bf_vector_iterator<T,Width>               _self;
public:

    typedef typename _base::proxy_ref_type          proxy_ref_type;
    typedef typename _base::const_proxy_ref_type    const_proxy_ref_type;
    typedef typename _base::iterator_category       iterator_category;
    typedef typename _base::value_type              value_type;
    typedef typename _base::pointer                 pointer;
    typedef const_proxy_ref_type                    reference;
    typedef typename _base::difference_type         difference_type;

    using _base::operator typename _base::bool_type;

    const_bf_vector_iterator()
        :_base()
    { }

    const_bf_vector_iterator(_self const& rhs)
        :_base( static_cast<_base>(rhs) )
    { }

    explicit const_bf_vector_iterator(reference const& x)
        :_base( static_cast<proxy_ref_type>(x) )
    { }

    const_bf_vector_iterator(storage_ptr_t ptr, std::size_t offset)
        :_base(ptr, offset)
    { }

    reference operator*() const {
        return this->const_deref();
    }

    reference operator[](intmax_t index) const {
        _self ret(*this);
        ret.advance(index);
        return ret.const_deref();
    }

    _self operator=(_self const& rhs) {
        this->assign(static_cast<_base>(rhs));
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

    _self& operator+=(int rhs) {
        this->advance(rhs);
        return *this;
    }

    _self& operator-=(int rhs) {
        this->advance(rhs * -1);
        return *this;
    }

    _self operator+(int rhs) const {
        _self ret(*this);
        ret.advance(rhs);
        return ret;
    }

    _self operator-(int rhs) const {
        _self ret(*this);
        ret.advance(rhs * -1);
        return ret;
    }

    difference_type operator-(_self const& rhs) const {
        return this->distance(static_cast<_base>(rhs) );
    }
    
    bool operator==(_self const& rhs) const {
        return this->is_equal(rhs);
    }

    bool operator!=(_self const& rhs) const {
        return !this->is_equal(rhs);
    }

    bool operator<(_self const& rhs) const {
        return this->is_less(static_cast<_base>(rhs));
    }
};

template<typename T, std::size_t Width>
class bf_vector_reverse_iterator
    :protected detail::bitfield_vector_iterator_base<T,Width>
{
    typedef detail::bitfield_vector_iterator_base<T,Width>  _base;
    typedef bf_vector_reverse_iterator<T,Width>             _self;
public:

    typedef typename _base::proxy_ref_type          proxy_ref_type;
    typedef typename _base::const_proxy_ref_type    const_proxy_ref_type;
    typedef typename _base::iterator_category       iterator_category;
    typedef typename _base::value_type              value_type;
    typedef typename _base::pointer                 pointer;
    typedef typename _base::proxy_ref_type          reference;
    typedef typename _base::difference_type         difference_type;

    using _base::operator typename _base::bool_type;

    bf_vector_reverse_iterator()
        :_base()
    { }

    bf_vector_reverse_iterator(storage_ptr_t ptr, std::size_t offset)
        :_base(ptr, offset)
    { }

    bf_vector_reverse_iterator(_self const& rhs)
        :_base( static_cast<_base>(rhs) )
    { }

    explicit bf_vector_reverse_iterator(reference const& x)
        :_base( static_cast<proxy_ref_type>(x) )
    { }

    reference operator*() const {
        return this->deref();
    }

    reference operator[](intmax_t index) const {
        _self ret(*this);
        ret.advance( index * -1);
        return ret.deref();
    }

    _self operator=(_self const& rhs) {
        this->assign(static_cast<_base>(rhs));
    }

    _self& operator++() {
        this->previous();
        return *this;
    }

    _self operator++(int) {
        _self ret(*this);
        this->previous();
        return ret;
    }

    _self& operator--() {
        this->next();
        return *this;
    }

    _self operator--(int) {
        _self ret(*this);
        this->next();
        return ret;
    }

    _self& operator+=(int rhs) {
        this->advance(rhs * -1);
        return *this;
    }

    _self& operator-=(int rhs) {
        this->advance(rhs);
        return *this;
    }

    _self operator+(int rhs) const {
        _self ret(*this);
        ret.advance(rhs * -1);
        return ret;
    }

    _self operator-(int rhs) const {
        _self ret(*this);
        ret.advance(rhs);
        return ret;
    }

    difference_type operator-(_self const& rhs) const {
        return rhs.distance( static_cast<_base>(*this) );
    }
    
    bool operator==(_self const& rhs) const {
        return this->is_equal(rhs);
    }

    bool operator!=(_self const& rhs) const {
        return !this->is_equal(rhs);
    }

    bool operator<(_self const& rhs) const {
        return rhs.is_greater(static_cast<_base>(*this));
    }
};

template<typename T, std::size_t Width>
class const_bf_vector_reverse_iterator
    :protected detail::bitfield_vector_iterator_base<T,Width>
{
    typedef detail::bitfield_vector_iterator_base<T,Width>  _base;
    typedef const_bf_vector_reverse_iterator<T,Width>       _self;
public:

    typedef typename _base::proxy_ref_type          proxy_ref_type;
    typedef typename _base::const_proxy_ref_type    const_proxy_ref_type;
    typedef typename _base::iterator_category       iterator_category;
    typedef typename _base::value_type              value_type;
    typedef typename _base::pointer                 pointer;
    typedef const_proxy_ref_type                    reference;
    typedef typename _base::difference_type         difference_type;

    using _base::operator typename _base::bool_type;

    const_bf_vector_reverse_iterator()
        :_base()
    { }

    const_bf_vector_reverse_iterator(_self const& rhs)
        :_base( static_cast<_base>(rhs) )
    { }

    const_bf_vector_reverse_iterator(storage_ptr_t ptr, std::size_t offset)
        :_base(ptr, offset)
    { }

    explicit const_bf_vector_reverse_iterator(reference const& x)
        :_base( static_cast<proxy_ref_type>(x) )
    { }

    reference operator*() const {
        return this->const_deref();
    }

    reference operator[](intmax_t index) const {
        _self ret(*this);
        ret.advance(index * -1);
        return ret.const_deref();
    }

    _self operator=(_self const& rhs) {
        this->assign(static_cast<_base>(rhs));
    }

    _self& operator++() {
        this->previous();
        return *this;
    }

    _self operator++(int) {
        _self ret(*this);
        this->previous();
        return ret;
    }

    _self& operator--() {
        this->next();
        return *this;
    }

    _self operator--(int) {
        _self ret(*this);
        this->next();
        return ret;
    }

    _self& operator+=(int rhs) {
        this->advance(rhs * -1);
        return *this;
    }

    _self& operator-=(int rhs) {
        this->advance(rhs);
        return *this;
    }

    _self operator+(int rhs) const {
        _self ret(*this);
        ret.advance(rhs * -1);
        return ret;
    }

    _self operator-(int rhs) const {
        _self ret(*this);
        ret.advance(rhs);
        return ret;
    }

    difference_type operator-(_self const& rhs) const {
        return rhs.distance( static_cast<_base>(*this) );
    }
    
    bool operator==(_self const& rhs) const {
        return this->is_equal(rhs);
    }

    bool operator!=(_self const& rhs) const {
        return !this->is_equal(rhs);
    }

    bool operator<(_self const& rhs) const {
        return rhs.is_greater(static_cast<_base>(*this));
    }
};

} // end detail

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
    typedef detail::bf_vector_iterator<T,Width>
        iterator;
    typedef detail::const_bf_vector_iterator<T,Width>
        const_iterator;
    typedef detail::bf_vector_reverse_iterator<T,Width>
        reverse_iterator;
    typedef detail::const_bf_vector_reverse_iterator<T,Width>
        const_reverse_iterator;

    typedef T                                           value_type;
    typedef Allocator                                   allocator_type;
    typedef std::size_t                                 size_type;
    typedef std::ptrdiff_t                              difference_type;
    typedef typename _base::storage_type*               pointer;
    typedef typename _base::storage_type const*         const_pointer;
    typedef detail::proxy_reference_type<value_type,Width>
        reference;
    typedef detail::const_proxy_reference_type<value_type,Width>
        const_reference;

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

    /** Begin and end and all variations of begin and end for all iterator
     *  types supported by this data structure.
     */
    //@{
    iterator                begin();
    const_iterator          begin() const;

    iterator                end();
    const_iterator          end() const;

    reverse_iterator        rbegin();
    const_reverse_iterator  rbegin() const;

    reverse_iterator        rend();
    const_reverse_iterator  rend() const;

    const_iterator          cbegin() const;
    const_iterator          cend() const;

    const_reverse_iterator  crbegin() const;
    const_reverse_iterator  crend() const;
    //@}




    /** Direct member access to front and back for both const and non const 
     *  types.
     */
    //@{
    reference   front();
    reference   back();
    const_reference front()  const;
    const_reference back()   const;
    //@}


    size_type size() const;
    size_type max_size() const;


    void resize(size_type sz, value_type c = value_type() );    
    size_type capacity() const;
    bool empty() const;
    void reserve(size_type n);
    reference operator[](size_type n);
    const_reference operator[](size_type n) const;
    reference at(size_type n);
    const_reference at(size_type n) const;


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
