//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_VECTOR_BASE_HPP
#define BOOST_BITFIELD_VECTOR_BASE_HPP
#include <cstddef>

namespace boost { namespace detail {

template <typename T, typename Allocator>
struct bitfield_vector_base {
    typedef T value_type;
    typedef typename Allocator::template rebind<unsigned char>::other
        rebound_alloc_type;
    typedef Allocator allocator;

    /** This is the structure which gains the benifit of EBO and holds
     *  pointers to the start and end of the memory allocated as well
     *  as keeps track of the number of bits in use within that allocated
     *  memory.
     */
    struct bitfield_vector_impl
        : rebound_alloc_type
    {
        bitfield_vector_impl()
            :rebound_alloc_type(),
            _start(0),
            _end(0),
            _bits_in_use(0)
        { }

        bitfield_vector_impl(const rebound_alloc_type& alloc)
            :rebound_alloc_type(alloc),
            _start(0),
            _end(0),
            _bits_in_use(0)
        { }

        typename rebound_alloc_type::pointer    _start;
        typename rebound_alloc_type::pointer    _end;
        typename rebound_alloc_type::size_type  _bits_in_use;
    };

    /** This is an instance of the bitfield_vector_impl type. */
    bitfield_vector_impl _impl;

    /** Allocator retrieval implementation functions. */
    //@{
    rebound_alloc_type& get_allocator_impl() {
        return *static_cast<rebound_alloc_type*>(&this->_impl);
    }

    rebound_alloc_type const& get_allocator_impl() const {
        return *static_cast<rebound_alloc_type const*>(&this->_impl);
    }
    //@}

    /** Returns a copy of the allocator.*/
    allocator get_allocator() const {
        return allocator(get_allocator_impl());
    }

    /** Default constructor for vector base. */
    bitfield_vector_base()
        :_impl()
    { }

    /** Constructor over an allocator. */
    bitfield_vector_base(allocator const& alloc)
        :_impl(alloc)
    { }

    /** Array Constructor. */
    bitfield_vector_base(std::size_t n)
        :_impl()
    {
        this->_impl._start = this->allocate_impl(n);
        this->_impl._end = this->_impl._start + n;
    }

    /** Array + Allocator Constructor. */
    bitfield_vector_base(std::size_t n, allocator const& alloc)
        :_impl(alloc)
    {
        this->_impl._start = this->allocate_impl(n);
        this->_impl._end = this->_impl._start + n;
    }
    
    /** Destructor. */
    ~bitfield_vector_base() {
        deallocate_impl(this->_impl._start, this->_impl._end
            - this->_impl._start);
    }



    typename rebound_alloc_type::pointer allocate_impl(std::size_t n) {
        return n != 0 ? _impl.allocate( n ): 0;
    }

    void
    deallocate_impl(typename rebound_alloc_type::pointer ptr, std::size_t n) {
        if(ptr) {
            _impl.deallocate(ptr,n);
        }
    }
};

}} // end boost::detail

#endif
