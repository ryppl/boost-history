//
// explore.hpp - container printing.
//
// This version is highly untested and should at least be run on more than one
// compiler before considering for public view.
//
// Copyright (c) 2007 Danny Havenith
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//

#ifndef __EXPLORE_HPP_INCLUDED__
#define __EXPLORE_HPP_INCLUDED__

#include <iostream>
#include <iterator>
#include <boost/functional/detail/container_fwd.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/foreach.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/range/functions.hpp>
#include <boost/range/iterator_range.hpp>


namespace explore {
    using namespace boost::mpl;

    namespace detail {

        //
        // some boilerplate stuff for the is_streamable metafunction
        //

        struct AlmostAnything
        {
            template<typename T> AlmostAnything( T const &);
        };

        // 
        // operator<< cannot have a variable argument list, so we add this
        // one to the overload set, which should have quite low priority.
        //
        char operator<<( std::ostream &, AlmostAnything const &);

        template< typename T> T& make_t();

        // TODO: cleanup, not using char return type.
        template< typename T>
        struct is_streamable : bool_< sizeof( std::cout << make_t<T>()) != sizeof( char)> 
        {
        };

    };


    template< typename T>
    struct is_streamable : detail::is_streamable<T> {};


    //
    // Generally, if we can stream it, we'll stream it. If not, we'll try
    // to treat it as a container of some kind.
    //
    template< typename T>
    struct print_as_container : not_< is_streamable< T> > {};


    // 
    // Now follow a list exceptions that we want to print using our own functions
    // instead of the provided operator<< for those types.
    //

    // treat arrays as containers even though they are streamable
    // (normally they are streamed as pointer-to-void)
    //
    template< typename T, std::size_t size>
    struct print_as_container< T[size]> : true_ {};

    //
    // use our own implementation of streaming instead of the one 
    // provided for iterator range.
    //
    template< typename T>
    struct print_as_container< boost::iterator_range<T> > : true_ {};

    //
    // oh, what the heck. Let's overload for explore::iterator_range 
    // as well, just in case somebody uses this one by accident for 
    // the print function.
    //
    template< typename T>
    struct print_as_container< explore::iterator_range<T> > : true_ {};


    //
    // make sure we use streaming for std containers
    //
    template <class T, class Allocator> 
    struct print_as_container< std::deque< T, Allocator> > : true_ {};

    template <class T, class Allocator> 
    struct print_as_container< std::list<T, Allocator> > : true_ {};

    template <class T, class Allocator> 
    struct print_as_container< std::vector<T, Allocator> > : true_ {};

    template <class Key, class T, class Compare, class Allocator> 
    struct print_as_container< std::map<Key,T, Compare, Allocator> > : true_ {};

    template <class Key, class T, class Compare, class Allocator> 
    struct print_as_container< std::multimap<Key,T, Compare, Allocator> > : true_ {};

    template <class Key, class Compare, class Allocator> 
    struct print_as_container< std::set<Key, Compare, Allocator> > : true_ {};

    template <class Key, class Compare, class Allocator> 
    struct print_as_container< std::multiset<Key, Compare, Allocator> > : true_ {};

    template <class T1, class T2>  
    struct print_as_container< std::pair<T1,T2> > : true_ {};




    //
    // is_map metafunction, probably needs to be extended for 
    // hash_maps etc.
    // explore expects any type T for which is_map<T> is true_ to 
    // have std::pair as value_type and will adapt the default formatting of
    // those pairs.
    //
    template< typename T>
    struct is_map : false_ {};

    template< typename K, typename V, typename P, typename A>
    struct is_map< std::map< K, V, P, A> > : true_ {};

    template< typename K, typename V, typename P, typename A>
    struct is_map< std::multimap< K, V, P, A> > : true_ {};

    //
    // A container policy determines whether an item of a certain type 
    // gets broken up into items or if its going to be streamed directly
    //
    // by default, delegate to the 'global' metafunction,
    // which normally returns true iff a type cannot be streamed
    //
    struct default_container_policy 
    {
        template< typename T>
        struct print_as_container : explore::print_as_container<T> {};
    };


    //
    // Meta functions that are used by the default implementation of format
    // These mainly deal with the fact that a format is a sequence of range
    // format selector types and that we want to re-use the last element of that
    // range when there are no other elements left.
    //

