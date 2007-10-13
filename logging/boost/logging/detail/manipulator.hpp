// manipulator.hpp

// Boost Logging library
//
// Author: John Torjo, www.torjo.com
//
// Copyright (C) 2007 John Torjo (see www.torjo.com for email)
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.
// See http://www.torjo.com/log2/ for more details


#ifndef JT28092007_manipulator_HPP_DEFINED
#define JT28092007_manipulator_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>

namespace boost { namespace logging {


/** 
@brief Manipulators = Formatters and/or destinations.

All formatters need to derive from a <b>common %base class</b>. Same goes for destinations.

Remember:
- formatter - allows formatting the message before writing it (like, prepending extra information - an index, the time, thread id, etc)
- destination - is a place where the message is to be written to (like, the console, a file, a socket, etc)

In your @ref boost::logging::writer::format_write "format_write" object, you can have several formatters and destinations. Note that each formatter class and each destination class is a @c %manipulator.



When dealing with formatters, the first thing you must do is to specify that common %base class.
All formatter classes will derive from it, directly or indirectly. Again, same goes for destinations.


Note that the %formatter %base class and the %destination %base class don't need to be the same. Example:

@code
typedef optimize::cache_string_one_str<> cache_string;
typedef formatter::base<cache_string&, op_equal::same_type > format_base;


struct write_time : format_base {
    void operator()(cache_string & str) const {
        char t[10];
        time_t now = time(0);
        tm details = *localtime(&now);
        strftime(t, 10, "%M:%S ", &details);
        str.prepend_string(t);
    }
};
//...

typedef destination::base<const std::string &, op_equal::same_type> destination_base;
struct write_to_cout : destination_base {
    void operator()(param msg) const {
        std::cout << msg ;
    }
};

@endcode

This namespace contains the classes formatter or destination classes need to use as %base class.



@section manipulator_base Manipulator base classes

Your formatter or destination class should derive from either of the following:
- base - recommented %base class 
- base_no_operator_call - %base class , not implementing the operator() call




@section share_data Sharing data for manipulator classes

The manipulator classes need to contain data as constant:

@code
struct my_formatter : format_base {
    ...
private:
    const int m_idx;
};
@endcode

As long as data is constant, it's all ok - that is, no matter what functions get called, all the data in the formatter/destination
must remain constant. We need constant functors - just like in STL - because internally, we copy formatters/destinations: that is, we keep
several copies of a certain object - they all need to be syncronized. In case the objects' data is constant, that's no problem.

In case the data needs to be changed - it needs to be shared. Several copies of the same instance must point to the same data. In other words,
use shared pointers:

@code
struct write_to_file {
    typedef boost::shared_ptr<std::ofstream> ptr;
    write_to_file(const std::string & filename) : m_out(new std::ofstream(filename.c_str())) {}
    void operator()(const std::string & msg) const {
        (*m_out) << msg << std::endl ;
    }
    mutable ptr m_out;
};
@endcode

Since quite a few of the manipulator objects will need this, it's quite acceptable to have a class that deals with this:
the non_const_context class.


*/
namespace manipulator {

    namespace detail {
        struct default_type {};
        template<class type_> struct ptr_finder { 
            template<class other_type> struct find { typedef other_type type ; };            
        };
        template<> struct ptr_finder<default_type> {
            template<class other_type> struct find { typedef other_type* type ; };
        };
    }

/** 
Formatters and/or destinations are Manipulators.
    
All formatter or destination class need to directly or indirectly derive from this.

All formatters need to derive from a common %base class.
When dealing with formatters, the first thing you must do is to specify that common base class.
All formatter classes will derive from it, directly or indirectly.

Same goes for destinations.

Note that the formatter base class and the destination base class don't need to be the same.

You should derive your formatter or destination class from this, ONLY if you want to provide 
your own operator() which could take more than one argument. Otherwise, derive from base.


@section implementing_op_equal_ Implementing operator==

Formatter objects must be able to be compared. Same goes for destination objects. This is especially 
needed when a formatter or destination is to be erased - from a @ref boost::logging::writer::format_write "format_write" object.

There are multiple ways to be able to compare formatter/destination objects - see op_equal namespace.

By default, @ref boost::logging::op_equal::same_type_op_equal_base is used.




@param implement_op_equal see @ref implementing_op_equal_ 

@param ptr_type_ the type used to hold a pointer to the type. By defaut, it's the type itself.

*/
template<
        class implement_op_equal = op_equal::same_type_op_equal_base,
        class ptr_type_ = detail::default_type > 
            struct base_no_operator_call : implement_op_equal {

