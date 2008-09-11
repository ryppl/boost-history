/*
    Copyright 2005-2007 Adobe Systems Incorporated
   
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_MOVE_HPP
#define BOOST_MOVE_HPP

#include <cassert>
#include <memory>

#include <boost/iterator.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/move/detail/config.hpp>
#include <boost/move/detail/has_swap_overload.hpp>

/*************************************************************************************************/

// Illegal forward declare.

namespace std
{
    template <class T> class auto_ptr;
}

/*************************************************************************************************/

namespace boost {

/*************************************************************************************************/

namespace move_detail {

/*************************************************************************************************/

#if !defined(BOOST_MOVE_NO_HAS_MOVE_ASSIGN)

/*************************************************************************************************/

template <typename T>  
struct class_has_move_assign {  
    class type {
        typedef T& (T::*E)(T t);  
        typedef char (&no_type)[1];  
        typedef char (&yes_type)[2];  
        template <E e> struct sfinae { typedef yes_type type; };  
        template <class U>  
        static typename sfinae<&U::operator=>::type test(int);  
        template <class U>  
        static no_type test(...);  
    public:  
        enum {value = sizeof(test<T>(1)) == sizeof(yes_type)};  
    };
 };  

/*************************************************************************************************/

template<typename T>
struct has_move_assign : boost::mpl::and_<boost::is_class<T>, class_has_move_assign<T> > {};

/*************************************************************************************************/

class test_can_convert_anything { };

/*************************************************************************************************/

#endif // BOOST_MOVE_NO_HAS_MOVE_ASSIGN

/*************************************************************************************************/

/*
	REVISIT (sparent@adobe.com): This is a work around for Boost 1.34.1 and VC++ 2008 where
	boost::is_convertible<T, T> fails to compile.
*/

template <typename T, typename U>
struct is_convertible : boost::mpl::or_<
	boost::is_same<T, U>,
	boost::is_convertible<T, U>
> { };

/*************************************************************************************************/

} //namespace move_detail


/*************************************************************************************************/

/*!
\ingroup move_related
\brief move_from is used for move_ctors.
*/

template <typename T>
struct move_from
{
    explicit move_from(T& x) : source(x) { }
    T& source;
};

/*************************************************************************************************/

/*!
\ingroup move_related
\brief swap_from is used for move_ctors.
*/

template <typename T>
struct swap_from
{
    swap_from(T& x)
        : m(x)
    {}

    operator T() const
    {
        T x;
        using std::swap;
        swap(this->m, x);
        return x;
    }