    //
    // metafunction that removes the first element of a sequence unless that would
    // make the sequence empty
    //
    template< typename sequence_type>
    struct pop_front_if : 
        eval_if< bool_<size<sequence_type>::value == 1>,
        identity<sequence_type>,
        pop_front< sequence_type>
        >
    {};

    //
    // metafunction that replaces the leftmost element of a sequence with another type,
    // except if the sequence contained only 1 element, in which case the new head type is
    // pushed in front of the last one.
    //
    template< typename sequence_type, typename head_type>
    struct replace_head : 
        push_front< BOOST_DEDUCED_TYPENAME pop_front_if< sequence_type>::type, head_type> {};


    //
    // A range format describes how the elements of a range are being
    // formatted.
    // This version supports only simple 'opener', 'delimiter', 'closer'
    // formats.
    struct basic_range_format
    {
        typedef char char_;

        //
        // not used currently.
        static char_ *header() { return "";}
        static char_ *footer() { return "";}

        //
        // default implementation of the next_format
        // meta function.
        // This defines how the format for the next recursion level is 
        // chosen.
        template< typename sequence_type>
        struct next_format : pop_front_if< sequence_type> {};
    };

    // TODO: support other char types by making this a template
    struct default_range_format : basic_range_format
    {
        // only for sequences
        static char_ opening() { return '[';}
        static char_ closing() { return ']';}
        static char_ *delimiter() { return ", ";}

        // string_ escape( const string_ &);
    };

    struct map_pair_format : basic_range_format
    {
        static char_ *opening() { return "";}
        static char_ *closing() { return "";}
        static char_ delimiter() { return ':';}
        // string_ escape( const string_ &);
    };

    struct map_pair_format_selector
    {
        template< typename T>
        struct range_format: identity< map_pair_format> {};
    };

    struct map_range_format : default_range_format
    {
        template< typename sequence_type>
        struct next_format : replace_head< sequence_type, map_pair_format_selector> {};
    };

    // select a range format based on the type of the container
    struct default_range_format_selector
    {

        template< typename T>
        struct range_format : if_<
            is_map< T>,
            map_range_format,
            default_range_format>{};

    };


    struct default_format : boost::mpl::list< default_range_format_selector> {};

    //
    // Several container printer overloads.
    //
    template< typename format_type,
        typename container_policy_type,
        typename item_type
    >
    std::ostream &print_container( const item_type &item, std::ostream &stream )
    {
        // the first element of the format sequence is our current format-selector
        typedef BOOST_DEDUCED_TYPENAME front<format_type>::type format_selector;

        // the format-selector gives us a formatter, based on the type of the thing we need to format
        typedef BOOST_DEDUCED_TYPENAME format_selector::template range_format<item_type>::type formatter;

        // the formatter in it's turn tells us what the format for the next depth in the tree will be
        typedef BOOST_DEDUCED_TYPENAME formatter::template next_format< format_type>::type next_format_type;


        stream << formatter::opening();
        bool printing_first_item = true;

        typedef BOOST_DEDUCED_TYPENAME boost::range_const_iterator< const item_type>::type iterator_type;
        typedef BOOST_DEDUCED_TYPENAME boost::iterator_value< iterator_type>::type value_type;

        for ( 
            iterator_type i = boost::const_begin( item);
            i != boost::const_end( item);
        ++i)
        {
            if (!printing_first_item)
            {
                stream << formatter::delimiter();
            }

            print( *i, stream, next_format_type(), container_policy_type());
            printing_first_item = false;
        }
        stream << formatter::closing();

        return stream;
    }

    // TODO: generalize for tuples
    template< typename format_type,
        typename container_policy_type,
        typename F,
        typename S
    >
    std::ostream &print_container( const std::pair<F,S> &item, std::ostream &stream )
    {
        typedef std::pair<F,S> item_type;

        // the first element of the format sequence is our current format-selector
        typedef BOOST_DEDUCED_TYPENAME front<format_type>::type format_selector;

        // the format-selector gives us a formatter, based on the type of the thing we need to format
        typedef BOOST_DEDUCED_TYPENAME format_selector:: template range_format<item_type>::type formatter;

        // the formatter in it's turn tells us what the format for the next depth in the tree will be
        typedef BOOST_DEDUCED_TYPENAME formatter:: template next_format< format_type>::type next_format_type;

        stream << formatter::opening();
        print( item.first, stream, next_format_type(), container_policy_type());
        stream << formatter::delimiter();
        print( item.second, stream, next_format_type(), container_policy_type());
        return stream << formatter::closing();
    }

