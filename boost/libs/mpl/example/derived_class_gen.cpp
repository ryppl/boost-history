//----------------------------------------------------------------------------
// boost mpl/example/derived_class_generator.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//----------------------------------------------------------------------------
//
// Copyright (c) 2001
// David Abrahams, Aleksey Gurtovoy, Mat Marcus
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

//----------------------------------------------------------------------------

// This file is a part of boost::mpl library tutorial; some parts of the
// tutorial are aimed to demonstrate how one can use the library to 
// implement metaprogramming facilities close or equivalent to the 
// ones presented in Andrei Alexandrescu's "Modern C++ Design" book 
// (ISBN 0-201-70431-5), and the Loki Library. Authors of the tutorial 
// acknowledge some parts of their work as derivative by posting the 
// Loki Library's original copyright notice:

// The Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// The author or Addison-Welsey Longman make no representations about the 
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.

//----------------------------------------------------------------------------


#include "boost/mpl/accumulate.hpp"
#include "boost/mpl/transform.hpp"
#include "boost/mpl/at.hpp"
#include "boost/mpl/type_list.hpp"

namespace aux {
template<typename T1, typename T2>
struct inherit_two : T1, T2
{
};

template<typename F>
struct node_generator
{
    template<typename Base, typename T>
    struct apply
    {
        typedef typename boost::mpl::unary_function<F,T>::type element_t;
        typedef inherit_two<Base, element_t> type;
    };
};
} // namespace aux

// a modified 'derived_class_generator' definition
template<typename Types, typename F>
struct derived_class_generator
{
    struct empty
    {
        // NEW: provide the result class with 'field_list'
        // member, that will contain a list of 
        // F(T0),F(T1),..,F(Tn) node types
        	typedef typename boost::mpl::transform<
                  Types
                , boost::mpl::type_list<>
                , F
                >::sequence field_list;
    };
    
    typedef typename boost::mpl::accumulate< // former for_each
          Types
        , empty
        , aux::node_generator<F>
        >::type type;
};

// 'field' function implementation
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1200

namespace aux {
template<long N, typename T>
struct field_type
{
    typedef typename boost::mpl::at<N, typename T::field_list>::type type;
};

}

template<long N, typename T>
typename aux::field_type<N, T>::type&
field(T& t)
{
    typedef typename aux::field_type<N, T>::type field_t;
    return static_cast<field_t&>(t);
}

#else

template<long N, typename T>
typename boost::mpl::at<N, typename T::field_list>::type&
field(T& t)
{
    typedef typename boost::mpl::at<N, typename T::field_list>::type field_t;
    return static_cast<field_t&>(t);
}

#endif // defined(BOOST_MSVC) && BOOST_MSVC <= 1200

// usage example
struct holder_gen
{
    template<typename T> struct apply
    {
        struct type { T m_value; };
    };
};

typedef derived_class_generator<
      boost::mpl::type_list<char,short,long>
    , holder_gen
    >::type my_tuple_t;

int main()
{
	my_tuple_t t;
	field<0>(t).m_value = 10;
	field<1>(t).m_value = 5;
	field<2>(t).m_value = 7;
	// field<3>(t).m_value = 1; // error
	return 0;
}