    T& m;
};


/*************************************************************************************************/

#if !defined(BOOST_MOVE_NO_HAS_MOVE_ASSIGN)

/*************************************************************************************************/

/*!
\ingroup move_related
\brief The is_boost_movable trait can be used to identify types that are movable using this library.
*/
template <typename T>
struct is_boost_movable : boost::mpl::and_<
                        boost::mpl::or_<
                            boost::is_convertible<T, move_from<T> >,
                            boost::is_convertible<move_from<T>, T>
                        >,
                        move_detail::has_move_assign<T>,
                        boost::mpl::not_<boost::is_convertible<move_detail::test_can_convert_anything, T> >
                    > { };

/*************************************************************************************************/

#else // BOOST_MOVE_NO_HAS_MOVE_ASSIGN

// On compilers which don't have adequate SFINAE support, treat most types as unmovable,
// unless the trait is specialized.

template <typename T>
struct is_boost_movable : boost::mpl::false_ { };

#endif

/*************************************************************************************************/

struct copy_tag {};
struct swap_tag {};
struct boost_move_tag {};
struct custom_move_tag {};

template <typename T, class Enable = void>
struct move_type :
    boost::mpl::if_<is_boost_movable<T>, boost_move_tag,
        typename boost::mpl::if_<boost::detail::has_swap_overload<T>,
            swap_tag, copy_tag>::type > {};

template <class T>
struct move_type<std::auto_ptr<T> > {
    typedef custom_move_tag type;
};

/*************************************************************************************************/

/*!
\ingroup move_related
\brief The is_movable trait can be used to identify movable types.
*/
template <typename T>
struct is_movable : boost::mpl::not_<boost::is_same<
                        typename boost::move_type<T>::type, copy_tag > > {};


/*************************************************************************************************/

#if !defined(BOOST_NO_SFINAE)

/*************************************************************************************************/

/*!
\ingroup move_related
\brief move_type_sink is used to select between overloaded operations according to
 the value of move_type<T> and whether it is convertible to type U.
\sa move
*/

template <typename Tag, typename T, typename U = T, typename R = void*>
struct move_type_sink
    : boost::enable_if<
        boost::mpl::and_<
            boost::move_detail::is_convertible<T, U>,
            boost::is_same<Tag, typename boost::move_type<T>::type>
        >,
        R
    > {};

/*************************************************************************************************/

/*!
\ingroup move_related
\brief copy_sink and move_sink are used to select between overloaded operations according to
 whether type T is movable and convertible to type U.
\sa move
*/

template <typename T,
          typename U = T,
          typename R = void*>
struct copy_sink : boost::enable_if<
                        boost::mpl::and_<
                            boost::move_detail::is_convertible<T, U>,                           
                            boost::mpl::not_<is_movable<T> >
                        >,
                        R
                    >
{ };

/*************************************************************************************************/

/*!
\ingroup move_related
\brief move_sink and copy_sink are used to select between overloaded operations according to
 whether type T is movable and convertible to type U.
 \sa move
*/

template <typename T,
          typename U = T,
          typename R = void*>
struct move_sink : boost::enable_if<
                        boost::mpl::and_<
                            boost::move_detail::is_convertible<T, U>,                            
                            is_movable<T>
                        >,
                        R
                    >
{ };

/*************************************************************************************************/

/*!
\ingroup move_related
\brief This version of move is selected when T is_movable . It in turn calls the move
constructor. This call, with the help of the return value optimization, will cause x to be moved
instead of copied to its destination. See adobe/test/move/main.cpp for examples.

*/
template <typename T>
T move(T& x, typename move_type_sink<boost_move_tag, T>::type = 0) { return T(move_from<T>(x)); }

/*************************************************************************************************/

/*!
\ingroup move_related
\brief This version of move is selected when T is not movable . The net result will be that
x gets copied.
*/
template <typename T>
T& move(T& x, typename move_type_sink<copy_tag, T>::type = 0) { return x; }

/*************************************************************************************************/

/*!
\ingroup move_related
\brief This version of move is selected when T is swappable.
*/
template <typename T>
T move(T& x, typename move_type_sink<swap_tag, T>::type = 0) { return T(swap_from<T>(x)); }

/*************************************************************************************************/

#else // BOOST_NO_SFINAE

// On compilers without SFINAE, define copy_sink to always use the copy function.

template <typename T,
          typename U = T,
          typename R = void*>
struct copy_sink
{
    typedef R type;
};

// Always copy the element unless this is overloaded.

template <typename T>
T& move(T& x) {
    return x;
}

#endif // BOOST_NO_SFINAE

/*************************************************************************************************/

/*!
\ingroup move_related
\brief This version of move is for auto_ptrs.
*/
template <class T>
std::auto_ptr<T>& move(std::auto_ptr<T>& x) {
    return x;
}

/*************************************************************************************************/

/*!
\ingroup move_related
\brief Iterator pair version of move. Similar to std::copy but with move semantics, 
for movable types, otherwise with copy semantics.
*/
template <typename I, // I models InputIterator
          typename O> // O models OutputIterator
O move(I f, I l, O result)
{
    while (f != l) {
        *result = boost::move(*f);
        ++f; ++result;
    }
    return result;
}

/*************************************************************************************************/

/*!
\ingroup move_related
\brief \ref concept_convertible_to_range version of move. Similar to copy but with move semantics, 
for movable types, otherwise with copy semantics.
*/
template <typename I, // I models InputRange
          typename O> // O models OutputIterator
inline O move(I& in, O out) { return boost::move(boost::begin(in), boost::end(in), out); }

/*************************************************************************************************/
 
/*!
\ingroup move_related
\brief Iterator pair version of move_backwards. Similar to std::copy_backwards but with move semantics, 
for movable types, otherwise with copy semantics.
*/
template <typename I, // I models BidirectionalIterator
          typename O> // O models BidirectionalIterator
O move_backward(I f, I l, O result)
{
    while (f != l) {
        --l; --result;
        *result = boost::move(*l);
    }
    return result;
}

/*************************************************************************************************/

/*!
\ingroup move_related
\brief \ref concept_convertible_to_range version of move_backwards. Similar to std::copy_backwards but 
with move semantics, for movable types, otherwise with copy semantics.
*/
template <typename I, // I models BidirectionalRange
          typename O> // O models BidirectionalIterator
inline O move_backward(I& in, O out)
{ return move_backward(boost::begin(in), boost::end(in), out); }

/*************************************************************************************************/

/*!
\ingroup move_related
\brief Similar to std::back_insert_iterator but 
with move semantics, for movable types, otherwise with copy semantics.
*/

template <typename C> // C models Container
class back_move_iterator : public boost::iterator<std::output_iterator_tag, void, void, void, void>
{
    C* container_m;
    