    //
    // the print_item methods of this class are called whenever print has deduced that
    // the item to print is some container-type
    //
    struct container_printer
    {
        template< 
            typename format_type,
            typename container_policy_type,
            typename item_type
        >
        static std::ostream &print_item( const item_type &item, std::ostream &stream)
        {
            return print_container<format_type, container_policy_type>( item, stream);
        }


    };

    struct pointer_printer
    {
        template< typename format_type,
            typename container_policy_type,
            typename item_type
        >
        static std::ostream &print_item( const item_type *item, std::ostream &stream )
        {
            if( NULL == item )
            {
                return stream << (char *)"null"; 
            }
            //
            // DH - changed until we figure out what special things to do with pointer types
            // return print(*item, stream, format_type(), container_policy_type());
            return stream << item;
        }
    };

    struct item_printer
    {
        template< typename format_type,
            typename container_policy_type,
            typename item_type
        >
        static std::ostream &print_item( const item_type &item, std::ostream &stream )
        {
            return stream << item;
        }
    };

    /**
    * This is the main entrance to print. 
    * This function tries to stream it's first argument (the item) to the stream given as a second argument. 
    * If the item has no stream operator, print will try to treat it as some form of container and call itself recursively
    * on every element of the container.
    *
    * Print ask the container policy whether to treat the item as a container. If directed so, it will delegate to the
    * print_container free function in namespace scope.
    * Otherwise, it will try to stream the item using the expression "stream << item".
    *
    * \param &item the item to stream
    * \param &stream the stream that receives the item or its elements
    * \param &format the format to use (string escaping, delimiters etc.)
    * \param &policy a type that decides when a type needs to be treated as a container.
    * \return the original stream (second argument)
    */
    template< typename format_type,
        typename container_policy_type ,
        typename item_type
    >
    std::ostream &print( const item_type &item, 
    std::ostream &stream, 
    const format_type &format,
    const container_policy_type &policy)
    {
        typedef  typename eval_if< 
            typename container_policy_type:: template print_as_container< item_type>::type,
            identity<container_printer>,
            eval_if< 
            typename boost::is_pointer<item_type>::type,
            identity<pointer_printer>,
            identity<item_printer>
            > 
        >::type printer_type;


        return printer_type::template print_item< format_type, container_policy_type>( item, stream);
    }

    template < typename item_type>
    std::ostream &print( const item_type &item, std::ostream &stream = std::cout)
    {
        return print( item, stream, default_format(), default_container_policy());
    }

    template< typename format_type,
        typename container_policy_type ,
    class InputIterator
    >
    std::ostream &print( InputIterator first,
    InputIterator last, 
    std::ostream &stream, 
    const format_type &format,
    const container_policy_type &policy)
    {
        return print( boost::make_iterator_range(first, last), stream, format, policy );
    }

    template<class InputIterator>
    std::ostream &print(InputIterator first, InputIterator last, std::ostream &stream = std::cout)
    {
        return print( boost::make_iterator_range(first, last), stream );
    }


    //
    // The container_formatter type to allow "inline" streaming like the following
    //
    // std::cout << container_format( my_formatter) % my_container1  << ...
    //

    template< typename container, typename formatter>
    struct container_format_class
    {
        container_format_class( const container &c)
            : container_( c)
        {
            // nop
        };

        const container &get_container() const
        {
            return container_;
        }
    private:
        const container &container_;

    };

    template< typename formatter>
    struct unbound_container_format_class
    {
    };

    template< typename formatter>
    unbound_container_format_class< formatter> container_format( const formatter &)
    {
        return unbound_container_format_class<formatter>();
    }

    unbound_container_format_class< default_format> container_format()
    {
        return unbound_container_format_class< default_format>();
    }

    template< typename container, typename formatter>
    container_format_class< container, formatter> operator%( unbound_container_format_class<formatter> &u, const container &c)
    {
        return container_format_class< container, formatter>( c);
    }

    template< typename container, typename formatter>
    std::ostream &operator<<( std::ostream &strm, const container_format_class< container, formatter> &cf)
    {
        return print( cf.get_container(), strm);
    }

}

#endif //__EXPLORE_HPP_INCLUDED__