    typedef base_no_operator_call<implement_op_equal, ptr_type_> self_type;
    typedef typename detail::ptr_finder<ptr_type_> type_finder;
    typedef typename type_finder::find<self_type>::type ptr_type;

    virtual ~base_no_operator_call() {}
};


/** 
    Helper - in case your formatter or destination, in its operator(), it has only one argument
    (which is most of the time, unless you're doing something extreme ;)), you should derive from this.


    Examples:
    @code
    // takes a "string&" as argument:
    struct append_enter : single_param_base<std::string&> {
        // param - typedef defined in base class; param = std::string&
        void operator()(param a) {
            a += "\n";
        }
    }

    // takes a "const std::string&" as argument
    struct write_to_cout : single_param_base<const std::string&> {
        void operator()(param msg) const {
            std::cout << msg << std::endl ;
        }
    };
    @endcode


@param implement_op_equal see @ref implementing_op_equal_ 

*/
template<
        class arg_type, 
        class implement_op_equal = op_equal::same_type_op_equal_base,
        class ptr_type_ = detail::default_type > 
            struct base : base_no_operator_call<implement_op_equal, ptr_type_> {

    typedef base<arg_type, implement_op_equal, ptr_type_> self_type;
    typedef typename detail::ptr_finder<ptr_type_> type_finder;
    typedef typename type_finder::find<self_type>::type ptr_type;

    typedef arg_type param;

    typedef typename boost::remove_const<param>::type non_const_param;
    // used as msg_type in format_and_write classes
    typedef typename boost::remove_reference<non_const_param> raw_param;

    virtual ~base() {}
    virtual void operator()(param val) const = 0;
};

/** 
    In case your manipulator (formatter or destination) needs to hold non-const context information, it can to derive from this.
    This automatically creates a shared pointer to the context information.

    Also, it provides the following operations:

    @c context(), which returns a <tt>context_type &</tt> reference

    Example:

@code
struct write_to_file : destination_base, destination::non_const_context<std::ofstream> {
write_to_file(const char* filename) : non_const_context_base(filename) {}
void operator()(param msg) const {
    context() << msg ;
}
};
@endcode


    @remarks
    In case your manipulator has constant data, you don't need this
*/
template<class context_type> struct non_const_context {

    // this can be used in the parent class, to forward data from its constructor
    typedef non_const_context<context_type> non_const_context_base;
private:
    typedef non_const_context<context_type> self_type;
    typedef boost::shared_ptr<context_type> ptr_type;

protected:
    non_const_context() : m_context(new context_type) {}
    non_const_context(const non_const_context& other) : m_context(other.m_context) {}
    
    BOOST_LOGGING_FORWARD_CONSTRUCTOR_WITH_NEW(non_const_context,m_context,context_type)

    context_type & context() const    { return *(m_context.get()); }

private:
    mutable ptr_type m_context;
};

















} // namespace manipulator

/** 
@brief Formatter is a manipulator. It allows you to format the message before writing it to the destination(s)

talk about format_base

FIXME

@sa manipulator::base, manipulator::base_no_opearator_call, manipulator::non_const_context
*/
namespace formatter {
    using boost::logging::manipulator::base;
    using boost::logging::manipulator::base_no_operator_call;
    using boost::logging::manipulator::non_const_context;
}

/**  
@brief Destination is a manipulator. It contains a place where the message, after being formatted, is to be written to.

Some viable destinations are : the console, a file, a socket, etc.


talk about destination_base

FIXME

*/
namespace destination {
    using boost::logging::manipulator::base;
    using boost::logging::manipulator::base_no_operator_call;
    using boost::logging::manipulator::non_const_context;
}

}}

#endif