 public:
    typedef C container_type;
    
    explicit back_move_iterator(C& x) : container_m(&x) { }
    
    back_move_iterator& operator=(typename C::value_type x)
    { container_m->push_back(boost::move(x)); return *this; }
    
    back_move_iterator& operator*() { return *this; }
    back_move_iterator& operator++() { return *this; }
    back_move_iterator& operator++(int) { return *this; }
};

/*************************************************************************************************/

/*!
\ingroup move_related
\brief Similar to std::back_inserter but 
with move semantics, for movable types, otherwise with copy semantics.
*/

template <typename C> // C models Container
inline back_move_iterator<C> back_mover(C& x) { return back_move_iterator<C>(x); }

/*************************************************************************************************/

template <class T>
inline void move_construct(std::auto_ptr<T>* p, std::auto_ptr<T>& x) {
    ::new(static_cast<void*>(p)) std::auto_ptr<T>(x);
}

/*************************************************************************************************/

#if !defined(BOOST_NO_SFINAE)

/*************************************************************************************************/

/*!
\ingroup move_related
\brief Placement move construction, selected when T is_movable is true
*/

template <typename T> // T models Regular
inline void move_construct(T* p, T& x, typename move_type_sink<boost_move_tag, T>::type = 0)
{
    ::new(static_cast<void*>(p)) T(move_from<T>(x));
}

/*************************************************************************************************/

/*!
\ingroup move_related
\brief Placement copy construction, selected when T is_movable is false
*/
template <typename T> // T models Regular
inline void move_construct(T* p, const T& x, typename move_type_sink<copy_tag, T>::type = 0)
{
    ::new(static_cast<void*>(p)) T(x);
}

/*************************************************************************************************/

/*!
\ingroup move_related
\brief Placement copy construction, selected when T is swappable.
*/
template <typename T> // T models Regular
inline void move_construct(T* p, T& x, typename move_type_sink<swap_tag, T>::type = 0)
{
    ::new(static_cast<void*>(p)) T();
    using std::swap;
    swap(*p, x);
}

/*************************************************************************************************/

/*!
\ingroup move_related
\brief Similar to std::uninitialized_copy but 
with move semantics, for movable types.
*/
template <typename I, // I models InputIterator
          typename F> // F models ForwardIterator
F uninitialized_move(I f, I l, F r,
        typename move_sink<typename boost::iterator_value<I>::type>::type = 0)
{
    while (f != l) {
        boost::move_construct(&*r, *f);
        ++f; ++r;
    }
    return r;
}

/*************************************************************************************************/

/*!
\ingroup move_related
\brief Behaves as to std::uninitialized_copy , invoked when I's value_type is not movable.
*/
template <typename I, // I models InputIterator
          typename F> // F models ForwardIterator
F uninitialized_move(I f, I l, F r,
        typename copy_sink<typename boost::iterator_value<I>::type>::type = 0)
{
    return std::uninitialized_copy(f, l, r);
}

/*************************************************************************************************/

#else // BOOST_NO_SFINAE

template <typename T>
inline void move_construct(T* p, const T& x)
{
    ::new(static_cast<void*>(p)) T(x);
}

template <typename I, // I models InputIterator
          typename F> // F models ForwardIterator
F uninitialized_move(I f, I l, F r)
{
    return std::uninitialized_copy(f, l, r);
}

#endif // BOOST_NO_SFINAE

/*************************************************************************************************/

} // namespace boost

/*************************************************************************************************/

#endif

/*************************************************************************************************/
