//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_STACK_ALLOC_POLICY_HPP
#define BOOST_BITFIELD_TUPLE_STACK_ALLOC_POLICY_HPP

namespace boost { namespace details {

/** This structure is going to be used when an allocator isn't assked for.
 *  This means that the entire class is allocated on the stack.
 */
template <typename T>
struct stack_alloc_base_policy {

    /** Default Constructor. */
    stack_alloc_base_policy()
        :_data()
    { }

    /** Copy constructor. */
    stack_alloc_base_policy( stack_alloc_base_policy<T> const& x )
        :_data( x._data )
    { }
    
    /** Value constructor. */
    stack_alloc_base_policy( T x )
        :_data( x )
    { }
    
    stack_alloc_base_policy<T> const& operator=( stack_alloc_base_policy const& x ) {
        _data  =  x._data;
        return *this;
    }

    /** get_data () this function exists within this class and the 
     *      allocator_base_policy.
     *  Returns a copy of _data.
     */
    //@{
    T const& get_data() const { return _data; }
    T& get_data() { return _data; }
    //@}

    T _data;
};

}} // end boost::details

#endif
