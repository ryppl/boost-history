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
#include <algorithm>
#include <iterator>
#include <limits>
#include <stdexcept>

namespace boost {

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

    _self const& operator=(_self const& rhs) {
        this->assign(static_cast<_base>(rhs));
        return *this;
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

    _self const& operator=(_self const& rhs) {
        this->assign(static_cast<_base>(rhs));
        return *this;
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

    _self const& operator=(_self const& rhs) {
        this->assign(static_cast<_base>(rhs));
        return *this;
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

    _self const& operator=(_self const& rhs) {
        this->assign(static_cast<_base>(rhs));
        return *this;
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

/// TODO: Fix reverse iterator!!!!!!!
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

    /** Constructable over an allocator and default constructor. */
    explicit bitfield_vector(Allocator const& alloc = Allocator() )
        :_base(alloc)
    { }

    /** fill constructor
     *  Fills the vector with N items with value of value.
     *  Throws: std::bad_alloc (or anything which is thrown out of
     *  the allocators allocate function).
     */
    explicit bitfield_vector(size_type n,
                             value_type const& value = T(),
                             allocator_type const& alloc = Allocator() )
        :_base(alloc)
    {
        allocate_and_fill(n,value);
    }


    template <class InputIterator>
    bitfield_vector(InputIterator first, InputIterator last,
        Allocator const& = Allocator());

    bitfield_vector(_self const& x );

    /** This calls a the base classes destructor and 
     *  that frees all of the memory.
     */
    ~bitfield_vector() { }

    _self& operator=(_self const& x) {
    }

    /** Begin and end and all variations of begin and end for all iterator
     *  types supported by this data structure.
     */
    //@{
    iterator                begin() {
        return iterator(this->m_impl.m_start,0);
    }

    const_iterator          begin() const {
        return const_iterator(this->m_impl.m_start,0);
    }

    iterator                end() {
        return begin() + (this->m_impl.m_bits_in_use / Width);
    }

    const_iterator          end() const {
        return begin() + (this->m_impl.m_bits_in_use / Width);
    }

    reverse_iterator        rbegin() {
        return reverse_iterator(this->m_impl.m_start +
            (this->m_impl.m_bits_in_use/CHAR_BIT),
            this->m_impl.m_bits_in_use%CHAR_BIT);
    }

    const_reverse_iterator  rbegin() const {
        return const_reverse_iterator(this->m_impl.m_start +
            (this->m_impl.m_bits_in_use/CHAR_BIT),
            this->m_impl.m_bits_in_use%CHAR_BIT);
    }

    reverse_iterator        rend() {
        return rbegin() + (this->m_impl.m_bits_in_use / Width);
    }

    const_reverse_iterator  rend() const {
        return rbegin() + (this->m_impl.m_bits_in_use / Width);
    }

    const_iterator          cbegin() const {
        return begin();
    }

    const_iterator          cend() const {
        return end();
    }

    const_reverse_iterator  crbegin() const {
        return rbegin();
    }

    const_reverse_iterator  crend() const {
        return rend();
    }
    //@}

    /** Direct member access to front and back for both const and non const 
     *  access.
     */
    //@{
    reference   front() {
        return *begin();
    }

    reference   back() {
        return *(--end());
    }

    const_reference front()  const {
        return *begin();
    }

    const_reference back()   const {
        return *(--end());
    }
    //@}


    /** size and Max size.
     *  size - Returns the number of elements within the bitfield_vector.
     *  max_size - Returns the maximum_nu
     */
    size_type size() const {
        return this->m_impl.m_bits_in_use/Width;
    }

    /** This is being defined and the number of bitfields which can be put into
     *  the largest character array.
     */
    size_type max_size() const {
        typedef typename mpl::divides<
            typename mpl::times<
                mpl::integral_c<
                    size_type,
                    (~size_type(0))
                >,
                mpl::size_t<CHAR_BIT>
            >::type,
            mpl::size_t<Width>
        >::type             result;
        return result::value;
    }

    /** Returns the maximum number of elements which could be placed into 
     *  the currently allocated memory.
     */
    size_type capacity() const {
        return ((this->m_impl.m_end - this->m_impl.m_start) * CHAR_BIT) / Width;
    }
    
    /** Returns true if there are no elements in the container. */
    bool empty() const {
        return !(this->m_impl.m_bits_in_use > 0);
    }


    /** vector swap function.
     *  swaps two bitfield_vectors.
     */
    void swap(_self& vec) {
        using namespace std;
        swap(this->m_impl.m_start, vec.m_impl.m_start);
        swap(this->m_impl.m_end, vec.m_impl.m_end);
        swap(this->m_impl.m_bits_in_use, vec.m_impl.m_bits_in_use);
    }

    /** Returns the current allocator the implementation of this function is
     *  inside of the base class for bitfield_vector because the base class is
     *  responsible for working with the allocator directely.
     */
    allocator_type get_allocator() const {
        return this->_base::get_allocator();
    }

    /** Because all bitfields are trivially destructable
     *  I'm skipping the part where I attempt to call
     *  the destructor and instead simply setting the number of bit in use to 0
     *  is the same a clear. Also notice that this really can't be used to call
     *  the destructor because to do so I would need to take its address which
     *  I can't.
     */
    void clear() {
        this->m_impl.m_bits_in_use = 0;
    }

    /** Reserves a section of memory which can be used to hold equal to or 
     *  greater then n elements.
     */
    void reserve(size_type n) {
        if(n > max_size())
            throw std::length_error();

        if(capacity() >= n )
            return;

        // size_type elements_allocated = this->m_impl.m_bits_in_use/Width;
        // size_type next_size_in_bits = n * Width;
        size_type next_size = 1;
        // find correct power of 2 for holding n elements
        while(n > next_size)
            next_size *= 2;

        // calculate number of bits needed for next allocation.
        next_size *= Width;

        // calculate the number of bytes needed to hold the elements.
        next_size += (next_size/CHAR_BIT) + size_type((next_size%CHAR_BIT) > 0);


        // preform memory allocation and make sure to correctly handle
        // the exception.
        detail::storage_ptr_t old_storage = this->m_impl.m_start;
        // if allocate_impl throws the assignment will never complete.
        this->m_impl.m_start = this->allocate_impl(next_size);
        // copy to new memory
        std::memcpy(old_storage,
            this->m_impl.m_start,
            this->m_impl.m_end - old_storage);

        // deallocate old memory
        this->deallocate_impl(old_storage,this->m_impl.m_end - old_storage);

        // set the end to the new end.
        this->m_impl.m_end = this->m_impl.m_start + next_size;
    }



    /** Resize to a size given in elements. */
    void resize(size_type sz, value_type c = value_type() ) {
        size_type next_size_in_bits = Width * sz;
        
        // fewer elements than needed.
        if(next_size_in_bits <= this->m_impl.m_bits_in_use ) {
            this->m_impl.m_bits_in_use = next_size_in_bits;
            return;
        }

        // not enough space
        if(next_size_in_bits > ((this->m_impl.m_end - this->m_impl.m_start) * Width)) {
            // while(
        }
        
    }
    reference operator[](size_type n);    
    const_reference operator[](size_type n) const;
    reference at(size_type n);
    const_reference at(size_type n) const;


    template <class InputIterator>
    void assign(InputIterator first, InputIterator last);
    void assign(size_type n, value_type const& u);

    /**
     *
     */
    void push_back(value_type const& x) {
        check_for_resizing();
        iterator iter = end();
        *iter = x;
        this->m_impl.m_bits_in_use += Width;
    }
    void pop_back();
    iterator insert(iterator position, value_type const& x);
    void insert(iterator position, size_type n, value_type const& x);

    template <class InputIterator>
    void insert(iterator position, InputIterator first, InputIterator last);

    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);


protected:




    /** allocates a chunck of memory of the correct size and then
     *  correctly fills that memory with value for each of the
     *  bitfields within it.
     */
    void allocate_and_fill(size_type n, value_type value) {

        // figure out exactly how many bits are going to be needed.
        size_type min_allocation_size = n * Width;

        // calculate that size in bytes.
        min_allocation_size = (min_allocation_size/CHAR_BIT)
            + size_type((min_allocation_size%CHAR_BIT)!=0);


        // calculate the correct size of the allocation
        // by getting the closest power of 2 greater then the number of bytes
        // needed to stor n bitfields in.
        size_type corrected_allocation_size = 1;
        while(corrected_allocation_size < min_allocation_size) {
            corrected_allocation_size *= 2;
        }

        this->m_impl.m_start = this->allocate_impl(corrected_allocation_size);
        this->m_impl.m_end = this->m_impl.m_start + corrected_allocation_size;

        // once allocation is completed next comes filling the bitfields
        // with val.
        this->m_impl.m_bits_in_use = n*Width;
        using namespace std;
        fill(begin(),end(),value);
    }

    void check_for_resizing() {
        size_type size_of_alloc = (this->m_impl.m_end - this->m_impl.m_start);
        difference_type remaing_bits = ((size_of_alloc*CHAR_BIT) -
            this->m_impl.m_bits_in_use);
        if(size_type(remaing_bits) < Width) {
            std::size_t next_allocation_size =
                detail::next_allocation_size<Width>()(
                    size_of_alloc, this->m_impl.m_bits_in_use);

            pointer ptr = this->allocate_impl( next_allocation_size );

            std::memcpy(static_cast<void*>(ptr),
                        static_cast<void*>(this->m_impl.m_start),
                        size_of_alloc);
            this->deallocate_impl(this->m_impl.m_start, size_of_alloc);
            this->m_impl.m_start = ptr;
            this->m_impl.m_end = ptr + next_allocation_size;
        }
    }
    
};

} // end boost

#endif
