// op_equal.hpp

// Boost Logging library
//
// Author: John Torjo, www.torjo.com
//
// Copyright (C) 2007 John Torjo (see www.torjo.com for email)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.
 
// See http://www.boost.org for updates, documentation, and revision history.
// See http://www.torjo.com/log2/ for more details

#ifndef JT28092007_op_equal_HPP_DEFINED
#define JT28092007_op_equal_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>
#include <vector>

namespace boost { namespace logging { 

/** 
    @brief Different ways to implement operator==

    You need this when:
    - your logger uses Formatters and Destinations, and
    - you've added formatters and/or destinations to your logger
    - at a later time, you want to erase a certain formatter and/or destination from your logger

    If so, we need a way to identify the formatter/destination to be erased.
*/
namespace op_equal {



    /** 
        @brief A unique ID is given to each default-constructed object. If object A is copy-constructed from B, or copy-assigned from B, it will "inherit" its ID.

        A lot of times this is usually enough. Since formatters/destinations either have const context data,
        or a shared context (@ref boost::logging::manipulator::non_const_context "non_const_context class"), this is enough.
    */
    struct unique_id {
    protected:
        unique_id() : m_id( next_id() ) {}

    public:
        bool are_unique_ids_equal(const unique_id & other) const { return m_id == other.m_id; }
    private:
        static int next_id() {
            static threading::mutex cs;
            static int id = 0;
            threading::scoped_lock lk(cs);
            return ++id;
        }

        int m_id;
    };
    inline bool operator==(const unique_id& a, const unique_id & b) { return a.are_unique_ids_equal(b); }
    namespace detail { namespace {
        struct make_sure_unique_id_next_id_is_initialized {
            struct use_unique_id : unique_id {};

            make_sure_unique_id_next_id_is_initialized() {
                // trick the compiler to make sure the statics in next_id() are initialized before main()!
                use_unique_id a, b;
                if ( a == b)
                    std::vector<int> v;
            }
        };
    }}



    /** 
        @brief Implements operator==, which compares two objects. If they have the same type, it will return true

        For trivial @ref manipulator "manipulators", that don't have any context, this is usually enough.

        If your @ref manipulator "manipulators" have context, you should either use non_const_context class, or same_type_op_equal class.
    */
    struct same_type {
    protected:
        same_type() {}
        same_type(const same_type&) {}
    public:
        inline bool operator ==(const same_type& other) const { return typeid(*this) == typeid(other); }
    };





    struct same_type_op_equal_top {
        /*
            if you get a compile time error here, your formatter or destination needs to derive 
            from same_type_op_equal<your_type>, and implement operator== as a member function , in your class
        */
        virtual bool equals(const same_type_op_equal_top &) const = 0;
    protected:
        same_type_op_equal_top() {}
        same_type_op_equal_top(const same_type_op_equal_top&) {}
    };
    inline bool operator ==(const same_type_op_equal_top& a, const same_type_op_equal_top&b) { return a.equals(b); }

    /** 
        @brief Base class when you want to implement operator== that will compare based on type and member operator==

        @sa same_type_op_equal
    */
    struct same_type_op_equal_base : virtual same_type_op_equal_top {};


    /** 
        @brief Implements operator==, which compares two objects. If they have the same type, it will compare them using the type's member operator==.

        The only constraint is that operator== must be a *member* function
    */
    template<class type> struct same_type_op_equal : same_type_op_equal_base {

        virtual bool equals(const same_type_op_equal_top & other) const {
            if ( typeid(*this) != typeid(other))
                return false;
            const type & real_other = reinterpret_cast<const type&>(other);

            // this forces 'type' to implement operator==
            return ((const type&)*this).operator ==( real_other);
        }
    };

}}}

#endif

